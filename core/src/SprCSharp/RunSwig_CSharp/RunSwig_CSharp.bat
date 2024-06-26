@echo off
setlocal enabledelayedexpansion
:: =============================================================================
::  SYNOPSIS:
::	RunSwig_CSharp target
::	    target	    DLL構成
::		ALL	    すべてのプロジェクトを含む構成とする.
::		Physics	    Physicsを含む最小の構成とするを含む構成とする.
::	
::  DESCRIPTION:
::      ファイルの依存関係を調べて、CSharpSwig.bat を最適に実行する.
::	※ このスクリプトで作成した makefile を実行する.
::
::	構成に組み込むプロジェクトの名称は make_projs_file.py によって作成
::	されるバッチファイルを実行することで環境変数 PROJECTS に取り込む.
::
::  NOTE:
::	このバッチファイルから nmake を実行するので, PATH に入れておくか
::	setup を実行しておくこと.
::
:: =============================================================================
::  VERSION:
::     Ver 1.0	 2015/03/18 F.Kanehori  初版
::     Ver 2.0	 2016/02/08 F.Kanehori  wrapper file 統合
::     Ver 3.0	 2016/12/07 F.Kanehori  リンク構成指定実装
::     Ver 3.1   2016/12/15 F.Kanehori	ラッパファイル作成方式変更
::     Ver 3.2	 2017/01/16 F.Kanehori	NameManger 導入
::     Ver 3.3   2017/08/07 F.Kanehori	Bug fixed.
::     Ver 4.0   2021/01/20 F.Kanehori	Setup 導入移行期間開始
::     Ver 4.1   2021/07/19 F.Kanehori	見直し.
:: =============================================================================
set PROG=%~n0
set DEBUG=1

:: ----------------------------------------------------------------------
::  このスクリプトは "<SprTop>/core/src/SprCSharp/RunSwig_CSharp" に置く
::
set CWD=%cd%
cd /d %~dp0\..\..\..\..
set SprTop=%CD%
cd ..
set SprBase=%CD%
cd %CWD%
set CspDir=%SprTop%\core\src\SprCSharp
set RSCspDir=%CspDir%\RunSwig_CSharp

:: ------------
::  引数の処理
:: ------------
set TARGET=%1
if "%TARGET%" equ "" (
    echo Usage: %PROG% target
    echo     target     ALL ^| Physics ^| ...
    exit /b
)
echo %~nx0
if %DEBUG% == 1 (
    echo+  TARGET:  %TARGET%
)

:: ------------------------
::  共通環境変数を読み込む
:: ------------------------
set NAMEMANAGER=%CspDir%\NameManager\NameManager.bat
if not exist %NAMEMANAGER% (
	:: NameManager.bat が存在しないときは何もしない
	echo "NameManager.bat" not found.
	exit /b
)
call %NAMEMANAGER%
if %DEBUG% == 1 (
    call :show_abspath INCDIR %INCDIR%
    call :show_abspath SRCDIR %SRCDIR%
    call :show_abspath ETCDIR %ETCDIR%
    call :show_abspath CS_SRC %CS_SRC%
    call :show_abspath CS_IMP %CS_IMP%
    call :show_abspath CS_EXP %CS_EXP%
    call :show_abspath TARGETFILE %TARGETFILE%
)

:: ----------------------------------------
::  依存関係にはないと見做すファイルの一覧
:: ----------------------------------------
set EXCLUDES=

:: -----------------------------------
::  makefile に出力するときのパス表記
:: -----------------------------------
set INCDIROUT=..\..\include
set SRCDIROUT=..\..\src
set CSHARPSWIG=RunSwig_CSharp\%CSHARPSWIG%

:: --------------------
::  使用するファイル名
:: --------------------
set MAKEFILE=Makefile_CSharp.swig

::-----------------------------------------------------------------------------
::  モジュールの依存関係表を作成する.
::
set MODULE_DEPTS=%RSCspDir%\module.depts
%PYTHON% %RSCspDir%\make_projs_file.py %MODULE_DEPTS%

::-----------------------------------------------------------------------------
::  処理するモジュール一覧を作成
::
set PROJECTS=Base
set SWIGMACRO=

if /i "%TARGET%" equ "ALL" (
	for /f "tokens=1,*" %%m in (%MODULE_DEPTS%) do (
    		set PROJECTS=!PROJECTS! %%m
	)
) else (
	for %%t in (%TARGET:,= %) do (
		if "%%t" equ "PH" (
			set REAL_TARGET=Physics
		) else (
			set REAL_TARGET=%%t
		)
		for /f "tokens=1,*" %%m in (%MODULE_DEPTS%) do (
			if "%%m" equ "!REAL_TARGET!" (
	    			set PROJECTS=!PROJECTS! %%n !REAL_TARGET!
	    			set PROJECTS=!PROJECTS:,= !
	    			:: 重複要素を取り除く
	    			call :bag_to_set PROJECTS_tmp "!PROJECTS!"
	    			set PROJECTS=!PROJECTS_tmp!
			)
    		)
	)
	:: swig用のマクロを定義する
	if "!TARGET!" equ "!TARGET:,= !" (
		set SWIGMACRO=%TARGET%_ONLY
	) else (
		set SWIGMACRO=!TARGET:,=_and_!
	)
)
if "%PROJECTS%" equ "" (
    echo %PROG%: Error: invalid target name "%TARGET%"
    exit /b
)

::-----------------------------------------------------------------------------
::  .i ファイルの一覧を作成
::
set IFILES=
for %%p in (%PROJECTS%) do (
    set IFILES=!IFILES! ./%%p.i
)

set IFILES=%IFILES:~1%

echo+  Projects:  [%PROJECTS%]
echo+  SWIGMACRO: [%SWIGMACRO%]
echo+  IFiles:    [%IFILES%]

::-----------------------------------------------------------------------------
::  モジュールにまたがる初期化
::
cd %CspDir%
rem echo+  SIGNATUREFILE:     %SIGNATUREFILE%
rem echo+  WRAPPERSBUILTFILE: %WRAPPERSBUILTFILE%
call :truncate_file %SIGNATUREFILE%
del /f %WRAPPERSBUILTFILE% > NUL 2>&1

::-----------------------------------------------------------------------------
::  作業ディレクトリの作成
::
for %%d in (%CS_SRC% %CS_IMP% %CS_EXP%) do (
	set TMPDIR=%%d\tmp
	if not exist !TMPDIR! (
		echo making directory !TMPDIR!
		mkdir !TMPDIR!
	)
)

::-----------------------------------------------------------------------------
::  処理開始
::
echo+  add path: "%MAKEPATH%"
PATH=%MAKEPATH%;%PATH%

echo+  making ...
cd %CspDir%
set MAKE=%NMAKE% /nologo
for %%p in (%PROJECTS%) do (
    echo+    Project: %%p
    set MKFILE=%MAKEFILE%.%%p
    call :collect_headers %%p
    call :make_makefile %%p !MKFILE! "!INCHDRS!" "!SRCHDRS!" "%IFILES%"
    rem cd %CspDir%
    rem echo cmd /c %MAKE% -f !MKFILE!
	cmd /c %MAKE% -f !MKFILE!
    rem cd %CWD%
)
cd %CS_SRC%

::-----------------------------------------------------------------------------
::  wrapper file をまとめる
::
if exist %WRAPPERSBUILTFILE% (
    echo+  combining wrapper files
    set WF_SRC=%SUBDIR_SRC:/=\%\%MOD_WRAPPERFILE_SRC%
    set WF_IMP=%SUBDIR_IMP:/=\%\%MOD_WRAPPERFILE_IMP%
    set WF_EXP=%SUBDIR_EXP:/=\%\%MOD_WRAPPERFILE_EXP%
    type ..\!WF_SRC!.prologue > ..\!WF_SRC!
    type ..\!WF_IMP!.prologue > ..\!WF_IMP!
    type ..\!WF_EXP!.prologue > ..\!WF_EXP!
    for %%f in (..\%SUBDIR_SRC:/=\%\tmp\*.cs)  do ( type %%f >> ..\!WF_SRC! )
    for %%f in (..\%SUBDIR_IMP:/=\%\tmp\*.cs)  do ( type %%f >> ..\!WF_IMP! )
    for %%f in (..\%SUBDIR_EXP:/=\%\tmp\*.cpp) do ( type %%f >> ..\!WF_EXP! )
    echo } >> ..\!WF_SRC!
    echo.   } >> ..\!WF_IMP!
    echo } >> ..\!WF_IMP!
    echo } >> ..\!WF_EXP!
)

::-----------------------------------------------------------------------------
::  TARGET をファイルに記録する
::
echo+  last target: %TARGET%
echo %TARGET% > %TARGETFILE%

::-----------------------------------------------------------------------------
::  処理終了
::
endlocal
exit /b

::-----------------------------------------------------------------------------
::  ファイルの初期化（空のファイルとする）
::-----------------------------------------------------------------------------
:truncate_file
    if exist %1 del /F %1
    type NUL > %1
exit /b

::-----------------------------------------------------------------------------
::  ファイルへの追加
::-----------------------------------------------------------------------------
:append_file
    if exist %1 type %1 >> %2
exit /b

::-----------------------------------------------------------------------------
::  引数1 で与えられたプロジェクトのヘッダ情報を収集する
::-----------------------------------------------------------------------------
:collect_headers
    set PROJECT=%1

    :: 依存ファイル情報を集める
    ::
    set INCHDRS=
    for %%f in (%INCDIR%\%PROJECT%\*.h) do (
        call :one_of "%EXCLUDES%" %%~nxf
        if "!$result!" equ "no" set INCHDRS=!INCHDRS! %%~nxf
    )
    call :add_prefix "!INCHDRS:~1!" %INCDIROUT%\%PROJECT%
    set INCHDRS=%$string:\=/%
    if %DEBUG% gtr 2 echo INCHDRS  [%INCHDRS%]

    set SRCHDRS=
    for %%f in (%SRCDIR%\%PROJECT%\*.h) do (
        call :one_of "%EXCLUDES%" %%~nxf
        if "!$result!" equ "no" set SRCHDRS=!SRCHDRS! %%~nxf
    )
    call :add_prefix "!SRCHDRS:~1!" %SRCDIROUT%\%PROJECT%
    set SRCHDRS=%$string:\=/%
    if %DEBUG% gtr 2 echo SRCHDRS  [%SRCHDRS%]

exit /b

::-----------------------------------------------------------------------------
::  makefile を作成する
::      引数1   モジュール名
::      引数2   makefile 名
::      引数3   "依存ヘッダファイル名リスト"
::      引数4   "依存ソースファイル名リスト"
::-----------------------------------------------------------------------------
:make_makefile
    setlocal enabledelayedexpansion
    set MODULE=%1
    set MKFILE=%2
    set INCHDRS=%~3
    set SRCHDRS=%~4
    set INTFILES=%~5
    if %DEBUG% gtr 1 (
        echo MODULE   [%MODULE%]
        echo MKFILE   [%MKFILE%]
        echo INCHDRS  [%INCHDRS%]
        echo SRCHDRS  [%SRCHDRS%]
        echo INTFILES [%INTFILES%]
    )

    set TARGET_SRC=./%SUBDIR_SRC%/CS%MODULE%.cs
    set TARGET_ALL=%TARGET_SRC%
    rem ------------------------------------------------------
    rem set TARGET_IMP=%SUBDIR_IMP%/CS%MODULE%.cs
    rem set TARGET_EXP=%SUBDIR_EXP%/CS%MODULE%.cpp
    rem set TARGET_ALL=%TARGET_SRC% %TARGET_IMP% %TARGET_EXP%
    rem ------------------------------------------------------

    set DEPENDENCIES=%INCHDRS% %SRCHDRS% %INTFILES%
    if %DEBUG% gtr 1 (
        echo TARGET       [%TARGET%]
        echo DEPENDENCIES [%DEPENDENCIES%]
    )

    echo #  Do not edit. RunSwig_CSharp.bat will update this file.   > %MKFILE%
    echo #  File: %MKFILE%	>> %MKFILE%
    echo.			>> %MKFILE%
    echo INCHDRS=\>> %MKFILE%
    for %%f in (%INCHDRS%) do (
        echo %%f \>> %MKFILE%
    )
    echo.			>> %MKFILE%
    echo SRCHDRS=\>> %MKFILE%
    for %%f in (%SRCHDRS%) do (
        echo %%f \>> %MKFILE%
    )
    echo.			>> %MKFILE%
    echo INTFILES=\>> %MKFILE%
    for %%f in (%INTFILES%) do (
        echo %%f \>> %MKFILE%
    )
    echo.			>> %MKFILE%
    echo FIXED_WRAPPERS=\>> %MKFILE%
    echo ./SprCSharp/wrapper.cs		\>> %MKFILE%
    echo ./SprImport/wrapper.cs		\>> %MKFILE%
    echo ./SprExport/wrapper.cpp	\>> %MKFILE%
    echo.			>> %MKFILE%
    echo WRAPPERS=\>> %MKFILE%
    echo ./SprCSharp/module.wrapper.cs	\>> %MKFILE%
    echo ./SprImport/module.wrapper.cs	\>> %MKFILE%
    echo ./SprExport/module.wrapper.cpp	\>> %MKFILE%
    echo.			>> %MKFILE%

    echo all:	%TARGET_ALL%    >> %MKFILE%
    echo.			>> %MKFILE%
    echo %TARGET_SRC%:	$(INCHDRS) $(SRCHDRS) $(INTFILES) $(FIXED_WRAPPERS)>> %MKFILE%
    echo.	call %CSHARPSWIG% %MODULE% %SWIGMACRO%	>> %MKFILE%
    echo.				>> %MKFILE%
    echo $(INCHDRS):			>> %MKFILE%
    echo.				>> %MKFILE%
    echo $(SRCHDRS):			>> %MKFILE%
    echo.				>> %MKFILE%
    echo $(INTFILES):			>> %MKFILE%
    echo.				>> %MKFILE%
    echo $(FIXED_WRAPPERS):		>> %MKFILE%
    echo.				>> %MKFILE%
    echo $(WRAPPERS):			>> %MKFILE%
    echo.				>> %MKFILE%
    endlocal
exit /b

::-----------------------------------------------------------------------------
::  引数2 で与えられた名前が 引数1 で与えられたリスト中にあるか調べる
::  結果は $result に返す（yes または no）
::-----------------------------------------------------------------------------
:one_of
    set $result=no
    for %%f in (%~1) do (if "%2" equ "%%f" (set $result=yes&& exit /b))
exit /b

::-----------------------------------------------------------------------------
::  引数1 で与えられた変数に、引数2 で指定された prefix を追加する
::  結果は $string に返す
::-----------------------------------------------------------------------------
:add_prefix
    set $string=
    for %%f in (%~1) do (set $string=!$string! %2\%%f)
    set $string=%$string:~1%
exit /b

::-----------------------------------------------------------------------------
::  集合から重複した要素を取り除く
::-----------------------------------------------------------------------------
:bag_to_set
	setlocal enabledelayedexpansion
	set SET=
	for %%b in (%~2) do (
		set IN=0
		for %%s in (!SET!) do (
			if "%%b" equ "%%s" set IN=1
		)
		if !IN! == 0 set SET=!SET! %%b
	)
	endlocal && (set %1=%SET:~1%)
exit /b

::-----------------------------------------------------------------------------
::  引数の引用符を除去する
::-----------------------------------------------------------------------------
:dequote
	set $ret=%~1
	exit /b

::-----------------------------------------------------------------------------
::  デバッグ用
::-----------------------------------------------------------------------------
:show_abspath
    echo+  %1:  %~f2
exit /b

::end RunSwig_CSharp.bat

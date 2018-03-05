@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	build_monitor [[old-revision]:[new-revision]]
::
::  DESCRIPTION
::	引数で指定した2つのレビジョンについて、svn からビルド結果と実行結果を
::	取り出してレポートを作成する. 引数によるレビジョン指定を省略したときは
::	ファイル revision.old 及び revsion.new で指定したレビジョンを使用する.
::	作成したレポートは, ファイル ./report/YYYY-MMDD.report に出力する.
::	ここで YYYY-MMDD は、新レビジョンに対応する日付である.
::  CAUTION
::	出力ファイルは Windows から見ることを前提として, 文字コードはすべて
::	ShiftJIS に変換する.
::
::  VERSION
::	Ver 1.0  2013/01/16 F.Kanehori	Unix 版より移植
::	Ver 2.0  2017/12/20 F.Kanehori	GitHub 版に改造
::	Ver 2.1  2017/12/27 F.Kanehori	レポートファイル名を変更
::	Ver 2.2  2018/01/02 F.Kanehori	NEWREV ログはテスト結果から直接得る
::	Ver 2.21 2018/01/10 F.Kanehori	レポート表記を一部変更
::	Ver 2.22 2018/02/22 F.Kanehori	レポート表記を一部変更
::	Ver 2.23 2018/03/05 F.Kanehori	Bug fixed.
:: ============================================================================
set PROG=%~n0

::----------------------------------------------
::  下位の処理プログラムに渡すオプション
::	  -V  バーバス情報出力
::	  -D  デバッグ情報出力
::  このバッチスクリプトで使用する変数の定義
::	  -k  作業ファイルを残す
::	  -m  メールを送信する（未実装）
::	  -h  使用方法の表示
::	  -v  バーバス情報出力
::	  -D  デバッグ情報出力
::----------------------------------------------
set OPT_K=0
set OPT_V=0
set OPT_W=0
set OPT_D=0
set VAL_D=0
set OPT_M=0
set ARGC=0
:next_arg
       if "%1" == "-k" ( set OPT_K=1 && shift && set /a ARGC-=1 && goto :next_arg
) else if "%1" == "-m" ( set OPT_M=1 && shift && set /a ARGC-=1 && goto :next_arg
) else if "%1" == "-h" ( call :usage && exit /b
) else if "%1" == "-v" ( set OPT_V=1 && shift && set /a ARGC-=1 && goto :next_arg
) else if "%1" == "-V" ( set OPT_W=1 && shift && set /a ARGC-=1 && goto :next_arg
) else if "%1" == "-D" ( set OPT_D=1 && set VAL_D=%2&& shift && shift && set /a ARGC-=2 && goto :next_arg
) else (
    set ARG=%1
    if "!ARG:~0,1!" == "-" (
	echo %PROG%: invalid option -- [%1]
	call :usage && exit /b
    )
)
for %%a in ( %* ) do set /a ARGC+=1
if %ARGC% gtr 1 (
    call :usage && exit /b
)
if %VAL_D% geq 1 ( echo ARGC=%ARGC% )

::----------------------------------------------
::  下位処理プログラムのオプションの定義
::
set MYDIFFOPT=
set FILTEROPT=
if %OPT_W% geq 1 ( set MYDIFFOPT=%MYDIFFOPT% -v)
if %OPT_W% geq 1 ( set FILTEROPT=%FILTEROPT% -v)
if %OPT_D% geq 1 ( set MYDIFFOPT=%MYDIFFOPT% -D %VAL_D% )
if %OPT_D% geq 1 ( set FILTEROPT=%FILTEROPT% -D %VAL_D% )
call :trim MYDIFFOPT "%MYDIFFOPT%"
call :trim FILTEROPT "%FILTEROPT%"
rem if %OPT_M% == 1 ( set REPORTBYMAIL=1 )

::----------------------------------------------
::  現在の日付と時刻
::
set DATESTR=%date:~0,4%-%date:~5,2%%date:~8,2%
set TIMESTR=%time:~0,8%

::----------------------------------------------
::  結果を報告するメールに関して
::	  REPORTBYMAIL	1 (引数 -m を指定) なら結果をメールで知らせる
::
set REPORTBYMAIL=0
set MAILSUBJECT=Daily-build Report on %DATESTR% %TIMESTR%
set MAILTOADDR=
set MAILFMADDR=

::----------------------------------------------
::  使用するディレクトリの定義
::
set BASEDIR=.
set BINDIR=%BASEDIR%\bin
set REPDIR=%BASEDIR%\..\report
set ETCDIR=%BASEDIR%\etc
set TMPDIR=%REPDIR%\tmp
if not exist %TMPDIR% ( mkdir %TMPDIR% )

::----------------------------------------------
::  使用するプログラムの定義
::
set ORDER=perl %BINDIR%\order.pl
set MYDIFF=perl %BINDIR%\mydiff.pl
set FILTER=perl %BINDIR%\filter.pl
set NKF=%BINDIR%\nkf.exe
set AWK=%BINDIR%\gawk.exe
::SENDMAIL=/usr/sbin/sendmail

set VCS=python ..\bin\VersionControlSystem.py
set GETFIELD=%AWK% -f %BINDIR%\field.awk
set GREP=%AWK% -f %BINDIR%\grep.awk
set EXCLUDE=%AWK% -f %BINDIR%\exclude.awk

::----------------------------------------------
::  レビジョン範囲の決定
::
set OLDREV=0
set NEWREV=0
if %ARGC% == 1 (
    call :backquote OLDREV "echo %1 ^| %GETFIELD% -v sep1=":" -v field=1"
    call :backquote NEWREV "echo %1 ^| %GETFIELD% -v sep1=":" -v field=2"
)
if %VAL_D% geq 1 (
    echo -- OLDREV-1 [%OLDREV%]
    echo -- NEWREV-1 [%NEWREV%]
    call :show_options
)

::----------------------------------------------
::  次のsvn(URL)を使用する
::
set STBLOGURL=core/test/log/StubBuild.log
set BLDLOGURL=core/test/log/Build.log
set RUNLOGURL=core/test/log/Run.log
set SPLLOGURL=core/test/log/SamplesBuild.log
set STBERRURL=core/test/log/StubBuildError.log
set BLDERRURL=core/test/log/BuildError.log
set RUNERRURL=core/test/log/RunError.log
set SPLERRURL=core/test/log/SamplesBuildError.log
if %OPT_V% == 1 (
    echo use following URLs:
    echo   %STBLOGURL%
    echo   %BLDLOGURL%
    echo   %RUNLOGURL%
    echo   %SPLLOGURL%
    echo   %STBERRURL%
    echo   %BLDERRURL%
    echo   %RUNERRURL%
    echo   %SPLERRURL%
)

::----------------------------------------------
::  使用するファイル名 (入力側)
::  (出力するファイル名は後程決める)
::
set OLDREVFILE=%ETCDIR%\revision.old
set NEWREVFILE=%ETCDIR%\revision.new

::----------------------------------------------
::  シグナルトラップの設定
::
rem trap "cleanup 1" SIGHUP SIGINT SIGQUIT SIGTERM

::----------------------------------------------
::  比較の基となるレビジョン(old-revision)の決定
::
if %OLDREV% == 0 (
    :: old-revision ファイルが存在すればそこから取り出す
    if exist %OLDREVFILE% (
	call :backquote OLDREV "%GETFIELD% %OLDREVFILE%"
	if "!OLDREV!" == "" ( set OLDREV=0 )
    )
    if !OLDREV! == 0 (
	echo %PROG%: Error: old-revision が指定されていません
	exit /b
    )
)

::----------------------------------------------
::  比較するレビジョン(new-revision)の決定
::
if %NEWREV% == 0 (
    :: new-revision ファイルが存在すればそこから取り出す
    if exist %NEWREVFILE% (
	call :backquote NEWREV "%GREP% -v pat=rev %NEWREVFILE%"
	if "!NEWREV!" == "HEAD" (
	    :: レビジョンの代わりに HEAD と指定されていたら最新のレビジョンを使用する
	    call :backquote NEWREV "%GREP% -v pat=head %NEWREVFILE%"
	    call :backquote NEWREV "echo !NEWREV! ^| %GETFIELD% -v field=1"
	    if "!NEWREV!" == "HEAD" (
		if %OPT_V% == 1 ( set /p=extracting HEAD info from GitHub ... < NUL )
		call :backquote NEWREV "%VCS% -g HEAD"
		call :backquote NEWREV "echo !NEWREV! ^| %GETFIELD% -v sep1=,"
		if %OPT_V% == 1 ( echo done )
	    )
	)
    )
    if !NEWREV! == 0 (
	echo %PROG%: Error: new-revision が指定されていません.
        exit /b
    )
)
if %VAL_D% geq 1 (
    echo -- OLDREV-2 [%OLDREV%]
    echo -- NEWREV-2 [%NEWREV%]
)

::----------------------------------------------
::  svn から OLDREV の日付と時刻を取得する
::
call :backquote TMPDATA "%VCS% -g %OLDREV%"
call :backquote OLDDATE "echo !TMPDATA! ^| %GETFIELD% -v sep1=, -v field=3"
call :backquote OLDTIME "echo !TMPDATA! ^| %GETFIELD% -v sep1=, -v field=4"
 
::----------------------------------------------
::  svn から NEWREV の日付と時刻を取得する
::
call :backquote TMPDATA "%VCS% -g %NEWREV%"
call :backquote NEWDATE "echo !TMPDATA! ^| %GETFIELD% -v sep1=, -v field=3"
call :backquote NEWTIME "echo !TMPDATA! ^| %GETFIELD% -v sep1=, -v field=4"
if %OPT_V% == 1 (
    echo OLD-revision: %OLDREV% ^(%OLDDATE% %OLDTIME%^)
    echo NEW-revision: %NEWREV% ^(%NEWDATE% %NEWTIME%^)
)

::----------------------------------------------
::  出力するファイル名を決める (NEWDATE を用いる)
::
::     TMPOLDxxxFILE	svn cat -r $OLDREV | order の結果
set TMPOLDSTBFILE=%TMPDIR%\r%OLDREV%.stblog.old
set TMPOLDBLDFILE=%TMPDIR%\r%OLDREV%.bldlog.old
set TMPOLDRUNFILE=%TMPDIR%\r%OLDREV%.runlog.old
set TMPOLDSPLFILE=%TMPDIR%\r%OLDREV%.spllog.old
set TMPOLDSTBERRF=%TMPDIR%\r%OLDREV%.stberrlog.old
set TMPOLDBLDERRF=%TMPDIR%\r%OLDREV%.blderrlog.old
set TMPOLDRUNERRF=%TMPDIR%\r%OLDREV%.runerrlog.old
set TMPOLDSPLERRF=%TMPDIR%\r%OLDREV%.splerrlog.old
::     TMPNEWxxxFILE	svn cat -r $NEWREV | order の結果
set TMPNEWSTBFILE=%TMPDIR%\r%NEWREV%.stblog.new
set TMPNEWBLDFILE=%TMPDIR%\r%NEWREV%.bldlog.new
set TMPNEWRUNFILE=%TMPDIR%\r%NEWREV%.runlog.new
set TMPNEWSPLFILE=%TMPDIR%\r%NEWREV%.spllog.new
set TMPNEWSTBERRF=%TMPDIR%\r%NEWREV%.stberrlog.new
set TMPNEWBLDERRF=%TMPDIR%\r%NEWREV%.blderrlog.new
set TMPNEWRUNERRF=%TMPDIR%\r%NEWREV%.runerrlog.new
set TMPNEWSPLERRF=%TMPDIR%\r%NEWREV%.splerrlog.new
::     REPROTFILE	レポートファイル
set REPORTFILE=%REPDIR%\%DATESTR%.report
::     参考情報ファイル
set STBLOGDIFFFILE=%REPDIR%\%DATESTR%.stblog.diff
set BLDLOGDIFFFILE=%REPDIR%\%DATESTR%.bldlog.diff
set RUNLOGDIFFFILE=%REPDIR%\%DATESTR%.runlog.diff
set SPLLOGDIFFFILE=%REPDIR%\%DATESTR%.spllog.diff
::     TMPFILE		作業ファイル
set TMPFILE=%TMPDIR%\build_monitor.tmp
set TMPSTBLOGDIFFFILE=%TMPDIR%\%DATESTR%.stblog.diff
set TMPBLDLOGDIFFFILE=%TMPDIR%\%DATESTR%.bldlog.diff
set TMPRUNLOGDIFFFILE=%TMPDIR%\%DATESTR%.runlog.diff
set TMPSPLLOGDIFFFILE=%TMPDIR%\%DATESTR%.spllog.diff
set TMPSTBERRDIFFFILE=%TMPDIR%\%DATESTR%.stberrlog.diff
set TMPBLDERRDIFFFILE=%TMPDIR%\%DATESTR%.blderrlog.diff
set TMPRUNERRDIFFFILE=%TMPDIR%\%DATESTR%.runerrlog.diff
set TMPSPLERRDIFFFILE=%TMPDIR%\%DATESTR%.splerrlog.diff

if %OPT_V% == 1 (
    call :abs_path REPORTFILE_ABS %REPORTFILE%
    echo output to:
    echo   !REPORTFILE_ABS!
)

::----------------------------------------------
::  OLDREV と NEWREV のログファイルを取り出す
::
if %OPT_V% == 1 ( set /p=extracting log info ... < NUL )
::  OLDREV のログは GitHub から取り出す.
git show %OLDREV%:%STBLOGURL% | %ORDER% > %TMPOLDSTBFILE%
git show %OLDREV%:%BLDLOGURL% | %ORDER% > %TMPOLDBLDFILE%
git show %OLDREV%:%RUNLOGURL% | %ORDER% > %TMPOLDRUNFILE%
git show %OLDREV%:%SPLLOGURL% | %ORDER% > %TMPOLDSPLFILE%
git show %OLDREV%:%STBERRURL% | %ORDER% > %TMPOLDSTBERRF%
git show %OLDREV%:%BLDERRURL% | %ORDER% > %TMPOLDBLDERRF%
git show %OLDREV%:%RUNERRURL% | %ORDER% > %TMPOLDRUNERRF%
git show %OLDREV%:%SPLERRURL% | %ORDER% > %TMPOLDSPLERRF%

::  NEWREV のログは DailyBuild の結果から直接取り出す.
set TOPDIR=..\..\..
rem git show %NEWREV%:%STBLOGURL% | %ORDER% > %TMPNEWSTBFILE%
rem git show %NEWREV%:%BLDLOGURL% | %ORDER% > %TMPNEWBLDFILE%
rem git show %NEWREV%:%RUNLOGURL% | %ORDER% > %TMPNEWRUNFILE%
rem git show %NEWREV%:%SPLLOGURL% | %ORDER% > %TMPNEWSPLFILE%
rem git show %NEWREV%:%STBERRURL% | %ORDER% > %TMPNEWSTBERRF%
rem git show %NEWREV%:%BLDERRURL% | %ORDER% > %TMPNEWBLDERRF%
rem git show %NEWREV%:%RUNERRURL% | %ORDER% > %TMPNEWRUNERRF%
rem git show %NEWREV%:%SPLERRURL% | %ORDER% > %TMPNEWSPLERRF%
type %TOPDIR%\%STBLOGURL:/=\% | %ORDER% > %TMPNEWSTBFILE%
type %TOPDIR%\%BLDLOGURL:/=\% | %ORDER% > %TMPNEWBLDFILE%
type %TOPDIR%\%RUNLOGURL:/=\% | %ORDER% > %TMPNEWRUNFILE%
type %TOPDIR%\%SPLLOGURL:/=\% | %ORDER% > %TMPNEWSPLFILE%
type %TOPDIR%\%STBERRURL:/=\% | %ORDER% > %TMPNEWSTBERRF%
type %TOPDIR%\%BLDERRURL:/=\% | %ORDER% > %TMPNEWBLDERRF%
type %TOPDIR%\%RUNERRURL:/=\% | %ORDER% > %TMPNEWRUNERRF%
type %TOPDIR%\%SPLERRURL:/=\% | %ORDER% > %TMPNEWSPLERRF%
if %OPT_V% == 1 ( echo done )

::----------------------------------------------
::  差分をとる
::
if %OPT_V% == 1 ( set /p=taking diff ... < NUL )
set MYDIFFOPT0=%MYDIFFOPT% -p 0
set MYDIFFOPT1=%MYDIFFOPT% -p 1
%MYDIFF% %MYDIFFOPT1% %TMPOLDSTBFILE% %TMPNEWSTBFILE% | %FILTER% %FILTEROPT% > %TMPSTBLOGDIFFFILE%
%MYDIFF% %MYDIFFOPT0% %TMPOLDBLDFILE% %TMPNEWBLDFILE% | %FILTER% %FILTEROPT% > %TMPBLDLOGDIFFFILE%
%MYDIFF% %MYDIFFOPT0% %TMPOLDRUNFILE% %TMPNEWRUNFILE% | %FILTER% %FILTEROPT% > %TMPRUNLOGDIFFFILE%
%MYDIFF% %MYDIFFOPT0% %TMPOLDSPLFILE% %TMPNEWSPLFILE% | %FILTER% %FILTEROPT% > %TMPSPLLOGDIFFFILE%
%MYDIFF% %MYDIFFOPT1% %TMPOLDSTBERRF% %TMPNEWSTBERRF% | %FILTER% %FILTEROPT% > %TMPSTBERRDIFFFILE%
%MYDIFF% %MYDIFFOPT0% %TMPOLDBLDERRF% %TMPNEWBLDERRF% | %FILTER% %FILTEROPT% > %TMPBLDERRDIFFFILE%
%MYDIFF% %MYDIFFOPT0% %TMPOLDRUNERRF% %TMPNEWRUNERRF% | %FILTER% %FILTEROPT% > %TMPRUNERRDIFFFILE%
%MYDIFF% %MYDIFFOPT0% %TMPOLDSPLERRF% %TMPNEWSPLERRF% | %FILTER% %FILTEROPT% > %TMPSPLERRDIFFFILE%
if %OPT_V% == 1 ( echo done )

::----------------------------------------------
::  参考情報を作成する(utf8)
::
if %OPT_V% == 1 ( set /p=converting to utf8 ... < NUL )
%NKF% -w8 %TMPSTBLOGDIFFFILE%	> %STBLOGDIFFFILE%
%NKF% -w8 %TMPBLDLOGDIFFFILE%	> %BLDLOGDIFFFILE%
%NKF% -w8 %TMPRUNLOGDIFFFILE% 	> %RUNLOGDIFFFILE%
%NKF% -w8 %TMPSPLLOGDIFFFILE%	> %SPLLOGDIFFFILE%
if %OPT_V% == 1 ( echo done )

::----------------------------------------------
::  レポートファイルにまとめる
::     ただし日付行は除く(pattern is /日付 : [0-9]{2}\.[0-9]{2}\.[0-9]{2}/)
::
if %OPT_V% == 1 ( set /p=making report file ... < NUL )
echo Report on %DATESTR% %TIMESTR%			>  %REPORTFILE%
echo old log revision: %OLDREV% (%OLDDATE% %OLDTIME%)	>> %REPORTFILE%
echo new log revision: %NEWREV% (%NEWDATE% %NEWTIME%)	>> %REPORTFILE%

echo.					>> %REPORTFILE%
echo.====================		>> %REPORTFILE%
echo. StubBuildError.log  		>> %REPORTFILE%
echo.====================		>> %REPORTFILE%
%EXCLUDE% %TMPNEWSTBERRF%		>> %REPORTFILE%

echo.					>> %REPORTFILE%
echo.====================		>> %REPORTFILE%
echo. BuildError.log			>> %REPORTFILE%
echo.====================		>> %REPORTFILE%
%EXCLUDE% %TMPNEWBLDERRF%		>> %REPORTFILE%

echo.					>> %REPORTFILE%
echo.====================		>> %REPORTFILE%
echo. Run.log の差分			>> %REPORTFILE%
echo.====================		>> %REPORTFILE%
%EXCLUDE% %TMPRUNLOGDIFFFILE%		>> %REPORTFILE%

echo.					>> %REPORTFILE%
echo.====================		>> %REPORTFILE%
echo. SamplesBuildError.log		>> %REPORTFILE%
echo.====================		>> %REPORTFILE%
%EXCLUDE% %TMPNEWSPLERRF%		>> %REPORTFILE%

::----------------------------------------------
::  エンコーディングを utf8 にする
::
%NKF% -w8 %REPORTFILE% > %TMPFILE%
copy %TMPFILE% %REPORTFILE%	> NUL
del %TMPFILE% 			> NUL
if %OPT_V% == 1 ( echo done )

::----------------------------------------------
::  結果をメールする
::
if %REPORTBYMAIL% == 1 (
    if %OPT_V% == 1 ( set /p=mailing report file to ... < NUL )
    rem echo From: %MAILFMADDR%		>  %TMPFILE%
    rem echo To: %MAILTOADDR%		>> %TMPFILE%
    rem echo Subject: %MAILSUBJECT%	>> %TMPFILE%
    rem echo 				>> %TMPFILE%
    rem %NKF% -j %REPORTFILE%		>> %TMPFILE%
    rem %SENDMAIL% -t			<  %TMPFILE%
    if %OPT_V% == 1 ( echo done )
)

::----------------------------------------------
::  後始末をして終了
::
call :cleanup
exit /b


:: ==============================================
::  このスクリプトを終了する前に必ず呼び出すこと
:: ==============================================
:cleanup
    if not %OPT_K% == 1 (
	if %OPT_V% == 1 ( set /p=clean up ... < NUL )
	rem del %STBLOGDIFFFILE%
	rem del %BLDLOGDIFFFILE%
	rem del %RUNLOGDIFFFILE%
	rem del %SPLLOGDIFFFILE%
	set DELLIST=
	for %%f in (%TMPDIR%^\*) do ( set DELLIST=!DELLIST! %%f )
	if not "!DELLIST!" == "" ( del !DELLIST! )
	if %OPT_V% == 1 ( echo done )
    )
exit /b

:: ===========================================
::  コマンドの実行結果を変数に設定する
::    call :backquote 変数名 "コマンド"
:: ===========================================
:backquote
    rem echo for /f %%i in ('%2') do set %1=%%i
    for /f %%i in ('%2') do set %1=%%i
exit /b

:: ===========================================
::  前後の空白を取り除く
::    call :trim 変数名 文字列
:: ===========================================
:trim
    set TRIM_ARG=%2
    set TRIM_ARG=%TRIM_ARG:"=%
    if "%TRIM_ARG%" == "" (
	exit /b
    )
    :trim_again_1
    if "%TRIM_ARG:~0,1%" == " " (
	set TRIM_ARG=%TRIM_ARG:~1%
	goto :trim_again_1
    )
    :trim_again_2
    if "%TRIM_ARG:~-1%" == " " (
	set TRIM_ARG=%TRIM_ARG:~0,-1%
	goto :trim_again_2
    )
    set %1=%TRIM_ARG%
exit /b

:: ====================
::  絶対パスを返す
:: ====================
:abs_path
	set %1=%~f2
exit /b

:: ====================
::  使用方法を表示する
:: ====================
:usage
    echo Usage: %PROG% [options] [xxxx:yyyy]
    echo 	xxxx: old-revision-number
    echo 	yyyy: new-revision-number
    echo 	options:
    echo 	  -k:	keep temporary files
    rem echo 	  -m:	mail report to
    echo 	  -h:	show usage
    echo 	  -v:	set verbose mode (default)
    echo 	  -s:	set silent mode
    echo 	  -D n:	set debug level to 'n'
exit /b

:: ======================
::  オプションを表示する
:: ======================
:show_options
    echo options:
    if %OPT_K% == 1 echo.  -k
    if %OPT_M% == 1 echo.  -m
    if %OPT_V% == 1 echo.  -v
    if not %OPT_D% == 0 echo.  -D %VAL_D%
    echo.  MYDIFFOPT [%MYDIFFOPT%]
    echo.  FILTEROPT [%FILTEROPT%]
exit /b

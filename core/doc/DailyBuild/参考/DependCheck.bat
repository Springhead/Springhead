@echo off
rem ========================================================================
rem  SYNOPSIS
rem	DependCheck [/h] [/k]
rem
rem  RELEASE
rem	    V1.0    2014/06/26	F.Kanehori
rem	    V1.1    2016/08/10	F.Kanehori	Check version の整理
rem ========================================================================
setlocal enabledelayedexpansion

if "%1" equ "/h" (call :usage %0 && exit /b)
set opt_k=
if "%1" equ "/k" (set opt_k=1 && shift)

set CWD=%CD%
set BINDIR=C:\bin
set GREP=%BINDIR%\grep.bat
set DIFF=%BINDIR%\diff.exe
set EXTRACT=extract.pl
set PATr="RelativePath"
set PATb="CustomBuild Include"
set PATn="None Include"
set PATi="ClInclude Include"
set PATc="ClCompile Include"
set PATt="Text Include"

rem set TOOLSET=09 10 11 12
set TOOLSET=10 12 14
for %%t in (%TOOLSET%) do (
    set TMP%%t_1=%CWD%\_tmp%%t_1.txt
    set TMP%%t_2=%CWD%\_tmp%%t_2.txt
)
set OUTTMP=_tmp_out.txt

call :get_proj_name %CWD%
if not %RC% == 0 exit /b

rem 依存ファイル名を抽出する
rem
rem call %GREP% %PATr% %PROJ09% >  %TMP09_1%

call %GREP% %PATi% %PROJ10% >  %TMP10_1%
call %GREP% %PATc% %PROJ10% >> %TMP10_1%
call %GREP% %PATb% %PROJ10% >> %TMP10_1%
call %GREP% %PATn% %PROJ10% >> %TMP10_1%

rem call %GREP% %PATi% %PROJ11% >  %TMP11_1%
rem call %GREP% %PATc% %PROJ11% >> %TMP11_1%
rem call %GREP% %PATn% %PROJ11% >> %TMP11_1%
rem call %GREP% %PATt% %PROJ11% >> %TMP11_1%

call %GREP% %PATi% %PROJ12% >  %TMP12_1%
call %GREP% %PATc% %PROJ12% >> %TMP12_1%
call %GREP% %PATn% %PROJ12% >> %TMP12_1%
call %GREP% %PATt% %PROJ12% >> %TMP12_1%

call %GREP% %PATi% %PROJ14% >  %TMP14_1%
call %GREP% %PATc% %PROJ14% >> %TMP14_1%
call %GREP% %PATn% %PROJ14% >> %TMP14_1%
call %GREP% %PATt% %PROJ14% >> %TMP14_1%

rem call %EXTRACT% RelativePath %TMP09_1% | sort > %TMP09_2%
call %EXTRACT% Include	    %TMP10_1% | sort > %TMP10_2%
rem call %EXTRACT% Include	    %TMP11_1% | sort > %TMP11_2%
call %EXTRACT% Include	    %TMP12_1% | sort > %TMP12_2%
call %EXTRACT% Include	    %TMP14_1% | sort > %TMP14_2%

rem 差分を表示する
rem
rem call :diff_out 10 09
rem call :diff_out 10 11
call :diff_out 10 12
call :diff_out 12 14

rem 後始末
rem
:done
if not defined opt_k (
    for %%t in (%TOOLSET%) do (
	del _tmp%%t_1.txt
	del _tmp%%t_2.txt
    )
    del %OUTTMP%
)
endlocal
exit /b

rem ============================
rem  プロジェクトファイル名取得
rem ============================
:get_proj_name
    set DIRNAME=%~n1
    set PJ09=%DIRNAME%9.vcproj
    set PJ10=%DIRNAME%10.vcxproj
    set PJ11=%DIRNAME%11.0.vcxproj
    set PJ12=%DIRNAME%12.0.vcxproj
    set PJ14=%DIRNAME%14.0.vcxproj
    set PROJ09=%1\%PJ09%
    set PROJ10=%1\%PJ10%
    set PROJ11=%1\%PJ11%
    set PROJ12=%1\%PJ12%
    set PROJ14=%1\%PJ14%
    set RC=0
    rem if not exist %PROJ09% (echo %PROJ09% not exist! && set RC=-1)
    if not exist %PROJ10% (echo %PROJ10% not exist! && set RC=-1)
    rem if not exist %PROJ11% (echo %PROJ11% not exist! && set RC=-1)
    if not exist %PROJ12% (echo %PROJ12% not exist! && set RC=-1)
    if not exist %PROJ14% (echo %PROJ14% not exist! && set RC=-1)
exit /b

rem ============================
rem  差分出力
rem ============================
:diff_out
    call set FILE1=%%TMP%1_2%%
    call set FILE2=%%TMP%2_2%%
    if exist %OUTTMP% del %OUTTMP%
    %DIFF% %FILE1% %FILE2% > %OUTTMP%
    for /F "usebackq" %%s in ('%OUTTMP%') do set SIZE=%%~zs
    if %SIZE% gtr 0 (
	echo *** %PJ%%1
	echo --- %PJ%%2
	type %OUTTMP%
    )
exit /b

rem ============================
rem  show usage
rem ============================
:usage
    echo %~n1: [/k] [/h]
    echo    /k	keep temporary result file
    echo    /h	show this
exit /b


@echo off
setlocal enabledelayedexpansion

set EPStoSVG=0
set VERBOSE=1

if "%1" equ "" (
    echo %0: argument required
    echo Usage: %0 { pdf ^| html }
    exit /b
)
set ARGS=%2 %3 %4 %5 %6 %7 %8 %9

rem set ADDPATH=.
rem if "%1" equ "pdf"  (set ADDPATH=F:\C-Drive-Save\texlive\2014\bin\win32)
rem if "%1" equ "pdf"  (set ADDPATH=C:\texlive\2014\bin\win32)
rem if "%1" equ "html" (set ADDPATH=C:\texlive\2018\bin\win32)
rem set path=%ADDPATH%;%path%

if "%1" equ "pdf" (
    cmd /c make clean
    cmd /c make.bat

) else if "%1" equ "html" (
    set CWD=%CD%
    if "!CWD:~-13!" equ "HowToUseCMake" (set OPT=)
    if "!CWD:~-3!" equ "tmp" (set OPT=-S)

    if "!OPT!" equ "-S" (cd ..)

    set ARGS=-E -H -K -R !OPT! %ARGS%
    if %EPStoSVG% neq 1	set ARGS=!ARGS! -t
    if %VERBOSE% equ 1	set ARGS=!ARGS! -v

    echo test start at "%CD%"
    python buildhtml.py !ARGS! main_html.tex

    if "!OPT!" equ "-S" (cd tmp)
    echo test end at "%CD%"

) else (
    echo %0: invalid argument
    echo Usage: %0 { pdf ^| html }
)

endlocal
exit /b

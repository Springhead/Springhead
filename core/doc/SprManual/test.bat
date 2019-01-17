@echo off
setlocal enabledelayedexpansion

if "%1" equ "" (
    echo %0: argument required
    echo Usage: %0 { pdf ^| html }
    exit /b
)
set ARGS=%2 %3 %4 %5 %6 %7 %8 %9

set ADDPATH=.
if "%1" equ "pdf"  (set ADDPATH=F:\C-Drive-Save\texlive\2014\bin\win32)
if "%1" equ "html" (set ADDPATH=C:\texlive\2018\bin\win32)
set path=%ADDPATH%;%path%

if "%1" equ "pdf" (
    cmd /c make clean
    cmd /c make.bat

) else if "%1" equ "html" (
    set CWD=%CD%
    if "!CWD:~-9!" equ "SprManual" (set OPT=)
    if "!CWD:~-3!" equ "tmp" (set OPT=-S)

    if "!OPT!" equ "-S" (cd ..)

    echo test start at "%CD%"
    python buildhtml.py -v -E -K -R -c -t !OPT! %ARGS% main_html.tex

    if "!OPT!" equ "-S" (cd tmp)
    echo test end at "%CD%"

) else (
    echo %0: invalid argument
    echo Usage: %0 { pdf ^| html }
)

endlocal
exit /b

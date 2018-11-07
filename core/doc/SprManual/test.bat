@echo off
setlocal enabledelayedexpansion

set CWD=%CD%
if "%CWD:~-9%" equ "SprManual" (set OPT=)
if "%CWD:~-3%" equ "tmp" (set OPT=-S)

if "%OPT%" equ "-S" (cd ..)

echo test start at "%CD%"
python buildhtml.py -v -K -t %OPT% %* main_html.tex

if "%OPT%" equ "-S" (cd tmp)
echo test end at "%CD%"

endlocal
exit /b

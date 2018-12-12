@echo off
setlocal enabledelayedexpansion

set PYTHON=..\..\buildtool\win32\python.exe
set SCRIPT=SlnConv.py

%PYTHON% %SCRIPT% -s 14.0 -d 15.0 -f -v %1

endlocal
exit /b

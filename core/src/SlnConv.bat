@echo off
setlocal enabledelayedexpansion

call :get_script_dir DIR %0

set PYTHON=%DIR%..\..\buildtool\win32\python.exe
set SCRIPT=SlnConv.py

%PYTHON% %DIR%%SCRIPT% -s 14.0 -d 15.0 -u -f -v %*

endlocal
exit /b

:get_script_dir
	set %1=%~p2
exit /b

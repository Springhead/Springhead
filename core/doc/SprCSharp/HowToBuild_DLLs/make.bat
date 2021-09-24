@echo off
setlocal enabledelayedexpansion

set CURRDATE=%date:~0,4%-%date:~-5,2%%date:~8,2%
set CURRTIMETMP=%time: =0%
set CURRTIME=%CURRTIMETMP:~0,2%%CURRTIMETMP:~3,2%

nmake DATE=%CURRDATE% TIME=%CURRTIME% %*

endlocal
exit /b

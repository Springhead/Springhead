if "%SPRINGHEAD%"=="" if exist C:\home\%USERNAME%\project\Springhead set SPRINGHEAD=C:\home\%USERNAME%\project\Springhead
if "%SPRINGHEAD%"=="" if exist C:\users\%USERNAME%\project\Springhead set SPRINGHEAD=C:\users\%USERNAME%\project\Springhead
set PATHOLD=%PATH%
set PATH=%SPRINGHEAD%\core\bin\win64;%SPRINGHEAD%\dependency\bin\win64;
echo %PATH%
14.0\x64\Release\BoxStack.exe
set PATH=%PATHOLD%

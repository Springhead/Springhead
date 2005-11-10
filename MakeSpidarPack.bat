if "%1"=="SUB" goto SUB
set PATH=\\192.168.251.1\VSS\win32;%PATH%
set SSDIR=\\192.168.251.1\VSS
if exist SpidarPack del /F /S /Q SpidarPack
if not exist SpidarPack mkdir SpidarPack
if not exist SpidarPack\Springhead mkdir SpidarPack\Springhead
if not exist SpidarPack\Springhead\src mkdir SpidarPack\Springhead\src
cd SpidarPack\Springhead\src
call %0 SUB Base
call %0 SUB WinBasis
call %0 SUB Device
call %0 SUB HIS
cd ..\..
minitar -c -z9 ../SprSpidar.tar.gz *.*
cd ..
goto END

:SUB
if not exist %2 mkdir %2
cd %2
rem ss Get $/Project/Springhead\src\%2 -R -W -I-
svn export svn+ssh://sprsvn/export/spr/svn/repository/Springhead/trunk/src/%2
cd ..

:END

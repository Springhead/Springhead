@echo off
set MAKEFILE=EmbPython.mak.txt
set PATHORG=%PATH%;

set PATH=%PATH%;..\..\bin
set MODULES=Base Foundation Collision Physics FileIO EmbPython Framework Graphics GraphicsD3D HumanInterface
set PLACES=include src



set PATH=%PATH%;..\..\bin;..\..\bin\swig

set SRCINTF=
for %%p in (%TARGET%) do for %%f in (../../include/%%p/*.h) do set SRCINTF=!SRCINTF! ../../include/%%p/%%f
set SRCIMP=
for %%p in (%TARGET%) do for %%f in (../%%p/*.h) do set SRCIMP=!SRCIMP! ../../src/%%p/%%f

echo #	Do not edit. RunSwig.bat will update this file.> %MAKEFILE%
echo all: ..\..\src\EmbPython\EPBase.cpp>>%MAKEFILE%
echo ..\..\src\EmbPython\EPBase.cpp: %SRCINTF% %SRCIMP%>>%MAKEFILE%
echo 	call .\EmbPythonSwig.bat Base >>%MAKEFILE%
echo 	call .\EmbPythonSwig.bat Foundation >>%MAKEFILE%
echo 	call .\EmbPythonSwig.bat FileIO Foundation >>%MAKEFILE%
echo 	call .\EmbPythonSwig.bat Collision Foundation >>%MAKEFILE%
echo 	call .\EmbPythonSwig.bat Physics Collision Foundation >>%MAKEFILE%
echo 	call .\EmbPythonSwig.bat Graphics Physics Foundation >>%MAKEFILE%
echo 	call .\EmbPythonSwig.bat Creature Graphics Physics Foundation >>%MAKEFILE%
echo 	call .\EmbPythonSwig.bat HumanInterface Graphics Foundation >>%MAKEFILE%
echo 	call .\EmbPythonSwig.bat Framework FileIO HumanInterface Graphics Physics Foundation >>%MAKEFILE%

make -f%MAKEFILE%

set PATH=%PATHORG%
set PATHORG=

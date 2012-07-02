@echo off
set MAKEFILE=EmbPython.mak.txt
set PATHORG=%PATH%;

set PATH=%PATH%;..\..\bin
set MODULES=Base Foundation Collision Physics FileIO EmbPython Framework Graphics GraphicsD3D HumanInterface
set PLACES=include src



rem Create swigtemp folders.
if not exist ..\..\swigtemp mkdir ..\..\swigtemp
for %%P in (%PLACES%) do if not exist ..\..\swigtemp\%%P mkdir ..\..\swigtemp\%%P
for %%P in (%PLACES%) do for %%M in (%MODULES%) do if not exist ..\..\swigtemp\%%P\%%M mkdir ..\..\swigtemp\%%P\%%M

rem Convert
for %%P in (%PLACES%) do for %%M in (%MODULES%) do for %%F in (..\..\%%P\%%M\*.h) do for %%G in (..\..\..\swigtemp\%%P\%%M\%%~nxF) do if "%%~tF" GTR "%%~tG" nkf -s %%F >..\..\swigtemp\%%P\%%M\%%~nxF

if not exist ..\..\swigtemp\src\EmbPython\Utility mkdir ..\..\swigtemp\src\EmbPython\Utility
for %%f in (..\EmbPython\*.bat) do copy ..\EmbPython\%%f ..\..\swigtemp\src\EmbPython\%%f
for %%f in (..\EmbPython\*.i) do copy ..\EmbPython\%%f ..\..\swigtemp\src\EmbPython\%%f
for %%f in (..\EmbPython\Utility\*.cpp) do nkf -s < %%f > ..\..\swigtemp\src\EmbPython\%%f
for %%f in (..\EmbPython\Utility\*.i)   do nkf -s < %%f > ..\..\swigtemp\src\EmbPython\%%f


cd ..\..\swigtemp\src\EmbPython
set PATH=%PATH%;..\..\..\bin;..\..\..\bin\swig

set SRCINTF=
for %%p in (%TARGET%) do for %%f in (../../include/%%p/*.h) do set SRCINTF=!SRCINTF! ../../../include/%%p/%%f
set SRCIMP=
for %%p in (%TARGET%) do for %%f in (../%%p/*.h) do set SRCIMP=!SRCIMP! ../../../src/%%p/%%f

echo #	Do not edit. RunSwig.bat will update this file.> %MAKEFILE%
echo all: ..\..\..\src\EmbPython\EPBase.cpp>>%MAKEFILE%
echo ..\..\..\src\EmbPython\EPBase.cpp: %SRCINTF% %SRCIMP%>>%MAKEFILE%
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

cd ..\..\..\src\EmbPython
set PATH=%PATHORG%
set PATHORG=

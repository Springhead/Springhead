set MAKEFILE=EmbPython.mak.txt
set PATHORG=%PATH%;


set PATH=%PATH%;..\..\bin
if not exist ..\..\swigtemp mkdir ..\..\swigtemp
if not exist ..\..\swigtemp\src mkdir ..\..\swigtemp\src
if not exist ..\..\swigtemp\include mkdir ..\..\swigtemp\include
if not exist ..\..\swigtemp\src\Foundation mkdir ..\..\swigtemp\src\Foundation
if not exist ..\..\swigtemp\include\Base mkdir ..\..\swigtemp\include\Base
if not exist ..\..\swigtemp\src\EmbPython mkdir ..\..\swigtemp\src\EmbPython
if not exist ..\..\swigtemp\src\EmbPython\Utility mkdir ..\..\swigtemp\src\EmbPython\Utility
if not exist ..\..\swigtemp\include\EmbPython mkdir ..\..\swigtemp\include\EmbPython

for %%f in (..\..\include\EmbPython\*.h) do nkf -s < ..\..\include\EmbPython\%%f > ..\..\swigtemp\include\EmbPython\%%f
for %%f in (..\EmbPython\*.h) do nkf -s < ..\EmbPython\%%f > ..\..\swigtemp\src\EmbPython\%%f
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
echo all: EPBase.cpp>>%MAKEFILE%
echo EPBase.cpp: %SRCINTF% %SRCIMP%>>%MAKEFILE%
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

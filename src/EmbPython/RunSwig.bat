set MAKEFILE=EmbPython.mak.txt
set PATHORG=%PATH%;
set PATH=%PATH%;..\..\bin;..\..\bin\swig

echo #	Do not edit. RunSwig.bat will update this file.> %MAKEFILE%
echo all: %MODULE%Stub.cpp>>%MAKEFILE%
echo %MODULE%Stub.cpp: %SRCINTF% %SRCIMP%>>%MAKEFILE%
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

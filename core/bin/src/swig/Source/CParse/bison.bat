@echo off
set PATHORG=%PATH%
set PATH=..\..\..\..\..\bin
set BISON_SIMPLE=..\..\..\..\..\bin\bison.simple
set MAKEFILE=bison.mak
set INPUTNAME=parser

echo #	Do not edit. bison.bat will update this file.> %MAKEFILE%
echo all: %INPUTNAME%.c>>%MAKEFILE%
echo %INPUTNAME%.c: %INPUTNAME%.y>>%MAKEFILE%
echo 	bison -d %INPUTNAME%.y >>%MAKEFILE%
echo 	del %INPUTNAME%.h >>%MAKEFILE%
echo 	del %INPUTNAME%.c >>%MAKEFILE%
echo 	rename %INPUTNAME%.tab.c %INPUTNAME%.c >>%MAKEFILE%
echo 	rename %INPUTNAME%.tab.h %INPUTNAME%.h >>%MAKEFILE%

make -f%MAKEFILE%
set PATH=%PATHORG%
set PATHORG=

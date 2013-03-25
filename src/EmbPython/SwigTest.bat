@echo off
del EPBase.cpp
set MAKEFILE=EmbPython.mak.txt
set PATHORG=%PATH%;

set PATH=%PATH%;..\..\bin

make -f%MAKEFILE%

set PATH=%PATHORG%
set PATHORG=
pause

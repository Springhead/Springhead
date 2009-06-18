@echo off
echo "swig -dll" generating entry point for Scilab.
set PATHORG=%PATH%;
set PATH=%SPRINGHEAD2%\bin\swig;%SPRINGHEAD2%\bin

set MODULE=Scilab
set MAKEFILE=%MODULE%Stub.mak.txt
echo #	Do not edit. ScilabSwig.bat will update this file.> %MAKEFILE%
echo all: %MODULE%Stub.cpp>>%MAKEFILE%
echo %MODULE%Stub.cpp: Scilab.i>>%MAKEFILE%
echo 	swig -dll -c++ Scilab.i>>%MAKEFILE%
echo 	copy ScilabStub.hpp ..\..\include\Scilab>>%MAKEFILE%
echo 	del ScilabStub.hpp>>%MAKEFILE%
make -f%MAKEFILE%

set PATH=%PATHORG%
set PATHORG=

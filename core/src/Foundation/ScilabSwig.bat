@echo off
echo "swig -dll" generating entry point for Scilab.
set PATHORG=%PATH%;
set PATH=..\..\bin\swig;..\..\bin

set MODULE=Scilab
set MAKEFILE=%MODULE%Stub.mak.txt
echo #	Do not edit. ScilabSwig.bat will update this file.> %MAKEFILE%
echo all: ..\..\include\Scilab\ScilabStub.hpp>>%MAKEFILE%
echo ..\..\include\Scilab\ScilabStub.hpp: Scilab.i>>%MAKEFILE%
echo 	swig -dll -c++ Scilab.i>>%MAKEFILE%
echo 	copy ScilabStub.hpp ..\..\include\Scilab>>%MAKEFILE%
echo 	del ScilabStub.hpp>>%MAKEFILE%
make -f%MAKEFILE%

set PATH=%PATHORG%
set PATHORG=

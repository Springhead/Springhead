@echo off
echo "swig -dll" generating entry point for Scilab.
set PATHORG=%PATH%;
set PATH=%SPRINGHEAD2%\bin\swig

cd
swig -dll -c++ Scilab.i
move ScilabStub.hpp ..\..\include\Scilab

set PATH=%PATHORG%
set PATHORG=

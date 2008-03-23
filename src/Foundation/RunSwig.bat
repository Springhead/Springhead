@echo off

if "%1"=="LISTSRC" goto ListSrc
cmd /v:on /c%0 LISTSRC %1 %2 %3 %4 %5 %6 %7 %8 %9
goto end

:ListSrc
set TARGET=%2 %3 %4 %5 %6 %7 %8 %9
set MODULE=%2
set MAKEFILE=%MODULE%.mak
rem makefile‚Ìì¬‚ÆMake‚ÌŽÀs
set PATHORG=%PATH%;
set PATH=%SPRINGHEAD2%\bin
set SRC=
for %%p in (%TARGET%) do for %%f in (../../include/%%p/*.h) do set SRC=!SRC! ../../include/%%p/%%f
echo source files:%SRC%

echo #	Do not edit. RunSwig.bat will update this file.> %MODULE%.i
echo %%module %MODULE%>> %MODULE%.i
echo %%include "../../include/base/Env.h">> %MODULE%.i
for %%p in (%SRC%) do echo %%include "%%p">> %MODULE%.i

echo #	Do not edit. RunSwig.bat will update this file.> %MAKEFILE%
echo all: %MODULE%>>%MAKEFILE%
echo %MODULE%: %SRC%>>%MAKEFILE%
echo 	swig\swig -refl -c++ %MODULE%.i>>%MAKEFILE%
make -f%MAKEFILE%
set PATH=%PATHORG%
set PATHORG=
:end

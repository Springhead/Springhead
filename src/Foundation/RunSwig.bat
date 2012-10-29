@echo off

if "%1"=="LISTSRC" goto ListSrc
cmd /v:on /c%0 LISTSRC %1 %2 %3 %4 %5 %6 %7 %8 %9
goto end

:ListSrc
set TARGET=%9 %8 %7 %6 %5 %4 %3 %2
set MODULE=%2
set MAKEFILE=%MODULE%Stub.mak.txt
rem makefile‚Ìì¬‚ÆMake‚ÌŽÀs
set PATHORG=%PATH%;


rem Create SJIS World
set PATH=..\..\bin;..\..\bin\swig

set AUXDEP=../../include/Springhead.h ../../include/base/Env.h ../../include/Base/BaseDebug.h ../../src/Foundation/UTTypeDesc.h

set SRCINTF=
for %%p in (%TARGET%) do for %%f in (../../include/%%p/*.h) do set SRCINTF=!SRCINTF! ../../include/%%p/%%f
set SRCIMP=
for %%p in (%TARGET%) do for %%f in (../%%p/*.h) do set SRCIMP=!SRCIMP! ../%%p/%%f

set SRCINTFDEP=
for %%p in (%TARGET%) do for %%f in (../../include/%%p/*.h) do set SRCINTFDEP=!SRCINTFDEP! ../../include/%%p/%%f
set SRCIMPDEP=
for %%p in (%TARGET%) do for %%f in (../%%p/*.h) do set SRCIMPDEP=!SRCIMPDEP! ../../src/%%p/%%f

echo #	Do not edit. RunSwig.bat will update this file.> %MODULE%.i
echo %%module %MODULE%>> %MODULE%.i
echo #define DOUBLECOLON :: >> %MODULE%.i
echo %%include "../../include/Springhead.h">> %MODULE%.i
echo %%include "../../include/base/Env.h">> %MODULE%.i
for %%p in (%SRCINTF%) do echo %%include "%%p">> %MODULE%.i
echo %%include "../../include/Base/BaseDebug.h">> %MODULE%.i
echo %%include "../../src/Foundation/UTTypeDesc.h">> %MODULE%.i
for %%p in (%SRCIMP%) do echo %%include "%%p">> %MODULE%.i

echo #	Do not edit. RunSwig.bat will update this file.> %MAKEFILE%
echo all: ..\..\src\%MODULE%\%MODULE%Stub.cpp>>%MAKEFILE%
echo ..\..\src\%MODULE%\%MODULE%Stub.cpp: %AUXDEP% %SRCINTFDEP% %SRCIMPDEP%>>%MAKEFILE%
echo 	swig -spr -w305,312,319,325,401,402 -DSWIG_%MODULE% -c++ %MODULE%.i>>%MAKEFILE%
make -f%MAKEFILE%
set PATH=%PATHORG%
set PATHORG=
:end

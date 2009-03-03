@echo off

if "%1"=="LISTSRC" goto ListSrc
cmd /v:on /c%0 LISTSRC %1 %2 %3 %4 %5 %6 %7 %8 %9
goto end

:ListSrc
if exist AccessDesc*.h del AccessDesc*.h
if exist TypeDesc*.h del TypeDesc*.h
if exist MakeTypeDesc*.txt del MakeTypeDesc*.txt
set TARGET=%9 %8 %7 %6 %5 %4 %3 %2
set MODULE=%2
set MAKEFILE=%MODULE%Stub.mak.txt
rem makefile‚Ìì¬‚ÆMake‚ÌŽÀs
set PATHORG=%PATH%;
set PATH=%SPRINGHEAD2%\bin;%SPRINGHEAD2%\bin\swig
set SRCINTF=
for %%p in (%TARGET%) do for %%f in (../../include/%%p/*.h) do set SRCINTF=!SRCINTF! ../../include/%%p/%%f
set SRCIMP=
for %%p in (%TARGET%) do for %%f in (../%%p/*.h) do set SRCIMP=!SRCIMP! ../%%p/%%f

rem echo interface files:%SRCINTF%
rem echo src files:%SRCIMP%

echo #	Do not edit. RunSwig.bat will update this file.> %MODULE%.i
echo %%module %MODULE%>> %MODULE%.i
echo %%include "../../include/Springhead.h">> %MODULE%.i
echo %%include "../../include/base/Env.h">> %MODULE%.i
for %%p in (%SRCINTF%) do echo %%include "%%p">> %MODULE%.i
echo %%include "../../src/Base/BaseDebug.h">> %MODULE%.i
echo %%include "../../src/Foundation/UTTypeDesc.h">> %MODULE%.i
for %%p in (%SRCIMP%) do echo %%include "%%p">> %MODULE%.i

echo #	Do not edit. RunSwig.bat will update this file.> %MAKEFILE%
echo all: %MODULE%Stub.cpp>>%MAKEFILE%
echo %MODULE%Stub.cpp: %SRCINTF% %SRCIMP%>>%MAKEFILE%
echo 	swig -spr -w312,401,402 -DSWIG_%MODULE% -c++ %MODULE%.i>>%MAKEFILE%
echo 	copy Spr%MODULE%Decl.hpp ..\..\include\%MODULE% >>%MAKEFILE%
make -f%MAKEFILE%
set PATH=%PATHORG%
set PATHORG=
:end

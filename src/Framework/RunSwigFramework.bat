@echo off
call ..\Foundation\RunSwig.bat Framework Foundation

set MODULE=FWOldSpringhead
set MAKEFILE=%MODULE%Stub.mak.txt
rem makefile‚Ìì¬‚ÆMake‚ÌŽÀs
set PATHORG=%PATH%;
set PATH=..\..\bin
set SRCIMP=..\Framework\FWOldSpringheadNodeHandler.h

echo src files:%SRCIMP%

echo #	Do not edit. %0 will update this file.> %MODULE%.i
echo %%module %MODULE%>> %MODULE%.i
echo %%include "../../include/Springhead.h">> %MODULE%.i
echo %%include "../../include/base/Env.h">> %MODULE%.i
echo %%include "../../include/Base/BaseDebug.h">> %MODULE%.i
echo %%include "../../src/Foundation/UTTypeDesc.h">> %MODULE%.i
for %%p in (%SRCIMP%) do echo %%include "%%p">> %MODULE%.i

echo #	Do not edit. RunSwig.bat will update this file.> %MAKEFILE%
echo all: %MODULE%Stub.cpp>>%MAKEFILE%
echo %MODULE%Stub.cpp: %SRCIMP%>>%MAKEFILE%
echo 	swig\swig -spr -w312,401,402 -DSWIG_OLDNODEHANDLER -c++ %MODULE%.i>>%MAKEFILE%
make -f%MAKEFILE%
set PATH=%PATHORG%
set PATHORG=
:end

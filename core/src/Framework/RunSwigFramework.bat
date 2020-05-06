@echo off
call ..\Foundation\RunSwig.bat Framework Foundation

set MODULE=FWOldSpringhead
set MAKEFILE=%MODULE%Stub.mak.txt
rem makefileÇÃçÏê¨Ç∆MakeÇÃé¿çs
set PATHORG=%PATH%;


rem Create SJIS World
set PATH=..\..\bin;
if not exist ..\..\swigtemp mkdir ..\..\swigtemp
if not exist ..\..\swigtemp\src mkdir ..\..\swigtemp\src
if not exist ..\..\swigtemp\include mkdir ..\..\swigtemp\include
if not exist ..\..\swigtemp\src\Foundation mkdir ..\..\swigtemp\src\Foundation
if not exist ..\..\swigtemp\include\Base mkdir ..\..\swigtemp\include\Base
if not exist ..\..\swigtemp\src\Framework mkdir ..\..\swigtemp\src\Framework
if not exist ..\..\swigtemp\include\Framework mkdir ..\..\swigtemp\include\Framework

nkf -s < ..\..\include\Springhead.h > ..\..\swigtemp\include\Springhead.h
nkf -s < ..\..\include\base\Env.h > ..\..\swigtemp\include\base\Env.h
nkf -s < ..\..\include\Base\BaseDebug.h > ..\..\swigtemp\include\Base\BaseDebug.h
nkf -s < ..\..\src\Foundation\UTTypeDesc.h > ..\..\swigtemp\src\Foundation\UTTypeDesc.h
nkf -s < ..\..\src\Framework\FWOldSpringheadNodeHandler.h > ..\..\swigtemp\src\Framework\FWOldSpringheadNodeHandler.h


cd ..\..\swigtemp\src\Foundation
set PATH=..\..\..\bin;..\..\..\bin\swig
set SRCIMP=..\Framework\FWOldSpringheadNodeHandler.h
set SRCIMPDEP=..\..\..\src\Framework\FWOldSpringheadNodeHandler.h

echo src files:%SRCIMP%

echo #	Do not edit. %0 will update this file.> %MODULE%.i
echo %%module %MODULE%>> %MODULE%.i
echo %%include "../../include/Springhead.h">> %MODULE%.i
echo %%include "../../include/base/Env.h">> %MODULE%.i
echo %%include "../../include/Base/BaseDebug.h">> %MODULE%.i
echo %%include "../../src/Foundation/UTTypeDesc.h">> %MODULE%.i
for %%p in (%SRCIMP%) do echo %%include "%%p">> %MODULE%.i

echo #	Do not edit. RunSwig.bat will update this file.> %MAKEFILE%
echo all: ..\..\..\src\Framework\%MODULE%Stub.cpp>>%MAKEFILE%
echo ..\..\..\src\Framework\%MODULE%Stub.cpp: %SRCIMPDEP%>>%MAKEFILE%
echo 	swig\swig -spr -w312,325,401,402 -DSWIG_OLDNODEHANDLER -c++ %MODULE%.i>>%MAKEFILE%
echo 	copy Spr%MODULE%Decl.hpp ..\..\..\include\%MODULE% >>%MAKEFILE%
echo 	copy %MODULE%Stub.cpp ..\..\..\src\Framework >>%MAKEFILE%
echo 	copy %MODULE%Decl.hpp ..\..\..\src\Framework >>%MAKEFILE%

make -f%MAKEFILE%
set PATH=%PATHORG%
set PATHORG=
:end

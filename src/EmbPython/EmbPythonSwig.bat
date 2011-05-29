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
set CPP=SprEP%MODULE%.cpp
set HPP=SprEP%MODULE%.h
set API_INCLUDE=../../include

rem makefile‚Ìì¬‚ÆMake‚ÌŽÀs
set PATHORG=%PATH%;

set PATH=..\..\bin;..\..\bin\swig
set SRCINTF=
for %%f in (%API_INCLUDE%/%MODULE%/*.h) do set SRCINTF=!SRCINTF! %API_INCLUDE%/%MODULE%/%%f



echo #	Do not edit. RunSwig.bat will update this file.> %MODULE%.i
echo %%module %MODULE%>> %MODULE%.i
echo %%include "%API_INCLUDE%/Springhead.h">> %MODULE%.i
echo %%include "%API_INCLUDE%/base/Env.h">> %MODULE%.i
echo %%include "%API_INCLUDE%/Python/Add_decl.h">> %MODULE%.i
echo //--  >> %MODULE%.i
echo #define PyAPI_FUNC(r)	r			>> %MODULE%.i
echo #define LONG_MAX		0X7FFFFFFFL	>> %MODULE%.i
echo #define UCHAR_MAX		0xFF		>> %MODULE%.i
echo #define NULL			0			>> %MODULE%.i
echo #undef __PYDECL					>> %MODULE%.i
echo #define __PYDECL					>> %MODULE%.i
echo //--  >> %MODULE%.i
echo %%include "%API_INCLUDE%/Python/pyport.h">> %MODULE%.i
echo %%include "%API_INCLUDE%/Python/patchlevel.h">> %MODULE%.i
echo %%include "%API_INCLUDE%/Python/Python.h">> %MODULE%.i
echo %%include "%API_INCLUDE%/Python/object.h">> %MODULE%.i
echo //--  >> %MODULE%.i
echo %%include "%API_INCLUDE%/EmbPython/SprEPUtility.h">> %MODULE%.i
if not "%9"=="" echo %%include "%API_INCLUDE%/EmbPython/SprEP%9.h">> %MODULE%.i
if not "%8"=="" echo %%include "%API_INCLUDE%/EmbPython/SprEP%8.h">> %MODULE%.i
if not "%7"=="" echo %%include "%API_INCLUDE%/EmbPython/SprEP%7.h">> %MODULE%.i
if not "%6"=="" echo %%include "%API_INCLUDE%/EmbPython/SprEP%6.h">> %MODULE%.i
if not "%5"=="" echo %%include "%API_INCLUDE%/EmbPython/SprEP%5.h">> %MODULE%.i
if not "%4"=="" echo %%include "%API_INCLUDE%/EmbPython/SprEP%4.h">> %MODULE%.i
if not "%3"=="" echo %%include "%API_INCLUDE%/EmbPython/SprEP%3.h">> %MODULE%.i

echo //--  >> %MODULE%.i
for %%p in (%SRCINTF%) do echo %%include "%%p">> %MODULE%.i

rem SWIG PART
call swig.exe -cpperraswarn -sprpy -c++ %MODULE%.i & if errorlevel 1 echo !!!!SWIG FAILED!!!!! & @pause

rem AStyle PART
call ..\..\bin\AStyle.exe  --style=allman --indent=tab "%CPP%" "%HPP%" & if errorlevel 1 @pause
del /Q .\SprEP%MODULE%.*.orig

rem Arrange PART
set HEADER=%API_INCLUDE%\EmbPython\SprEP%MODULE%.h
move /Y %HPP% %HEADER%
echo #include "%HEADER%" > %MODULE%.tmp
type %CPP% >> %MODULE%.tmp

move /Y %MODULE%.tmp %CPP%

:end

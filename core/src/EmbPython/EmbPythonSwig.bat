@echo off
if "%1"=="" goto end
if "%1"=="LISTSRC" goto ListSrc
cmd /v:on /c%0 LISTSRC %1 %2 %3 %4 %5 %6 %7 %8 %9
goto end

:ListSrc
set TARGET=%9 %8 %7 %6 %5 %4 %3 %2
set MODULE=%2
set CPP=EP%MODULE%.cpp
set HPP=EP%MODULE%.h
set SPRH=SprEP%MODULE%.h
set API_INCLUDE=../../include

rem makefile‚Ìì¬‚ÆMake‚ÌŽÀs
set PATHORG=%PATH%;

set PATH=..\..\bin;..\..\bin\swig
rem set SRCINTF=
rem for %%f in (%API_INCLUDE%/%MODULE%/*.h) do set SRCINTF=!SRCINTF! %API_INCLUDE%/%MODULE%/%%f

:swig
echo Swig Part
call swig.exe -cpperraswarn -sprpy -c++ %MODULE%.i & if errorlevel 1 echo !!!!SWIG FAILED!!!!! & @pause

:astyle
echo AStyle Part
call ..\..\bin\AStyle.exe  --style=allman --indent=tab "%CPP%" "%HPP%" "%SPRH%" & if errorlevel 1 @pause
del /Q .\SprEP%MODULE%.*.orig
del /Q .\EP%MODULE%.*.orig

:arrange
echo Arrange Part
set APIHEADER=%API_INCLUDE%\EmbPython\SprEP%MODULE%.h
set HEADER=EP%MODULE%.h

move /Y %SPRH% %APIHEADER%
echo #include "%APIHEADER%" > %MODULE%.tmp
echo #include "%HEADER%" >> %MODULE%.tmp
echo #pragma warning(disable:4244) >> %MODULE%.tmp
type %CPP% >> %MODULE%.tmp

move /Y %MODULE%.tmp %CPP%

:end

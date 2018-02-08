@echo off
setlocal enabledelayedexpansion

call :set_abspath CORE ..
call :set_abspath TOOL ..\..\buildtool
call :set_abspath DEPT ..\..\dependency\src
call :set_abspath ODIR ..\..\generated\doc
call :set_abspath TEST ..\test

set DOXYFILE=springhead.doxy
set TARGETNAME=Reference
set TARGETDIR=%ODIR%\%TARGETNAME%
set JSDIR=%TARGETDIR%\js

set PATH=%CORE%\bin;%TOOL%;%TOOL%\Graphviz\bin;%TEST%\bin;%PATH%

mkdir HTML

:: *** This does not work! ***
:: If HTML Help Compiler exists..
goto :skip
set SRCHDIR="C:\Program Files (x86)"
set SRCHEXE=hhc.exe
set HHCPATH=
set FINDCMD=dir /a-d /b /s %SRCHDIR% ^^^| findstr "%SRCHEXE%"
for /f "usebackq delims=" %%a in (`%FINDCMD%`) do set HHCPATH=%%a
call set HHCDIR=!HHCPATH:\%SRCHEXE%=!
if "%HHCPATH%" neq "" (
	rem Don't laugh!  Need these stupid code actually!
	set PATH="%HHCDIR%";!PATH!
	set PATH=!PATH:"=!
)
if "%HHCPATH%" neq "" (
	rem (type %DOXYFILE% & echo HHC_LOCATION=!HHCPATH!& echo GENERATE_HTMLHELP=YES& echo GENERATE_TREEVIEW=NO& echo OUTPUT_DIRECTORY=%ODIR%& echo CHM_FILE=..\%TARGETNAME%.chm) | doxygen - 2> doxygen_chm.log
	(type %DOXYFILE% & echo HHC_LOCATION=!HHCPATH!& echo GENERATE_HTMLHELP=YES& echo GENERATE_TREEVIEW=NO& echo CHM_FILE=%TARGETNAME%.chm) | doxygen - 2> doxygen_chm.log
	rem Output file is put on ./HTML
	rem Copy it to generated/doc and rm ./HTML
)
:skip

(type %DOXYFILE% & echo OUTPUT_DIRECTORY=%ODIR%) | doxygen - 2> doxygen.log

if exist %TARGETDIR% rmdir /Q /S %TARGETDIR% 2>NUL
rename %ODIR%\HTML %TARGETNAME%
mkdir %JSDIR%
copy %DEPT%\MathJax.js %JSDIR%
rmdir /S /Q HTML 2>NUL
del doxygen*.tmp 2>NUL

echo Document generated at "%TARGETDIR%".
endlocal
exit /b

:set_abspath
	set %1=%~f2
exit /b

@echo off
setlocal enabledelayedexpansion

call :set_abspath CORE ..
call :set_abspath TOOL ..\..\buildtool
call :set_abspath DEPT ..\..\dependency\src
call :set_abspath ODIR ..\..\generated\doc

set DOXYFILE=springhead.doxy
set TARGETNAME=Reference
set TARGETDIR=%ODIR%\%TARGETNAME%
set JSDIR=%TARGETDIR%\js

set PATH=%CORE%\bin;%TOOL%;%TOOL%\Graphviz\bin;%TOOL%\HHW;%PATH%

mkdir HTML

(type %DOXYFILE% & echo GENERATE_HTMLHELP=YES& echo GENERATE_TREEVIEW=NO& echo OUTPUT_DIRECTORY=%ODIR%& echo CHM_FILE=..\%TARGETNAME%.chm) | doxygen - 2> doxygen_chm.log
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

set ORG_PATH=%PATH%

set X64PATH=C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE
set X32PATH=C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE
set ADDPATH=..\..\Springhead2Test\src\Foundation
set PROG=devenv.exe
if exist "%X32PATH%\%PROG%" set PATH=%X32PATH%;%ADDPATH%;%PATH%
if exist "%X64PATH%\%PROG%" set PATH=%X64PATH%;%ADDPATH%;%PATH%

set SBV10_FNAME=%2
if %SBV10_FNAME:~-2% equ 10 set SBV10_FNAME=%SBV10_FNAME:~0,-2%
if "%SBV10_FNAME%" equ "Debug" goto :NEXT
if "%~n3" equ "Template" goto :NEXT
rem if not exist %1\%SBV10_FNAME%10.sln goto :NEXT

if "%DAILYBUILD_EXECUTE_SAMPLEBUILD%" neq "skip" echo   Samples: %~n3: %SBV10_FNAME%

rem ビルドをしてログを"SamplesBuild.log"に出力
echo *** %~n3: %SBV10_FNAME% *** >> log\SamplesBuild.log
devenv %1\%SBV10_FNAME%10.sln /build  Debug > log\SamplesBuildTemp.log
type log\SamplesBuildTemp.log >> log\SamplesBuild.log
echo. >> log\SamplesBuild.log
echo. >> log\SamplesBuild.log

rem エラーがある行だけ"SamplesBuildError.log"に出力
echo *** %~n3: %SBV10_FNAME% *** >> log\SamplesBuildError.log
type log\SamplesBuildTemp.log | find " error " >> log\SamplesBuildError.log
echo. >> log\SamplesBuildError.log
echo. >> log\SamplesBuildError.log
del log\SamplesBuildTemp.log

if exist %1\Debug\%SBV10_FNAME%.exe set SAMPLEBUILD_S=ビルド成功
if exist %1\Debug\%SBV10_FNAME%.exe set SAMPLEBUILD_LIST=%SAMPLEBUILD_LIST%%~n3:%SBV10_FNAME%,
if not exist %1\Debug\%SBV10_FNAME%.exe set SAMPLEBUILD_F=ビルド失敗
if not exist %1\Debug\%SBV10_FNAME%.exe set SAMPLEBUILD_F_LIST=%SAMPLEBUILD_F_LIST%%~n3:%SBV10_FNAME%,
rem if not exist %1\Debug\%2.exe goto NEXT

:NEXT
set PATH=%ORG_PATH%
set ORG_PATH=
set ERROR=
set SBV10_FNAME=
exit /b

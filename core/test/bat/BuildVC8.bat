set ORG_PATH=%PATH%
set PATH=C:\Program Files\Microsoft Visual Studio 8\Common7\IDE;%PATH%

rem カレントディレクトリを環境変数に登録する
for /F usebackq %%f in (`cd`) do set CURRENTDIR=%%f

rem ビルドをしてログを"Build.log"に出力
echo *** %2 *** >> log\Build.log
devenv %1\%28.sln /build  Debug > log\BuildTemp.log
type log\BuildTemp.log >> log\Build.log
echo. >> log\Build.log
echo. >> log\Build.log

rem エラーがある行だけ"BuildError.log"に出力
echo *** %2 *** >> log\BuildError.log
type log\BuildTemp.log | find " error " >> log\BuildError.log
echo. >> log\BuildError.log
echo. >> log\BuildError.log
del log\BuildTemp.log

if exist %1\Debug\%2.exe set BUILD_S=ビルド成功
if exist %1\Debug\%2.exe set BUILD_LIST=%BUILD_LIST%%2,
if not exist %1\Debug\%2.exe set BUILD_F=ビルド失敗
if not exist %1\Debug\%2.exe set BUILD_F_LIST=%BUILD_F_LIST%%2,
if not exist %1\Debug\%2.exe goto NEXT

rem 実行してログを"Run.log"に出力
cd %1
echo *** %2 *** >> %CURRENTDIR%\log\Run.log
set ERRORLEVEL=0
if exist Debug\%2.exe Debug\%2.exe > %CURRENTDIR%\log\RunTemp.log
if exist Debug\%2.exe set ERROR=%ERRORLEVEL%
cd %CURRENTDIR%
type log\RunTemp.log >> log\Run.log
echo. >> log\Run.log
echo. >> log\Run.log

rem ***** エラーがある行だけ"RunError.log"に出力 ***** 
echo *** %2 *** >> log\RunError.log
if not "%ERROR%"=="0"  type log\RunTemp.log | find " error " >> log\RunError.log
echo. >> log\RunError.log
echo. >> log\RunError.log

if "%ERROR%"=="0" set RUN_S=実行成功
if "%ERROR%"=="0" set RUN_LIST=%RUN_LIST%%2,
if not "%ERROR%"=="0" set RUN_F=実行失敗
if not "%ERROR%"=="0" set RUN_F_LIST=%RUN_F_LIST%%2,

del log\RunTemp.log

:NEXT
set PATH=%ORG_PATH%
set ORG_PATH=
set ERROR=

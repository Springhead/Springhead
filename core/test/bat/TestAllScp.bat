rem === テストを実行する ===

rem === ラベル付けに使う環境変数の初期化 ===

set DATESL=%DATE:~2,10%
set DATE=%DATESL:/=.%
set BUILD_S=
set BUILD_F=
set BUILD_LIST=ビルド成功（
set BUILD_F_LIST=ビルド失敗（
set RUN_S=
set RUN_F=
set RUN_LIST=実行成功（
set RUN_F_LIST=実行失敗（

echo 日付 : %DATE% > log\Build.log
echo --- ビルドのログ ---  >> log\Build.log
echo. >> log\Build.log

echo 日付 : %DATE% > log\BuildError.log
echo --- ビルドエラーのログ --- >> log\BuildError.log
echo. >> log\BuildError.log

echo 日付 : %DATE% > log\Run.log
echo --- 実行のログ ---  >> log\Run.log
echo. >> log\Run.log

echo 日付 : %DATE% > log\RunError.log
echo --- 実行エラーのログ --- >> log\RunError.log
echo. >> log\RunError.log

echo 日付 : %DATE% > log\History.log
echo --- 更新履歴のログ --- >> log\History.log
echo. >> log\History.log

rem === ビルドテストを行う(テストを行うファイルは引数で指定) ===
rem call bat\BuildVC8(RunDebug).bat (引数1) (引数2) 
rem (引数1) : プロジェクトファイルの場所
rem (引数2) : プロジェクトファイル名(BuildMFC-.dspの前の部分, BuildBCB-.bpgの前の部分)
for /D %%p in (..\src\tests\*) do for /D %%n in (%%p\*) do call bat\BuildVC8.bat %%n %%~nn
set BUILD_LIST=%BUILD_LIST:~0,-1%）
set BUILD_F_LIST=%BUILD_LIST:~0,-1%）
set RUN_LIST=%RUN_LIST:~0,-1%）
set RUN_F_LIST=%RUN_LIST:~0,-1%）


if "%BUILD_S%" == "ビルド成功" svn copy svn+ssh://sprsvn/export/spr/svn/repository/Springhead2/trunk svn+ssh://sprsvn/export/spr/svn/repository/Springhead2/tags/BuildSucceed%date% -m %BUILD_LIST% %RUN_LIST%

rem === Springhead2の更新履歴をHistory.logに出力 ===
svn log svn+ssh://sprsvn/export/spr/svn/repository/Springhead2/trunk > log/History.log
if exist log\result.log del log\result.log
if "%BUILD_S%" == "ビルド成功" echo %BUILD_LIST% %RUN_LIST%>>log\result.log
if "%BUILD_F%" == "ビルド失敗" echo %BUILD_F_LIST% %RUN_F_LIST%>>log\result.log

rem === ログをSambaにコピーする ===
set PUTTYPROF=spr
set SERVNAME=springhead.info
set DIRPATH=/home/WWW/springhead/springhead2
set WEBBASE=%SERVNAME%:%DIRPATH%
pscp -r -p -load %PUTTYPROF% log\*.log %WEBBASE%/

set PUTTYPROF=seven
set SERVNAME=seven77.ddo.jp
set DIRPATH=/home/springhead/seven_http
set WEBBASE=%SERVNAME%:%DIRPATH%
pscp -r -p -load %PUTTYPROF% log\*.log %WEBBASE%/


rem **** ログをSVNにコミットする ****
cd log 
svn commit -m "Autobuild done."
cd ..

rem **** 使用した環境変数のクリア **** 
set DATE=
set BUILD_S=
set BUILD_F=
set BUILD_LIST=
set BUILD_F_LIST=
set RUN_LIST=
set RUN_F_LIST=

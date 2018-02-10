@echo off

python DailyBuild.py %*
exit /b

set PLAT=-p x64
set CONF=-c Release
set RESULT=result/dailybuild.result
set CONTROL=dailybuild.control
set SECTION=Windows

pushd bin
python SpringheadTest.py -S .    %PLAT% %CONF% %RESULT% %CONTROL% %SECTION%
python SpringheadTest.py tests   %PLAT% %CONF% %RESULT% %CONTROL% %SECTION%
python SpringheadTest.py Samples %PLAT% %CONF% %RESULT% %CONTROL% %SECTION%
popd

exit /b

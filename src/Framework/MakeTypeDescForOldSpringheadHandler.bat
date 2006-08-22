@echo off
rem include/引数/*.h から， struct の定義を抜き出して，TypeDescの定義を作る．
rem bin/typedesc.exe が抜き出しを行う．
if "%1"=="EXECUTE" goto Execute

set MAKEFILE=MakeTypeDescForOldSpringheadHandlerMAK.txt
set OUT=TypeDescDumpImpForOldSpringheadHandler.h
set HEADER=TypeDescDumpForOldSpringheadHandler.h
set TARGET=FWOldSpringheadNodeHandler.h

rem makefileの作成とMakeの実行
set PATHORG=%PATH%;
set PATH=%SPRINGHEAD2%\bin;%PATH%
echo source files:%TARGET%

echo #	Do not edit. MakeTypeDescForOldSpringheadHandler.bat will update this file.> %MAKEFILE%
echo all: %OUT%>>%MAKEFILE%
echo %OUT%: %TARGET%>>%MAKEFILE%
echo 	MakeTypeDescForOldSpringheadHandler.bat EXECUTE>>%MAKEFILE%
make -f%MAKEFILE%
set PATH=%PATHORG%
set PATHORG=

..\Foundation\MakeTypeDesc.bat Framework
goto end

:Execute
rem Makeが呼び出す．

echo //	Do not edit. MakeTypeDescForOldSpringheadHandler.bat will update this file.> %HEADER%
echo //	Do not edit. MakeTypeDescForOldSpringheadHandler.bat will update this file.> %OUT%
for %%f in (%TARGET%) do echo #include "%%f" >> %HEADER%
for %%f in (%TARGET%) do typedesc %%f >> %OUT%
:end

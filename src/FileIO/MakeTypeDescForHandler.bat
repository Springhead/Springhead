@echo off
call ..\Foundation\MakeTypeDesc.bat FileIO
rem include/引数/*.h から， struct の定義を抜き出して，TypeDescの定義を作る．
rem bin/typedesc.exe が抜き出しを行う．
if "%1"=="EXECUTE" goto Execute

set MAKEFILE=MakeTypeDescForHandlerMAK.txt
set OUT=TypeDescDumpImpForHandler.h
set HEADER=TypeDescDumpForHandler.h
set TARGET=FIOldSpringheadNode.h

rem makefileの作成とMakeの実行
set PATH=%SPRINGHEAD2%\bin
echo source files:%TARGET%

echo #	Do not edit. MakeTypeDesc.bat will update this file.> %MAKEFILE%
echo all: %OUT%>>%MAKEFILE%
echo %OUT%: %TARGET%>>%MAKEFILE%
echo 	MakeTypeDesc.bat EXECUTE>>%MAKEFILE%
make -f%MAKEFILE%
goto end


:Execute
rem Makeが呼び出す．

echo //	Do not edit. MakeTypeDesc.bat will update this file.> %HEADER%
echo //	Do not edit. MakeTypeDesc.bat will update this file.> %OUT%
for %%f in (%TARGET%) do echo #include "%%f" >> %HEADER%
for %%f in (%TARGET%) do typedesc %%f >> %OUT%
:end

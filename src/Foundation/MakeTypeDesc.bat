@echo off
rem include/引数/*.h から， struct の定義を抜き出して，TypeDescの定義を作る．
rem bin/typedesc.exe が抜き出しを行う．
if "%1"=="LISTSRC" goto ListSrc
if "%1"=="EXECUTE" goto Execute

set OUT=TypeDescDumpImp.h
set HEADER=TypeDescDump.h
set TARGET=%1 %2 %3 %4 %5 %6 %7 %8 %9
set MAKEFILE=MakeTypeDescMAK.txt
cmd /v:on /c%0 LISTSRC
goto end

:ListSrc
rem makefileの作成とMakeの実行
set PATH=%SPRINGHEAD2%\bin
set SRC=
for %%p in (%TARGET%) do for %%f in (../../include/%%p/*.h) do set SRC=!SRC! ../../include/%%p/%%f
echo source files:%SRC%

echo #	Do not edit. MakeTypeDesc.bat will update this file.> %MAKEFILE%
echo all: %OUT%>>%MAKEFILE%
echo %OUT%: %SRC%>>%MAKEFILE%
echo 	MakeTypeDesc.bat EXECUTE>>%MAKEFILE%
make -f%MAKEFILE%
goto end


:Execute
rem Makeが呼び出す．

echo //	Do not edit. MakeTypeDesc.bat will update this file.> %OUT%
echo //	Do not edit. MakeTypeDesc.bat will update this file.> %HEADER%
for %%p in (%TARGET%) do for %%f in (../../include/%%p/*.h) do echo #include "../../include/%%p/%%f" >> %HEADER%

for %%p in (%TARGET%) do for %%f in (../../include/%%p/*.h) do typedesc ../../include/%%p/%%f >> %OUT%
if exist UTBaseType.h typedesc UTBaseType.h >> %OUT%
:end

@echo off
rem include/引数/*.h から， struct の定義を抜き出して，TypeDescの定義を作る．
rem bin/typedesc.exe が抜き出しを行う．
if "%1"=="LISTSRC" goto ListSrc
if "%1"=="EXECUTE" goto Execute

set DESCIF=TypeDescDump%1.h
set DESCIMP=TypeDescDumpImp%1.h
set STUBIF=IfStubDump%1.h
set STUBIMP=IfStubDumpImp%1.h
set TARGET=%1 %2 %3 %4 %5 %6 %7 %8 %9
set MAKEFILE=MakeTypeDescMAK.txt
cmd /v:on /c%0 LISTSRC
goto end

:ListSrc
rem makefileの作成とMakeの実行
set PATHORG=%PATH%;
set PATH=%SPRINGHEAD2%\bin;%PATH%
set SRC=
for %%p in (%TARGET%) do for %%f in (../../include/%%p/*.h) do set SRC=!SRC! ../../include/%%p/%%f
echo source files:%SRC%

echo #	Do not edit. MakeTypeDesc.bat will update this file.> %MAKEFILE%
echo all: %DESCIMP%>>%MAKEFILE%
echo %DESCIMP%: %SRC%>>%MAKEFILE%
echo 	..\Foundation\MakeTypeDesc.bat EXECUTE>>%MAKEFILE%
make -f%MAKEFILE%
set PATH=%PATHORG%
set PATHORG=
goto end


:Execute
rem Makeが呼び出す．

echo //	Do not edit. MakeTypeDesc.bat will update this file.> %DESCIF%
echo //	Do not edit. MakeTypeDesc.bat will update this file.> %DESCIMP%
echo //	Do not edit. MakeTypeDesc.bat will update this file.> %STUBIF%
echo //	Do not edit. MakeTypeDesc.bat will update this file.> %STUBIMP%

echo namespace Spr{; >> %STUBIF%

for %%p in (%TARGET%) do for %%f in (../../include/%%p/*.h) do echo #include "../../include/%%p/%%f" >> %DESCIF%

for %%p in (%TARGET%) do for %%f in (../../include/%%p/*.h) do typedesc ../../include/%%p/%%f %DESCIMP% %STUBIF% %STUBIMP%
if exist UTBaseType.h typedesc UTBaseType.h %DESCIMP%

echo }	//	namespace Spr; >> %STUBIF%

:end

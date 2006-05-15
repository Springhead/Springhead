rem include/*/*.h から， struct の定義を抜き出して，
rem FIDesc.cppにTypeDescの定義を作る．．
rem bin/typedesc.exe が抜き出しを行う．
set OUT=TypeDescDumpImp.h
set HEADER=TypeDescDump.h
set TARGET=Collision Foundation Graphics Physics FileIO Creature

set PATH=%SPRINGHEAD2%\bin
del %OUT%
del %HEADER%
echo //	Do not edit. MakeTypeDesc.bat will update this file.> %OUT%
for %%p in (%TARGET%) do for %%f in (../../include/%%p/*.h) do echo #include "../../include/%%p/%%f" >> %HEADER%

for %%p in (%TARGET%) do for %%f in (../../include/%%p/*.h) do typedesc ../../include/%%p/%%f >> %OUT%
typedesc FIBaseType.h >> %OUT%

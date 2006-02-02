rem include/*/*.h から， struct の定義を抜き出して，
rem FIDesc.cppにTypeDescの定義を作る．．
rem bin/typedesc.exe が抜き出しを行う．
set OUT=TypeDescDump.cpp
set HEADER=TypeDescDump.h

set PATH=%SPRINGHEAD2%\bin
del %OUT%
del %HEADER%
echo //	Do not edit. MakeTypeDesc.bat will update this file.> %OUT%
for /D %%p in (../../include/*) do for %%f in (../../include/%%p/*.h) do echo #include "../../include/%%p/%%f" >> %HEADER%

for /D %%p in (../../include/*) do for %%f in (../../include/%%p/*.h) do typedesc ../../include/%%p/%%f >> %OUT%
typedesc FIBaseType.h >> %OUT%

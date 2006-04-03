rem include/*/*.h から， struct の定義を抜き出して，
rem FIDesc.cppにTypeDescの定義を作る．．
rem bin/typedesc.exe が抜き出しを行う．
set OUT=TypeDescDumpImp.h
set HEADER=TypeDescDump.h

set PATH=%SPRINGHEAD2%\bin
del %OUT%
del %HEADER%
echo //	Do not edit. MakeTypeDesc.bat will update this file.> %OUT%
for %%p in (Collision Foundation Graphics Physics) do for %%f in (../../include/%%p/*.h) do echo #include "../../include/%%p/%%f" >> %HEADER%
echo #include "../Collision/Collision.h" >> %HEADER%
echo #include "../Foundation/Foundation.h" >> %HEADER%
echo #include "../Physics/Physics.h" >> %HEADER%
echo #include "../Graphics/Graphics.h" >> %HEADER%

for %%p in (Collision Foundation Graphics Physics) do for %%f in (../../include/%%p/*.h) do typedesc ../../include/%%p/%%f >> %OUT%
typedesc FIBaseType.h >> %OUT%

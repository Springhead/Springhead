rem .h から， struct の定義を抜き出す．
rem bin/typedesc.exe が抜き出しを行う．
set OUT=TypeDescDumpImpForHandler.h
set HEADER=TypeDescDumpForHandler.h
set TARGET=FIOldSpringheadNode.h

set PATH=%SPRINGHEAD2%\bin
del %OUT%
del %HEADER%
echo //	Do not edit. MakeTypeDescForHandler.bat will update this file.> %OUT%
for %%f in (%TARGET%) do echo #include "%%f" >> %HEADER%

for %%f in (%TARGET%) do typedesc %%f >> %OUT%

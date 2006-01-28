rem include/*/*.h から， struct XXXDesc と struct XXXState の定義を抜き出して，
rem FIDesc.cppに文字列定数の定義として書き出す．
rem bin/typedesc.exe が抜き出しを行う．

set PATH=%SPRINGHEAD2%\bin
del FIDesc.cpp
echo //	Don't edit. MakeTypeDesc.bat update this file.> FIDesc.cpp
for /D %%p in (../../include/*) do for %%f in (../../include/%%p/*.h) do typedesc <../../include/%%p/%%f >> FIDesc.cpp

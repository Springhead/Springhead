set PATH=%SPRINGHEAD2%\bin
del FIDesc.cpp
echo //	Don't edit. MakeTypeDesc.bat update this file.> FIDesc.cpp
for /D %%p in (../../include/*) do for %%f in (../../include/%%p/*.h) do typedesc <../../include/%%p/%%f >> FIDesc.cpp

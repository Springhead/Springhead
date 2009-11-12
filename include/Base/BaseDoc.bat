call CopyBase.bat
set PATHORG=%PATH%
set PATH=..\..\BIN;..\..\BIN\Graphviz\bin;%PATH%
cd ..
doxygen Base/base.doxy 2>Base/base.log
set PATH=%PATHORG%
:END

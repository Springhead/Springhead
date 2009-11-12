set PATHORG=%PATH%
set PATH=..\..\BIN;..\..\BIN\Graphviz\bin;%PATH%
cd ..
doxygen Graphics/Graphics.doxy 2>Graphics/Graphics.log
set PATH=%PATHORG%
:END

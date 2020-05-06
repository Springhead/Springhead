set PATHORG=%PATH%
set PATH=..\..\BIN;..\..\BIN\Graphviz\bin;%PATH%
cd ..
doxygen Collision/Collision.doxy 2>Collision/Collision.log
set PATH=%PATHORG%
:END

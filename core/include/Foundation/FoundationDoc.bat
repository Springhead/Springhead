set PATHORG=%PATH%
set PATH=..\..\BIN;..\..\BIN\Graphviz\bin;%PATH%
cd ..
doxygen Foundation/Foundation.doxy 2>Foundation/Foundation.log
set PATH=%PATHORG%
:END

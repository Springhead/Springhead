set PATHORG=%PATH%
set PATH=..\..\BIN;..\..\BIN\Graphviz\bin;%PATH%
cd ..
doxygen FileIO/FileIO.doxy 2>FileIO/FileIO.log
set PATH=%PATHORG%
:END

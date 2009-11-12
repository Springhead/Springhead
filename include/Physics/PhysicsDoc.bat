set PATHORG=%PATH%
set PATH=..\..\BIN;..\..\BIN\Graphviz\bin;%PATH%
cd ..
doxygen Physics/Physics.doxy 2>Physics/Physics.log
set PATH=%PATHORG%
:END

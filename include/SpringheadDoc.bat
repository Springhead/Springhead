set PATHORG=%PATH%
set PATH=..\BIN;..\BIN\Graphviz\bin;%PATH%
doxygen springhead.doxy 2> springhead.log
set PATH=%PATHORG%

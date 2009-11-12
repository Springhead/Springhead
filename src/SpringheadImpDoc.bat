set PATHORG=%PATH%
set PATH=..\BIN;..\BIN\Graphviz\bin;%PATH%
mkdir html
doxygen springheadImp.doxy
set PATH=%PATHORG%

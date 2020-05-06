set PATHORG=%PATH%
set PATH=..\BIN;..\BIN\Graphviz\bin;%PATH%
mkdir html
doxygen temp.doxy
set PATH=%PATHORG%
:END

set PATHORG=%PATH%
set PATH=..\BIN;..\BIN\Graphviz\bin;%PATH%
doxygen temp.doxy 2> temp.log
set PATH=%PATHORG%
:END

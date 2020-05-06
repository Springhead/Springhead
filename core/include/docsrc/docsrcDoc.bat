set PATHORG=%PATH%
set PATH=..\BIN;..\BIN\Graphviz\bin;%PATH%
cd ..
doxygen docsrc/docsrc.doxy 2>docsrc/docsrc.log
set PATH=%PATHORG%
:END

set PATHORG=%PATH%
set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin;%PATH%
doxygen docsrc.doxy 2>docsrc.log
set PATH=%PATHORG%
:END

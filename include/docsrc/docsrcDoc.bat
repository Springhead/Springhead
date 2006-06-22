set PATHORG=%PATH%
set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin;%PATH%
cd ..
doxygen docsrc/docsrc.doxy 2>docsrc/docsrc.log
set PATH=%PATHORG%
:END

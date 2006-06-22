set PATHORG=%PATH%
set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin;%PATH%
doxygen Physics.doxy 2>Physics.log
set PATH=%PATHORG%
:END

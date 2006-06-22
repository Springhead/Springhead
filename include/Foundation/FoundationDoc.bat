set PATHORG=%PATH%
set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin;%PATH%
doxygen Foundation.doxy 2>Foundation.log
set PATH=%PATHORG%
:END

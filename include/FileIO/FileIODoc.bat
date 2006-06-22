set PATHORG=%PATH%
set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin;%PATH%
doxygen FileIO.doxy 2>FileIO.log
set PATH=%PATHORG%
:END

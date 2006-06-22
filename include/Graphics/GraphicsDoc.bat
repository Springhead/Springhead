set PATHORG=%PATH%
set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin;%PATH%
doxygen Graphics.doxy 2>Graphics.log
set PATH=%PATHORG%
:END

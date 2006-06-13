set PATHORG=%PATH%
set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin;%PATH%
mkdir html
doxygen temp.doxy
set PATH=%PATHORG%
:END

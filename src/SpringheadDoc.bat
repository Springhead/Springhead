set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin
mkdir html
del /Q html\*.*
doxygen springhead.doxy
:END

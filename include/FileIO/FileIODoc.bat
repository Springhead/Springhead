set PATHORG=%PATH%
set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin;%PATH%
cd ..
doxygen FileIO/FileIO.doxy 2>FileIO/FileIO.log
set PATH=%PATHORG%
:END

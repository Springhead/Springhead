set PATHORG=%PATH%
set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin;%PATH%
cd ..
doxygen Physics/Physics.doxy 2>Physics/Physics.log
set PATH=%PATHORG%
:END

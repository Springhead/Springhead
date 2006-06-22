set PATHORG=%PATH%
set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin;%PATH%
cd ..
doxygen Foundation/Foundation.doxy 2>Foundation/Foundation.log
set PATH=%PATHORG%
:END

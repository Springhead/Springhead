set PATHORG=%PATH%
set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin;%PATH%
cd ..
doxygen Graphics/Graphics.doxy 2>Graphics/Graphics.log
set PATH=%PATHORG%
:END

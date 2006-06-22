set PATHORG=%PATH%
set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin;%PATH%
cd ..
doxygen Collision/Collision.doxy 2>Collision/Collision.log
set PATH=%PATHORG%
:END

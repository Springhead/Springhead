set PATHORG=%PATH%
set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin;%PATH%
doxygen Collision.doxy 2>Collision.log
set PATH=%PATHORG%
:END

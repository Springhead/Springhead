call CopyBase.bat
set PATHORG=%PATH%
set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin;%PATH%
cd ..
doxygen Base/base.doxy 2>Base/base.log
set PATH=%PATHORG%
:END

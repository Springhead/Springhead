cd Base&call CopyBase.Bat&cd..
set PATHORG=%PATH%
set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin;%PATH%
for %%f in (docsrc Base Foundation Collision Physics Graphics FileIO) do cd %%f&doxygen %%f.doxy 2> %%f.log
doxygen springhead.doxy
set PATH=%PATHORG%
:END

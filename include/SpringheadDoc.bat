cd Base&call CopyBase.Bat&cd..
set PATHORG=%PATH%
set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin;%PATH%
for %%f in (docsrc Base Foundation Collision Physics Graphics FileIO) do cd %%f & doxygen %%f.doxy 2> %%f.log & cd ..
doxygen springhead.doxy 2> springhead.log
set PATH=%PATHORG%
:END

cd Base&call CopyBase.Bat&cd..
set PATHORG=%PATH%
set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin;%PATH%
:for %%f in (docsrc Base Foundation Collision Physics Graphics FileIO) do doxygen %%f\%%f.doxy 2> %%f\%%f.log
doxygen springhead.doxy 2> springhead.log
set PATH=%PATHORG%
:END

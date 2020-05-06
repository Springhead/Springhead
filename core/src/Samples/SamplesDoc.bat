set PATHORG=%PATH%
set PATH=..\..\BIN;..\..\BIN\Graphviz\bin;%PATH%
cd ..
doxygen Samples/SamplesDoc.doxy 2>Samples/SamplesDoc.log
set PATH=%PATHORG%
:END

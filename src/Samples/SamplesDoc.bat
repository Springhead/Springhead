set PATHORG=%PATH%
set PATH=%SPRINGHEAD2%\BIN;%SPRINGHEAD2%\BIN\Graphviz\bin;%PATH%
cd ..
doxygen Samples/SamplesDoc.doxy 2>Samples/SamplesDoc.log
set PATH=%PATHORG%
:END

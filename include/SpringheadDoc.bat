set PATHORG=%PATH%
set PATH=..\BIN;..\BIN\Graphviz\bin;%PATH%
(type springhead.doxy & echo GENERATE_HTMLHELP=YES) | doxygen - 2> springhead_tree.log
doxygen springhead.doxy 2> springhead.log
set PATH=%PATHORG%

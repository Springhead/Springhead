set FILENAME=trajik
set CURRDATE=%date:~-8,2%-%date:~-5,2%-%date:~-2,2%
set CURRTIMETMP=%time: =0%
set CURRTIME=%CURRTIMETMP:~0,2%-%CURRTIMETMP:~3,2%
set TIMESTAMP=%CURRDATE%_%CURRTIME%

echo \newcommand{\timestamp}{%CURRDATE%--%CURRTIME%} > timestamp.tex

platex   %FILENAME%
jbibtex  %FILENAME%
platex   %FILENAME%
platex   %FILENAME%
dvipdfmx -f otf_rhei_bhira.map %FILENAME%
copy %FILENAME%.pdf %FILENAME%_%TIMESTAMP%.pdf



setlocal enabledelayedexpansion

set FILENAME=main
set CURRDATE=%date:~-8,2%-%date:~-5,2%-%date:~-2,2%
set CURRTIMETMP=%time: =0%
set CURRTIME=%CURRTIMETMP:~0,2%-%CURRTIMETMP:~3,2%
set TIMESTAMP=%CURRDATE%_%CURRTIME%

echo \newcommand{\timestamp}{%CURRDATE%--%CURRTIME%} > timestamp.tex

platex   %FILENAME%
platex   %FILENAME%
mendex   %FILENAME%.idx
platex   %FILENAME%
dvipdfmx %FILENAME%
copy %FILENAME%.pdf %FILENAME%_%TIMESTAMP%.pdf

if "%1" equ "-t" (
	%FILENAME%.pdf
	del %FILENAME%_%TIMESTAMP%.pdf *.aux *.log *.ilg
)

endlocal
exit /b

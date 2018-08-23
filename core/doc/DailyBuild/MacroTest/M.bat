@echo off
del M.pdf > NUL 2>&1
cmd /c platex M.tex
cmd /c dvipdfmx M.dvi
cmd /c M.pdf
exit /b

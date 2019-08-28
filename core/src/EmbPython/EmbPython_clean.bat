@echo off
setlocal enabledelayedexpansion

set TARGETS=Base Collision Creature FileIO Foundation Framework Graphics HumanInterface Physics

for %%t in (%TARGETS%) do (
	del /Q EP%%t.cpp EP%%t.h Makefile_EmbPython.swig.%%t swig_%%t.log > NUL 2>&1
)

endlocal
exit /b

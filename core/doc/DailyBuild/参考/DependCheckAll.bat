@echo off

set CWD=%CD%
set DIRS=
set DIRS=%DIRS% Base Collision Creature EmbPython FileIO
set DIRS=%DIRS% Foundation Framework Graphics GraphicsD3D HumanInterface
set DIRS=%DIRS% Physics

for %%d in (%DIRS%) do (
    echo.
    echo ================
    echo  %%d
    echo ================
    cd %%d
    call DependCheck
    cd %CWD%    
)

exit /b

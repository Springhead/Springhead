@echo off

if /i "%1" == "-h" (
	echo $0 [ set ^| restore ]
	exit /b
)
set HEADER=SprPHFemMeshNew.h
set TESTFN=SprPHFemMeshNew_For_SprCsSwigTest.h
set SAVEFN=%HEADER%_SAVE
set INCDIR=..\..\..\include\Physics

if /i "%1" == "set" (
	if not exist %INCDIR%\%SAVEFN% (
		echo saving %HEADER% to %SAVEFN%
		ren %INCDIR%\%HEADER% %SAVEFN% >NUL 2>&1
	)
	echo copying %TESTFN% to %INCDIR%\%HEADER%
	copy %TESTFN% %INCDIR%\%HEADER% >NUL 2>&1
)
if /i "%1" == "restore" (
	if exist %INCDIR%\%SAVEFN% (
		del %INCDIR%\%HEADER% >NUL 2>&1
		ren %INCDIR%\%SAVEFN% %HEADER% >NUL 2>&1
	) else (
		echo no saved file found
	)
)
echo+
ls -l %INCDIR%/%HEADER% %INCDIR%/%TESTFN% %INCDIR%/%SAVEFN%

exit /b


@echo off

if /i "%1" == "-h" (
	echo $0 [ set ^| restore ]
	exit /b
)
set HEADER=SprPHFemMeshNew.h
set TSTHDR=SprPHFemMeshNew_For_SprCsSwigTest.h
set SOURCE=SprCsSwigTest.cpp
set TSTSRC=SprCsSwigTest.cpp
set VCXPRJ=Physics16.0.vcxproj
set TSTVCX=SprCsSwigTest_Physics16.0.vcxproj
set SAVHDR=%HEADER%_SAVE
set SAVVCX=%VCXPRJ%_SAVE
set INCDIR=..\..\..\include\Physics
set SRCDIR=..\..\..\src\Physics

if /i "%1" == "set" (
	if not exist %INCDIR%\%SAVHDR% (
		echo saving %HEADER% to %SAVHDR%
		ren %INCDIR%\%HEADER% %SAVHDR% >NUL 2>&1
		echo saving %VCXPRJ% to %SAVVCX%
		ren %SRCDIR%\%VCXPRJ% %SAVVCX% >NUL 2>&1
	)
	echo copying %TSTHDR% to %INCDIR%\%HEADER%
	copy %TSTHDR% %INCDIR%\%HEADER% >NUL 2>&1
	echo copying %TSTSRC% to %SRCDIR%\%SOURCE%
	copy %TSTSRC% %SRCDIR%\%SOURCE% >NUL 2>&1
	echo copying %TSTVCX% to %SRCDIR%\%VCXPRJ%
	copy %TSTVCX% %SRCDIR%\%VCXPRJ% >NUL 2>&1
)
if /i "%1" == "restore" (
	if exist %INCDIR%\%SAVHDR% (
		del %INCDIR%\%HEADER% >NUL 2>&1
		del %SRCDIR%\%SOURCE% >NUL 2>&1
		del %SRCDIR%\%VCXPRJ% >NUL 2>&1
		ren %INCDIR%\%SAVHDR% %HEADER% >NUL 2>&1
		ren %SRCDIR%\%SAVVCX% %VCXPRJ% >NUL 2>&1
	) else (
		echo no saved file found
	)
)
echo+
cmd /c ls -l %INCDIR%/%HEADER% %INCDIR%/%TSTHDR% %INCDIR%/%SAVHDR%
cmd /c ls -l %SRCDIR%/%SOURCE% %SRCIDR%/%TSTSRC%
cmd /c ls -l %SRCDIR%/%VCXPRJ% %SRCIDR%/%TSTVCX% %SRCDIR%/%SAVVCX%

exit /b


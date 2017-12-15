rem ***** ビルドをしてログを"Build.log"に出力 *****  
rem 今のところWin32 MFC Debugでビルドする
echo *** %2 *** >> log\Build.log
msdev ..\%1\%2.dsp /make "%2 - Win32 MFC Debug" > log\BuildTemp.log
type log\BuildTemp.log >> log\Build.log
echo. >> log\Build.log
echo. >> log\Build.log

rem ***** エラーがある行だけ"BuildError.log"に出力 ***** 
echo *** %2 *** >> log\BuildError.log
type log\BuildTemp.log | find " error " >> log\BuildError.log
echo. >> log\BuildError.log
echo. >> log\BuildError.log

del log\BuildTemp.log

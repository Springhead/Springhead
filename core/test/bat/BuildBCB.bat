rem ***** BCBでビルドをしてログを"Build.log"に出力 *****  

echo *** %2 *** >> log\Build.log
bcb /m ..\..\%1\%2.bpg 
type ..\..\%1\%2.err >> log\Build.log
echo. >> log\Build.log
echo. >> log\Build.log

rem ***** エラーがある行だけ"BuildError.log"に出力 ***** 
echo *** %2 *** >> log\BuildError.log
type ..\..\%1\%2.err | find /V "[C++]"  >> log\BuildError.log
echo. >> log\BuildError.log
echo. >> log\BuildError.log


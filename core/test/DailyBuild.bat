<<<<<<< HEAD
@echo off
python DailyBuild.py -u %*
echo.
python DailyBuild.py -U %*
exit /b
=======
@echo off
python DailyBuild.py %*
exit /b
>>>>>>> 0984c6a1c... dailybuild: Process does not have terminal if run as task.

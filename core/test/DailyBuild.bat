@echo off
python DailyBuild.py -u %*
echo.
python DailyBuild.py -U %*
exit /b

@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	hook.bat test-repository
::	    test_repository:	テストを実行するディレクトリ.
::
::  DESCRIPTION
::	DailyBuild.bat から呼び出される.
::	テストに入る前にやっておくべきこと(コミットする前にテストすることなど)
::	を実行するためのフックスクリプト.
::	このファイルは, demo マシンの "I:\DailyBuild" にコピーすること.
::
:: ============================================================================
set PROG=%~n0
echo hook [%PROG%] was called.

endlocal
exit /b

:: end: hook.bat

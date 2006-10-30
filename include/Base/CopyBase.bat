@echo off
if "%1"=="execute" goto execute
del ..\..\include\Base\*.h
set HEADERS=Env BaseDebug BaseUtility TVector TMatrix TinyVec TinyMat Affine TQuaternion TMatrixUtility BaseTypeInfo VectorDebug
for %%f in (%HEADERS%) do call %0 execute %%f
goto end

:execute
rem echo //	このファイルを編集しないでください．CopyBase.bat がビルドの度に上書きします． > ..\..\include\Base\tmp
rem echo //	編集が必要な場合は，src/Base/%2.h を編集してください >> ..\..\include\Base\tmp
xcopy /D /Y ..\..\src\Base\%2.h ..\..\include\Base\

:end

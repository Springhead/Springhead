if "%1"=="execute" goto execute
del *.h
set HEADERS=Env BaseDebug BaseUtility TVector TMatrix TinyVec TinyMat Affine TQuaternion TMatrixUtility
for %%f in (%HEADERS%) do call %0 execute %%f
goto end

:execute
echo //	このファイルを編集しないでください．CopyBase.bat がビルドの度に上書きします． > ..\..\include\Base\%2.h
echo //	編集が必要な場合は，src/Base/%2.h を編集してください >> ..\..\include\Base\%2.h
copy ..\..\include\Base\%2.h+..\..\src\Base\%2.h ..\..\include\Base\%2.h

:end

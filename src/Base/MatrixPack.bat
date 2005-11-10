set PATH=%SPRINGHEAD%\BIN;%SPRINGHEAD%\BIN\Graphviz\bin;%PATH%
mkdir ..\Package
mkdir ..\Package\TMatrixPackage
del /Q ..\Package\TMatrixPackage\*.*
echo #define PTM_PACK > PTM_PACK.h
for %%i in (TQuaternion.h TinyMat.h TinyVec.h TMatrix.* TVector.* TMatrixUtility.* Affine.h Base.doxy) do copy /B PTM_PACK.h + %%i ..\Package\TMatrixPackage\%%i
cd ..\Package\TMatrixPackage
echo /** @mainpage ポータブルテンプレート行列クラスライブラリ > PTM.h
echo     @ref PTM "ポータブルテンプレート行列クラスライブラリ" >> PTM.h
echo     @ref Affine "アフィン行列クラスライブラリ" >> PTM.h
echo */ >> PTM.h

doxygen Base.doxy
:start html\index.html
lha32 u PTM.2004...lzh *.h
lha32 u PTMDoc.2004...lzh html\
:end

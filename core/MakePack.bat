setlocal
if exist pack\springhead rmdir /S /Q pack\springhead
if not exist pack mkdir pack
cd pack
svn export http://springhead.info/spr2/Springhead/trunk ./Springhead

cd springhead

rem 公開できないファイルの削除と設定変更
del /F /S /Q closed
echo #undef USE_CLOSED_SRC > core/include/UseClosedSrcOrNot.h

set PATH=buildtool;%PATH%

minitar -c -z9 ../Springhead.tgz *.*
cd ..
cd ..

endlocal
exit /b

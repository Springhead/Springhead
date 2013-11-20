if exist pack\springhead2 rmdir /S /Q pack\springhead2
if not exist pack mkdir pack
cd pack
svn export http://springhead.info/spr2/Springhead2/trunk ./Springhead2

cd springhead2

rem 公開できないファイルの削除と設定変更
del /F /S /Q closed
echo #undef USE_CLOSED_SRC > include/UseClosedSrcOrNot.h

set PATH=%PATH%;bin

minitar -c -z9 ../Springhead2.tgz *.*
cd ..
cd ..

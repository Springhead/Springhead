if exist pack\springhead rmdir /S /Q pack\springhead
if not exist pack mkdir pack
cd pack
svn export svn+ssh://sprsvn/export/spr/svn/repository/Springhead/trunk ./Springhead

rmdir /S /Q springhead\src\samples\data\Tachikoma
cd springhead

minitar -c -z9 ../Springhead.tgz *.*
cd ..
cd ..

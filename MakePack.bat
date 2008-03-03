if exist pack\springhead2 rmdir /S /Q pack\springhead2
if not exist pack mkdir pack
cd pack
svn export .. ./Springhead2

cd springhead2

minitar -c -z9 ../Springhead2.tgz *.*
cd ..
cd ..

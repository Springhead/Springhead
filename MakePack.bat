if exist pack\springhead2 del /S /Q /F pack\springhead2
if not exist pack mkdir pack
cd pack
svn export svn+ssh://springhead/home/svn/spr2/Springhead2/trunk ./Springhead2

cd springhead2

minitar -c -z9 ../Springhead2.tgz *.*
cd ..
cd ..

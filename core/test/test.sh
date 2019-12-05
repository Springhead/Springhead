#!/bin/bash -f

pushd ../..
_cwd=`pwd`
_words=`echo $_cwd | sed 's:/: :g'`
popd

SPRTOP=
for _word in $_words
do
	echo $_word
	SPRTOP=$_word
done

echo "DailyBuild.sh -U $SPRTOP DailyBuildResult/Result"
./DailyBuild.sh -U $SPRTOP DailyBuildResult/Result

exit 0

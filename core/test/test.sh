#!/bin/bash -f

export DAILYBUILD_UPDATE_SPRINGHEAD=skip
export DAILYBUILD_CLEANUP_WORKSPACE=skip

export DAILYBUILD_ELIMINATE_CLOSED=exec
export DAILYBUILD_EXECUTE_STUBBUILD=exec
export DAILYBUILD_EXECUTE_BUILDRUN=exec
export DAILYBUILD_EXECUTE_SAMPLEBUILD=exec

export DAILYBUILD_GEN_HISTORY=skip
export DAILYBUILD_COMMIT_RESULTLOG=skip
export DAILYBUILD_COPYTO_BUILDLOGskip
export DAILYBUILD_EXECUTE_MAKEDOC=skip
export DAILYBUILD_COPYTO_WEBBASEskip

DefFile=SprEnvDef.sh
if [ -f $DefFile ]; then
        . $DefFile
else
	echo "$DefFile not found"
	exit 1
fi

CONF=Release
PLAT=x64
GCCV=5.4.0
VRBS=0
TEST_REPOSITORY=SpringheadTest
DAILYBUILD_RESULT=DailyBuildResult/Result

cd ../../../$TEST_REPOSITORY/core/test
echo test directory:  `pwd`
echo test started at: `date -R`

OPTS="-c $CONF -p $PLAT"
cmnd="python TestMainGit.py $OPTS $TEST_REPOSITORY $DAILYBUILD_RESULT"
echo $cmnd
$cmnd

echo test ended at: `date -R`

exit 0

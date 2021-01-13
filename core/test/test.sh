#!/bin/bash -f

# Parse command line arguments.
#
FLAGS=
Sflag=
while [ "$1" != "" ]
do
    case "$1" in
	"-S")	Sflag=-S;;
	*)	FLAGS="$FLAGS $1";;
    esac
    shift
done

# Set test environment.
#
export DAILYBUILD_UPDATE_SPRINGHEAD=skip
export DAILYBUILD_CLEANUP_WORKSPACE=skip

export DAILYBUILD_ELIMINATE_CLOSED=exec
export DAILYBUILD_EXECUTE_STUBBUILD=exec
export DAILYBUILD_EXECUTE_EMBPYTHON=exec
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

# Setup if $Sflag speciried.
#
if [ "$Sflag" = "-S" ]; then
	echo "Execute setup process."
	pushd ../src
	./setup.sh -f
	if [ $? -eq 0 ]; then
		echo "Setup done"
	else
		echo "Setup failed"
		exit -1
	fi
	popd 
fi

# Test start.
#
CONF=Release
PLAT=x64
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

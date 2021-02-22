#! /bin/sh

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
StartDir=$HOME/Project/Springhead/core/test
DefFile=$StartDir/SprEnvDef.sh

if [ -f $DefFile ]; then
	. $DefFile
fi

# Use -A option to skip following steps.
#	Update Springhead to HEAD status - git pull.
#	Cleanup SpringheadTest - rm and git clone.

TEST_REPOSITORY=SpringheadTest
DAILYBUILD_RESULT=DailyBuildResult/Result

cd $StartDir
echo "python DailyBuild.py -u $FLAGS $TEST_REPOSITORY $DAILYBUILD_RESULT"
python DailyBuild.py -u $FLAGS $TEST_REPOSITORY $DAILYBUILD_RESULT
echo
echo "python DailyBuild.py -U $Sflag $FLAGS $TEST_REPOSITORY $DAILYBUILD_RESULT"
python DailyBuild.py -U $Sflag $FLAGS $TEST_REPOSITORY $DAILYBUILD_RESULT

exit 0

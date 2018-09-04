#! /bin/sh

# Set test environment.

ENVDEF="SprPath.sh"

if [ -f ${ENVDEF} ]; then
	source ${ENVDEF}
fi

# Use -A option to skip following steps.
#	Update Springhead to HEAD status - git pull.
#	Cleanup SpringheadTest - rm and git clone.

TEST_REPOSITORY=SpringheadTest
DAILYBUILD_RESULT=DailyBuildResult/Result

python DailyBuild.py -u $* $TEST_REPOSITORY $DAILYBUILD_RESULT
echo
python DailyBuild.py -U $* $TEST_REPOSITORY $DAILYBUILD_RESULT

exit 0

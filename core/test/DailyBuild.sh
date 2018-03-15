#! /bin/sh

# Use -A option to skip following steps.
#	Update Springhead to HEAD status - git pull.
#	Cleanup SpringheadTest - rm and git clone.

TEST_REPOSITORY=SpringheadTest
python DailyBuild.py $* $TEST_REPOSITORY

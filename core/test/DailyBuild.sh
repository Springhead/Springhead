#! /bin/sh

# Set test environment.
export SPR_PYTHON=/usr/local/bin/python
export SPR_TOP=$HOME/Project/Springhead
export SPR_DEPENDENCY=$SPR_TOP/dependency
export SPR_EXTLIBS=$HOME/ExtLibs
export SPR_BOOST_INC=$SPR_EXTLIBS/boost-numeric-bindings-R86799
export SPR_GL_INC=$SPR_DEPENDENCY/src

# Use -A option to skip following steps.
#	Update Springhead to HEAD status - git pull.
#	Cleanup SpringheadTest - rm and git clone.

TEST_REPOSITORY=SpringheadTest
DAILYBUILD_RESULT=DailyBuildResult/Result

python DailyBuild.py -u $* $TEST_REPOSITORY $DAILYBUILD_RESULT
echo
python DailyBuild.py -U $* $TEST_REPOSITORY $DAILYBUILD_RESULT

exit 0

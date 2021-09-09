#!/bin/bash -f

# Function definition
#
function usage() {
	echo "Usage: $0 [--do-not-clone] [--hook]"
}

# Parse command line arguments.
#
CLONE=""
HOOK=""
HOOKONLY=""
while [ "$1" != "" ]
do
    case "$1" in
        "--hook")	  HOOK="--hook";;
        "--hook-only")	  HOOKONLY="--hook-only";;
        "--do-not-clone") CLONE="--do-not-clone";;
	*)		  usage; exit 0;;
    esac
    shift
done

# Check
#
if [ "$CLONE" == "" ]; then
	read -p "cloning from remote repository ... ok [y/n]? " yn
	if [ "$yn" != "y" ]; then
		usage
		echo "abort"
		exit 1
	fi
fi

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

# Execute test
#
CMND="./DailyBuild.sh -S $CLONE $HOOK $HOOKONLY"
echo $CMND
$CMND
echo "rc $?"

exit 0


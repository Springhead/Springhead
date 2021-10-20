#!/bin/bash
# =============================================================================
#  SYNOPSIS
#	showresult [options]
#
#  OPTIONS
#	-l dir,  --log dir		Log directory name [log].
#	-o file, --outfile file		Output file name [stdout].
#	-r file, --resultfile file	Test result file name [result.log].
#	-t dir, --topdir dir		Top directory name of source tree.
#
#  DESCRIPTION
#	Convert format of "result.log" to a little readble.
#	Adapter of ShowResult.py.
#
# ---------------------------------------------------------------------
#  VERSION
#     Ver 1.0	 2021/09/27 F.Kanehori	Converted from DOIS batch.
# =============================================================================
PROG=`basename $0`
SCRIPTDIR=$(cd $(dirname $0);pwd)

LOGDIR=log
OUTFILE=stdout
RESULTFILE=result.log
TOPDIR=.
VERBOSE=

for OPT in "$@"
do
    case $OPT in
	-l | --log)
	    LOGDIR=$2
	    shift 2
	    ;;
	-o | --outfile)
	    OUTFILE=$2
	    shift 2
	    ;;
	-r | --resultfile)
	    RESULTFILE=$2
	    shift 2
	    ;;
	-t | --topdir)
	    TOPDIR=$2
	    shift 2
	    ;;
	-v | --verbose)
	    VERBOSE=$1
	    shift
	    ;;
	-h | --help)
	    python $SCRIPTDIR/ShowResult.py --help
	    exit 0
	    ;;
	-- | -)
            shift 1
            param+=( "$@" )
            break
            ;;
        -*)
            echo "$PROG: illegal option -- '$(echo $1 | sed 's/^-*//')'" 1>&2
            exit 1
            ;;
        *)
            if [[ ! -z "$1" ]] && [[ ! "$1" =~ ^-+ ]]; then
                param+=( "$1" )
                shift 1
            fi
            ;;
    esac
done

ARGS="$VERBOSE -t $TOPDIR -l $LOGDIR -r $RESULTFILE -o $OUTFILE"
python $SCRIPTDIR/ShowResult.py $ARGS $param

exit 0

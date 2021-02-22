#!/bin/bash
# ============================================================================
#  SYNOPSIS
#       buildswig [-f | -F]
#         -c		make clean && make を実行する
#
#  DESCRIPTION
#       swig バイナリを生成する
#       Makefile がないときは configure から実行する
# ----------------------------------------------------------------------------
#  VERSION
#       Ver 1.0  2020/11/16 F.Kanehori  改定初版
# ============================================================================
PROG=`basename $0 .sh`
DEBUG=0

# Directories
#
SPRTOP=../../../..
COREDIR=${SPRTOP}/core
SWIGDIR=${COREDIR}/bin/src/swig

# Get command line options
#
clean_flag=false
if [ "$1" = "-c" ]; then
	clean_flag=true
	shift
fi

# Change directory to ${SWIGDIR}
#
startdir=`pwd`
cd ${SWIGDIR}

# Execute configure if Makefile does not exist
#
if [ ! -e Makefile ]; then
	chmod +x autogen.sh configure
	touch NEWS README AUTHORS ChangeLog
	autoreconf -i 2>&1 | tee autoconf.log
	./autogen.sh 2>&1 | tee autogen.log
	./configure CPPFLAGS=-Wwrite-strings 2>&1 | tee configure.log
	clean_flag=true
fi

# Clean up old objects
#
if [ $clean_flag = true ]; then
	make --no-print-directory clean 2>&1 
fi

# Execute make
#
make --no-print-directory 2>&1 | tee make.log

# Return to start directory
#
cd ${startdir}

exit 0
# end: buildwig.sh

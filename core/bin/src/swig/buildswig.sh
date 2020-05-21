#!/bin/bash

SPRTOP=../../../..
COREDIR=${SPRTOP}/core
SWIGDIR=${COREDIR}/bin/src/swig

# Change directory to ${SWIGDIR}
#
startdir=`pwd`
cd ${SWIGDIR}
chmod +x autogen.sh configure
touch NEWS README AUTHORS ChangeLog
autoreconf -i > autoconf.log 2>&1
./autogen.sh > autogen.log 2>&1
./configure CPPFLAGS=-Wwrite-strings > configure.log 2>&1
make 2>&1 | tee make.log
cd ${startdir}

exit 0

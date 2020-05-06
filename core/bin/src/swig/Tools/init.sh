#!/bin/bash

SRC=./Source/Modules

cp $SRC/swigmain.cxx-unix $SRC/swigmain.cxx

#cp $SRC/swigmain-sprpy.cxx $SRC/swigmain.cxx
#cp $SRC/swigmain-sprpy-sprcs.cxx $SRC/swigmain.cxx

chmod +x ./autogen.sh
chmod +x ./compile
chmod +x ./config.status
chmod +x ./configure
chmod +x ./depcomp
chmod +x ./install-sh
chmod +x ./missing
chmod +x ./shtool
chmod +x ./ylwrap


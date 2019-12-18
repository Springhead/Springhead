#!/bin/sh

# Use python version 3.4.
#
export PATH="/usr/local/bin:$PATH"
export SPR_PYTHON=/usr/local/bin/python

# Additional include and library path.
#
export SPR=$HOME/Project/Springhead
export SPR_DEPENDENCY=$SPR/dependency
export SPR_EXTLIBS=$HOME/ExtLibs

export SPR_BOOST_INC=$SPR_EXTLIBS/boost-numeric-bindings-R86799
export SPR_GL_INC=$SPR_DEPENDENCY/src
export SPR_LINUX_GNU_LIB=/usr/lib/x86_64-linux-gnu
export SPR_LINUX_GCC_LIB=/usr/lib/gcc/x86_64-linux-gnu/5

#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  SYNOPSIS:
#	EmbPython_clean
#
#  DESCRIPTION:
#	RunSwig_EmbPython が生成したファイルを削除する.
#
# ==============================================================================
#  Version:
#	Ver 1.0	 2020/02/26 F.Kanehori	Windows batch file から移植.
# ==============================================================================
version = 1.0

import sys
import os

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]

# ----------------------------------------------------------------------
#  Import Springhead python library.
#
sys.path.append('../RunSwig')
from FindSprPath import *
spr_path = FindSprPath(prog)
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from FileOp import *

# ----------------------------------------------------------------------
#  処理開始
#
targets	= [ "Base", "Collision", "Creature", "FileIO", "Foundation",
	    "Framework", "Graphics", "HumanInterface", "Physics" ]

fop = FileOp()
for target in targets:
	fop.rm('EP%s.cpp' % target)
	fop.rm('EP%s.h' % target)
	fop.rm('Makefile_EmbPython.swig.%s' % target)
	fop.rm('swig_%s.log' % target)

# ----------------------------------------------------------------------
#  終了
#
sys.exit(0)

# end: EmbPython_clean.py

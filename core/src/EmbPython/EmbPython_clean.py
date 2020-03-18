#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	python EmbPython_clean.py [options]
#	options
#	    -v		set verbose mode.
#
#  DESCRIPTION:
#	EunSwig_EmbPython が生成するファイルを削除する。
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2019/08/28 F.Kanehori	First version.
#	Ver 1.01 2019/09/11 F.Kanehori	Add include directory to target.
# ======================================================================
version = 1.0

import sys
import os
import re
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]

# ----------------------------------------------------------------------
#  Helper methods
#
def remove(fname):
	if not os.path.exists(fname):
		return
	if verbose:
		print('  removing %s' % fname)
	os.remove(fname)

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options]'
parser = OptionParser(usage = usage)
#
parser.add_option('-v', '--verbose', dest='verbose',
			action='count', default=0,
			help='set verbose mode')
parser.add_option('-V', '--version', dest='version',
			action='store_true', default=False,
			help='show version')
#
(options, args) = parser.parse_args()
if options.version:
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
verbose = options.verbose

# ----------------------------------------------------------------------
#  Main program
#
projs = ['Base', 'Collision', 'Creature', 'FileIO', 'Foundation',
	 'Framework', 'Graphics', 'HumanInterface', 'Physics']

for proj in projs:
	remove('EP%s.cpp' % proj)
	remove('EP%s.h' % proj)
	remove('Makefile_EmbPython.swig.%s' % proj)
	remove('swig_%s.log' % proj)
	remove('../../include/EmbPython/SprEP%s.h' % proj)

sys.exit(0)

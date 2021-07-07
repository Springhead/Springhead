#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# =============================================================================
#  SYNOPSIS:
#	MakeDoc [options]
#	options:
#	    -v:		    Set verbose level (0: silent).
#	    -V:		    Show version.
#
#  DESCRIPTION:
#	Generate "CMakeGitbook" manual.
#
# -----------------------------------------------------------------------------
#  VERSION:
#     Ver 1.0	 2020/04/01 F.Kanehori	First version.
#     Ver 1.1	 2021/07/07 F.Kanehori	DailyBuildTestTools 導入.
# =============================================================================
version = '1.1'

import sys
import os
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]

# ----------------------------------------------------------------------
#  このスクリプトは ".../core/doc/CMakeGitbook" に置く	
#
ScriptFileDir = os.sep.join(os.path.abspath(__file__).split(os.sep)[:-1])
prog = sys.argv[0].replace('/', os.sep).split(os.sep)[-1].split('.')[0]
TopDir = '/'.join(ScriptFileDir.split(os.sep)[:-3])
SrcDir = '%s/core/src' % TopDir

# ----------------------------------------------------------------------
#  Import Springhead python library.
#
libdir = '%s/RunSwig/pythonlib' % SrcDir
sys.path.append(libdir)
from Util import *
from Proc import *

# ----------------------------------------------------------------------
#  Programs
#
python = 'python'

# ----------------------------------------------------------------------
#  Paths
#
gitbookpath = '%s%s\\AppData\\Roaming\\npm' % \
		(os.environ['HOMEDRIVE'], os.environ['HOMEPATH'])

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options]'
parser = OptionParser(usage = usage)
parser.add_option('-v', '--verbose', dest='verbose',
			action='count', default=0,
			help='set verbose mode')
parser.add_option('-D', '--dry-run', dest='dry_run',
			action='store_true', default=False,
			help='set dry-run mode')
parser.add_option('-V', '--version', dest='version',
			action='store_true', default=False,
			help='show version')

# ----------------------------------------------------------------------
#  Process for command line
#
(options, args) = parser.parse_args()
if options.version:
	print('%s: Version %s' % (prog, str(version)))
	sys.exit(0)
if len(args) != 0:
	parser.error("incorrect number of arguments")

# get options
verbose = options.verbose
dry_run = options.dry_run

# ----------------------------------------------------------------------
#  Process start.
#
outdir = '../../../generated/doc/CMakeGitbook/'
cmnd = '%s\\gitbook.cmd build ./ %s' % (gitbookpath, outdir)
#print('cmnd: %s' % cmnd)

proc = Proc(dry_run=dry_run, verbose=verbose)
stat = proc.execute(cmnd).wait()
if stat == 0:
	print('%s: CMakeGitbook document generated.' % prog)

sys.exit(0)

# end: MakeDoc.py

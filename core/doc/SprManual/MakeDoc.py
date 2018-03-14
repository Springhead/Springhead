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
#	Generate Springehad reference manual.
#
# -----------------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2018/02/21 F.Kanehori	First version.
#	Ver 1.01 2018/03/14 F.Kanehori	Dealt with new Proc class.
# =============================================================================
version = 1.01

import sys
import os
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]

# ----------------------------------------------------------------------
#  Import Springhead python library.
#
sys.path.append('../../src/RunSwig')
from FindSprPath import *
spr_path = FindSprPath('SpringheadTest')
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from Util import *
from Proc import *

# ----------------------------------------------------------------------
#  Programs
#
make = 'make' if Util.is_unix() else 'nmake'

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
curr_date = Util.date(format='%Y-%m%d')
curr_time = Util.time(format='%H%M')
opts = 'DATE=%s TIME=%s' % (curr_date, curr_time)
cmnd = '%s %s' % (make, opts)

proc = Proc(dry_run=dry_run, verbose=verbose)
stat = proc.execute(cmnd).wait()
if stat == 0:
	print('%s: SprManual generated.' % prog)

# ----------------------------------------------------------------------
#  Clean up.
#
cmnd = '%s clean' % make
proc = Proc(dry_run=dry_run, verbose=verbose)
stat = proc.execute(cmnd).wait()

sys.exit(0)

# end: MakeDoc.py

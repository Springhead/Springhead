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
#  *TODO*
#	We've not succeeded to make correct pdf file ("SprManual.pdf") with
#	texlive 2018 until now.  So we use texlive 2018 only for make html
#	version of SprManual ("main_html.html") and still use texlive 2014
#	to make pdf version.  We should resolve this problem!
#
# -----------------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2018/02/21 F.Kanehori	First version.
#	Ver 1.1  2018/12/06 F.Kanehori	Now both pdf and html version available.
# =============================================================================
version = 1.1

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
python = 'python'
make_pdf = 'make' if Util.is_unix() else 'nmake'
make_html = 'buildhtml.py'

# ----------------------------------------------------------------------
#  Paths
#
texlive2014_path = 'C:/texlive/2014/bin/win32'
texlive2018_path = 'C:/texlive/2018/bin/win32'

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
#  (1) create pdf version.
curr_date = Util.date(format='%Y-%m%d')
curr_time = Util.time(format='%H%M')
opts = 'DATE=%s TIME=%s' % (curr_date, curr_time)
cmnd = '%s %s' % (make_pdf, opts)
addpath = texlive2014_path

proc = Proc(dry_run=dry_run, verbose=verbose)
stat = proc.execute(cmnd, addpath=addpath).wait()
if stat == 0:
	print('%s: SprManual.pdf generated.' % prog)

# (2) create html version.
opts = '-v -E -K -R -c'
cmnd = '%s %s %s main_html.tex' % (python, make_html, opts)
addpath = texlive2018_path

proc = Proc(dry_run=dry_run, verbose=verbose)
stat = proc.execute(cmnd, addpath=addpath).wait()
if stat == 0:
	print('%s: SprManual.html generated.' % prog)

# ----------------------------------------------------------------------
#  Clean up.
#
cmnd = '%s clean' % make_pdf
addpath = texlive2014_path
proc = Proc(dry_run=dry_run, verbose=verbose)
stat = proc.execute(cmnd, addpath=addpath).wait()

sys.exit(0)

# end: MakeDoc.py

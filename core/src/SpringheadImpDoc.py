#!/usr/bin/env python
# -*- coding: utf-8 -*-
# =============================================================================
#  SYNOPSIS:
#	SpringheadImpDoc [options]
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
# =============================================================================
version = 1.0

import sys
import os
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
doxy_file = 'springheadImp.doxy'
target_name = 'Developper'

# ----------------------------------------------------------------------
#  Import Springhead python library.
#
sys.path.append('../src/RunSwig')
from FindSprPath import *
spr_path = FindSprPath('SpringheadTest')
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from Error import *
from Util import *
from Proc import *
from FileOp import *

# ----------------------------------------------------------------------
#  Directories.
#
rootdir = spr_path.relpath()
coredir = spr_path.relpath('core')
testdir = spr_path.relpath('test')
tooldir = '%s/buildtool' % rootdir
dpt_dir = '%s/dependency/src' % rootdir
out_dir = '%s/generated/doc' % rootdir
target_dir = '%s/%s' % (out_dir, target_name)
js_dir = '%s/js' % target_dir
#
path_list = [
	'%s/bin' % coredir,
	tooldir,
	'%s/Graphviz/bin' % tooldir,
	'%s/bin' % testdir
]
addpath = Util.pathconv(os.pathsep.join(path_list))
wrkdir = 'HTML'

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
if verbose:
	print('  tooldir:    %s' % tooldir)
	print('  dpt_dir:    %s' % dpt_dir)
	print('  out_dir:    %s' % out_dir)
	print('  target_dir: %s' % target_dir)
	print('  js_dir:     %s' % js_dir)
	print('  addpath:    %s' % addpath)

# for global use
fop = FileOp(dry_run=dry_run, verbose=verbose)

# ----------------------------------------------------------------------
#  Make HTML files.
#
if not os.path.exists(wrkdir):
	os.mkdir(wrkdir)
#
overrides = list(map(lambda x: 'echo %s' % x, [
	'OUTPUT_DIRECTORY=%s' % out_dir,
	'GENERATE_HTMLHELP=',
	'HHC_LOCATION='
]))
cmnd1 = 'cmd /c type %s &%s' % (doxy_file, '&'.join(overrides))
cmnd2 = 'doxygen -'
log_file = 'doxgen.log'
#
proc1 = Proc(verbose=verbose, dry_run=dry_run)
proc2 = Proc(verbose=verbose, dry_run=dry_run)
proc1.exec(cmnd1, addpath=addpath, stdout=Proc.PIPE)
proc2.exec(cmnd2, addpath=addpath,
		  stdin=proc1.proc.stdout, stderr=log_file)
stat1 = proc1.wait()
stat2 = proc2.wait()
if stat2 != 0:
	msg = 'making html files failed.'
	Error(prog).print(msg, alive=True)
#
if os.path.exists(target_dir):
	fop.rm('%s/*' % target_dir, recurse=True)
	os.rmdir(target_dir)
fop.mv('%s/%s' % (out_dir, wrkdir), target_name)
fop.cp('%s/MathJax.js' % dpt_dir, js_dir)
#
if os.path.exists(wrkdir):
	fop.rm(wrkdir, recurse=True)
	os.rmdir(wrkdir)
files = glob.glob('doxygen*.tmp')
for f in files:
	fop.rm(f, force=True)
print('Deocument generate at "%s".' % target_dir)

sys.exit(0)

# end: SpringheadImpDoc.py

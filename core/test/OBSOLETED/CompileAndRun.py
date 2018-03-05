#!/usr/bin/env python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	CompileAndRun [options]
#	options:
#	  -c cfname:	Control file name <"dailybuild.control">.
#	  -d:		Dry run <False>.
#	  -v:		Verbose level <0: silect>.
#	  -V:		Print version information <False>.
#
#  DESCRIPTION:
#
#  VERSION:
#	Ver 1.0  2017/12/07 F.Kanehori	V‹Kì¬.
# ======================================================================
version = '1.0'
import sys
import os
from optparse import OptionParser
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]

# ----------------------------------------------------------------------
#  Local python library
#
sys.path.append('../src/RunSwig')
from FindSprPath import *
spr_path = FindSprPath(prog)
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from Proc import *
from Util import *
from Error import *

# ----------------------------------------------------------------------
#  Constants
#
bindir = 'bin'
script = 'BatchTestControl.py'

toolset = '14.0'
platform = 'x64'
config = 'Release'
targets = ['src', 'src/tests', 'src/Samples']
section = 'dailybuild'

# ----------------------------------------------------------------------
#  Globals
#
cwd = os.getcwd()
spr_topdir = spr_path.abspath()
testdir = spr_path.abspath('test')
proc = Proc(verbose=0)
err = Error(prog)

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: python %prog [options]'
parser = OptionParser(usage = usage)
parser.add_option('-c', '--control-file',
			dest='cfname', default='dailybuild.control',
			help='control file name (default: "%default")')
parser.add_option('-d', '--dry-run',
			dest='dryrun', action='store_true', default=False,
			help='dry run')
parser.add_option('-v', '--verbose',
			dest='verbose', action='count', default=0,
			help='set verbose mode')
parser.add_option('-V', '--version',
			dest='version', action='store_true', default=False,
			help='show version')

# ----------------------------------------------------------------------
#  Process for command line
#
(options, args) = parser.parse_args()
if options.version:
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
if len(args) > 0:
	err.print('incorrect number of arguments\n', prompt='error')
	proc.exec('python %s.py -h' % prog).wait()
	sys.exit(-1)
cfname = options.cfname
verbose = options.verbose

if verbose:
	print('  control-file:	%s' % cfname)
	print('  target:	%s' % target)

# ----------------------------------------------------------------------
#  Invoke test controller.
#
args = '-P %s -C %s -u' % (platform, config)
args += ' -f %s -s %s' % (cfname, section)
if Util.is_windows():
	args += ' -t %s' % toolset

os.chdir('%s/%s' % (testdir, bindir))
for target in targets:
	tmpargs = args
	if target == 'src':
		tmpargs += ' -i'
	tmpargs += ' -d %s' % target
	cmnd = 'python %s %s' % (script, tmpargs)
	if options.dryrun:
		print(cmnd)
		continue
	status = proc.exec(cmnd).wait()
	if status != 0:
		err.print(msg, prompt='error')

os.chdir(cwd)

# ----------------------------------------------------------------------
#  Process end.
#
sys.exit(0)

# end: CompileAndRun.py

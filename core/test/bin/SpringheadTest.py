#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# =============================================================================
#  SYNOPSIS:
#	SpringheadTest [options] test-dir res-file ctl-file [section]
#	arguments:
#	    test-dir:	Test directory (relative to ".../core/src").
#	    res-file:	Test result file path (relative to ".../core/test").
#	    ctl-file:	Test control file name (leaf name).
#	    section:	Test control section name.
#	options:
#	    -c CONFIGS:	    Configurations to be tested (CONFS).
#	    -p PLATFORMS:   Platforms to be tested (PLATS).
#	    -r:		    Force rebuild.
#	    -s:		    No progress report.
#	    -t TOOLSET:	    C-compiler version.
#				Windows: Visual Studio version (str).
#				unix:    gcc version (dummy).
#	    -C USAGE:	    Closed src usage (auto <default>, use, unuse).
#	    -D:		    Dry-run mode.
#	    -S:		    Start result from scratch.
#	    -T TIMEOUT:	    Set timeout value in sec (0 for not set).
#			    This override to the coontrol file.
#	    -v:		    Set verbose level (0: silent).
#	    -V:		    Show version.
#
#	* see "ConstDefs.py" for CONFS and PLATS.
#
#  DESCRIPTION:
#	Test program for Splinghead library and its applications.
#	This program was intended to run 'dailybuild' test promarily, but is
#	extended to test all cases; i.e. all combination of platforms and
#	configurations.
#	Each test is controlled by control-file (e.g. "dailybuild.control")
#	and arguments to this program.
#
# -----------------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2016/11/17 F.Kanehori	First version.
#	Ver 2.0  2018/02/22 F.Kanehori	全体の見直し.
#	Ver 2.01 2018/03/14 F.Kanehori	Dealt with new Error class.
#	Ver 2.02 2018/03/22 F.Kanehori	Change date/time info format.
# =============================================================================
version = 2.01

import sys
import os
import glob
import pickle
from optparse import OptionParser
import signal

from ConstDefs import *
from ControlParams import *
from Traverse import *
from TestResult import *
from ClosedSrcControl import *
from KeyInterruption import *

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
date_format = '%Y/%m/%d %H:%M:%S'

# ----------------------------------------------------------------------
#  Import Springhead python library.
#
sys.path.append('../../src/RunSwig')
from FindSprPath import *
spr_path = FindSprPath('SpringheadTest')
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from Error import *
from Util import *
from Proc import *
from FileOp import *
from TextFio import *

# ----------------------------------------------------------------------
#  Simple helpers.
#
def make_list(obj, default=None):
	if obj is None:
		if default is None:
			return []
		return make_list(default)
	if isinstance(obj, list):
		newobj = []
		for elm in obj:
			for item in elm.split(','):
				newobj.append(item)
		return newobj
	return [obj]


# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options] '\
	+ 'test-dir res-file ctl-file [section]\n'\
	+ '    test-dir:\ttest directory (relative to ".../core/src")\n'\
	+ '    res-file:\ttest result file path (relative to ".../core/test")\n'\
	+ '    ctl-file:\ttest control file name (leaf name)\n'\
	+ '    section:\ttest control section name'
parser = OptionParser(usage = usage)
parser.add_option('-a', '--audit', dest='audit',
			action='store_true', default=False,
			help='audit trail [default: %default]')
parser.add_option('-c', '--configs', dest='configs',
			action='append', default=None,
			help='configurations to be tested')
parser.add_option('-p', '--platforms', dest='platforms',
			action='append', default=None,
			help='platforms to be tested')
parser.add_option('-r', '--rebuild',
			dest='rebuild', action='store_true', default=False,
			help='force rebuild')
parser.add_option('-s', '--silent', dest='silent',
			default=False,
			help='no progress report [default: %default]')
parser.add_option('-t', '--toolset', dest='toolset',
			default='14.0',
			help='Visual Studio version [default: %default]')
parser.add_option('-C', '--closed-src-usage', dest='closed_src_usage',
			default='auto',
			help="closed src usage (auto, use or unuse) [default: '%default']",
			metavar='USAGE')
parser.add_option('-D', '--dry-run', dest='dry_run',
			action='store_true', default=False,
			help='set dry-run mode [default: %default]')
parser.add_option('-S', '--scratch', dest='scratch',
			action='store_true', default=False,
			help='start result from scratch')
parser.add_option('-T', '--timeout', dest='timeout',
			type='int', default=0,
			help='set timeout (0 for not set) [default: %default]',
			metavar='SEC')
parser.add_option('-v', '--verbose', dest='verbose',
			action='count', default=0,
			help='set verbose mode')
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
if len(args) != 4:
	parser.error("incorrect number of arguments")

# get arguments
test_dir = args[0]
res_file = '%s/%s' % (spr_path.abspath('test'), args[1])
ctl_file = args[2]
section = args[3]

top = Util.pathconv(test_dir)
if not os.path.exists(top):
	top = '%s/%s' % (spr_path.abspath('src'), top)
	if not os.path.exists(top):
		Error(prog).abort('bad test directory: "%s"' % test_dir)
top = Util.upath(top)

# get options
toolset = options.toolset
platforms = make_list(options.platforms, PLATS)
configs = make_list(options.configs, CONFS)
csusage = options.closed_src_usage
csusage_list = {'auto': CSU.AUTO, 'use': CSU.USE, 'unuse': CSU.UNUSE}
if csusage not in csusage_list.keys():
	Error(prog).abort('invalid option: %s' % csusage)
csusage = csusage_list[csusage]
rebuild= options.rebuild
timeout= options.timeout
report = not options.silent
audit = options.audit
dry_run = options.dry_run
verbose = options.verbose
scratch = options.scratch

if verbose:
	print('SpringheadTest:')
	print('  test directory: %s' % test_dir)
	print('  results file:   %s' % res_file)
	print('  control file:   %s' % ctl_file)
	print('  section name:   %s' % section)
	print('  toolset:        %s' % toolset)
	print('  platform:       %s' % platforms)
	print('  config:         %s' % configs)
	print('  cs usage:       %s' % csusage)
	print('  rebuild:        %s' % rebuild)
	print('  timeout:        %s' % timeout)
	print('  report:         %s' % report)
	print('  audit:          %s' % audit)
	print('  dry_run:        %s' % dry_run)
	print('  scratch:        %s' % scratch)

# ----------------------------------------------------------------------
#  Test start.
#
print()
print('test start at: %s' % Util.now(format=date_format))
print('with args: %s' % args)

# test id
#	'Test_id' affects only log file header.
testid = TESTID.OTHER
src_path = Util.upath(spr_path.abspath('src'))
if test_dir == '.':
	test_path = spr_path.abspath('src')
else:
	test_path = Util.upath(os.path.abspath(test_dir))
if test_path.lower() == src_path.lower():
	testid = TESTID.STUB
if 'tests' in test_path:
	testid = TESTID.TESTS
if 'Samples' in test_path:
	testid = TESTID.SAMPLES

# move to test directory
cwd = os.getcwd()
topdir = '%s/%s' % (src_path, Util.upath(test_dir))
os.chdir(Util.pathconv(topdir))

# preparations
head_dir = spr_path.abspath('inc')
tmpl_dir = '%s/bin' % spr_path.abspath('test')
csc_head = '%s/SprUseClosedSrcOrNot.h' % head_dir
use_tmpl = '%s/UseClosedSrc.h.template' % tmpl_dir
unuse_tmpl = '%s/UnuseClosedSrc.h.template' % tmpl_dir

if scratch:
	print('scratch result file')
res = TestResult(res_file, scratch, verbose=1)
csc = ClosedSrcControl(csc_head, use_tmpl, unuse_tmpl, dry_run, verbose)

# traverse start
trv = Traverse(testid,
		res, csc, ctl_file, section,
		toolset, platforms, configs, csusage, rebuild,
		timeout, report, audit, dry_run, verbose)
signal.signal(signal.SIGINT, KEYINTR.handler)
stat = trv.traverse(top)
res.finish()
csc.revive()

# back to start directory and make "result.log".
os.chdir(cwd)
cmnd = 'python GenResultLog.py'
outf = '-o ../log/result.log'
args = 'r %s %s %s' % (res_file, platforms[0], configs[0])
print(' '.join([cmnd, outf, args]))
proc = Proc(dry_run=options.dry_run, verbose=options.verbose)
proc.execute([cmnd, outf, args], shell=True).wait()

# done
print('test ended at: %s' % Util.now(format=date_format))
sys.exit(0)

# end: SpringheadTest.py

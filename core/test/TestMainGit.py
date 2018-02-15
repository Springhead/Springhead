#!/usr/bin/env python
# -*- coding: utf-8 -*-
# =============================================================================
#  SYNOPSIS:
#	TestMainGit [options]
#	options:
#	    -c CONFIG:	    Configuration to test (Debug, Release or Trace).
#	    -p PLATFORM:    Platform to test (x86, x64)
#	    -t TOOLSET:	    C-compiler version.
#				Windows: Visual Studio version (str).
#				unix:    gcc version (dummy).
#	    -v:		    Set verbose level (0: silent).
#	    -V:		    Show version.
#
#  DESCRIPTION:
#	Execute dailybuild test.
#
# -----------------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2018/02/15 F.Kanehori	First version.
# =============================================================================
version = 1.0

import sys
import os
import glob
from time import sleep
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]

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
from TextFio import *

# ----------------------------------------------------------------------
#  Local methods.
#
def make_list(obj, default=None):
	# Make simple list of objects.
	# Arguments 'obj' may be comma separated names or list of names.
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

def check_exec(name, os_type=True):
	# Return False only if environment variable 'name' is defined
	# and its value is 'skip'.  Return True otherwise.
	if not os_type:
		os_name = 'unix' if Util.is_unix() else 'Windows'
		print('do not exec ..%s.. by %s' % os_name)
		return False
	#
	val = os.getenv(name)
	judge = True if val is None or val != 'skip' else False
	if not judge:
		print('skip ..%s..' % name)
	return judge

def copy_dir(fop, dir, copyto):
	for root, dirs, files in os.walk(dir, topdown=False):
		for f in files:
			copy_file(fop, f, copyto)
		for d in dirs:
			os.makedirs(os.path.abspath(d))
			copy_dir(fop, d, copyto)

def copy_file(fop, f, copyto):
	leaf = fmpath.split('/')[-1]
	path = '%s/%s' % (copyto, leaf)
	print('  copying file "%s" to "%s"' % (f, path))
	fop.cp(f, path, force=True)

def remove_dir(dir):
	for root, dirs, files in os.walk(dir, topdown=False):
		for f in files:
			path = '%s/%s' % (root, f)
			os.chmod(path, stat.S_IWRITE)
			os.unlink(path)
		for d in dirs:
			path = '%s/%s' % (root, d)
			remove_dir(path)
	if Util.is_windows(): sleep(1)
	os.rmdir(dir)

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options] test-repositoy'
parser = OptionParser(usage = usage)
parser.add_option('-c', '--config', dest='config',
			action='append', default='Release',
			help='test configurations [default: %default]',
			metavar='CONFIG')
parser.add_option('-p', '--platform', dest='platform',
			action='append', default='x64',
			help='test platforms [default: %default]',
			metavar='PLATFORM')
parser.add_option('-t', '--toolset', dest='toolset',
			default='14.0',
			help='Visual Studio version [default: %default]')
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
if len(args) != 1:
	parser.error("incorrect number of arguments")

# get arguments
topdir = spr_path.abspath()
repository = '%s/../%s' % (topdir, args[0])
repository = Util.upath(os.path.abspath(repository))
if not os.path.isdir(repository):
	msg = '"%s" is not a directory' % repository
	Error(prog).print(msg)

# get options
toolset = options.toolset
platform = options.platform
config = options.config
if platform not in ['x86', 'x64']:
	msg = 'invalid platform name "%s"' % platform
	Error(prog).print(msg)
if config not in ['Debug', 'Release', 'Trace']:
	msg = 'invalid configuration name "%s"' % config
	Error(prog).print(msg)
verbose = options.verbose
dry_run = options.dry_run

print('Test parameters:')
if Util.is_windows():
	print('   toolset id:      [%s]' % toolset)
print('   platform:        [%s]' % platform)
print('   configuration:   [%s]' % config)
print('   test repository: [%s]' % repository)

# ----------------------------------------------------------------------
#  Go to test repository.
#
if not os.path.exists(repository):
	print('making test repository: [%s]' % repository)
	os.makedirs(repository, exist_ok=True)
start_dir = os.getcwd()
os.chdir(repository)

if not os.path.exists('core/test/bin'):
	msg = 'test repository "%s/core" may be empty' % repository
	Error(prog).print(msg)

# ----------------------------------------------------------------------
#  Test Go!
#
if check_exec('DAILYBUILD_EXECUTE_TESTALL'):
	os.chdir('core/test')
	#
	test_dirs = [
		['.', '-S'],	# Springhead library (need result init'ed)
		['tests'],	# src/tests
		['Samples']	# src/Samples
	]
	init_opts = ['-S', None, None]
	#
	csusage = 'unuse'	#  We do not use closed sources.
	cmnd = 'python bin/SpringheadTest.py'
	opts = '-p %s -c %s -C %s' % (platform, config, csusage)
	args = 'result/dailybuild.result dailybuild.control '
	if Util.is_unix():
		args += 'unix'
	else:
		args += 'Windows'
		opts += ' -t %s' % toolset
	proc = Proc(verbose=verbose, dry_run=dry_run)
	for tmp in test_dirs:
		t_opts = opts
		t_args = args
		if len(tmp) == 2:
			t_opts = '%s %s' % (opts, tmp[1])
		t_args = '%s %s' % (tmp[0], args)
		proc.exec('%s %s %s' % (cmnd, t_opts, t_args))
		stat = proc.wait()
		if (stat != 0):
			msg = 'test failed (%d)' % stat
			Error(proc).print(msg, exitcode=stat)
	#
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Make document (doxygen).
#
if check_exec('DAILYBUILD_EXECUTE_MAKEDOC', Util.is_windows()):
	print('making documents')
	#
	os.chdir('core/include')
	cmnd = 'python SpringheadDoc.py'
	cmnd = 'cmd /c SpringheadDoc.bat'
	proc = Proc(verbose=verbose, dry_run=dry_run)
	proc.exec(cmnd).wait()
	#
	os.chdir('../include')
	cmnd = 'python SpringheadImpDoc.py'
	cmnd = 'cmd /c SpringheadImpDoc.bat'
	proc = Proc(verbose=verbose, dry_run=dry_run)
	proc.exec(cmnd).wait()
	#
	os.chdir('../doc/SprManual')
	cmnd = 'python MakeDoc.py'
	cmnd = 'cmd /c make.bat'
	proc = Proc(verbose=verbose, dry_run=dry_run)
	proc.exec(cmnd).wait()
	#
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Copy files generated by dailybuild to Web.
#
if check_exec('DAILYBUILD_COPYTO_WEBBASE', Util.is_windows()):
	print('copying generated files to web')
	#
	docroot = '//haselab/HomeDirs/WWW/docroots'
	webbase = '%s/springhead/dailybuild/generated' % docroot
	#
	os.chdir('generated')
	glist = glob.glob('*')
	dlist = []
	flist = []
	for g in glist:
		if os.path.isdir(g):
			dlist.append(g)
		elif os.isfile(g):
			flist.append(g)
	print('  web base:  [%s]' % webbase)
	print('  dir list:  [%s]' % ' '.join(dlist))
	print('  file list: [%s]' % ' '.join(flist))
	fop = FileOp(dry_run=dry_run, verbose=verbose)
	for d in dlist:
		d_abs = Util.upath(os.path.abspath(d))
		remove_dir(d_abs)
		os.makedirs(d_abs)
		copy_dir(fop, d_abs, webbase)
	for f in flist:
		f_abs = Util.upath(os.path.abspath(f))
		copy_file(fop, f_abs, webbase)
	#
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Done.
#
sys.exit(0)

# end: TestMainGit.py

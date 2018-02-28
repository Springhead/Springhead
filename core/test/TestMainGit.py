#!/usr/bin/env python
# -*- coding: utf-8 -*-
# =============================================================================
#  SYNOPSIS:
#	TestMainGit [options]
#	options:
#	    -c CONF:	    Configuration to test (Debug, Release or Trace).
#	    -p PLAT:	    Platform to test (x86, x64)
#	    -t TOOLSET:	    C-compiler version.
#				Windows: Visual Studio version (str).
#				unix:    gcc version (dummy).
#	    -v:		    Set verbose level (0: silent).
#	    -V:		    Show version.
#
#  DESCRIPTION:
#	Execute dailybuild test.
#	On unix machine, some test steps' execution can be controlled by
#	following parameters;
#	    unix_gen_history:	    Generate "History.log" and "Test.date".
#	    unix_copyto_buildlog:   Copy log files to the server.
#	    unix_execute_makedoc:   Make cocumentations.
#	    unix_copyto_webbase:    Copy generated files to the server.
#	These control parameters are hard coded on this file.
#
# -----------------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2018/02/28 F.Kanehori	First version.
# =============================================================================
version = 1.0
python_test = True

import sys
import os
import glob
from time import sleep
from optparse import OptionParser

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
#  Control parameter for test on unix.
#
unix_gen_history	= Util.is_windows()
unix_copyto_buildlog	= Util.is_windows()
unix_execute_makedoc	= True
unix_copyto_webbase	= Util.is_windows()

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
result_log = 'result.log'
history_log = 'History.log'
date_record = 'Test.date'

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

def make_dir(newdir):
	if dry_run:
		print('  mkdir: %s' % Util.upath(newdir))
		return
	os.makedirs(newdir, exist_ok=True)

def copy_dir(fop, dir, copyto):
	for root, dirs, files in os.walk(dir, topdown=False):
		for f in files:
			src = '%s/%s' % (root, f)
			copy_file(fop, src, copyto, debug=True)
		for d in dirs:
			todir = '%s/%s' % (copyto, d)
			make_dir(os.path.abspath(todir))
			copy_dir(fop, d, todir)

def copy_file(fop, f, copyto, debug=False):
	leaf = f.split('/')[-1]
	path = '%s/%s' % (copyto, leaf)
	if debug:
		print('    copying "%s" -> "%s"' % (leaf, path))
	fop.cp(f, path, force=True)

def flush():
	sys.stdout.flush()
	sys.stderr.flush()

def Print(msg):
	print(msg)
	flush()

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options] test-repositoy'
parser = OptionParser(usage = usage)
parser.add_option('-c', '--conf', dest='conf',
			action='store', default='Release',
			help='test configuration [default: %default]')
parser.add_option('-p', '--plat', dest='plat',
			action='store', default='x64',
			help='test platform [default: %default]')
parser.add_option('-t', '--toolset', dest='toolset',
			action='store', default='14.0',
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
plat = options.plat
conf = options.conf
if plat not in ['x86', 'x64']:
	msg = 'invalid platform name "%s"' % plat
	Error(prog).print(msg)
if conf not in ['Debug', 'Release', 'Trace']:
	msg = 'invalid configuration name "%s"' % conf
	Error(prog).print(msg)
verbose = options.verbose
dry_run = options.dry_run

print('Test parameters:')
if Util.is_windows():
	print('   toolset id:      [%s]' % toolset)
print('   platform:        [%s]' % plat)
print('   configuration:   [%s]' % conf)
print('   test repository: [%s]' % repository)

# ----------------------------------------------------------------------
#  Go to test repository.
#
if not os.path.exists(repository):
	print('making test repository: [%s]' % repository)
	os.makedirs(repository, exist_ok=True)
testdir = spr_path.abspath('test')
start_dir = os.getcwd()
os.chdir(repository)

if not os.path.exists('core/test/bin'):
	msg = 'test repository "%s/core" may be empty' % repository
	Error(prog).print(msg)

# ----------------------------------------------------------------------
#  Test Go!
#
if check_exec('DAILYBUILD_EXECUTE_TESTALL'):
	os.chdir('%s/bin' % testdir)
	#
	test_dirs = []
	if check_exec('DAILYBUILD_EXECUTE_STUBBUILD'):
		test_dirs.append(['.', '-S'])
	if check_exec('DAILYBUILD_EXECUTE_BUILDRUN'):
		test_dirs.append(['tests'])
	if check_exec('DAILYBUILD_EXECUTE_SAMPLEBUILD'):
		test_dirs.append(['Samples'])
	#
	csusage = 'unuse'	#  We do not use closed sources.
	cmnd = 'python SpringheadTest.py'
	opts = '-p %s -c %s -C %s' % (plat, conf, csusage)
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
		flush()
	#
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Make history log file.
#
if check_exec('DAILYBUILD_GEN_HISTORY', unix_gen_history):
	Print('making history log')
	os.chdir('%s/bin' % testdir)
	#
	rslt_path = '../log/%s' % result_log
	hist_path = '../log/%s' % history_log
	cmnd = 'python VersionControlSystem.py -g -f %s all' % rslt_path
	Proc().exec(cmnd, stdout=hist_path).wait()
	flush()
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Make test date/time information file.
#
if check_exec('DAILYBUILD_GEN_HISTORY', unix_gen_history):
	Print('making test date information')
	os.chdir('%s/log' % testdir)
	#
	date_and_time = Util.now('%Y-%m%d %H:%M:%S')
	lines = [
		'** DO NOT EDIT THIS FILE **',
		'generated by "%s"' % prog,
		'- - - %s' % date_and_time
	]
	fio = TextFio(date_record, 'w')
	if fio.open() == 0:
		fio.writelines(lines)
		fio.close()
	else:
		Error(prog).print('cannot make "Test.date".', alive=True)
	flush()
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Copy log files to the server.
#
if check_exec('DAILYBUILD_COPYTO_BUILDLOG', unix_copyto_buildlog):
	os.chdir('%s/log' % testdir)
	#
	docroot = '//haselab/HomeDirs/WWW/docroots'
	webbase = '%s/springhead/dailybuild/log' % docroot
	print('web base: %s' % webbase)
	#
	print('  clearing...')
	fop = FileOp(dry_run=dry_run, verbose=verbose)
	fop.rm('%s/*' % webbase)
	#
	print('  copying to %s' % webbase)
	"""
	flist = glob.glob('*.log')
	for f in flist:
		Print('    %s' % Util.upath(f))
		f_abs = Util.upath(os.path.abspath(f))
		copy_file(fop, f_abs, webbase)
	"""
	fop.cp('*.log', webbase)
	flush()
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Make document (doxygen).
#
if check_exec('DAILYBUILD_EXECUTE_MAKEDOC', unix_execute_makedoc):
	print('making documents')
	#
	os.chdir('core/include')
	Print('  SpringheadDoc')
	if python_test:
		cmnd = 'python SpringheadDoc.py'
	else:
		cmnd = 'cmd /c SpringheadDoc.bat'
	proc = Proc(verbose=verbose, dry_run=dry_run)
	proc.exec(cmnd).wait()
	#
	os.chdir('../src')
	Print('  SpringheadImpDoc')
	if python_test:
		cmnd = 'python SpringheadImpDoc.py'
	else:
		cmnd = 'cmd /c SpringheadImpDoc.bat'
	proc = Proc(verbose=verbose, dry_run=dry_run)
	proc.exec(cmnd).wait()
	#
	os.chdir('../doc/SprManual')
	Print('  SprManual')
	if python_test:
		cmnd = 'python MakeDoc.py'
	else:
		cmnd = 'cmd /c make.bat'
	proc = Proc(verbose=verbose, dry_run=dry_run)
	proc.exec(cmnd).wait()
	#
	flush()
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Copy generated files to the server.
#
if check_exec('DAILYBUILD_COPYTO_WEBBASE', unix_copyto_webbase):
	Print('copying generated files to web')
	#
	docroot = '//haselab/HomeDirs/WWW/docroots'
	webbase = '%s/springhead/dailybuild/generated' % docroot
	#
	os.chdir('generated')
	"""
	glist = glob.glob('*')
	dlist = []
	flist = []
	for g in glist:
		if os.path.isdir(g):
			dlist.append(g)
		elif os.path.isfile(g):
			flist.append(g)
	print('  web base:  [%s]' % webbase)
	print('  dir list:  [%s]' % ' '.join(dlist))
	print('  file list: [%s]' % ' '.join(flist))
	fop = FileOp(dry_run=dry_run, verbose=verbose)
	for d in dlist:
		d_local = Util.upath(os.path.abspath(d))
		d_remote = '%s/%s' % (webbase, d)
		print('  clearing %s...' % d_remote)
		fop.rm('%s' % d_remote, recurse=True)
		make_dir(d_remote)
		print('  copying directory %s -> %s' % (d_local, d_remote))
		copy_dir(fop, d_local, d_remote)
	for f in flist:
		f_local = Util.upath(os.path.abspath(f))
		print('  copying file %s -> %s' % (f_local, webbase))
		copy_file(fop, f_local, webbase)
	"""
	fop = FileOp(dry_run=dry_run, verbose=verbose)
	fop.rm('%s/doc' % webbase)
	fop.cp('doc', webbase)
	#
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Done.
#
sys.exit(0)

# end: TestMainGit.py

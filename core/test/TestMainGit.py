#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	TestMainGit [options]
#	options:
#	    -c CONF:	    Test configuration (Debug, Release or Trace).
#	    -p PLAT:	    Test platform (x86 or x64)
#	    -t TOOLSET:	    C-compiler version.
#				Windows: Visual Studio version (str).
#				unix:    gcc version (dummy).
#	    -v:		    Set verbose level (0: silent).
#	    -D:		    Show command but do not execute it.
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
#  VERSION:
#	Ver 1.0  2018/03/05 F.Kanehori	First version.
#	Ver 1.1  2018/04/26 F.Kanehori	Commit result.log to git server.
# ======================================================================
version = 1.1

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
		print('do not exec ..%s.. by %s' % (name, os_name))
		return False
	#
	val = os.getenv(name)
	judge = True if val is None or val != 'skip' else False
	if not judge:
		print('skip ..%s..' % name)
	return judge

def copy_all(src, dst, rm_topdir=True, dry_run=False, verbose=0):
	Print('  clearing "%s"' % dst)
	fop = FileOp(info=1, dry_run=dry_run, verbose=verbose)
	fop.rm('%s/*' % dst, recurse=True)
	if os.path.exists(dst) and rm_topdir:
		if Util.is_windows(): sleep(1)	# Kludge
		fop.rmdir(dst)
		if Util.is_windows(): sleep(1)	# Kludge
	#
	os.chdir(src)
	Print('  copying "%s" to "%s"' % (src, dst))
	names = os.listdir()
	for name in names:
		if os.path.isfile(name):
			fop.cp(name, dst)
		elif os.path.isdir(name):
			dst_dir = '%s/%s' % (dst, name)
			fop.cp(name, dst_dir)

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
	Error(prog).abort(msg)

# get options
toolset = options.toolset
plat = options.plat
conf = options.conf
if plat not in ['x86', 'x64']:
	msg = 'invalid platform name "%s"' % plat
	Error(prog).abort(msg)
if conf not in ['Debug', 'Release', 'Trace']:
	msg = 'invalid configuration name "%s"' % conf
	Error(prog).abort(msg)
verbose = options.verbose
dry_run = options.dry_run

shell = True if Util.is_unix() else False

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
	Error(prog).abort(msg)

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
	if verbose:
		opts += '-v'
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
		proc.execute('%s %s %s' % (cmnd, t_opts, t_args), shell=shell)
		stat = proc.wait()
		if (stat != 0):
			msg = 'test failed (%d)' % stat
			Error(proc).abort(msg, exitcode=stat)
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
	proc = Proc(verbose=verbose, dry_run=dry_run)
	proc.execute(cmnd, shell=shell, stdout=hist_path).wait()
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
		Error(prog).error('cannot make "Test.date".')
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Copy log files to the web server.
#
if check_exec('DAILYBUILD_COPYTO_BUILDLOG', unix_copyto_buildlog):
	Print('copying log files to web')
	#
	docroot = '//haselab/HomeDirs/WWW/docroots'
	webbase = '%s/springhead/dailybuild/log' % docroot
	logdir = '%s/log' % testdir
	#
	copy_all(logdir, webbase, False, dry_run)
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Create commit.id which holds newest commit id on GitHub.
#
if check_exec('DAILYBUILD_COMMIT_RESULTLOG', unix_copyto_buildlog):
	Print('creating "commit.id"')
	logdir = '%s/log' % testdir
	os.chdir(logdir)
	#
	proc = Proc(verbose=verbose, dry_run=dry_run)
	cmnd = 'git log --abbrev-commit --oneline --max-count=1'
	proc.execute(cmnd, shell=shell,
			stdout=Proc.PIPE, stderr=Proc.STDOUT)
	rc, out, err = proc.output()
	if rc == 0:
		commit_id = out.split()[0]
		fio = TextFio('commit.id', 'w')
		if fio.open() == 0:
			fio.writeline(commit_id)
			fio.close()
	else:
		Print('  ** extracting HEAD log from GitHub failed **')
	#
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Commit result.log and commit.id to logal git server.
#
if check_exec('DAILYBUILD_COMMIT_RESULTLOG', unix_copyto_buildlog):
	Print('committing result.log to local git server')
	logdir = '%s/log' % testdir
	os.chdir(logdir)
	#
	url = 'http://git/haselab.net/DailyBuild/log'
	files = 'result.log commit.id'
	#
	proc = Proc(verbose=verbose, dry_run=dry_run)
	cmnd = 'git commit --message="today\'s result %s' % files
	rc = proc.execute(cmnd, shell=shell).wait()
	if rc == 0:
		cmnd = 'git push'
		rc = proc.execute(cmnd, shell=shell).wait()
		if rc != 0:
			Print('  ** git push failed **')
	else:
		Print('  ** nothing to commit **')
	#
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Make document (doxygen).
#
if check_exec('DAILYBUILD_EXECUTE_MAKEDOC', unix_execute_makedoc):
	Print('making documents')
	#
	os.chdir('core/include')
	Print('  SpringheadDoc')
	cmnd = 'python SpringheadDoc.py'
	proc = Proc(verbose=verbose, dry_run=dry_run)
	proc.execute(cmnd, shell=shell).wait()
	#
	os.chdir('../src')
	Print('  SpringheadImpDoc')
	cmnd = 'python SpringheadImpDoc.py'
	proc = Proc(verbose=verbose, dry_run=dry_run)
	proc.execute(cmnd, shell=shell).wait()
	#
	os.chdir('../doc/SprManual')
	Print('  SprManual')
	cmnd = 'python MakeDoc.py'
	proc = Proc(verbose=verbose, dry_run=dry_run)
	proc.execute(cmnd, shell=shell).wait()
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
	copy_all('generated', webbase, True, dry_run)
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Done.
#
sys.exit(0)

# end: TestMainGit.py

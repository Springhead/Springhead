#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	TestMainGit [options] test-repository result-repository
#	options:
#	    -c CONF:	    Test configuration (Debug, Release or Trace).
#	    -p PLAT:	    Test platform (x86 or x64)
#	    -t TOOLSET:	    C-compiler version.
#				Windows: Visual Studio version (str).
#				unix:    gcc version (dummy).
#	    -v:		    Set verbose level (0: silent).
#	    -D:		    Show command but do not execute it.
#	    -V:		    Show version.
#	    -X:		    Build and Run only (do not commit/copy).
#
#	test-repository, result-repository:
#	    Relative to the directory where Springhead directory exists.
#
#  DESCRIPTION:
#	Execute dailybuild test.
#	On unix machine, some test steps' execution can be controlled by
#	following parameters;
#	    unix_commit_resultlog:  Commit "result.log" and "Springhead.commit.id".
#	    unix_gen_history:	    Generate "History.log" and "Test.date".
#	    unix_copyto_buildlog:   Copy log files to the server.
#	    unix_execute_makedoc:   Make cocumentations.
#	    unix_copyto_webbase:    Copy generated files to the server.
#	These control parameters are hard coded on this file.
#
#  VERSION:
#     Ver 1.0    2018/03/05 F.Kanehori	First version.
#     Ver 1.1    2018/04/26 F.Kanehori	Commit result.log to git server.
#     Ver 1.2    2018/05/01 F.Kanehori	Git pull for DailyBuild/Result.
#     Ver 1.3    2018/08/16 F.Kanehori	Do not make documents on unix.
#     Ver 1.4    2018/09/04 F.Kanehori	Test on unix released.
#     Ver 1.5    2018/09/10 F.Kanehori	RevisionInfo.py implemented.
#     Ver 1.6    2019/01/10 F.Kanehori	Add closed-source control.
#     Ver 1.7    2019/08/05 F.Kanehori	Add HowToUseCMake document.
#     Ver 1.8    2020/10/12 F.Kanehori	Add build of EmbPython Library.
#     Ver 1.9    2021/03/03 F.Kanehori	Add HowToBuildSpringhead_Windows.
#     Ver 1.10   2021/03/24 F.Kanehori	Add SprInstallGuide.
#     Ver 1.11   2021/05/20 F.Kanehori	Rename 'CMake' to 'BuildUsingCMake'.
#     Ver 1.12   2021/07/06 F.Kanehori	DailyBuildTestTools の導入.
#     Ver 1.13   2021/08/04 F.Kanehori	Add nkf path for web report.
#     Ver 1.14   2022/01/19 F.Kanehori	Add option -X for debug.
#     Ver 1.14.1 2022/03/30 F.Kanehori	Change log server host name.
# ======================================================================
version = '1.14'

import sys
import os
import glob
from time import sleep
from optparse import OptionParser

# ----------------------------------------------------------------------
#  このスクリプトは ".../core/test" に置く	
#
ScriptFileDir = os.sep.join(os.path.abspath(__file__).split(os.sep)[:-1])
prog = sys.argv[0].replace('/', os.sep).split(os.sep)[-1].split('.')[0]
TopDir = '/'.join(ScriptFileDir.split(os.sep)[:-2])
SrcDir = os.path.abspath('%s/core/src' % TopDir).replace(os.sep, '/')
TestDir = os.path.abspath('%s/core/test' % TopDir).replace(os.sep, '/')
BaseDir = os.path.abspath('%s/..' % TopDir).replace(os.sep, '/')
ToolsDir = '%s/DailyBuildTestTools' % BaseDir

# ----------------------------------------------------------------------
#  Import Springhead python library.
#
libdir = '%s/RunSwig/pythonlib' % SrcDir
sys.path.append(libdir)
from Error import *
from Util import *
from Proc import *
from FileOp import *
from TextFio import *

# ----------------------------------------------------------------------
#  Control parameter for test on unix.
#
unix_commit_resultlog	= True
unix_gen_history	= True
unix_copyto_buildlog	= True
unix_execute_makedoc	= Util.is_windows()
unix_copyto_webbase	= Util.is_windows()

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
result_log = 'result.log'
history_log = 'History.log'
date_record = 'Test.date'
commit_id = 'Springhead.commit.id'
log_user = 'demo'
log_server = 'haselab.net'

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
		print('do not exec ..%s.. on %s' % (name, os_name))
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

def date_str_conv(from_str):
	# from:	'Dow Mon dd hh:mm:ss YYYY +0900'
	# to:   'YYYY-mmdd hh:mm:ss'
	# ** There should be some useful library method! (datetime?) **
	montab = {'Jan': 1, 'Feb': 2, 'Mar': 3, 'Apr': 4, 'May': 5, 'Jun': 6,
		  'Jul': 7, 'Aug': 8, 'Sep': 9, 'Oct':10, 'Nov':11, 'Dec':12 }
	dt = from_str.split()
	yyyy = dt[4]
	mmdd = '%02d%02d' % (montab[dt[1]], int(dt[2]))
	return '%s-%s %s' % (yyyy, mmdd, dt[3])

def flush():
	sys.stdout.flush()
	sys.stderr.flush()

def Print(msg):
	print(msg)
	flush()

def get_username(fname):
	fio = TextFio(fname)
	if fio.open() < 0:
		Error(prog).error(fio.error())
		return ''
	lines = fio.read()
	fio.close()
	return lines[0]

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options] test-repositoy result-repository'
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
parser.add_option('-X', '--build-and-run-only', dest='X',
			action='store_true', default=False,
			help='build and run only')

# ----------------------------------------------------------------------
#  Process for command line
#
(options, args) = parser.parse_args()
if options.version:
	print('%s: Version %s' % (prog, str(version)))
	sys.exit(0)
if len(args) != 2:
	parser.error("incorrect number of arguments")

# get arguments
repository = '%s/%s' % (BaseDir, args[0])
repository = Util.upath(os.path.abspath(repository))
if not os.path.isdir(repository):
	msg = '"%s" is not a directory' % repository
	Error(prog).abort(msg)

result_repository = '%s/../%s' % (repository, args[1])
if not os.path.exists(result_repository):
	msg = '"%s" does not exist' % result_repository
	Error(prog).abort(msg)
if not os.path.isdir(result_repository):
	msg = '"%s" is not a directory' % result_repository
	Error(prog).abort(msg)
result_repository = Util.upath(os.path.abspath(result_repository))

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
	print('   toolset id:        [%s]' % toolset)
print('   platform:          [%s]' % plat)
print('   configuration:     [%s]' % conf)
print('   test repository:   [%s]' % repository)
print('   result repository: [%s]' % result_repository)

# ----------------------------------------------------------------------
#  Python's path
#	unix:	  default python
#	Windows:  .../DailyBuildTestTools/Python/python.exe.
#
if Util().is_windows():
	python = '%s/Python/python.exe' % ToolsDir
	print('using "%s"' % python)
else:
	python = 'python'
	print('using default python')

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
	Error(prog).abort(msg)

# ----------------------------------------------------------------------
#  Create closed-source-control file (UseClosedSrcOrNot.h).
#	Following script must be done at RunSwig directory!
#
runswigdir = '%s/RunSwig' % SrcDir
os.chdir(runswigdir)
cmnd = '%s CheckClosedSrc.py' % python
rc = Proc().execute(cmnd, shell=shell).wait()
os.chdir(repository)

# ----------------------------------------------------------------------
#  Remove log files.
#
Print('clearing log files')
os.chdir('%s/log' % TestDir)
fop = FileOp(info=1, dry_run=dry_run, verbose=verbose)
fop.rm('*')
os.chdir(repository)

# ----------------------------------------------------------------------
#  Test Go!
#
if check_exec('DAILYBUILD_EXECUTE_TESTALL'):
	os.chdir('%s/bin' % TestDir)
	#
	test_dirs = []
	if check_exec('DAILYBUILD_EXECUTE_STUBBUILD'):
		test_dirs.append(['.', '-S'])
	if check_exec('DAILYBUILD_EXECUTE_EMBPYTHON'):
		test_dirs.append(['EmbPython'])
	if check_exec('DAILYBUILD_EXECUTE_BUILDRUN'):
		test_dirs.append(['tests'])
	if check_exec('DAILYBUILD_EXECUTE_SAMPLEBUILD'):
		test_dirs.append(['Samples'])
	#
	csusage = 'unuse'	#  We do not use closed sources.
	cmnd = '%s SpringheadTest.py' % python
	opts = '-p %s -c %s -C %s' % (plat, conf, csusage)
	if verbose:
		opts += ' -v'
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
			Error(prog).abort(msg, exitcode=stat)
		flush()
	#
	os.chdir(repository)

if options.X:		# for debug only
	sys.exit(0)

# ----------------------------------------------------------------------
#  Build and run test has been done at this point.
#  Add some tools which DailyBuild needs hereafter.
#
if check_exec('DAILYBUILD_COMMIT_RESULTLOG', unix_commit_resultlog):
	demo_path_nkf = 'I:\\DailyBuild\\Springhead\\buildtool'
	demo_nkf = '%s\\nkf.exe' % demo_path_nkf
	if os.path.exists(demo_nkf):
		newpath = demo_path_nkf + os.pathsep + os.environ['PATH']
		os.environ['PATH'] = newpath
		print('add path: %s' % demo_path_nkf)

# ----------------------------------------------------------------------
#  Generate sprphys/Springhead HEAD commit-id file.
#
if check_exec('DAILYBUILD_COMMIT_RESULTLOG', unix_commit_resultlog):
	Print('generating sprphys/Springhead HEAD commit-id file')
	logdir = '%s/log' % TestDir
	os.chdir(logdir)
	#
	cmnd = '%s ../bin/RevisionInfo.py -S HEAD' % python
	proc = Proc(verbose=verbose, dry_run=dry_run)
	rc = proc.execute(cmnd, shell=shell,
			  stdout=commit_id, stderr=Proc.STDOUT).wait()
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Commit and push log files commit-id file to test result repository.
#
if check_exec('DAILYBUILD_COMMIT_RESULTLOG', unix_commit_resultlog):
	Print('copying test result and commit-id to test result repository')
	if Util.is_unix():
		target_dir = '%s/unix' % result_repository
		cert_dir = '../../..'
		aux_msg = '(unix)'
	else:
		target_dir = result_repository
		cert_dir = '../..'
		aux_msg = '(Windows)'
	logdir = '%s/log' % TestDir
	os.chdir(logdir)
	#
	logfiles = ['result.log']
	fop = FileOp()
	for f in logfiles:
		fop.cp(f, '%s/%s' % (target_dir, f))
	fop.cp(commit_id, '%s/%s' % (target_dir, commit_id))
	flush()
	#
	Print('committing files to test result repository.')
	os.chdir(target_dir)
	cmnds = [
		'git config --global push.default simple',
		'git config --global user.name "DailyBuild"',
		'git pull',
		'git commit --message="today\'s test result %s" %s %s' % \
			(aux_msg, ' '.join(logfiles), commit_id)
	]
	proc = Proc(verbose=verbose, dry_run=dry_run)
	for cmnd in cmnds:
		print('## %s' % cmnd)
		flush()
		rc = proc.execute(cmnd, shell=shell).wait()
		if rc != 0:
			break
	if rc == 0:
		user = get_username('%s/hasegit.user' % cert_dir)
		cmnd = 'git push http://%s@git.haselab.net/DailyBuild/Result' % user
		print('## %s' % cmnd)
		rc = proc.execute(cmnd, shell=shell).wait()
		if rc == 0:
			Print('  commit and push OK.')
	else:
		Print('  -> nothing to commit!')
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Make history log file.
#
if check_exec('DAILYBUILD_GEN_HISTORY', unix_gen_history):
	Print('making history log')
	logdir = '%s/log' % TestDir
	os.chdir(logdir)
	#
	hist_path = '%s/%s' % (logdir, history_log)
	extract = 'result.log'
	cmnd = '%s ../bin/RevisionInfo.py' % python
	if Util.is_unix():
		args = '-R -u -f %s all' % extract
	else:
		args = '-R -f %s all' % extract
	proc = Proc(verbose=verbose, dry_run=dry_run)
	proc.execute([cmnd, args], shell=shell, stdout=hist_path).wait()
	flush()
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Make test date/time information file.
#
if check_exec('DAILYBUILD_GEN_HISTORY', unix_gen_history):
	Print('making test date information')
	os.chdir('%s/log' % TestDir)
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
	if Util.is_unix():
		tohost = '%s@%s' % (log_user, log_server)
		todir = '/home/WWW/docroots/springhead/dailybuild/log.unix'
		fmdir = os.path.abspath('%s/log' % TestDir)
		proc = Proc(verbose=verbose, dry_run=dry_run)
		pkey = '%s/.ssh/id_rsa' % os.environ['HOME']
		opts = '-i %s -o "StrictHostKeyChecking=no"' % pkey
		cmnd = 'scp %s %s/* %s:%s' % (opts, fmdir, tohost, todir)
		print('## %s' % cmnd)
		rc = proc.execute(cmnd, shell=True).wait()
		if rc == 0:
			print('cp %s -> %s:%s' % (fmdir, tohost, todir))
		else:
			print('cp %s failed' % fmdir)
	else:
		dirname = 'log'
		docroot = '//haselab.net/HomeDirs/WWW/docroots'
		webbase = '%s/springhead/dailybuild/%s' % (docroot, dirname)
		logdir = '%s/log' % TestDir
		#
		copy_all(logdir, webbase, False, dry_run)
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Make document (doxygen).
#
if check_exec('DAILYBUILD_EXECUTE_MAKEDOC', unix_execute_makedoc):
	Print('making documents')
	#
	os.chdir('%s/core/include' % TopDir)
	Print('  SpringheadDoc')
	cmnd = '%s SpringheadDoc.py' % python
	proc = Proc(verbose=verbose, dry_run=dry_run)
	proc.execute(cmnd, shell=shell).wait()
	#
	os.chdir('%s/core/src' % TopDir)
	Print('  SpringheadImpDoc')
	cmnd = '%s SpringheadImpDoc.py' % python
	proc = Proc(verbose=verbose, dry_run=dry_run)
	proc.execute(cmnd, shell=shell).wait()
	#
	os.chdir('%s/core/doc/SprManual' % TopDir)
	Print('  SprManual')
	cmnd = '%s MakeDoc.py' % python
	proc = Proc(verbose=verbose, dry_run=dry_run)
	proc.execute(cmnd, shell=shell).wait()
	#
	os.chdir('%s/core/doc/SprInstallGuide' % TopDir)
	Print('  SprInstallGuide')
	cmnd = '%s MakeDoc.py' % python
	proc = Proc(verbose=verbose, dry_run=dry_run)
	proc.execute(cmnd, shell=shell).wait()
	#
	os.chdir('%s/core/doc/BuildUsingCMake' % TopDir)
	Print('  BuildUsingCMake')
	cmnd = '%s MakeDoc.py' % python
	proc = Proc(verbose=verbose, dry_run=dry_run)
	proc.execute(cmnd, shell=shell).wait()
	#
	os.chdir('%s/core/doc/CMakeGitBook' % TopDir)
	Print('  CMakeGitbook')
	cmnd = '%s MakeDoc.py' % python
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
	docroot = '//haselab.net/HomeDirs/WWW/docroots'
	webbase = '%s/springhead/dailybuild/generated' % docroot
	#
	copy_all('generated', webbase, True, dry_run)
	os.chdir(repository)

# ----------------------------------------------------------------------
#  Done.
#
sys.exit(0)

# end: TestMainGit.py

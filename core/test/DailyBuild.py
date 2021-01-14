#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	DailyBuild [options] test-repository result-repository
#	options:
#	  -c conf:	Configurations (Debug | Release).
#	  -p plat:	Platform (x86 | x64).
#	  -t tool:	Visual Studio toolset ID. (Windows only)
#	  -s file:	Setup file name (default: setup.conf).
#	  -S:		Execute setup process first.
#	  -d num:	Devenv selection number (default: 1).
#
#  DESCRIPTION:
#	テストの環境を整えてから TestMainGit.py を呼び出す.
#
#  NOTE:
#	****************************************************************
#	新しい Visual Studio に対応するには、"bin/visualStudio.py" の
#	  (1) __get_vsibnfo() に新しいバージョンの情報を追加する.
#	  (2) __get_vs_path() に devenv のパスを設定する.
#	必要がある.
#	****************************************************************
#
# ----------------------------------------------------------------------
#  VERSION:
#     Ver 1.00   2017/12/03 F.Kanehori	アダプタとして新規作成.
#     Ver 1.01   2017/12/25 F.Kanehori	TestMainGit.bat は無条件に実行.
#     Ver 1.02   2018/03/05 F.Kanehori	TestMainGit.py に移行.
#     Ver 1.03   2018/03/19 F.Kanehori	Proc.output() changed.
#     Ver 1.04   2018/03/22 F.Kanehori	Change git pull/clone step.
#     Ver 1.05   2018/05/01 F.Kanehori	Add: Result repository.
#     Ver 1.06   2020/12/14 F.Kanehori	Setup 導入テスト開始.
#     Ver 1.07   2021/01/07 F.Kanehori	Setup 自動実行設定追加.
#     Ver 1.07.1 2021/01/14 F.Kanehori	Bug fix.
# ======================================================================
version = "1.07.1"

import sys
import os
from optparse import OptionParser
import stat
from time import sleep
from datetime import *

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
url_git = 'https://github.com/sprphys/Springhead'
url_svn = 'http://springhead.info/spr2/Springhead/trunk/closed'
date_format = '%Y/%m/%d %H:%M:%S'

# ----------------------------------------------------------------------
#  Local python library
#
sys.path.append('../src/RunSwig')
from FindSprPath import *
spr_path = FindSprPath(prog)
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from FileOp import *
from TextFio import *
from Proc import *
from Util import *
from Error import *
from SetupFile import *

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: python %prog [options] test-repository result-repository'
parser = OptionParser(usage = usage)
parser.add_option('-c', '--conf', dest='conf',
			action='store', default='Release',
			help='test configuration [default: %default]')
if Util.is_windows():
	parser.add_option('-d', '--devenv-num', dest='devenv_num',
			action='store', default='1',
			help='devenv spcify number [default: %default]')
parser.add_option('-p', '--plat', dest='plat',
			action='store', default='x64',
			help='test platform [default: %default]')
parser.add_option('-s', '--setup-file', dest='setup_file',
			action='store', default='setup.conf',
			help='environment setup file [default: %default]')
if Util.is_windows():
	parser.add_option('-t', '--toolset-id', dest='tool',
			action='store', default='15.0',
			help='toolset ID [default: %default]')
parser.add_option('-u', '--update-only',
			dest='update_only', action='store_true', default=False,
			help='execute \'git pull\' only')
parser.add_option('-U', '--skip-update',
			dest='skip_update', action='store_true', default=False,
			help='skip \'git pull\'')
parser.add_option('-v', '--verbose',
			dest='verbose', action='count', default=0,
			help='set verbose mode')
parser.add_option('-D', '--dry-run', dest='dry_run',
			action='store_true', default=False,
			help='set dry-run mode')
parser.add_option('-S', '--setup', dest='setup',
			action='store_true', default=False,
			help='execute setup process first')
parser.add_option('-V', '--version',
			dest='version', action='store_true', default=False,
			help='show version')
parser.add_option('-A', '--as-is',
			dest='as_is', action='count', default=0,
			help='do not update nor clear test repository')

# ----------------------------------------------------------------------
#  Process for command line
#
(options, args) = parser.parse_args()
if options.version:
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
if len(args) != 2:
	Error(prog).error('incorrect number of arguments\n')
	Proc().execute('python %s.py -h' % prog, shell=True).wait()
	sys.exit(-1)

# get test repository name
repository = Util.upath(args[0])
result_repository = Util.upath(args[1])
conf = options.conf
plat = options.plat
tool = options.tool if Util.is_windows() else None
update_only = options.update_only
skip_update = options.skip_update
verbose = options.verbose
dry_run = options.dry_run
as_is = options.as_is
setup_file = options.setup_file
setup = options.setup
if Util.is_windows():
	devenv_num = options.devenv_num

if repository == 'Springhead':
	msg = 'Are you sure to test on "Springhead" directory? [y/n] '
	ans = input(msg)
	if ans != 'y':
		print('abort')
		exit(-1)

# ----------------------------------------------------------------------
#  Globals
#
spr_topdir = spr_path.abspath()
spr_srcdir = spr_path.abspath('src')
start_dir = spr_path.abspath('test')
prep_dir = os.path.abspath('%s/..' % spr_topdir)
proc = Proc(verbose=verbose, dry_run=dry_run)
setup_script = 'setup.sh' if Util.is_unix() else 'setup.bat'

# ----------------------------------------------------------------------
#  Local methods.
#
def check_exec(name):
	# Return False only if environment variable 'name' is defined
	# and its value is 'skip'.  Return True otherwise.
	val = os.getenv(name)
	judge = True if val is None or val != 'skip' else False
	if as_is:
		judge = False
	if not judge:
		print('skip ..%s..' % name)
	return judge

def pwd():
	# Print current workind directory.
	print('[%s]' % Util.upath(os.getcwd()))

def Print(data=None, indent=2):
	if data is None:
		return
	# Print with indent.
	if isinstance(data, list):
		for d in data:
			Print(d, indent)
	else:
		indent_str = ' ' * indent
		print('%s%s' % (indent_str, data))

def flush():
	sys.stdout.flush()
	sys.stderr.flush()

# ----------------------------------------------------------------------
#  Process start.
#
print('%s: start: %s' % (prog, Util.now(format=date_format)))
cwd = os.getcwd()

# ----------------------------------------------------------------------
#  1st step: Make Springhead up-to-date.
#
if check_exec('DAILYBUILD_UPDATE_SPRINGHEAD') and not skip_update:
	print('updating "Springhead"')
	flush()
	os.chdir(spr_topdir)
	pwd()
	cmnd = 'git pull --all'
	proc.execute(cmnd, stdout=Proc.PIPE, stderr=Proc.STDOUT, shell=True)
	rc, outstr, errstr = proc.output()
	Print(outstr.split('\n'))
	if errstr:
		Print('-- error --')
		Print(errstr.split('\n'))
	if rc != 0:
		Error(prog).abort('updating failed: status %d' % rc)
	#
	print('updating "DailyBuild/Result"')
	flush()
	os.chdir('../%s' % result_repository)
	pwd()
	cmnd = 'git pull --all'
	proc.execute(cmnd, stdout=Proc.PIPE, stderr=Proc.STDOUT, shell=True)
	rc, outstr, errstr = proc.output()
	Print(outstr.split('\n'))
	if errstr:
		Print('-- error --')
		Print(errstr.split('\n'))
	if rc != 0:
		Error(prog).abort('updating failed: status %d' % rc)
	#
	os.chdir(start_dir)
#
if update_only:
	print('%s: update-only specified.' % prog)
	print('%s: end: %s' % (prog, Util.now(format=date_format)))
	sys.exit(0)

# ----------------------------------------------------------------------
#  2nd step: Clearing test directory.
#
if check_exec('DAILYBUILD_CLEANUP_WORKSPACE'):
	os.chdir(prep_dir)
	pwd()
	if os.path.exists(repository):
		print('clearing "%s"' % repository)
		flush()
		# There are some files that shutil.rmtree() can not
		# remove.  And also some idle time needs to remove
		# top directory after all its contents are removed
		# -- mistery.. (Windows only?).
		if Util.is_unix():
			cmnd = '/bin/rm -rf %s' % repository
			Proc().execute(cmnd, shell=True).wait()
			
		else:
			FileOp().rm(repository, use_shutil=False, idle_time=1)
	else:
		print('test repository "%s" not exist' % repository)
	print()
	os.chdir(start_dir)

# ----------------------------------------------------------------------
#  3rd step: Clone repository.
#
if check_exec('DAILYBUILD_CLEANUP_WORKSPACE'):
	os.chdir(prep_dir)
	pwd()
	print('cloning test repository')
	flush()
	cmnd = 'git clone --recurse-submodules %s %s' % (url_git, repository)
	proc.execute(cmnd, stdout=Proc.PIPE, stderr=Proc.STDOUT, shell=True)
	rc, outstr, errstr = proc.output()
	Print(outstr.split('\n'))
	if errstr:
		Print('-- error --')
		Print(errstr.split('\n'))
	if rc != 0:
		Error(prog).abort('cloning failed: status %d' % rc)

	os.chdir(repository)
	pwd()
	print('updating submodules')
	cmnd = 'git submodule update --init'
	rc = proc.execute(cmnd, shell=True).wait()
	if rc != 0:
		Error(prog).abort('cloning failed: status %d' % rc)
	os.chdir(prep_dir)

# ----------------------------------------------------------------------
#  The process hereafter will be executed under test-repository.
#
os.chdir('%s/%s' % (prep_dir, repository))
cwd = os.getcwd()
pwd()
Print('moved to test repository')
Print()

# ----------------------------------------------------------------------
#  4th step: Setup process.
#
if setup:
	# execute setup -F if '-S' option specified
	print('execute setup process (-S)')
	os.chdir('core/src')

	cmnd = '%s%s%s' % (spr_srcdir, os.sep, setup_script)
	cmnd = Util.pathconv(cmnd)
	if Util.is_windows():
		args = '-R %s -f -d %s -s %s' % (repository, devenv_num, setup_file)
	else:
		args = '-R %s -f -s %s' % (repository, setup_file)
	if verbose:
		args += ' -v'
		print('%s: %s %s' % (prog, cmnd, args))
	stat = Proc().execute('%s %s' % (cmnd, args), shell=True).wait()
	os.chdir(cwd)
	if stat != 0:
		Error(prog).info('setup failed (%d)' % stat)
		sys.exit(1)
	print()

# set pargram paths to environment variable.
#
if os.path.exists(setup_file):
	# identify python first
	print('check contents (setup.conf)')
	os.chdir('core/src')

	# get python path from setup.conf
	fio = TextFio(setup_file, 'r')
	if fio.open() != 0:
		Error(prog).abort('can not open "%s"' % setup_file)
	lines = fio.read()
	fio.close()
	python_path = None
	for line in lines:
		tmp = line.split()
		if len(tmp) == 2 and tmp[0] == 'python':
			python_path = tmp[1]
			break
	if python_path is None:
		Error(prog).abort('can not found python path')
	print('using %s' % Util.upath(python_path))

	# setup paths
	cmnd = '%s setup.py -c %s' % (python_path, python_path)
	stat = proc.execute(cmnd, shell=True).wait()
	os.chdir(cwd)
	if stat == -1:
		Error(prog).info('can\'t setup test environment.')
		Error(prog).info('execute "%s" first.' % setup_script)
		sys.exit(1)
	if stat < 0:
		Error(prog).abort('botch: setup file not found')
	#
	sf = SetupFile(setup_file)
	sf.setenv()
	python = os.getenv('python')
	print()
	print('using setup file "%s"' % setup_file)
else:
	Error(prog).warn('setup file "%s" not found' % setup_file)


# ----------------------------------------------------------------------
#  5th step: Execute DailyBuild test.
#
os.chdir('core/test')
pwd()
Print('Test start:')
vflag = ' -v' if verbose else ''
cmnd = 'python TestMainGit.py'
args = '-p %s -c %s -t %s%s %s %s' % (plat, conf, tool, vflag, \
				      repository, result_repository)
rc = proc.execute([cmnd, args], shell=True).wait()
Print('rc: %s' % rc)

# ----------------------------------------------------------------------
#  Process end.
#
os.chdir(start_dir)
print('%s: end: %s' % (prog, Util.now(format=date_format)))
sys.exit(0)

# end: DailyBuild.py


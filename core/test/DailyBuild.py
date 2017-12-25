#!/usr/bin/env python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	DailyBuild [options] test-repository
#	options:
#	  -c conf:	Configurations (Debug | Release).
#	  -p plat:	Platform (x86 | x64).
#	  -t tool:	Visual Studio toolset ID. (Windows only)
#
#  DESCRIPTION:
#
#  VERSION:
#	Ver 1.0  2017/12/03 F.Kanehori	アダプタとして新規作成.
#	Ver 1.1  2017/12/25 F.Kanehori	TestMainGit.bat は無条件に実行.
# ======================================================================
version = '1.1'

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
#  Globals
#
spr_topdir = spr_path.abspath()
start_dir = spr_path.abspath('test')
prep_dir = os.path.abspath('%s/..' % spr_topdir)
proc = Proc(verbose=0)
err = Error(prog)

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: python %prog [options] test-repository'
parser = OptionParser(usage = usage)
parser.add_option('-C', '--configuration',
			dest='conf', default='Release',
			help='configuration {Debug | <Release>}')
parser.add_option('-P', '--platform',
			dest='plat', default='x64',
			help='platform {x86 | <x64>}')
parser.add_option('-r', '--repository',
			dest='repository', default='SpringheadTest',
			help='test repository name')
if Util.is_windows():
	parser.add_option('-t', '--toolset-id',
			dest='tool', default='14.0',
			help='toolset ID {<14.0>}')
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
if len(args) != 1:
	err.print('incorrect number of arguments\n', prompt='error')
	proc.exec('python %s.py -h' % prog).wait()
	sys.exit(-1)

# get test repository name
repository = args[0]
conf = options.conf
plat = options.plat
if Util.is_windows():
	tool = options.tool
verbose = options.verbose

if repository == 'Springhead':
	msg = 'Are you sure to test on "Springhead" directory? [y/n] '
	ans = input(msg)
	if ans != 'y':
		print('abort')
		exit(-1)

# ----------------------------------------------------------------------
#  Local methods.
#
def check_exec(name):
	# Return False only if environment variable 'name' is defined
	# and its value is 'skip'.  Return True otherwise.
	val = os.getenv(name)
	judge = True if val is None or val != 'skip' else False
	if not judge:
		print('skip ..%s..' % name)
	return judge

def pwd():
	# Print current workind directory.
	print('[%s]' % Util.upath(os.getcwd()))

def Print(data, indent=2):
	# Print with indent.
	if isinstance(data, list):
		for d in data:
			Print(d, indent)
	else:
		indent_str = ' ' * indent
		print('%s%s' % (indent_str, data))

# ----------------------------------------------------------------------
#  Process start.
#
print('%s: start: %s' % (prog, Util.now()))

# ----------------------------------------------------------------------
#  1st step: Make Springhead up-to-date.
#
if check_exec('DAILYBUILD_UPDATE_SPRINGHEAD'):
	pwd()
	print('updating "Springhead"')
	os.chdir(spr_topdir)
	cmnd = 'git pull --all'
	proc.exec(cmnd, stdout=Proc.PIPE, stderr=Proc.STDOUT)
	rc = proc.wait()
	outstr, errstr = proc.output()
	Print(outstr.split('\n'))
	if errstr:
		Print('-- error --')
		Print(errstr.split('\n'))
	if rc != 0:
		err.print('updating failed: status %d' % rc)
	os.chdir(start_dir)

# ----------------------------------------------------------------------
#  2nd step: Clearing test directory.
#
if check_exec('DAILYBUILD_CLEANUP_WORKSPACE'):
	os.chdir(prep_dir)
	pwd()
	if os.path.exists(repository):
		print('clearing "%s"' % repository)
		for root, dirs, files in os.walk(repository, topdown=False):
			for f in files:
				path = '%s/%s' % (root, f)
				os.chmod(path, stat.S_IWRITE)
				os.unlink(path)
			for d in dirs:
				path = '%s/%s' % (root, d)
				os.rmdir(path)
		if Util.is_windows(): sleep(1)
		os.rmdir(repository)
	else:
		print('test repository "%s" not exist' % repository)
	os.chdir(start_dir)
	print()

# ----------------------------------------------------------------------
#  3rd step: Clone repository.
#
if check_exec('DAILYBUILD_CLEANUP_WORKSPACE'):
	os.chdir(prep_dir)
	pwd()
	print('cloning test repository')
	cmnd = 'git clone %s %s' % (url_git, repository)
	rc = proc.exec(cmnd).wait()
	if rc != 0:
		err.print('cloning failed: status %d' % rc)

	os.chdir(repository)
	pwd()
	print('updating submodules')
	cmnd = 'git submodule update --init'
	rc = proc.exec(cmnd).wait()
	if rc != 0:
		err.print('cloning failed: status %d' % rc)

	"""
	print('checking out "closed"')
	cmnd = 'svn co %s %s' % (url_svn, 'closed')
	rc = proc.exec(cmnd).wait()
	if rc != 0:
		err.print('cloning failed: status %d' % rc)
	"""
	os.chdir(prep_dir)

# ----------------------------------------------------------------------
#  The process hereafter will be executed under test-repository.
#
os.chdir('%s/%s' % (prep_dir, repository))

# ----------------------------------------------------------------------
#  4th step: Execute DailyBuild test.
#
os.chdir('core/test')
pwd()
print('Test start:')
args = '/r %s /t %s /c %s /p %s' % (repository, tool, conf, plat)
cmnd = 'TestMainGit.bat %s' % args
rc = proc.exec(cmnd, shell=True).wait()
Print('rc: %s' % rc)

sys.exit(0)
# ----------------------------------------------------------------------
#  Process end.
#
os.chdir(start_dir)
print('%s: end: %s' % (prog, Util.now()))
sys.exit(0)

# end: DailyBuild.py


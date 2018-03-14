#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  SYNOPSIS:
#	UnityTest [options] scene
#	options:
#	    -e:		Trace command to be executed.
#	    -i file:	Init file name [default: "UnityTest.ini"].
#	    -t time:	Execution limit time in desconds [default: 15].
#	    -T:		Add time stamp.
#
#  DESCRIPTION:
#	Build and run unity scene automatically.
#	Only one scene at a time is available.
#
#  VERSION:
#	Ver 1.0  2016/06/20 F.Kanehori	初版
#	Ver 1.1  2016/06/23 F.Kanehori	replace V2.0 に対応
#	Ver 1.2  2016/10/13 F.Kanehori	Python module の改定に対応
#	Ver 1.3  2017/08/10 F.Kanehori	リポジトリの再構築に対応
#	Ver 1.4  2017/09/06 F.Kanehori	Python module の改定に対応
#	Ver 1.5  2017/11/16 F.Kanehori	Python module の移動に対応
#	Ver 1.6  2017/12/06 F.Kanehori	Python module の移動に対応
# ==============================================================================
version = 1.6
import sys
import os
import subprocess
from multiprocessing import Process
from time import sleep
from datetime import datetime
from datetime import timedelta
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
spr_path = FindSprPath(prog)
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from KvFile import *
from FileOp import *
from Util import *
from Proc import *
from Error import *

# ----------------------------------------------------------------------
#  Program names
#
python = 'python'

# ----------------------------------------------------------------------
#  Methods
#
def save_file(filename, msg):
	savename = filename + '.save'
	verbose('saving ' + filename)
	result = f_op.cp(filename, savename)
	if result != 0:
		error.print('can\'t save %s' % msg)
	return savename

def revive_file(filename, msg):
	savename = filename + '.save'
	verbose('reviving ' + filename)
	result = f_op.rm(filename)
	if result == 0:
		result = f_op.mv(savename, filename)
	if result != 0:
		error.print('can\'t revive %s' % msg)

def result_str(code):
	if code == proc.ETIME:
		return '0 (success - timeout)'
	str = 'success' if code >= 0 else 'fail'
	return '%d (%s)' % (code, str)

def verbose(msg, level=0):
	if options.verbose > level:
		print(msg)

def info(msg, name=None, has_next=False, continued=False):
	if options.verbose > 0 or not continued:
		if options.timestamp:
			sys.stdout.write(util.now() + ': ')
		if not name is None:
			sys.stdout.write(name + ': ')
	sys.stdout.write(msg)
	eol = '.. ' if not options.verbose and has_next else '\n'
	sys.stdout.write(eol)
	sys.stdout.flush()

# ----------------------------------------------------------------------
#  Process for command line
#
usage = "Usage: %prog [options] scene"
parser = OptionParser(usage = usage)
parser.add_option('-e', '--exec-trace',
			dest='exec_trace', action='store_true', default=False,
			help='trace command to be executed')
parser.add_option('-i', '--init',
			dest='inifile', default='UnityTest.ini',
			help='init file name [default: %default]', metavar='FILE')
parser.add_option('-t', '--timeout',
			dest='timeout', default=15, metavar='TIME',
			help='execution limit time in seconds [default: %default]')
parser.add_option('-T', '--timestamp',
			dest='timestamp', action='store_true', default=False,
			help='add time stamp')
parser.add_option('-v', '--verbose',
			dest='verbose', action='count', default=0,
			help='set verbose mode')
parser.add_option('-V', '--version',
			dest='version', action='store_true', default=False,
			help='show version')
(options, args) = parser.parse_args()

if options.version:
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
#
if len(args) != 1:
	parser.error("incorrect number of arguments")

# Scene name
scene   = args[0]
if scene[-6:] != '.unity':
	scene += '.unity'

if options.verbose:
	print('  inifile:\t%s' % (options.inifile))
	print('  scene:\t%s' % (scene))

# ----------------------------------------------------------------------
#  Globals
#
error = Error(prog)
util = Util()
proc = Proc(verbose=options.exec_trace)
f_op = FileOp(verbose=options.verbose)

# ----------------------------------------------------------------------
#  Read init file
#
mandatory_keys = ['Unity', 'Springhead', 'UnityProject', 'DllPath' ]

kvf = KvFile(options.inifile)
if (kvf.read() < 0):
	error.print(kvf.error())
if kvf.check(mandatory_keys) != 0:
	error.print(kvf.error())

# set defaults
defaults = {}
spr_root = kvf.get('Springhead')
defaults['ScenesDir']	 = 'Assets/Springhead/Samples'
defaults['TestRoot']	 = spr_root + '/core/test/UnityTest'
defaults['OutFile']	 = defaults['TestRoot'] + '/bin/player.exe'
defaults['LogFile']	 = defaults['TestRoot'] + '/log/build.log'
for key in defaults.keys():
	if kvf.get(key) is None:
		kvf.set(key, defaults[key])
if options.verbose:
	print('  ------------------------------------------------')
	for key in sorted(mandatory_keys):
		print('  %s:\t%s' % (key, kvf.get(key)))
	print('  ------------------------------------------------')
	for key in sorted(defaults.keys()):
		print('  %s:\t%s' % (key, kvf.get(key)))

# prepare directories
outdir = os.path.dirname(kvf.get('OutFile'))
logdir = os.path.dirname(kvf.get('LogFile'))
if not os.path.isdir(outdir):
	verbose('creating directory "%s"' % (outdir))
	os.makedirs(outdir)
if not os.path.isdir(logdir):
	verbose('creating directory "%s"' % (outdir))
	os.makedirs(logdir)

# ----------------------------------------------------------------------
#  Change ProjectSettings so as not to display runtime dialog
#  Also change EditorBuildSeetings file to select scene correctly.
#
info('compiling ', scene, has_next=True)

project_base = kvf.get('UnityProject')
project_setting_file = project_base + '/ProjectSettings/ProjectSettings.asset'
editor_setting_file = project_base + '/ProjectSettings/EditorBuildSettings.asset'
saved_project_setting_file = save_file(project_setting_file, 'ProjectSetting file')
saved_edito_setting_file = save_file(editor_setting_file, 'EditorBuildSetting file')

# replace parameter for PtojectSetting file
pattern ='"displayResolutionDialog: 1=displayResolutionDialog: 0"'
script = '%s replace.py' % python
params = ''
if options.verbose:
	params += ' -v'
ifile = saved_project_setting_file
ofile = project_setting_file
args = '%s %s %s %s %s' % (script, params, ifile, ofile, pattern)
verbose(args, 1)
proc = proc.exec(args)
status = proc.wait()
if status != 0:
	error.print('can\'t replace ProjectSettings file')

# replace parameter for EditorBuildSetting file
pattern = '"scene.unity=%s/%s"' % (kvf.get('ScenesDir'), scene)
ifile = 'EditorBuildSettings.asset.dat'
ofile = editor_setting_file
args = '%s %s %s %s %s' % (script, params, ifile, ofile, pattern)
verbose(args, 1)
proc = proc.exec(args)
status = proc.wait()
if status != 0:
	error.print('can\'t replace EdtorBuildSettings file')

# ----------------------------------------------------------------------
#  Build and Run
#
f_op.rm(util.pathconv(kvf.get('OutFile')))
#
projpath = '-projectPath %s' % kvf.get('UnityProject')
execmode = '-executeMethod BuildClass.Build'
quiet	 = '-batchmode -quit'
logfile	 = '-logfile %s' % kvf.get('LogFile')
outfile	 = '-buildWindows64Player %s' % kvf.get('OutFile')
target	 = '-target %s/%s' % (kvf.get('ScenesDir'), scene)
args = '%s %s %s %s %s %s' % (projpath, execmode, quiet, outfile, logfile, target)
cmnd = util.pathconv('%s %s' % (kvf.get('Unity'), args))
verbose(cmnd, 1)
#
proc = proc.exec(cmnd, shell=True)
status = proc.wait()
result = status if os.path.isfile(kvf.get('OutFile')) else -1
info(result_str(result), scene, continued=True)
exitcode = result
#
if result == 0:
	executable = util.pathconv(kvf.get('OutFile'))
	f_op.touch(executable)
	#
	info('executing ', scene, has_next=True)
	addpath = util.pathconv(kvf.get('DllPath'))
	#
	verbose(cmnd, 1)
	proc = proc.exec(executable, addpath=addpath)
	status = proc.wait(options.timeout)
	#
	info(result_str(result), scene, continued=True)
	exitcode = status

# ----------------------------------------------------------------------
#  Revice ProjectSettings
#
revive_file(project_setting_file, 'ProjectSetting file')
revive_file(editor_setting_file, 'EditorBuildSetting file')

sys.exit(exitcode)

# end: UnityTest.py

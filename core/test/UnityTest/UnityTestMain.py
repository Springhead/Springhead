#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	UnityTestMain [options]
#	options:
#	    -e:		Trace command to be executed.
#	    -i file:	Init file name [default: "UnityTest.ini"].
#	    -l file:	Log file name [default: "UnityTest.ini"].
#	    -t time:	Execution limit time in desconds [default: 15].
#	    -T:		Add time stamp.
#	    -1:		Execute only one scene at the top of the scene list.
#
#  DESCRIPTION:
#	Build and run sepcified scenes automatically.
#
#  VERSION:
#	Ver 1.0  2016/06/20 F.Kanehori	初版
#	Ver 1.1  2016/10/12 F.Kanehori	Python module の改定に対応
#	Ver 1.2  2017/09/06 F.Kanehori	Python module の改定に対応
#	Ver 1.3  2017/11/16 F.Kanehori	Python module の移動に対応
#	Ver 1.4  2017/12/06 F.Kanehori	Python module の移動に対応
# ======================================================================
version = 1.4
import sys
import os
import subprocess
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
unitytest_script = 'UnityTest.py'
extract_script = 'log_extraction.py'
append_script = 'log_append.py'

# ----------------------------------------------------------------------
def logmsg(msg, file):
	cmnd = 'echo %s' % msg
	proc = Proc(verbose=0)
	proc.exec(cmnd, stdout=file, append=True, shell=True)
	proc.wait()

def verbose(msg, level=0):
	if options.verbose > level:
		print(msg)

# ----------------------------------------------------------------------
#  Process for command line
#
usage = "Usage: %prog [options]"
parser = OptionParser(usage = usage)
parser.add_option('-e', '--exec-trace',
			dest='exec_trace', action='store_true', default=False,
			help='trace execute command [default: %default]')
parser.add_option('-i', '--init',
			dest='inifile', default='UnityTestMain.ini',
			help='init file name [default: %default]', metavar='FILE')
parser.add_option('-l', '--log',
			dest='logfile',
			help='log file name', metavar='FILE')
parser.add_option('-t', '--timeout',
			dest='timeout', default=15, metavar='TIME',
			help='execution limit time in seconds [default: %default]')
parser.add_option('-T', '--timestamp',
			dest='timestamp', action='store_true', default=False,
			help='add time stamp')
parser.add_option('-1', '--onlyone',
			dest='onlyone', action='store_true', default=False,
			help='execute only one scene at the top of the scene list')
parser.add_option('-v', '--verbose',
			dest='verbose', action='count', default=0,
			help='set verbose mode')
parser.add_option('-V', '--version',
			dest='version', action='store_true', default=False,
			help='show version')
(options, args) = parser.parse_args()

#
prog = sys.argv[0].split('\\')[-1].split('.')[0]
if options.version:
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
#
if len(args) != 0:
	parser.error("incorrect number of arguments")

# Scene name
if options.verbose:
	print('  inifile:\t%s' % (options.inifile))

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
mandatory_keys = [
	'Springhead', 'UnityProject', 'SceneList'
]

kvf = KvFile(options.inifile)
if (kvf.read() < 0):
	sys.exit(-1)
if kvf.check(mandatory_keys) != 0:
	sys.exit(-1)

# set defaults
defaults = {}
spr_root = kvf.get('Springhead')
defaults['TestRoot']	 = spr_root + '/core/test/UnityTest'
defaults['IniFile']	 = defaults['TestRoot'] + '/UnityTest.ini'
defaults['ScenesDir']	 = 'Assets/Springhead/Samples'
defaults['TestMainLogFile'] = defaults['TestRoot'] + '/log/TestMain.log'
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

# ----------------------------------------------------------------------
#  Execute 'UnityTestScript' for all scenes in 'SecneList'
#
scenes_dir = kvf.get('ScenesDir')
scene_list = kvf.get('SceneList')
if options.verbose:
	print('  scenes_dir:\t%s' % (scenes_dir))
	print('  scene_list:\t%s' % (scene_list))

# command for unity test
script = '%s %s' % (python, unitytest_script)
args = ' -i ' + kvf.get('IniFile')
if options.exec_trace:	args += ' -e'
if options.verbose:	args += ' -v'
if options.timeout:	args += ' -t ' + str(options.timeout)
if options.timestamp:	args += ' -T'

# clear log file
#
ifile = kvf.get('LogFile')
if ifile is None:
	ifile = kvf.get('TestRoot') + '/log/build.log'
ofile = options.logfile
if ofile is None:
	ofile = kvf.get('TestMainLogFile')
if options.verbose:
	print('  extract_log:\t%s' % (ofile))
f_op.rm(util.pathconv(ofile))

# command for log extraction
cmnd_e = '%s %s %s' % (python, extract_script, util.pathconv(ifile))
cmnd_a = '%s %s %s' % (python, append_script, util.pathconv(ofile))

# execute build and run
#
status = 0
scenes = scene_list.split()
for scene in scenes:
	cmnd = '%s %s %s' % (script, args, scene)
	verbose(cmnd, 0)
	proc = proc.exec(cmnd, shell=True)
	status = proc.wait()
	if status != 0 and status != proc.ETIME:
		error.print('unity returned %d' % status)
	# log extraction
	logmsg('======================', ofile)
	logmsg(' ' + scene, ofile)
	logmsg('======================', ofile)
	tmpf = ofile + '.tmp'
	verbose(cmnd_e, 0)
	proc1 = Proc(verbose=options.verbose)
	proc2 = Proc()
	proc1.exec(cmnd_e, stdout=proc.PIPE, stderr=proc.STDOUT)
	proc2.exec(cmnd_a, stdin=proc1.proc.stdout, stdout=proc.NULL, stderr=proc.NULL)
	status1 = proc1.wait()
	status2 = proc2.wait()
	sys.stdout.flush()
	
	# for debug
	if options.onlyone:
		break

sys.exit(status)

# end: UnityTestMain.py

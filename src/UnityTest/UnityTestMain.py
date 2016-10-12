#!/usr/bin/env python
# -*- coding: utf-8 -*-
# ======================================================================
#  FILE:
#	UnityTestMain.py
#
#  DESCRIPTION:
#	Build and run sepcified scenes automatically.
#
#  VERSION:
#	Ver 1.0  2016/06/20 F.Kanehori	First version
#	Ver 1.1  2016/10/12 F.Kanehori	Revised by using modules
# ======================================================================
version = 1.0
import sys
import os
import subprocess
from optparse import OptionParser

sys.path.append('../../bin/test')
from KvFile import *
from Util import *

# ----------------------------------------------------------------------
def logmsg(msg, file):
	cmd = 'echo ' + msg + ' >> ' + file
	#verbose(cmd, 0)
	return Util.exec(cmd, shell=True)

def verbose(msg, level=0):
	if options.verbose > level:
		print(msg)

def error(msg):
	Util.error(prog + ': Error: ' + msg)

# ----------------------------------------------------------------------
#  Process for command line
#
usage = "Usage: %prog [options]"
parser = OptionParser(usage = usage)
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
#  Read init file
#
mandatory_keys = ['UnityTestScript', 'Python', 'Springhead2', 'SceneList' ]

kvf = KvFile(options.inifile)
if (kvf.read() < 0):
	sys.exit(-1)
if kvf.check(mandatory_keys) != 0:
	sys.exit(-1)

# set defaults
defaults = {}
spr2root = kvf.get('Springhead2')
defaults['UnityProject'] = spr2root + '/src/Unity'
defaults['TestRoot']	 = spr2root + '/src/UnityTest'
defaults['IniFile']	 = defaults['TestRoot'] + '/UnityTest.ini'
defaults['ScenesDir']	 = 'Assets/Scenes'
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
script = kvf.get('UnityTestScript')
args = ' -i ' + kvf.get('IniFile')
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
cmd = 'del %s' % Util.dospath(ofile)
verbose(cmd, 0)
result = Util.exec(cmd, stdout=Util.NULL, stderr=Util.STDOUT,
		   shell=True, verbose=options.verbose)

# command for log extraction
extract = kvf.get('Python') + ' ' + kvf.get('ExtractScript')
extract += ' ' + ifile
extract = Util.dospath(extract)

# execute build and run
#
scenes = scene_list.split()
for scene in scenes:
	cmd = script + args + ' ' + scene
	verbose(cmd, 0)
	result = Util.exec(cmd, shell=True, verbose=options.verbose)
	if result != 0 and result != Util.ETIME:
		error('unity returns %d' % result)
	# log extraction
	logmsg('======================', ofile)
	logmsg(' ' + scene, ofile)
	logmsg('======================', ofile)
	verbose(extract, 0)
	result = Util.exec(extract, stdout=ofile, stderr=Util.STDOUT, shell=True)
	# for debug
	if options.onlyone:
		break

sys.exit(result)

# end: UnityTestMain.py

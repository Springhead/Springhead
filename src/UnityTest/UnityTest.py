#!/usr/bin/env python
# -*- coding: utf-8 -*-
# ======================================================================
#  FILE:
#	UnityTest.py
#
#  DESCRIPTION:
#	Build and run unity scene automatically.
#	Only one scene at a time is available.
#
#  VERSION:
#	Ver 1.0  2016/06/20 F.Kanehori	First version
#	Ver 1.1  2016/06/23 F.Kanehori	Correspond to 'replace' V2.0
#	Ver 1.2  2016/10/13 F.Kanehori	Revised by using modules
#	Ver 1.2a 2016/11/02 F.Kanehori	Fixed exec_trace default value.
# ======================================================================
version = 1.0
import sys
import os
import subprocess
from multiprocessing import Process
from time import sleep
from datetime import datetime
from datetime import timedelta
from optparse import OptionParser

sys.path.append('../../bin/test')
from KvFile import *
from Util import *

# ----------------------------------------------------------------------
def result_str(code):
	if code == Util.ETIME:
		return '0 (success - timeout)'
	str = 'success' if code >= 0 else 'fail'
	return '%d (%s)' % (code, str)

def verbose(msg, level=0):
	if options.verbose > level:
		print(msg)

def info(msg, name=None, has_next=False, continued=False):
	if options.verbose > 0 or not continued:
		if options.timestamp:
			sys.stdout.write(Util.now() + ': ')
		if not name is None:
			sys.stdout.write(name + ': ')
	sys.stdout.write(msg)
	eol = '.. ' if not options.verbose and has_next else '\n'
	sys.stdout.write(eol)
	sys.stdout.flush()

def fatal(msg, code = -1):
	Util.error(prog + ': Error: ' + msg)
	sys.exit(code)

# ----------------------------------------------------------------------
#  Process for command line
#
usage = "Usage: %prog [options] scene"
parser = OptionParser(usage = usage)
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

prog = sys.argv[0].split('\\')[-1].split('.')[0]
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
#  Read init file
#
mandatory_keys = ['Unity', 'Python', 'Springhead2', 'DllPath' ]

kvf = KvFile(options.inifile)
if (kvf.read() < 0):
	sys.exit(-1)
if kvf.check(mandatory_keys) != 0:
	sys.exit(-1)

# set defaults
defaults = {}
spr2root = kvf.get('Springhead2')
defaults['UnityProject'] = spr2root + '/src/Unity'
defaults['ScenesDir']	 = 'Assets/Scenes'
defaults['TestRoot']	 = spr2root + '/scr/UnityTest'
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

# flag for trace execute command
exec_trace = 0
#exec_trace = 1

# ----------------------------------------------------------------------
#  Change ProjectSettings so as not to display runtime dialog
#
info('compiling ', scene, has_next=True)
project_base = kvf.get('UnityProject')
setting_file = project_base + '/ProjectSettings/ProjectSettings.asset'
saved_file = setting_file + '.save'
verbose('saving ' + setting_file)

# save original one
args = Util.dospath('copy %s %s' % (setting_file, saved_file))
result = Util.exec(args, stdout=Util.NULL, stderr=Util.STDOUT, shell=True,
		   verbose=exec_trace)
if result != 0:
	fatal('can\'t save ProjectSettings file')

# replace parameter
replace_pattern ="displayResolutionDialog: 1=displayResolutionDialog: 0"
script = Util.dospath('%s replace.py' % kvf.get('Python'))
params = ''
if options.verbose:
	params += ' -v'
ifile = saved_file
ofile = setting_file
patterns = '"' + replace_pattern + '"'
args = '%s %s %s %s %s' % (script, params, ifile, ofile, patterns)
verbose(args, 1)
result = Util.exec(args, verbose=exec_trace)
if result != 0:
	fatal('can\'t replace ProjectSettings file')

# ----------------------------------------------------------------------
#  Build and Run
#
args = Util.dospath('del %s' % kvf.get('OutFile'))
verbose(args, 1)
result = Util.exec(args, stdout=Util.NULL, stderr=Util.STDOUT,
		   shell=True, verbose=exec_trace)
#
projpath = '-projectPath %s' % kvf.get('UnityProject')
execmode = '-executeMethod BuildClass.Build'
quiet	 = '-batchmode -quit'
logfile	 = '-logfile %s' % kvf.get('LogFile')
outfile	 = '-output %s' % kvf.get('OutFile')
target	 = '-target %s/%s' % (kvf.get('ScenesDir'), scene)
args = '%s %s %s %s %s %s' % (projpath, execmode, quiet, outfile, logfile, target)
cmnd = Util.dospath('%s %s' % (kvf.get('Unity'), args))
verbose(cmnd, 1)
#
result = Util.exec(cmnd, shell=True, verbose=exec_trace)
result = result if os.path.isfile(kvf.get('OutFile')) else -1
info(result_str(result), scene, continued=True)
exitcode = result
#
if result == 0:
	info('executing ', scene, has_next=True)
	cmnd = Util.dospath(kvf.get('OutFile'))
	addpath = Util.dospath(kvf.get('DllPath'))
	#
	verbose(cmnd, 1)
	result = Util.exec(cmnd, shell=False, addpath=addpath,
			   timeout=options.timeout, verbose=exec_trace)
	#
	info(result_str(result), scene, continued=True)
	exitcode = result

# ----------------------------------------------------------------------
#  Revice ProjectSettings
#
verbose('reviving ' + setting_file)

cmnd = 'del %s' % Util.dospath(setting_file)
verbose(cmnd, 1)
result = Util.exec(cmnd, shell=True, stdout=Util.NULL, stderr=Util.STDOUT,
		   verbose=exec_trace)

cmnd = Util.dospath('move %s %s' % (saved_file, setting_file))
verbose(cmnd, 1)
result = Util.exec(cmnd, shell=True, stdout=Util.NULL, stderr=Util.STDOUT,
		   verbose=exec_trace)

sys.exit(exitcode)

# end: UnityTest.py

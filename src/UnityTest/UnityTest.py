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
from KvFile import KvFile

# ----------------------------------------------------------------------
def get_date():
	today = str(datetime.today())
	return today[0:19].replace('-', '/')

def dir_part(path):
	return path[0:path.rindex('/')]

def s16(value):
	return -(value & 0b1000000000000000) | (value & 0b0111111111111111)

def result_str(code):
	str = 'success' if code >= 0 else 'fail'
	return '%d (%s)' % (code, str)

def verbose(msg, level=0):
	if options.verbose > level:
		print(msg)

def info(msg, name=None, has_next=False, continued=False):
	if options.verbose > 0 or not continued:
		if options.timestamp:
			sys.stdout.write(get_date() + ': ')
		if not name is None:
			sys.stdout.write(name + ': ')
	sys.stdout.write(msg)
	eol = '.. ' if not options.verbose and has_next else '\n'
	sys.stdout.write(eol)
	sys.stdout.flush()

def fatal(msg, code = -1):
	sys.stderr.write(script + ': Error: ' + msg + '\n')
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

script = sys.argv[0].split('\\')[-1].split('.')[0]
if options.version:
	print('%s: Version %s' % (script, version))
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
outdir = dir_part(kvf.get('OutFile'))
logdir = dir_part(kvf.get('LogFile'))
if not os.path.isdir(outdir):
	verbose('creating directory "%s"' % (outdir))
	os.makedirs(outdir)
if not os.path.isdir(logdir):
	verbose('creating directory "%s"' % (outdir))
	os.makedirs(logdir)

# ----------------------------------------------------------------------
#  Change ProjectSettings so as not to display runtime dialog
#
info('compiling ', scene, has_next=True)
project_base = kvf.get('UnityProject')
setting_file = project_base + '/ProjectSettings/ProjectSettings.asset'
saved_file = setting_file + '.save'
verbose('saving ' + setting_file)

# save original one
cmd = ' '.join(['copy', setting_file, saved_file, '> NUL'])
result = subprocess.call(cmd.replace('/', '\\'), shell=True)
if result != 0:
	fatal('can\'t save ProjectSettings file')

# replace parameter
replace_pattern ="displayResolutionDialog: 1=displayResolutionDialog: 0"
script = 'replace.py'
params = ''
if options.verbose:
	params += ' -v'
ifile = saved_file
ofile = setting_file
patterns = '"' + replace_pattern + '"'
cmd = ' '.join([kvf.get('Python'), script, params, ifile, ofile, patterns])
verbose(cmd, 1)
result = subprocess.call(cmd, shell=True)
if result != 0:
	fatal('can\'t replace ProjectSettings file')

# ----------------------------------------------------------------------
#  Build and Run
#
cmd = ' '.join(['del', kvf.get('OutFile'), '1>NUL 2>&1'])
verbose(cmd, 1)
result = subprocess.call(cmd.replace('/', '\\'), shell=True)
#
projpath = '-projectPath ' + kvf.get('UnityProject')
execmode = '-executeMethod BuildClass.Build'
quiet	 = '-batchmode -quit'
logfile	 = '-logfile ' + kvf.get('LogFile')
outfile	 = '-output ' + kvf.get('OutFile')
target	 = '-target ' + kvf.get('ScenesDir') + '/' + scene
args = ' '.join([projpath, execmode, quiet, outfile, logfile, target])
cmd  = ' '.join([kvf.get('Unity'), args])
verbose(cmd, 1)
#
result = subprocess.call(cmd.replace('/', '\\'), shell=True)
result = result if os.path.isfile(kvf.get('OutFile')) else -1
info(result_str(s16(result)), scene, continued=True)
exitcode = result
#
if result == 0:
	info('executing ', scene, has_next=True)
	cmd = kvf.get('OutFile')
	newpath = os.environ.get('PATH') + ';' + kvf.get('DllPath').replace('/', '\\')
	#
	verbose(cmd, 1)
	proc = subprocess.Popen(cmd.replace('/', '\\'), env={"PATH": newpath})
	sleep(float(options.timeout))
	if proc.poll() is None:
		# process is still alive
		proc.kill()
		result = 0
	else:
		result = p.poll()
	#
	info(result_str(s16(result)), scene, continued=True)
	exitcode = result

# ----------------------------------------------------------------------
#  Revice ProjectSettings
#
verbose('reviving ' + setting_file)

cmd = ' '.join(['del', setting_file, '1>NUL 2>&1'])
verbose(cmd, 1)
result = subprocess.call(cmd.replace('/', '\\'), shell=True)

cmd = ' '.join(['move', saved_file, setting_file, '1>NUL 2>&1'])
verbose(cmd, 1)
result = subprocess.call(cmd.replace('/', '\\'), shell=True)

sys.exit(exitcode)

# end: UnityTest.py

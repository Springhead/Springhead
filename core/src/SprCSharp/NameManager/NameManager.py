#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  FILE:
#	NameManager.py
#
#  SYNOPSIS:
#	NameManager [-d srctop] [-i inifile] [-o outfile]
#	    srctop:	Top directory name of system src tree (str).
#	    inifile:	Environment variable definition file name (str).
#	    outfile:	Batch file name to be generated (str).
#
#  DESCRIPTION:
#	Manager for common names used throughout the SprCSharp system.
#	Names are read from definition file (e.g. NameManager.ini) and
#	wrritten to Windows batch file (e.g. NameManager.bat).
#	To define and use these names as environment variables, call
#	batch file created by this script:
#	    call NameManager.bat
#
# ----------------------------------------------------------------------
#  VERSION:
#     Ver 1.00   2017/01/16 F.Kanehori	First release version.
#     Ver 1.01   2017/01/18 F.Kanehori	Change directory position (-d).
#     Ver 1.02   2017/01/27 F.Kanehori	Bug fixed.
#     Ver 1.03   2017/09/07 F.Kanehori	Change to new python library.
#     Ver 1.04   2017/11/29 F.Kanehori	Change python library path.
#     Ver 1.04.1 2018/03/12 F.Kanehori	Now OK for doxygen.
# ======================================================================
##  Program version.
version = "1.04.1"
debug = False
trace = False

import sys
import os
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Identify myself
#
ScriptFileDir = os.sep.join(os.path.abspath(__file__).split(os.sep)[:-1])
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
if trace:
	print('ENTER: %s %s' % (prog, ' '.join(sys.argv[1:])))
	sys.stdout.flush()

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++
SrcDir = '/'.join(ScriptFileDir.split(os.sep)[:-2])
SetupExists = os.path.exists('%s/setup.conf' % SrcDir)
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++

# ----------------------------------------------------------------------
#  Import Springhead python library.
#
sys.path.append('../../RunSwig')
from FindSprPath import *
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++
if SetupExists:
	libdir = '%s/RunSwig/pythonlib' % SrcDir
	srcdir = SrcDir.split('/')[-1]
else:
	#  移行後は FindSprPath のインポートは不要
	spr_path = FindSprPath(prog)
	libdir = spr_path.abspath('pythonlib')
	srcdir = 'src'
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++
sys.path.append(libdir)
from SetupFile import *
from KvFile import *
from Util import *
from Proc import *
from Error import *

# ----------------------------------------------------------------------
#  Set environment variables
#
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++
if SetupExists:
	print('%s: 移行処理' % prog)
	sf = SetupFile('%s/setup.conf' % SrcDir, verbose=1)
	sf.setenv()
	python = sf.getenv('python')
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options]'
parser = OptionParser(usage = usage)
parser.add_option('-d', '--srctop',
			dest='srctop', default=srcdir,
			metavar='DIR',
			help='top directory of src tree [default: %default]')
parser.add_option('-i', '--inifile',
			dest='inifile', default='NameManager.ini',
			metavar='FILE',
			help='name definiton file name [default: %default]')
parser.add_option('-o', '--outfile',
			dest='outfile', default='NameManager.bat',
			metavar='FILE',
			help='output batch file name [default: %default]')
parser.add_option('-t', '--test',
			dest='test_only', action='store_true', default=False,
			help='do not make batch file (for debug)')
parser.add_option('-v', '--verbose',
			dest='verbose', action='count', default=1,
			help='set verbose mode')
parser.add_option('-V', '--version',
			dest='version', action='store_true', default=False,
			help='show version')

# ----------------------------------------------------------------------
#  Process for command line
#
##  Command line options and arguments.
(options, args) = parser.parse_args()
if options.version:
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
if len(args) != 0:
	proc = Proc(verbose=options.verbose)
	proc.exec('%s %s.py -h' % (python, prog)).wait()
	parser.error("incorrect number of arguments")
	sys.exit(0)

# Options.
srctop	= options.srctop	# top directory of source tree
inifile	= options.inifile	# initial file name
outfile	= options.outfile	# outout file name
test_only = options.test_only	# do not make output file (for debug)
verbose	= options.verbose	# verbose mode

if verbose:
	print('argument used:')
	print('  srctop:    %s' % srctop)
	print('  inifile:   %s' % inifile)
	print('  outfile:   %s' % outfile)
	print('  test_only: %s' % test_only)
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if SetupExists:
		print('  python   : %s' % python)
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++
	print()

# ----------------------------------------------------------------------
#  Find base directory's absolute path.
#
dirlist = os.getcwd().split(os.sep)
found = False
for n in range(len(dirlist)):
	if dirlist[n] == srctop:
		found = True
		break
if not found:
	Error(prog).put('no such directory "%s"' % srctop)
##  Absolute path of base directory.
topdir = os.sep.join(dirlist[0:n]) if found else None

# ----------------------------------------------------------------------
#  Read name definitions.
#
kvf = KvFile(inifile, sep='=')
count = kvf.read(dic={'TOPDIR': topdir})
if count < 0:
	Error(prog).put(kvf.error())
##  List of keys in the inifile.
keys = sorted(kvf.keys())

if verbose:
	##  Make fixed length string.
	#   @param name		Any string.
	#   @param width	Field width (int).
	#   @returns		Fixed width string.
	def fixed(name, width):
		name += ':'
		if len(name) < width-1:
			name += ' ' * (width-len(name))
		return name

	print('names defined (%d):' % count)
	for key in keys:
		value = Util().pathconv(kvf.get(key), 'unix')
		if os.path.isfile(value):	kind = 'file'
		elif os.path.isdir(value):	kind = 'dir'
		else:				kind = 'name'
		print('  %s%s (%s)' % (fixed(key, 24), value, kind))

# ----------------------------------------------------------------------
#  Make Windows batch file to export names.
#	This is unnecessary if names can be exported directly!!
#
if test_only:
	sys.exit(0)

##  Header strings of output file.
headers	 = [ '@echo off', 'rem FILE: %s' % outfile, '' ]
##  Trailer strings of output file.
trailers = [ '', 'rem end: %s' % outfile ]

f = TextFio(outfile, 'w')
if f.open() < 0:
	Error(prog).put(f.error())
#
f.writelines(headers)
for key in keys:
	line = 'set %s=%s' % (key, Util().pathconv(kvf.get(key)))
	if verbose > 1:
		print(line)
	f.writeline(line)
f.writelines(trailers)
f.close()

sys.exit(0)

# end: NameManager.py

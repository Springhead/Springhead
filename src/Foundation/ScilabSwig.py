#!/usr/local/bin/python3.4
# -*- coding: utf-8 -*-
# ==============================================================================
#  FILE:
#	ScilabSwig.py
#
#  SYNOPSIS:
#	python ScilabSwig.py
#
#  DESCRIPTION:
#	Scilab モジュールについて, swig を実行するための makefile を作成し,
#	make を実行する (Scilab.i -> ../../include/Scilab/ScilabStub.hpp).
#
# ==============================================================================
#  Version:
#	Ver 1.0	 2017/04/19 F.Kanehori	Windows batch file から移植.
# ==============================================================================
version = 1.0
debug = False

import sys
import os
import glob
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Import Springhead2 python library.
#
cwd = os.getcwd().split(os.sep)[::-1]
for n in range(len(cwd)):
	if cwd[n] != 'src': continue
	spr2 = '/'.join(cwd[::-1][0:len(cwd)-n-1])
	break
libdir = '%s/bin/test' % spr2
sys.path.append('/usr/local/lib')
sys.path.append(libdir)
from TextFio import *
from Util import *
from Error import *

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split('\\')[-1].split('.')[0]
python_version = 34
module = 'Scilab'

# ----------------------------------------------------------------------
#  Globals
#
U = Util()
unix = U.is_unix()

# ----------------------------------------------------------------------
#  Directories
#
spr2top = U.pathconv(os.path.relpath(spr2), 'unix')
bindir = '%s/%s' % (spr2top, 'bin')
srcdir = '%s/%s' % (spr2top, 'src')
incdir = '%s/%s' % (spr2top, 'include')
swigdir = '%s/%s' % (bindir, 'swig')
pythondir = '%s/Python%s' % (bindir, python_version)

incdir_rel = U.pathconv(os.path.relpath(incdir), 'unix')
srcdir_rel = U.pathconv(os.path.relpath(srcdir), 'unix')

# ----------------------------------------------------------------------
#  Scripts
#
pythonexe = 'python%s' % (python_version if unix else '')
python = '%s/%s' % (pythondir, pythonexe)
swig = '%s/%s' % (swigdir, 'swig' if unix else 'swig.exe')
make = 'make' if unix else 'nmake'

# ----------------------------------------------------------------------
#  Files
#
makefile = '%s%s' % (module, 'Stub.mak.txt')
stubfile = '%sStub.hpp' % module
stubpath = '%s/%s/%s' % (incdir, module, stubfile)

# ----------------------------------------------------------------------
#  Paths
#
addpath = os.pathsep.join([bindir, swigdir])

# ----------------------------------------------------------------------
#  Main process
# ----------------------------------------------------------------------
#  オプションの定義
#
usage = 'Usage: %prog [options]'
parser = OptionParser(usage = usage)
parser.add_option('-v', '--verbose',
			dest='verbose', action='count', default=0,
			help='set verbose count')
parser.add_option('-V', '--version',
			dest='version', action='store_true', default=False,
			help='show version')

# ----------------------------------------------------------------------
#  コマンドラインの解析
#
(options, args) = parser.parse_args()
if options.version:
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
if len(args) != 0:
	parser.error("incorrect number of arguments")

verbose	= options.verbose

# ----------------------------------------------------------------------
#  makefile を生成する.
#
lines = []
lines.append('#\tDo not edit. %sSwig.py will update this file.' % module)
lines.append('all: %s' % stubpath)
lines.append('%s: %s.i' % (stubpath, module))
args = '-dll -c++ %s.i' % module
lines.append('\t%s %s' % (U.pathconv(swig), args))
if unix:
	lines.append('\tmv -f %s %s' % (stubfile, stubpath))
else:
	lines.append('\tcopy %s %s' % (stubfile, os.path.split(stubpath)[0]))
	lines.append('\tdel %s' % stubfile)
#
def output(fname):
	fobj = TextFio(fname, 'w', encoding='utf8')
	if fobj.open() < 0:
		E.print(fobj.error())
	if fobj.writelines(lines, '\n') < 0:
		E.print(fobj.error())
	fobj.close()
#
if verbose:
	path = '%s/%s' % (os.getcwd(), makefile)
	print('  creating "%s"' % U.pathconv(path, 'unix'))
lines = U.pathconv(lines)
output(makefile)

# ----------------------------------------------------------------------
#  make を実行する.
#
swdesig = '-' if unix else '/'
cmd = '%s %sf %s' % (make, swdesig, U.pathconv(makefile))
U.exec(cmd, addpath=addpath, shell=True, verbose=0, dry_run=debug)

sys.exit(0)

# end: ScilabSwig.py

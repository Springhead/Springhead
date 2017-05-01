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
#	Ver 1.0	 2017/04/24 F.Kanehori	Windows batch file から移植.
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
swig = '%s/swig -I%s/Lib' % (swigdir, swigdir)
make = 'make' if unix else 'nmake'

# ----------------------------------------------------------------------
#  Files
#
makefile = '%sStub.mak.txt' % module
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
parser.add_option('-c', '--clean',
			dest='clean', action='store_true', default=False,
			help='execute target clean')
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

clean	= options.clean
verbose	= options.verbose

# ----------------------------------------------------------------------
#  makefile を生成する.
#

lines = []
lines.append('#\tDo not edit. %sSwig.py will update this file.' % module)
lines.append('all: %s' % stubpath)
lines.append('%s: %s.i' % (stubpath, module))
swigargs = '-I%s/Lib' % pythondir
swigargs += ' -dll -c++ %s.i' % module
rm = 'rm' if unix else 'del'
quiet = '>/dev/null 2>&1' if unix else '>NUL 2>&1'
lines.append('\t%s %s' % (U.pathconv(swig), swigargs))
if unix:
	lines.append('\tmv -f %s %s %s' % (stubfile, stubpath, quiet))
else:
	lines.append('\tcopy %s %s %s' % (stubfile, os.path.split(stubpath)[0], quiet))
	lines.append('\tdel %s %s' % (stubfile, quiet))
lines.append('')
lines.append('clean:\t')
lines.append('\t-%s %s %s' % (rm, stubpath, quiet))
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
cmd = '%s -f %s' % (make, U.pathconv(makefile))
if clean:
	cmd += ' clean'
U.exec(cmd, shell=True, dry_run=debug)

sys.exit(0)

# end: ScilabSwig.py

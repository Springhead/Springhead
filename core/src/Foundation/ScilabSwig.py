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
#  ＊注意＊
#	ScilabStub.hpp は, dll.cpp を移植しないと作成できない.
#	Ver 1.0 では, makefile (ScilabStub.mak.txt) は作成するが, make は
#	実行しない (make clean も実行しない).
#
# ==============================================================================
#  Version:
#	Ver 1.0	 2017/05/10 F.Kanehori	Windows batch file から移植.
#	Ver 1.1	 2017/07/27 F.Kanehori	Python executable directory moved.
#	Ver 1.2	 2017/09/06 F.Kanehori	New python library に対応.
#	Ver 1.3  2017/11/08 F.Kanehori	Python library path の変更.
#	Ver 1.4  2017/11/29 F.Kanehori	Python library path の変更.
#	Ver 1.41 2018/03/07 F.Kanehori	Add trace code.
# ==============================================================================
version = 1.41
debug = False
trace = False

import sys
import os
import glob
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
module = 'Scilab'
if trace:
	print('ENTER: %s: %s' % (prog, sys.argv))
	sys.stdout.flush()

# ----------------------------------------------------------------------
#  Import Springhead2 python library.
#
sys.path.append('../RunSwig')
from FindSprPath import *
spr_path = FindSprPath(prog)
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from TextFio import *
from Util import *
from Proc import *

# ----------------------------------------------------------------------
#  Globals
#
util = Util()
proc = Proc(dry_run=debug)
unix = util.is_unix()

# ----------------------------------------------------------------------
#  Directories
#
sprtop = spr_path.abspath()
bindir = spr_path.abspath('bin')
incdir = spr_path.abspath('inc')
srcdir = spr_path.abspath('src')
swigdir = '%s/%s' % (bindir, 'swig')

incdir_rel = util.pathconv(os.path.relpath(incdir), 'unix')
srcdir_rel = util.pathconv(os.path.relpath(srcdir), 'unix')

# ----------------------------------------------------------------------
#  Scripts
#
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

clean	= options.clean
verbose	= options.verbose

# ----------------------------------------------------------------------
#  makefile を生成する.
#

lines = []
lines.append('#\tDo not edit. %sSwig.py will update this file.' % module)
lines.append('#')
lines.append('all: %s' % stubpath)
lines.append('%s: %s.i' % (stubpath, module))
lines.append('\t@echo "    *** creating %s"' % stubfile)
swigargs = '-dll -c++ %s.i' % module
if unix:
	rm = 'rm'
	quiet = '>/dev/null 2>&1'
	lines.append('\t%s %s' % (util.pathconv(swig), swigargs))
	lines.append('\tmv -f %s %s %s' % (stubfile, stubpath, quiet))
else:
	rm = 'del'
	quiet = '>NUL 2>&1'
	lines.append('\t%s %s' % (util.pathconv(swig), swigargs))
	lines.append('\tcopy %s %s %s' % (stubfile, os.path.split(stubpath)[0], quiet))
	lines.append('\tdel %s %s' % (stubfile, quiet))
lines.append('')
lines.append('clean:\t')
lines.append('\t-%s -f %s %s' % (rm, stubpath, quiet))
lines.append('\t-%s -f %s %s' % (rm, makefile, quiet))
#
def output(fname):
	fobj = TextFio(fname, 'w', encoding='utf8')
	if fobj.open() < 0:
		E.print(fobj.error(), exitcode=0)
	if fobj.writelines(lines, '\n') < 0:
		E.print(fobj.error(), exitcode=0)
	fobj.close()
#
if verbose:
	path = '%s/%s' % (os.getcwd(), makefile)
	print('  creating "%s"' % util.pathconv(path, 'unix'))
lines = util.pathconv(lines)
output(makefile)

# ----------------------------------------------------------------------
#  make を実行する.
#

# ***************************************************
#  ＊注意＊
#	unix では make 実行は行なわない (Ver 1.0).
if unix: sys.exit(0)
# ***************************************************

cmd = '%s -f %s' % (make, util.pathconv(makefile))
if clean:
	cmd += ' clean'
if trace:
	print('exec: %s' % cmd)
proc.exec(cmd)
proc.wait()

if trace:
	print('LEAVE: %s' % prog)
	sys.stdout.flush()
sys.exit(0)

# end: ScilabSwig.py

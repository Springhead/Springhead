#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  SYNOPSIS:
#	EmbPythonSwig module
#
#  DESCRIPTION:
#	swigを実行して、EmbPythonに関連するファイル(.cpp, .h)を生成する.
#
# ==============================================================================
#  Version:
#	Ver 1.0	 2020/02/20 F.Kanehori	Windows batch file から移植.
# ==============================================================================
version = 1.0
debug = False
trace = False

import sys
import os
import glob
import copy
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
if trace:
	print('ENTER: %s: %s' % (prog, sys.argv[1:]))
	sys.stdout.flush()

# ----------------------------------------------------------------------
#  Import Springhead python library.
#
sys.path.append('../RunSwig')
from FindSprPath import *
spr_path = FindSprPath(prog)
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from TextFio import *
from FileOp import *
from Proc import *
from Error import *

# ----------------------------------------------------------------------
#  Globals
#

#  ディレクトリの定義
coredir	= '../..'
topdir	= '%s/..' % coredir
incdir	= '%s/include' % coredir
swigdir = os.path.abspath('%s/bin/swig' % coredir)

#  使用するプログラム名
#
def installed(path, test='--help'):
	path = Util.wpath(path) if Util.is_windows() else path
	cmnd = '%s %s' % (path, test)
	rc = Proc().execute(cmnd,
		stdout=Proc.NULL, stderr=Proc.NULL, shell=True).wait()
	return rc == 0

swig   = 'swig'
astyle = 'astyle'
if not installed(astyle):
	bindir = '%s/buildtool' % topdir
	astyle = '%s/astyle' % bindir
	if not os.path.exists(bindir) or not installed(astyle):
		msg = '"astyle" not found'
		Error(prog).warn(msg)
		astyle = None

# ----------------------------------------------------------------------
#  オプションの定義
#
usage = 'Usage: %prog [options]'
parser = OptionParser(usage = usage)
parser.add_option('-D', '--dry_run',
                        dest='dry_run', action='store_true', default=False,
                        help='do not execute make')
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
	print('%s: Version %s' % (prog, str(version)))
	sys.exit(0)
if len(args) != 1:
	Error(prog).error('incorrect number of arguments\n')
	Proc().execute('python %s.py -h' % prog, shell=True).wait()
	sys.exit(-1)

module  = args[0]
dry_run = options.dry_run
verbose = options.verbose

# ----------------------------------------------------------------------
#  生成するファイル
#
cpp  = 'EP%s.cpp' % module
hpp  = 'EP%s.h' % module
sprh = 'SprEP%s.h' % module

# ----------------------------------------------------------------------
#  swigの実行
#
print('** Swig Part **', flush=True)
cmnd = '%s/%s' % (Util.pathconv(swigdir), swig)
args = '-I%s/Lib -cpperraswarn -sprpy -DSWIG_PY_SPR -c++ -DPYTHON_H_PATH="Python/Python.h" %s.i'\
	% (swigdir, module)
rc = Proc(dry_run=dry_run, verbose=verbose)\
	.execute('%s %s' % (cmnd, args), shell=True)\
	.wait()
sys.stdout.flush()
if rc != 0:
	msg = 'swig failed'
	Error(prog).error(msg)

# ----------------------------------------------------------------------
#  astyleの実行
#
if astyle is not None:
	print('** AStyle Part **', flush=True)
	cmnd = Util.wpath(astyle) if Util.is_windows() else astyle
	args = '--style=allman --indent=tab "%s" "%s" "%s"' % (cpp, hpp, sprh)
	rc = Proc(dry_run=dry_run, verbose=verbose)\
		.execute('%s %s' % (cmnd, args), shell=True)\
		.wait()
	sys.stdout.flush()
	if rc != 0:
		msg = 'astyle failed'
		Error(prog).error(msg)
	#
	fop = FileOp()
	fop.rm('SprEP%s.*.orig' % module)
	fop.rm('EP%s.*.orig' % module)

# ----------------------------------------------------------------------
#  ファイルを生成する
#
print('** Arange Part **', flush=True)
fop = FileOp()

api_header = '%s/EmbPython/SprEP%s.h' % (incdir, module)
header = 'EP%s.h' % module
if os.path.exists(api_header):
	fop.rm(api_header)
fop.mv(sprh, api_header)

with open(cpp, 'r') as f:
	lines_cpp = f.readlines()
lines = []
lines.append('#include "%s"\n' % api_header)
lines.append('#include "%s"\n' % header)
lines.append('#pragma warning(disable:4244)\n')
lines.extend(lines_cpp)

tmpf = '%s.tmp' % module
fio = TextFio(tmpf, 'w', nl='')
if fio.open() < 0:
	msg = 'can\'t open file "%s" (write)' % tmpf
	Error(prog).abort(msg)
rc = fio.writelines(lines)
fio.close()
if rc != 0:
	msg = '%s: write failed' % tmpf
	Error(prog).abort(msg)

if os.path.exists(cpp):
	fop.rm(cpp)
fop.mv(tmpf, cpp)

# ----------------------------------------------------------------------
#  終了
#
sys.exit(0)

# end: EmbPythonSwig.py

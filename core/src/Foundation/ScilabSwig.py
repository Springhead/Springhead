#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  SYNOPSIS:
#	python ScilabSwig.py
#
#  DESCRIPTION:
#	Scilab モジュールについて, swig を実行するための makefile を作成し,
#	make を実行する (Scilab.i -> ../../include/Scilab/ScilabStub.hpp).
#
#  ＊注意＊
#	ScilabStub.hpp は, dll.cpp を移植しないと作成できない.
#	Ver 1.0 及び 2.0 では, makefile (ScilabStub.mak.txt) は作成するが, 
#	make は実行しない (make clean も実行しない).
#	Scilab.i 及び ScilabStub.hpp は作成済みのものがコミットされている.
#
# ==============================================================================
#  Version:
#     Ver 1.0    2017/05/10 F.Kanehori	Windows batch file から移植.
#     Ver 1.1    2017/07/27 F.Kanehori	Python executable directory moved.
#     Ver 1.2    2017/09/06 F.Kanehori	New python library に対応.
#     Ver 1.3    2017/11/08 F.Kanehori	Python library path の変更.
#     Ver 1.4    2017/11/29 F.Kanehori	Python library path の変更.
#     Ver 1.5    2019/04/01 F.Kanehori	Python library path 検索方法変更.
#     Ver 1.6    2020/04/31 F.Kanehori	unix: gmake をデフォルトに.
#     Ver 1.7    2020/05/13 F.Kanehori	unix: Ver 1.05 に戻す.
#     Ver 1.8    2020/11/11 F.Kanehori	nmake のロゴを抑止する.
#     Ver 1.9    2020/12/16 F.Kanehori	Setup 導入テスト開始.
#     Ver 1.10   2021/02/17 F.Kanehori	Python 2.7 対応.
#     Ver 2.0    2021/03/25 F.Kanehori	全面見直し.
# ==============================================================================
from __future__ import print_function
version = '2.0'
debug = False

import sys
import os
import glob
from optparse import OptionParser

# ----------------------------------------------------------------------
#  このスクリプトは ".../core/src/Foundation" に置く	
#
ScriptFileDir = os.sep.join(os.path.abspath(__file__).split(os.sep)[:-1])
prog = sys.argv[0].replace('/', os.sep).split(os.sep)[-1].split('.')[0]
sys.path.append(os.path.abspath('%s/../RunSwig' % ScriptFileDir))
from Trace import *
trace = Trace().flag(prog)
if trace:
	print('ENTER: %s: %s' % (prog, sys.argv[1:]))
	sys.stdout.flush()

SrcDir = '/'.join(ScriptFileDir.split(os.sep)[:-1])
SetupExists = os.path.exists('%s/setup.conf' % SrcDir)

# ----------------------------------------------------------------------
#  モジュール名
#
module = 'Scilab'

# ----------------------------------------------------------------------
#  Springhead python library の導入
#
libdir = '%s/RunSwig/pythonlib' % SrcDir
sys.path.append(libdir)
from TextFio import *
from Util import *
from Proc import *
from SetupFile import *

# ----------------------------------------------------------------------
#  ディレクトリパスには絶対パスを使用する (cmake 使用時の混乱を避けるため)
#
if SetupExists:
	sf = SetupFile('%s/setup.conf' % SrcDir, verbose=1)
	sf.setenv()

sprtop = os.path.abspath('%s/../..' % SrcDir)
bindir = '%s/core/bin' % sprtop
srcdir = '%s/core/src' % sprtop
incdir = '%s/core/include' % sprtop
swigdir = '%s/%s' % (bindir, 'swig')

incdir_rel = Util.pathconv(os.path.relpath(incdir), 'unix')
srcdir_rel = Util.pathconv(os.path.relpath(srcdir), 'unix')

# ----------------------------------------------------------------------
#  外部スクリプト
#
if SetupExists:
	if Util.is_unix():
		make = sf.getenv('gmake')
	else:
		make = '%s /NOLOGO' % sf.getenv('nmake')
	swig = sf.getenv('swig')
else:
	swig = 'swig'
	make = 'gmake' if Util.is_unix() else 'nmake'
swig = '%s -I%s/Lib' % (swig, swigdir)

# ----------------------------------------------------------------------
#  Files and paths
#
makefile = '%sStub.mak.txt' % module
stubfile = '%sStub.hpp' % module
stubpath = '%s/%s/%s' % (incdir, module, stubfile)
addpath = os.pathsep.join([bindir, swigdir])

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
#  処理開始
# ---------------------------------------------------------------------

# ----------------------------------------------------------------------
#  makefile (ScilabStub.mak.txt) を生成する.
#
lines = []
lines.append('#\tDo not edit. %sSwig.py will update this file.' % module)
lines.append('#')
lines.append('all: %s' % stubpath)
lines.append('%s: %s.i' % (stubpath, module))
lines.append('\t@echo "    *** creating %s"' % stubfile)
swigargs = '-dll -c++ %s.i' % module
if Util.is_unix():
	rm = 'rm'
	quiet = '>/dev/null 2>&1'
	lines.append('\t%s %s' % (Util.pathconv(swig), swigargs))
	lines.append('\tmv -f %s %s %s' % (stubfile, stubpath, quiet))
else:
	rm = 'del'
	quiet = '>NUL 2>&1'
	lines.append('\t%s %s' % (Util.pathconv(swig), swigargs))
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
lines = Util.pathconv(lines)
output(makefile)
print('  generating "%s/%s"' % (Util.upath(os.getcwd()), makefile))

# ----------------------------------------------------------------------
#  make を実行する.
#

# ***************************************************
#  ＊注意＊
#	unix では make 実行は行なわない (Ver 1.0).
if Util.is_unix():
	if trace:
		print('LEAVE: %s' % prog)
		sys.stdout.flush()
	sys.exit(0)
# ***************************************************

cmd = '%s -f %s' % (make, Util.pathconv(makefile))
if clean:
	cmd += ' clean'
if trace:
	print('exec: %s' % cmd)
proc = Proc().execute(cmd)
proc.wait()

if trace:
	print('LEAVE: %s' % prog)
	sys.stdout.flush()
sys.exit(0)

# end: ScilabSwig.py

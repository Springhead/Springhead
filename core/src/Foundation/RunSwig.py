#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  FILE:
#	RunSwig.py
#
#  SYNOPSIS:
#	python RunSwig.py module target [target]...
#
#  DESCRIPTION:
#	指定されたモジュールについて, swig を実行するための makefile を作成し,
#	make を実行する.
#
# ==============================================================================
#  Version:
#     Ver 1.00   2017/04/24 F.Kanehori	Windows batch file から移植.
#     Ver 1.011  2017/06/29 F.Kanehori	Add messages.
#     Ver 1.01   2017/07/27 F.Kanehori	Python executable directory moved.
#     Ver 1.02   2017/09/06 F.Kanehori	New python library に対応.
#     Ver 1.03   2017/10/11 F.Kanehori	起動するpythonを引数化.
#     Ver 1.04   2017/11/08 F.Kanehori	Python library path の変更.
#     Ver 1.05   2017/11/29 F.Kanehori	Python library path の変更.
#     Ver 1.06   2018/07/03 F.Kanehori	空白を含むユーザ名に対応.
#     Ver 1.07   2019/02/26 F.Kanehori	Cmake環境に対応.
#     Ver 1.08   2019/04/01 F.Kanehori	Python library path 検索方法変更.
#     Ver 1.09   2020/04/30 F.Kanehori	unix: gmake をデフォルトに.
#     Ver 1.10   2020/05/13 F.Kanehori	unix: Ver 1.08 に戻す.
#     Ver 1.11   2020/12/09 F.Kanehori	Setup 導入期間開始.
#     Ver 1.12   2021/02/17 F.Kanehori	Python 2.7 対応.
# ==============================================================================
from __future__ import print_function
version = 1.12
trace = False

import sys
import os
import glob
import copy
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Locate myself
#
ScriptFileDir = os.sep.join(os.path.abspath(__file__).split(os.sep)[:-1])

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++
SrcDir = '/'.join(ScriptFileDir.split(os.sep)[:-1])
SetupExists = os.path.exists('%s/setup.conf' % SrcDir)
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
if trace:
	print('ENTER: %s' % prog)
	sys.stdout.flush()

# ----------------------------------------------------------------------
#  Import Springhead python library.
#
sys.path.append('../RunSwig')
from FindSprPath import *
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++
if SetupExists:
	libdir = '%s/RunSwig/pythonlib' % SrcDir
else:
	spr_path = FindSprPath(prog)
	libdir = spr_path.abspath('pythonlib')
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++
sys.path.append(libdir)
from TextFio import *
from Error import *
from Util import *
from Proc import *
from SetupFile import *

# ----------------------------------------------------------------------
#  Globals (part 1)
#
util = Util()
unix = util.is_unix()

# ----------------------------------------------------------------------
#  Directories
#
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++
if SetupExists:
	sf = SetupFile('%s/setup.conf' % SrcDir, verbose=1)
	sf.setenv()
	sprtop = os.path.abspath('%s/../..' % SrcDir)
	bindir = os.path.relpath('%s/../bin' % SrcDir)
	incdir = os.path.relpath('%s/../include' % SrcDir)
	srcdir = os.path.relpath(SrcDir)
else:
	sprtop = spr_path.abspath()
	bindir = spr_path.relpath('bin')
	incdir = spr_path.relpath('inc')
	srcdir = spr_path.relpath('src')
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++
swigdir = '%s/%s' % (bindir, 'swig')

incdir_rel = util.pathconv(os.path.relpath(incdir), 'unix')
srcdir_rel = util.pathconv(os.path.relpath(srcdir), 'unix')

# ----------------------------------------------------------------------
#  Be sure to invoke swig which is build for Springhead.
#
addpath = os.pathsep.join([swigdir, bindir])

# ----------------------------------------------------------------------
#  Main process
# ----------------------------------------------------------------------
#  オプションの定義
#
usage = 'Usage: %prog [options] module [projct [project]..]'
parser = OptionParser(usage = usage)
parser.add_option('-d', '--dry_run',
			dest='dry_run', action='store_true', default=False,
			help='dry_run (for debug)')
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++
parser.add_option('-P', '--python',
                        dest='python', action='store', default='python',
                        help='python command name')
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
if len(args) < 1:
	parser.error("incorrect number of arguments")

module = args[0]
target_list = args[::-1]

verbose	= options.verbose
dry_run	= options.dry_run

# ----------------------------------------------------------------------
#  Scripts
#
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++
if SetupExists:
	python = sf.getenv('python')
	if unix:
		make = sf.getenv('gmake')
	else:
		make = '%s /NOLOGO' % sf.getenv('nmake')
	swig = sf.getenv('swig')
else:
	if options.python:
		python = options.python
	make = 'gmake' if unix else 'nmake /NOLOGO'
	swig = 'swig'
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++

# ----------------------------------------------------------------------
#  Globals (part 2)
#
proc = Proc(dry_run=dry_run)

# ----------------------------------------------------------------------
#  Files
#
interfacefile = '%s.i' % module
makefile = '%sStub.mak.txt' % module
stubfile = '%sStub.cpp' % module

# ----------------------------------------------------------------------
#  ヘッダファイル情報を収集する.
#
incf_names = ['Springhead.h', 'Base/Env.h', 'Base/BaseDebug.h']
srcf_names = []		# ['Foundation/UTTypeDesc.h']
auxdep_inc = list(map(lambda x: '%s/%s' % (incdir_rel, x), incf_names))
auxdep_src = list(map(lambda x: '%s/%s' % (srcdir_rel, x), srcf_names))
auxdep = copy.deepcopy(auxdep_inc)
auxdep.extend(auxdep_src)

srcinf = []
srcimp = []
srcinfdep = []
srcimpdep = []
for target in target_list:
	srcinf.extend(glob.glob('%s/%s/*.h' % (incdir_rel, target)))
	srcimp.extend(glob.glob('%s/%s/*.h' % (srcdir_rel, target)))
	srcinfdep.extend(glob.glob('%s/%s/*.h' % (incdir_rel, target)))
	srcimpdep.extend(glob.glob('%s/%s/*.h' % (srcdir_rel, target)))
srcinf = util.pathconv(srcinf, 'unix')
srcimp = util.pathconv(srcimp, 'unix')
srcinfdep = util.pathconv(srcinfdep, 'unix')
srcimpdep = util.pathconv(srcimpdep, 'unix')
if verbose:
	print('srcinf: %s' % srcinf)
	print('srcimp: %s' % srcimp)
	print('srcinfdep: %s' % srcinfdep)
	print('srcimpdep: %s' % srcimpdep)

# ----------------------------------------------------------------------
#  インターフェイスファイルを生成する.
#
lines = []
lines.append('#\tDo not edit. RunSwig.py will update this file.')
lines.append('%%module %s' % module)
lines.append('#define DOUBLECOLON ::')
for fname in auxdep_inc:
	lines.append('%%include "%s"' % fname)
for fname in srcinf:
	lines.append('%%include "%s"' % fname)
for fname in auxdep_src:
	lines.append('%%include "%s"' % fname)
for fname in srcimp:
	lines.append('%%include "%s"' % fname)
#
def output(fname, lines):
	if verbose:
		print('%s:' % fname)
		for line in lines:
			print('  %s' % line)
	fobj = TextFio(fname, 'w', encoding='utf8')
	if fobj.open() < 0:
		Error(prog).put(fobj.error(), exitcode=0, alive=True)
	if fobj.writelines(lines, '\n') < 0:
		Error(prog).put(fobj.error(), exitcode=0, alive=True)
	fobj.close()
#
path = '%s/%s' % (os.getcwd(), interfacefile)
print('    *** %s: creating "%s"' % (prog, util.pathconv(path, 'unix')))
output(interfacefile, lines)

# ----------------------------------------------------------------------
#  makefile を生成する.
#
lines = []
lines.append('#\tDo not edit. RunSwig.py will update this file.')
lines.append('all:\t%s' % stubfile)
line = '%s:\t' % stubfile
line += ' '.join(auxdep)
line += ' ' + ' '.join(srcinfdep)
line += ' ' + ' '.join(srcimpdep)
lines.append(line)
####line = '\t%s/%s -I%s/Lib' % (swigdir, swig, swigdir)
line = '\t%s -I%s/Lib' % (swig, swigdir)
line += ' -spr -w305,312,319,325,401,402'
line += ' -DSWIG_%s -c++ %s' % (module, interfacefile)
lines.append(line)
#
path = '%s/%s' % (os.getcwd(), makefile)
print('    *** %s: creating "%s"' % (prog, util.pathconv(path, 'unix')))
lines = util.pathconv(lines)
output(makefile, lines)

# ----------------------------------------------------------------------
#  make を実行する.
#
cmd = '%s -f %s' % (make, util.pathconv(makefile))
proc.execute(cmd, addpath=addpath, shell=True)
status = proc.wait()
if status != 0:
	msg = '%s failed (%d)' % (make, status)
	Error(prog).put(msg, exitcode=0, alive=True)

if trace:
	print('LEAVE: %s' % prog)
	sys.stdout.flush()
sys.exit(0)

# end: RunSwig.py

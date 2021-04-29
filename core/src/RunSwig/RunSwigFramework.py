#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  SYNOPSIS:
#	python RunSwigFramework.py
#
#  DESCRIPTION:
#	Makefile.{win|unix} の FWOLDSPRSTUB ターゲットから呼び出される.
#	以下のファイルを生成する.
#
#	生成するファイル：
#	    include/SprFWOldSpringheadDecl.hpp
#	    src/FWOldSpringheadDecl.hpp
#	    src/FWOldSpringheadStub.cpp
#
#	作業ディレクトリ：
#	    src/RunSwig の下に tmp というディレクトリを作り使用する.
#
# ==============================================================================
#  Version:
#     Ver 1.0    2017/05/10 F.Kanehori	Windows batch file から移植.
#     Ver 2.0    2021/04/14 F.Kanehori	全面見直し.
#     Ver 2.0.1	 2021/04/14 F.Kanehori	bug fix.
# ==============================================================================
from __future__ import print_function
version = '2.0.1'

import sys
import os
import glob
from optparse import OptionParser

# ----------------------------------------------------------------------
#  このスクリプトは ".../core/src/RunSwig" に置く
#
ScriptFileDir = os.sep.join(os.path.abspath(__file__).split(os.sep)[:-1])
prog = sys.argv[0].replace('/', os.sep).split(os.sep)[-1].split('.')[0]
from Trace import *
trace =  Trace().flag(prog)
if trace:
	print('ENTER: %s: %s' % (prog, sys.argv[1:]))
	sys.stdout.flush()

SrcDir = '/'.join(ScriptFileDir.split(os.sep)[:-1])
SetupExists = os.path.exists('%s/setup.conf' % SrcDir)

# ----------------------------------------------------------------------
#  Springhead python library の導入
#
libdir = '%s/RunSwig/pythonlib' % SrcDir
sys.path.append(libdir)
from SetupFile import *
from TextFio import *
from FileOp import *
from Error import *
from Util import *
from Proc import *

# ------------------------------------------------------------------
#  セットアップファイルが存在するならば準備をしておく
#
if SetupExists:
	sf = SetupFile('%s/setup.conf' % SrcDir, verbose=1)
	sf.setenv()

# ----------------------------------------------------------------------
#  ディレクトリパスには絶対パスを使用する (cmake 使用時の混乱を避けるため)
#
sprtop = os.path.abspath('%s/../..' % SrcDir)
bindir = '%s/core/bin' % sprtop
incdir = '%s/core/include' % sprtop
srcdir = '%s/core/src' % sprtop
framework_dir = '%s/Framework' % srcdir
tmpdir = '%s/core/swigtmp' % sprtop
tmpdir = '%s/core/src/RunSwig/tmp' % sprtop

# ----------------------------------------------------------------------
#  モジュール名定義
#
module = 'FWOldSpringhead'

# ----------------------------------------------------------------------
#  生成するファイル名
#
interfacefile = '%s.i' % module
makefile = '%sStub.makefile' % module
stubcpp = '%sStub.cpp' % module

# ----------------------------------------------------------------------
#  内部メソッド
#
def output(fname, lines):
	fobj = TextFio(fname, 'w', encoding='utf8')
	if fobj.open() < 0:
		Error(prog).error(fobj.error())
	if fobj.writelines(lines, '\n') < 0:
		Error(prog).error(fobj.error())
	fobj.close()

# ----------------------------------------------------------------------
#  オプションの定義
#
usage = 'Usage: %prog [options]'
parser = OptionParser(usage = usage)
parser.add_option('-c', '--clean',
			dest='clean', action='store_true', default=False,
			help='execute target clean')
parser.add_option('-d', '--dry_run',
			dest='dry_run', action='store_true', default=False,
			help='dry_run (for debug)')
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
dry_run	= options.dry_run

# ----------------------------------------------------------------------
#  外部スクリプト
#
if SetupExists:
	python = sf.getenv('python')
	if Util.is_unix():
		make = sf.getenv('gmake')
	else:
		make = '%s /NOLOGO' % sf.getenv('nmake')
	swig = sf.getenv('swig')
	addpath = os.path.abspath('%s/../..' % SrcDir)
else:
	swig = 'swig'
	make = 'gmake' if Util.is_unix() else 'nmake /NOLOGO'

# ----------------------------------------------------------------------
#  RunSwig 下に作業領域を作成する
#
tmp_inc = '%s/include' % tmpdir
tmp_src = '%s/src' % tmpdir

#  ディレクトリを作成
incdir_names = ['Base', 'Framework']
srcdir_names = ['Foundation', 'Framework']
tmp_incdirs = list(map(lambda x: '%s/%s' % (tmp_inc, x), incdir_names))
tmp_srcdirs = list(map(lambda x: '%s/%s' % (tmp_src, x), srcdir_names))
for dir in tmp_incdirs:
	#print('[%s]' % dir)
	if not os.path.exists(dir):
		print('  creating %s' % dir, flush=True)
		os.makedirs(dir)
for dir in tmp_srcdirs:
	#print('[%s]' % dir)
	if not os.path.exists(dir):
		print('  creating %s' % dir, flush=True)
		os.makedirs(dir)

#  必要なファイルを作業領域にコピー
f_op = FileOp(verbose=verbose)
incf_names = ['Springhead.h', 'Base/Env.h', 'Base/BaseDebug.h']
srcf_names = ['Foundation/UTTypeDesc.h', 'Framework/FWOldSpringheadNodeHandler.h']
for file in incf_names:
	f_op.cp('%s/%s' % (incdir, file), '%s/include/%s' % (tmpdir, file))
for file in srcf_names:
	f_op.cp('%s/%s' % (srcdir, file), '%s/src/%s' % (tmpdir, file))

# ----------------------------------------------------------------------
#  ここからは作業領域に移って作業する.
#
cwd = os.getcwd()
os.chdir('%s/Foundation' % tmp_src)
if verbose:
	print('  chdir to %s' % Util.upath(os.getcwd()))

# ----------------------------------------------------------------------
#  swig のインターフェイスファイルを作成する.
#
srcimp = '%s/src/Framework/FWOldSpringheadNodeHandler.h' % tmpdir
srcimpdep = '%s/Framework/FWOldSpringheadNodeHandler.h' % srcdir
srcdir_rel = os.path.relpath(srcdir, framework_dir)
incdir_rel = os.path.relpath('%s/../include' % srcdir, framework_dir)
srcdir_rel = Util().pathconv(srcdir_rel)
incdir_rel = Util().pathconv(incdir_rel)

lines = []
lines.append('# Do not edit. %s will update this file.' % prog)
lines.append('%%module %s' % module)
for file in incf_names:
	lines.append('%%include "%s/%s"' % (incdir_rel, file))
for file in srcf_names:
	lines.append('%%include "%s/%s"' % (srcdir_rel, file))
if verbose:
	path = '%s/%s' % (os.getcwd(), interfacefile)
	print('  creating "%s"' % Util.upath(path))
output(interfacefile, lines)

# ----------------------------------------------------------------------
#  makefile を作成する.
#
srcimpdep_rel = os.path.relpath(srcimpdep)
swigdir = os.path.relpath('%s/core/bin/swig' % sprtop)
if SetupExists:
	swigbin = Util.pathconv(swig)
else:
	swigbin = Util.pathconv('%s/%s' % (swigdir, swig))
swigargs = '-I%s/Lib -spr -w312,325,401,402 -DSWIG_OLDNODEHANDLER -c++' % swigdir
cp = 'cp' if Util.is_unix() else 'copy'
rm = 'rm' if Util.is_unix() else 'del'
quiet = '>/dev/null 2>&1' if Util.is_unix() else '>NUL 2>&1'
fw_reldir = os.path.relpath(framework_dir)
inc_reldir = os.path.relpath(incdir)
#
lines = []
lines.append('# Do not edit. %s will update this file.' % prog)
lines.append('all:\t%s/%sStub.cpp' % (fw_reldir, module))
lines.append('%s/%sStub.cpp:\t%s' % (fw_reldir, module, srcimpdep_rel))
'''
if SetupExists:
	lines.append('\t%s %s %s' % (swig_bin, swigargs, interfacefile))
else:
	lines.append('\t%s/%s %s %s' % (swigdir, swig, swigargs, interfacefile))
'''
lines.append('\t%s %s %s' % (swigbin, swigargs, interfacefile))
lines.append('\t%s Spr%sDecl.hpp %s/%s %s' % (cp, module, inc_reldir, module, quiet))
lines.append('\t%s %sStub.cpp %s %s' % (cp, module, fw_reldir, quiet))
lines.append('\t%s %sDecl.hpp %s %s' % (cp, module, fw_reldir, quiet))
lines.append('')
lines.append('clean:\t')
lines.append('\t-%s -f %s/%s %s' % (rm, inc_reldir, module, quiet))
lines.append('\t-%s -f %s/%sStub.cpp %s' % (rm, fw_reldir, module, quiet))
lines.append('\t-%s -f %s/%sDecl.hpp %s' % (rm, fw_reldir, module, quiet))
if verbose:
	path = '%s/%s' % (os.getcwd(), makefile)
	print('  creating "%s"' % Util.upath(path))
lines = Util.pathconv(lines)
output(makefile, lines)

# ----------------------------------------------------------------------
#  make (swig) を実行する.
#
cmd = '%s -f %s' % (make, makefile)
if clean:
	cmd += ' clean'
if trace:
	print('exec: %s' % cmd)
proc = Proc(verbose=verbose, dry_run=dry_run)
proc.execute(cmd, shell=True)
status = proc.wait()
if status != 0:
	Error(prog).error('%s failed (%d)' % (make, status))

# ----------------------------------------------------------------------
#  処理終了.
#
os.chdir(cwd)
if trace:
	print('LEAVE: %s' % prog)
	sys.stdout.flush()
sys.exit(0)

# end: RunSwigFramework.py

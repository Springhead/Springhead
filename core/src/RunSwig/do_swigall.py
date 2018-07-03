#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  FILE:
#	do_swigall.py
#
#  SYNOPSIS:
#	python do_swigall.py
#
#  DESCRIPTION:
#	処理対象となる各プロジェクトのディレクトリを巡り, 次の処理を行なう.
#	  (1) make -f Makefile.swig の実行
#	  (2) python make_manager.py -r の実行
#
#	(1) 条件が満たされた場合には,
#	　　(a) python make_manager.py -t を実行して, 必要なディレクトリを
#		再スキャンした makefile (=Makefile.swig.tmp) をテンポラリに
#		作成する.
#	　　(b)	python RunSwig.py を実行する.
#
#	(2) (1)-(a) で作成したテンポラリの Makefile.swig.tmp を, 正式の makefile
#	    である Makefile.swig とする.
#	　　※	python RunSwig.py の実行より先に makefile を再作成する必然性は
#		ないが, 実行順序の違和感をなくすためにこうする.
#
#	Windows の場合：
#		RunSwig プロジェクトの構成プロパティ([NMake → 全般] のコマンド
#		ライン) に次の設定をしておくこと.
#		    ビルド	    make_manager.bat -A -c && nmake -f Makefile
#		    すべてリビルド  make_manager.bat -A -d -c && nmake -f Makefile
#		    クリーン	    make_manager.bat -A -d
#
#	依存関係を定義したファイル do_swigall.projs の書式：
#	    <line> ::= <project> ' ' <dependency-list>
#	    <project> ::= 処理の対象とするプロジェクト名 (=モジュール名)
#	    <dependency-list> ::= ε| <dependent-project> ',' <dependency-list>
#	    <dependent-project> ::= 依存関係にあるプロジェクト名 (=モジュール名)
#	    ※ 注意
#		<project>と<dependency-list>の間区切り文字は「空白1個」
#		<dependency-list>の要素間の区切り文字は「カンマ1個」
#
# ==============================================================================
#  Version:
#	Ver 1.0	 2012/10/25 F.Kanehori	First release version.
#	Ver 2.0	 2013/01/07 F.Kanehori	全面改訂
#	Ver 3.0	 2017/05/10 F.Kanehori	Windows batch file から移植.
#	Ver 3.01 2017/06/29 F.Kanehori	Revise some messages.
#	Ver 3.1  2017/07/24 F.Kanehori	Python executable directory moved.
#	Ver 3.2  2017/09/06 F.Kanehori	New python library に対応.
#	Ver 3.3  2017/10/11 F.Kanehori	起動するpythonを引数化.
#	Ver 3.4  2017/11/08 F.Kanehori	Python library path の変更.
#	Ver 3.5  2017/11/29 F.Kanehori	Python library path の変更.
#	Ver 3.6  2018/07/03 F.Kanehori	空白を含むユーザ名に対応.
# ==============================================================================
version = 3.6
debug = False
trace = False

import sys
import os
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
if trace:
	print('ENTER: %s: %s' % (prog, sys.argv[1:]))
	sys.stdout.flush()

# ----------------------------------------------------------------------
#  Import Springhead2 python library.
#
from FindSprPath import *
spr_path = FindSprPath(prog)
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from TextFio import *
from Error import *
from Util import *
from Proc import *
from FileOp import *

# ----------------------------------------------------------------------
#  Globals (part 1)
#
util = Util()
unix = util.is_unix()

# ----------------------------------------------------------------------
#  Directories
#
sprtop = spr_path.abspath()
bindir = spr_path.relpath('bin')
srcdir = spr_path.relpath('src')
etcdir = '%s/%s' % (srcdir, 'RunSwig')
runswigdir = '%s/%s' % (srcdir, 'RunSwig')

# ----------------------------------------------------------------------
#  Files
#
projfile = 'do_swigall.projs'
makefile = 'makefile.swig'

# ----------------------------------------------------------------------
#  Paths
#
if unix:
	makepath = '/usr/bin'
else:
	x32 = 'C:/Program Files'
	x64 = 'C:/Program Files (x86)'
	arch = None
	if os.path.exists(x32) and os.path.isdir(x32): arch = x32
	if os.path.exists(x64) and os.path.isdir(x64): arch = x64
	if arch is None:
		Error(prog).error('can not find "%s" path.' % make)
	makepath = '%s/Microsoft Visual Studio 12.0/VC/bin' % arch

swigpath = '%s/%s' % (srcdir, 'Foundation')
addpath = os.pathsep.join([bindir, swigpath, makepath])

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
parser.add_option('-P', '--python',
                        dest='python', action='store', default='python',
                        help='python command name')
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

clean   = options.clean
verbose = options.verbose

# ----------------------------------------------------------------------
#  Scripts
#
if options.python:
	python = options.python
make = 'make' if unix else 'nmake'
opts = '-P %s' % python
makemanager = '%s "%s/make_manager.py" %s' % (python, runswigdir, opts)

# ----------------------------------------------------------------------
#  Globals (part 2)
#
proc = Proc(verbose=verbose, dry_run=debug)
f_op = FileOp(verbose=verbose)

# ----------------------------------------------------------------------
#  プロジェクト依存定義ファイルを読み込む.
#
fio = TextFio('%s/%s' % (etcdir, projfile))
if fio.open() < 0:
	Error(prog).error(fio.error())
lines = fio.read()
fio.close()

# ----------------------------------------------------------------------
#  各プロジェクト毎に処理を行なう.
#
for line in lines:
	fields = line.split()
	if len(fields) < 1:
		continue
	proj = fields[0]
	dept = fields[1] if len(fields) > 1 else None
	if debug:
		print('proj: %s <- %s' % (proj, dept))

	print()
	print('*** %s ***' % proj)

	#  Change to target directory.
	cwd = os.getcwd()
	target_dir = '%s/%s' % (srcdir, proj)
	if debug:
		print('chdir: %s' % target_dir)
	os.chdir(target_dir)

	#  Do make.
	if clean:
		print('    %s: clean' % prog)
		f_op.rm('%s.i' % proj)
		f_op.rm('%sStub.cpp' % proj)
		f_op.rm('%sStub.mak.txt' % proj)
	else:
		cmd = '%s -f %s' % (make, makefile)
		print('    %s: %s' % (prog, cmd))
		proc.execute(cmd, addpath=addpath, shell=True)
		proc.wait()
		cmd = '%s -r' % util.pathconv(makemanager)
		print('    %s: %s' % (prog, Util.upath(cmd)))
		proc.execute(cmd, addpath=addpath, shell=True)
		proc.wait()

	#  Return to original directory.
	os.chdir(cwd)

if trace:
	print('LEAVE: %s' % prog)
	sys.stdout.flush()
sys.exit(0)

# end: do_swigall.py

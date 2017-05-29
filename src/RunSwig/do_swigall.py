#!/usr/local/bin/python3.4
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
# ==============================================================================
version = 3.0

import sys
import os
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
python_version = 3
debug = False

# ----------------------------------------------------------------------
#  Globals
#
E = Error(prog)
U = Util()
unix = U.is_unix()

# ----------------------------------------------------------------------
#  Directories
#
spr2top = U.pathconv(os.path.relpath(spr2), 'unix')
srcdir = '%s/%s' % (spr2top, 'src')
bindir = '%s/%s' % (spr2top, 'bin')
etcdir = '%s/%s' % (srcdir, 'RunSwig')
runswigdir = '%s/%s' % (srcdir, 'RunSwig')
pythondir = '/usr/bin'

# ----------------------------------------------------------------------
#  Scripts
#
pythonexe = 'python%s' % (python_version if unix else '')
python = '%s/%s' % (pythondir, pythonexe)
make = 'make' if unix else 'nmake'
makemanager = '%s %s/make_manager.py' % (python, runswigdir)

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
		E.print('can not find "%s" path.' % make)
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
#  プロジェクト依存定義ファイルを読み込む.
#
fio = TextFio('%s/%s' % (etcdir, projfile))
if fio.open() < 0:
	E.print(fio.error())
lines = fio.read()
fio.close()

# ----------------------------------------------------------------------
#  各プロジェクト毎に処理を行なう.
#
for line in lines:
	fields = line.split()
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
		U.rm(makefile, force=True)
		U.rm('%sStub.cpp' % proj, force=True)
		U.rm('%sStub.mak.txt' % proj, force=True)
	else:
		cmd = '%s -f %s' % (make, makefile)
		U.exec(cmd, addpath=addpath, shell=True, dry_run=debug)
		cmd = '%s -r' % U.pathconv(makemanager)
		U.exec(cmd, addpath=addpath, shell=True, dry_run=debug)

	#  Return to original directory.
	os.chdir(cwd)

sys.exit(0)

# end: do_swigall.py

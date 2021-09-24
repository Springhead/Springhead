#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  SYNOPSIS:
#	python do_swigall.py
#
#  DESCRIPTION:
#	Makefile.{win|unix} の RunSwig ターゲットから呼び出される.
#	処理対象となる各プロジェクトに対して RunSwig.py を実行することで
#	次の処理を行なう.
#	  (1) 処理対象プロジェクトのディレクトリに移動する.
#	  (2) python RunSwig.py を実行する. これにより,
#		(2-1) "<proj>.i" の生成
#		(2-2) "<proj>Stub.makefile" の生成
#		(2-3) make "<proj>Stub.makefile" の実行
#	      が行なわれ, 条件が満たされれば swig が実行される.
#
#	補足	Makefile.win は, Springhead.sln の RunSwig プロジェクトを
#		ビルドすると実行される.
#
# ==============================================================================
#  Version:
#     Ver 1.0	 2012/10/25 F.Kanehori	First release version.
#     Ver 2.0	 2013/01/07 F.Kanehori	全面改訂
#     Ver 3.0	 2017/05/10 F.Kanehori	Windows batch file から移植.
#     Ver 4.0    2021/04/08 F.Kanehori	全面見直し.
#     Ver 4.1    2021/08/30 F.Kanehori	ダミー stub ファイル削除は廃止.
# ==============================================================================
from __future__ import print_function
version = '4.1'
dry_run = False
debug = False

import sys
import os
if sys.version_info[0] >= 3:
	import subprocess
else:
	import subprocess32 as subprocess
import re
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
from Util import *
from Proc import *
from TextFio import *
from FileOp import *

# ----------------------------------------------------------------------
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
srcdir = '%s/core/src' % sprtop
etcdir = '%s/%s' % (srcdir, 'RunSwig')
runswigdir = '%s/%s' % (srcdir, 'RunSwig')
swigbindir = Util.pathconv('%s/swig' % bindir)

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
if debug:
	print('%s options=%s args=%s' % (prog, options, args))

verbose = options.verbose

# ----------------------------------------------------------------------
#  外部スクリプト
#
if SetupExists:
	python = sf.getenv('python')
	if Util.is_unix():
		make = sf.getenv('gmake')
	else:
		make = '%s /NOLOGO' % sf.getenv('nmake')
	makemanager = '%s "%s/make_manager.py"' % (python, runswigdir)
else:
	make = 'make' if Util.is_unix() else 'nmake /NOLOGO'
	makemanager = 'python "%s/make_manager.py"' % runswigdir

# ----------------------------------------------------------------------
#  プロジェクトとその依存関係の定義を取り込む
#
from project_dependencies import *

# ----------------------------------------------------------------------
#  Base 以外の各プロジェクト毎に処理を行なう.
#
from RunSwig_method import *
for proj,dept in sorted(proj_depts.items(), key=lambda x: x[0]):
	print('  ** %s **' % proj)
	sys.stdout.flush()

	#  処理対象プロジェクトのディレクトリへ移動する
	cwd = os.getcwd()
	target_dir = '%s/%s' % (srcdir, proj)
	if debug:
		print('chdir: %s' % target_dir)
	os.chdir(target_dir)

	#  RunSwig.py を実行する
	#	".i", ".makefile" を生成 → make (swig) を実行
	status = RunSwig(proj, dept, verbose=verbose, dry_run=dry_run)

	#  元のディレクトリに戻る
	os.chdir(cwd)

if trace:
	print('LEAVE: %s' % prog)
	sys.stdout.flush()
sys.exit(0)

# end: do_swigall.py

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
#     Ver 1.00	 2012/10/25 F.Kanehori	First release version.
#     Ver 2.00	 2013/01/07 F.Kanehori	全面改訂
#     Ver 3.00	 2017/05/10 F.Kanehori	Windows batch file から移植.
#     Ver 3.01	 2017/07/24 F.Kanehori	Python executable directory moved.
#     Ver 3.02	 2017/09/06 F.Kanehori	New python library に対応.
#     Ver 3.03	 2017/10/11 F.Kanehori	起動するpythonを引数化.
#     Ver 3.04	 2017/11/08 F.Kanehori	Python library path の変更.
#     Ver 3.05	 2017/11/29 F.Kanehori	Python library path の変更.
#     Ver 3.06	 2018/07/03 F.Kanehori	空白を含むユーザ名に対応.
#     Ver 3.07	 2019/02/26 F.Kanehori	Cmake環境に対応.
#     Ver 3.08	 2019/04/01 F.Kanehori	Python library path 検索方法変更.
#     Ver 3.09   2020/04/30 F.Kanehori	unix: gmake をデフォルトに.
#     Ver 3.10   2020/05/13 F.Kanehori	unix: Ver 3.08 に戻す.
# ==============================================================================
version = 3.09
debug = False
trace = False
dry_run = False

import sys
import os
import subprocess
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
bindir = spr_path.abspath('bin')
srcdir = spr_path.abspath('src')
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
	def s16(value):
		return -(value & 0b1000000000000000) | (value & 0b0111111111111111)
	cmnd = 'python find_path.py nmake.exe'
	proc = subprocess.Popen(cmnd, stdout=subprocess.PIPE,
				      stderr=subprocess.DEVNULL)
	out, err = proc.communicate()
	status = s16(proc.returncode)
	if status != 0:
		Error(prog).error('can not find "%s" path.' % make)
	#
	encoding = os.device_encoding(1)
	if encoding is None:
		encoding = 'UTF-8' if unix else 'cp932'
	makepath = out.decode(encoding) if out else None
	if makepath is None:
		Error(prog).error('can not decode "%s" path.' % make)
	print('nmake path found: %s' % makepath.replace(os.sep, '/'))

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
if debug:
	print('%s options=%s args=%s' % (prog, options, args))

clean   = options.clean
verbose = options.verbose

# ----------------------------------------------------------------------
#  Scripts
#
if options.python:
	python = options.python
make = 'make' if unix else 'nmake /NOLOGO'
#make = 'gmake' if unix else 'nmake /NOLOGO'
opts = '-P %s' % python
makemanager = '%s "%s/make_manager.py" %s' % (python, runswigdir, opts)

# ----------------------------------------------------------------------
#  Globals (part 2)
#
proc = Proc(verbose=verbose, dry_run=dry_run)
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

	#  Remove empty stub file if exists.
	stubfile = '%sStub.cpp' % proj
	if os.path.exists(stubfile) and os.path.getsize(stubfile) == 0:
		print('    rm %s' % stubfile)
		os.remove(stubfile)

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

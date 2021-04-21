#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  SYNOPSIS:
#	RunSwig_EmbPython
#
#  DESCRIPTION:
#	ファイルの依存関係を調べて、EmbPythonSwig.py を最適に実行する.
#	実行するプロジェクトは ../RunSwig/project_dependencies.py で定義する.
#	ただしプロジェクト Base は定義の有無に関わりなく実行する.
#
# ==============================================================================
#  Version:
#     Ver 1.0	 2020/02/19 F.Kanehori	Windows batch file から移植.
#     Ver 2.0	 2021/04/08 F.Kanehori	全面見直し.
# ==============================================================================
version = 2.0

import sys
import os
import glob
from optparse import OptionParser

# ----------------------------------------------------------------------
#  このスクリプトは ".../core/src/EmbPython" に置く	
#
ScriptFileDir = os.sep.join(os.path.abspath(__file__).split(os.sep)[:-1])
prog = sys.argv[0].replace('/', os.sep).split(os.sep)[-1].split('.')[0]
SrcDir = '/'.join(ScriptFileDir.split(os.sep)[:-1])
SetupExists = os.path.exists('%s/setup.conf' % SrcDir)

sys.path.append('%s/RunSwig' % SrcDir)
from Trace import *
trace =  Trace().flag(prog)
if trace:
	print('ENTER: %s: %s' % (prog, sys.argv[1:]))
	sys.stdout.flush()

# ----------------------------------------------------------------------
#  Kludge for unix
#	何故か、cmake で add_custom_command の WORKINGDIRECTORY 指定が
#	うまく機能しないようなので…
path = os.getcwd().split(os.sep)
if path[-1] == 'build' or path[-2] != 'src':
	os.chdir('..')

# ----------------------------------------------------------------------
#  Springhead python library の導入
#
libdir = '%s/RunSwig/pythonlib' % SrcDir
sys.path.append(libdir)
from SetupFile import *
from TextFio import *
from Proc import *
from Util import *
from Error import *

# ----------------------------------------------------------------------
#  セットアップファイルが存在するならば準備をしておく
#
if SetupExists:
	sf = SetupFile('%s/setup.conf' % SrcDir, verbose=1)
	sf.setenv()

# ----------------------------------------------------------------------
#  ディレクトリパスには絶対パスを使用する (cmake 使用時の混乱を避けるため)
#
basedir = os.path.abspath('%s/..' % SrcDir)
bindir	= '%s/bin' % basedir
srcdir	= '%s/src' % basedir
incdir	= '%s/include' % basedir
etcdir	= '%s/RunSwig' % srcdir
embpythondir = '.'

# ----------------------------------------------------------------------
#  依存関係にはないと見做すファイルの一覧
#
excludes = []

# ----------------------------------------------------------------------
#  makefile に出力するときのパス表記
#
incdir_out = '../../include'
srcdir_out = '../../src'
embdir_out = '../../src/EmbPython'

# ----------------------------------------------------------------------
#  使用するファイル名
#
projfile = 'do_swigall.projs'
makefile = 'Makefile_EmbPython.swig'

# ----------------------------------------------------------------------
#  外部スクリプト
#
if SetupExists:
	if Util.is_unix():
		make = sf.getenv('gmake')
	else:
		make = '%s /NOLOGO' % sf.getenv('nmake')
else:
	make = 'make' if Util.is_unix() else 'nmake /NOLOGO'
swig = 'EmbPythonSwig.py'

# ----------------------------------------------------------------------
#  内部使用メソッド
#

#  ヘッダファイル情報を収集する
#
def collect_headers(proj):
	#  swig interface file
	interfs = collect_headers_sub('.', '*.i', embdir_out)
	where = 'Utility'
	out = '%s/Utility' % embdir_out
	interfs2 = collect_headers_sub(where, '*.i', out)
	interfs.extend(interfs2)

	#  header files in include directory
	where = '%s/%s' % (incdir, proj)
	out = '%s/%s' % (incdir_out, proj)
	inchdrs = collect_headers_sub(where, '*.h', out)
	inchdrs.append('%s/EmbPython/SprEP%s.h' % (incdir, proj))

	#  header files in source directory
	where = '%s/%s' % (srcdir, proj)
	out = '%s/%s' % (srcdir_out, proj)
	srchdrs = collect_headers_sub(where, '*.h', out)

	return interfs, inchdrs, srchdrs

#
def collect_headers_sub(where, patt, out):
	cwd = os.getcwd()
	if where != '.':
		os.chdir(where)
	files = []
	for f in glob.glob(patt):
		if f not in excludes:
			files.append('%s/%s' % (out, f))
	os.chdir(cwd)
	return files

#  makefileを生成する
#
def make_makefile(module, fname, interfs, inchdrs, srchdrs):
	target = '%s/EP%s.cpp' % (embpythondir, module)
	dependencies = []
	dependencies.extend(interfs)
	dependencies.extend(inchdrs)
	dependencies.extend(srchdrs)
	if verbose:
		print('    target:       [%s]' % target)
		print('    dependencies: [%s]' % dependencies)

	lines = []
	lines.append('#  Do not edit. RunSwig_EmbPython will update this file.')
	lines.append('#  File: %s' % fname)
	lines.append('')
	lines.append('all:\t%s' % target)
	lines.append('%s:\t%s' % (target, ' '.join(dependencies)))
	lines.append('\t@python %s %s' % (swig, module))
	lines.append('')
	for f in dependencies:
		lines.append('%s:' % f)

	fio = TextFio(fname, 'w')
	if fio.open() != 0:
		msg = '%s: can not open file (write)' % fname
		Error(prog).abort(msg)
	rc = fio.writelines(lines)
	fio.close()
	if rc != 0:
		msg = '%s: write failed' % fname
		Error(prog).abort(msg)

#  verbose用
#
def print_list(title, elms):
	print('  %s' % title)
	for elm in elms:
		print('    %s' % elm)

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

dry_run = options.dry_run
verbose = options.verbose

# ----------------------------------------------------------------------
#  プロジェクトとその依存関係の定義を取り込む
#
from project_dependencies import *
proj_depts['Base'] = 'None'
projs = proj_depts.keys()

# ----------------------------------------------------------------------
#  Base 以外の各プロジェクト毎に処理を行なう.
#
for proj in projs:
	print('  ** %s **' % proj, flush=True)

	#  ヘッダ情報を収集する
	interfs, inchdrs, srchdrs = collect_headers(proj)
	if verbose:
		print_list('INTERFS', interfs)
		print_list('INCHDRS', inchdrs)
		print_list('SRCHDRS', srchdrs)

	#  makefileを作成する
	makefname = '%s.%s' % (makefile, proj)
	make_makefile(proj, makefname, interfs, inchdrs, srchdrs)

	#  swigを実行する
	cmnd = '%s -f %s' % (make, makefname)
	rc = Proc(dry_run=dry_run).execute(cmnd, shell=True).wait()
	if rc != 0:
		Error(prog).abort('%s failed' % make)

# ----------------------------------------------------------------------
#  終了
#
sys.exit(0)

# end: RunSwig_EmbPython.py

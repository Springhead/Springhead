#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  SYNOPSIS:
#	RunSwig_EmbPython
#
#  DESCRIPTION:
#	ファイルの依存関係を調べて、EmbPythonSwig.py を最適に実行する.
#	実行するプロジェクトは ../../src/RunSwig/do_swigall.projs に定義する.
#	ただしプロジェクト Base は定義の有無に関わりなく実行する.
#
# ==============================================================================
#  Version:
#	Ver 1.0	 2020/02/19 F.Kanehori	Windows batch file から移植.
# ==============================================================================
version = 1.0
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
from Proc import *
from Util import *
from Error import *

# ----------------------------------------------------------------------
#  Globals
#

#  ディレクトリの定義
basedir = '../..'
bindir	= '%s/bin' % basedir
srcdir	= '%s/src' % basedir
incdir	= '%s/include' % basedir
etcdir	= '%s/RunSwig' % srcdir
embpythondir = '.'

#  依存関係にはないと見做すファイルの一覧
#
excludes = []

#  makefile に出力するときのパス
#
incdir_out = '../../include'
srcdir_out = '../../src'
embdir_out = '../../src/EmbPython'

#  使用するファイル名
#
projfile = 'do_swigall.projs'
makefile = 'Makefile_EmbPython.swig'

#  使用するプログラム名
#
make = 'nmake' if Util.is_windows() else 'make'
swig = 'EmbPythonSwig.py'

# ----------------------------------------------------------------------
#  内部使用メソッド
# ---------------------------------------------------------------------

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

#
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
#  make はインストールされているか
#
cmnd = '%s -help' % make if Util.is_unix() else '%s /?' % make
rc_make = Proc(dry_run=dry_run).execute(cmnd,
		 stdout=Proc.NULL, stderr=Proc.NULL, shell=True).wait()
if rc_make != 0:
	Error(prog).abort('can\'t find "%s"' % make)

# ----------------------------------------------------------------------
#  処理するモジュールの一覧を作成
#
fname = '%s/%s' % (etcdir, projfile)
fio = TextFio(fname, 'r')
if fio.open() != 0:
	Error(prog).abort('open error: "%s"' % fname)
lines = fio.read()
fio.close()

projs = ['Base']
for line in lines:
	if not line: continue
	projs.append(line.split()[0])
if verbose:
	print('  PROJS: %s' % projs)

# ----------------------------------------------------------------------
#  処理開始
#
for proj in projs:
	print('Project: %s' % proj)

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
	rc = Proc(dry_run=dry_run).execute(cmnd,
		 stderr=Proc.NULL, shell=True).wait()
	if rc != 0:
		Error(prog).abort('%s failed' % make)

# ----------------------------------------------------------------------
#  終了
#
sys.exit(0)

# end: RunSwig_EmbPython.py

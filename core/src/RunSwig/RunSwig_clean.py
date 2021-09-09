#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  SYNOPSIS:
#	python RunSwig_clean [options]
#
#  OPTIONS:
#	-C, --cmake		cmake が生成したファイルを削除
#	-R, --RunSwig		RunSwig が生成したファイルを削除
#
#  DESCRIPTION:
#	すべてのプロジェクトに対して指定されたクリーンアップ処理を行なう.
#
# ==============================================================================
#  Version:
#     Ver 1.0	 2021/04/01 F.Kanehori	make_manager.py から独立.
# ==============================================================================
from __future__ import print_function
version = '1.0'

import sys
import os
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

# ----------------------------------------------------------------------
#  Springhead python library の導入
#
libdir = '%s/RunSwig/pythonlib' % SrcDir
sys.path.append(libdir)
from TextFio import *
from FileOp import *
from Error import *

# ----------------------------------------------------------------------
#  ディレクトリパスには絶対パスを使用する (cmake 使用時の混乱を避けるため)
#
sprtop = os.path.abspath('%s/../..' % SrcDir)
srcdir = '%s/core/src' % sprtop
incdir = '%s/core/include' % sprtop

# ---------------------------------------------------------------------
#  オプションの定義
#
usage = 'Usage: %prog [options]'
parser = OptionParser(usage = usage)
parser.add_option('-C', '--cmake',
			dest='cmake', action='store_true', default=False,
			help='remove cmake generated files')
parser.add_option('-R', '--RunSwig',
			dest='runswig', action='store_true', default=False,
			help='remove RunSwig generated files')
parser.add_option('-v', '--verbose',
			dest='verbose', action='count', default=0,
			help='set verbose count')
parser.add_option('-V', '--version',
			dest='version', action='store_true', default=False,
			help='show version')

# ----------------------------------------------------------------------
#  コマンド引数の解析
#
(options, args) = parser.parse_args()
if options.version:
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
if len(args) > 0:
	parser.error("incorrect number of arguments")

cmake = options.cmake
runswig = options.runswig
verbose = options.verbose

# ----------------------------------------------------------------------
#  処理開始
# ----------------------------------------------------------------------

#  プロジェクトとその依存関係の定義を取り込む.
#
from project_dependencies import *
cwd = os.getcwd()

#  CMake が作成したダミーファイルを削除する
#
if cmake:
	msg = '    removed: %s (cmake generated dummy file)'
	for proj in projs:
		os.chdir('%s/%s' % (srcdir, proj))
		fio = TextFio('RunSwig_gen_files.txt')
		if fio.open() < 0:
			Error(prog).error(fio.error())
		tmp_lines = fio.read()
		fio.close()
		for line in tmp_lines:
			for fname in line.split():
				if os.path.exists(fname) and \
				   os.path.getsize(fname) == 0:
					os.remove(fname)
					if verbose:
						print(msg % fname)
		os.chdir(cwd)

#  RunSwig が作成したーファイルを削除する
#
if runswig:
	f_op = FileOp()
	for proj in projs:
		os.chdir('%s/%s' % (srcdir, proj))
		removed = []
		makefile = '%sStub.makefile' % proj
		if os.path.exists(makefile):
			removed.append(makefile)
			f_op.rm(makefile)
		int_file = '%s.i' % proj
		if os.path.exists(int_file):
			removed.append(int_file)
			f_op.rm(int_file)
		stb_file = '%sStub.cpp' % proj
		if os.path.exists(stb_file):
			removed.append(stb_file)
			f_op.rm(stb_file)
		hpp_file = '%sDecl.hpp' % proj
		if os.path.exists(hpp_file):
			removed.append(hpp_file)
			f_op.rm(hpp_file)
		log_file = 'swig_spr.log'
		if os.path.exists(log_file):
			removed.append(log_file)
			f_op.rm(log_file)
		if removed:
			print('    removed: %s' % ', '.join(removed))
		os.chdir(cwd)
	#
	removed = []
	os.chdir(incdir)	
	files = ['FWOldSpringhead']
	for f in files:
		if os.path.exists(f):
			removed.append(f)
			f_op.rm(f)
	os.chdir(cwd)
	os.chdir('%s/Framework' % srcdir)
	files = ['FWOldSpringheadStub.cpp', 'FWOldSpringheadDecl.hpp']
	for f in files:
		if os.path.exists(f):
			removed.append(f)
			f_op.rm(f)
	if removed:
		print('    removed: %s' % ', '.join(removed))
	os.chdir(cwd)

#  終了
#
if trace:
	print('LEAVE: %s' % prog)
	sys.stdout.flush()
sys.exit(0)

# end: make_manager.py

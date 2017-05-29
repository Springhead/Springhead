#!/usr/local/bin/python3.4
# -*- coding: utf-8 -*-
# ==============================================================================
#  FILE:
#	create_mkf.py
#
#  SYNOPSIS:
#	python create_mkf.py makefile project dependencies
#	    makefile:	    Makefile名
#	    project:	    プロジェクト名
#	    dependencies:   依存するプロジェクト名のリスト(カンマ区切り)
#
#  DESCRIPTION:
#	RunSwig 実行のための makefile を作成するスクリプト.
#	プロジェクトの依存関係は, 引数 dependencies で指定する.
#	このスクリプトは, 各プロジェクトのビルドを行なうディレクトリで実行され,
#	makefile もそこに作成される.
#
# ==============================================================================
#  Version:
#	Ver 1.0	 2017/04/20 F.Kanehori	Windows batch file から移植.
# ==============================================================================
version = 1.0
debug = False

import sys
import os
import glob
import copy
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

# ----------------------------------------------------------------------
#  Globals
#
E = Error(prog)
U = Util()
unix = U.is_unix()

# ----------------------------------------------------------------------
#  Directories and paths
#
#spr2top	= '../..'
spr2top	= Util.pathconv(os.path.relpath(spr2), 'unix')
incdir	= '%s/%s' % (spr2top, 'include')
srcdir	= '%s/%s' % (spr2top, 'src')
bindir	= '%s/%s' % (spr2top, 'bin')
pythondir = '/usr/bin'
runswigdir = '%s/%s' % (srcdir, 'RunSwig')
swigdir   = '%s/%s' % (srcdir, 'Foundation')

# ----------------------------------------------------------------------
#  Directorie names to output to makefile
#
incdir_out = incdir
srcdir_out = '..'

# ----------------------------------------------------------------------
#  File names
#
stubfile = 'Stub.cpp'
tempfile = prog + '.tmp'

# ----------------------------------------------------------------------
#  Scripts
#
pythonexe = 'python%s' % (python_version if Util.is_unix() else '')
python = '%s/%s' % (pythondir, pythonexe)
makemanager = '%s %s/make_manager.py' % (python, U.pathconv(runswigdir))
swig = '%s %s/RunSwig.py' % (python, U.pathconv(swigdir))

# ----------------------------------------------------------------------
#  常に依存関係にあると見做すファイルの一覧.
#
fd_inc = ['Springhead.h', 'Base/Env.h', 'Base/BaseDebug.h']
fd_src = ['Foundation/UTTypeDesc.h']
fixdept_inc = list(map(lambda x: '%s/%s' % (incdir_out, x), fd_inc))
fixdept_src = list(map(lambda x: '%s/%s' % (srcdir_out, x), fd_src))
fixhdrs = copy.deepcopy(fixdept_inc)
fixhdrs.extend(fixdept_src)

# ----------------------------------------------------------------------
#  依存関係にはないと見做すファイルの一覧.
#
excludes = []

# ----------------------------------------------------------------------
#  内部使用メソッド
# ---------------------------------------------------------------------
#  指定されたディレクトリに存在する".h"ファイルをリストアップする.
#
def filelist(dir, excludes=[]):
	files = sorted(glob.glob('%s/*.h' % dir))
	list(map(lambda x: files.remove(x), excludes))
	return Util.pathconv(files, 'unix')


# ----------------------------------------------------------------------
#  Main process
# ---------------------------------------------------------------------
#  オプションの定義
#
usage = 'Usage: %prog [options]'
parser = OptionParser(usage = usage)
parser.add_option('-D', '--debug',
			dest='debug', action='store_true', default=False,
			help='for debug')
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
if len(args) != 3:
	parser.error("incorrect number of arguments")

makefile = args[0]
project  = args[1]
depends  = args[2] if args[2] != 'None' else ''

verbose = options.verbose
debug   = options.debug
if verbose:
	print('  makefile: %s' % makefile)
	print('  project:  %s' % project)
	print('  depends:  %s' % depends)
	print('  verbose:  %s' % verbose)
	print('  debug:    %s' % debug)

# ----------------------------------------------------------------------
#   Swig に渡す引数
#
swigargs = project
if depends != '':
	swigargs += ' %s' % depends.replace(',', ' ')

# ----------------------------------------------------------------------
#  ヘッダ情報の収集
#	依存関係にあるファイルの一覧を、変数 {inc|src}hdrs に集める.
#	毎回ディレクトリのリストをとる (新規ファイル追加対応).
#
inchdrs = filelist('%s/%s' % (incdir, project))
if depends != '':
	for dept in depends.split(','):
		inchdrs.extend(filelist('%s/%s' % (incdir, dept)))

srchdrs = filelist('%s/%s' % (srcdir, project))
replaced = '%s/%s' % (srcdir, project)
srchdrs = list(map(lambda x: x.replace(replaced, '.'), srchdrs))
if depends != '':
	for dept in depends.split(','):
		srchdrs.extend(filelist('%s/%s' % (srcdir, dept)))

if verbose:
	print('  FIXHDRS: %s' % fixhdrs)
	print('  INCHDRS: %s' % inchdrs)
	print('  SRCHDRS: %s' % srchdrs)

# ----------------------------------------------------------------------
#  makefile の内容を生成する.
#
if verbose:
	print('    making %s' % makefile.replace('.tmp', ''))
lines = []

lines.append('# THIS FILE IS AUTO-GENERATED. ** DO NOT EDIT THIS FILE **')
lines.append('# File: %s' % makefile.replace('.tmp', ''))
lines.append('')

lines.append('FIXHDRS=\\')
for f in fixhdrs:
	lines.append('%s \\' % f)
lines.append('')
lines.append('INCHDRS=\\')
for f in inchdrs:
	lines.append('%s \\' % f)
lines.append('')
lines.append('SRCHDRS=\\')
for f in srchdrs:
	lines.append('%s \\' % f)
lines.append('')

lines.append('all:\t%s%s' % (project, stubfile))
lines.append('')

lines.append('%s%s:\t$(FIXHDRS) $(INCHDRS) $(SRCHDRS)' % (project, stubfile))
lines.append(Util.pathconv('\t%s -t' % makemanager))
lines.append(Util.pathconv('\t%s %s' % (swig, swigargs)))
lines.append('')

lines.append('$(FIXHDRS):\t')
lines.append('\t')

lines.append('$(INCHDRS):\t')
lines.append('\t')

lines.append('$(SRCHDRS):\t')
lines.append('\t')

if verbose:
	for line in lines:
		print(line)

# ----------------------------------------------------------------------
#  ファイルに書き出す.
#
fobj = TextFio(makefile, 'w', encoding='utf8')
if fobj.open() < 0:
	E.print(fobj.error())
if fobj.writelines(lines, '\n') < 0:
	E.print(fobj.error())
fobj.close()

# ----------------------------------------------------------------------
#  処理終了.
#
sys.exit(0)

# end create_mkf.py

#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  SYNOPSIS:
#	python PrepForCMake.py src-dir dst-dir
#	arguments:
#	    src-dir:	'src' directory path on the original source tree.
#	    dst-dir:	'src' directory path on the build tree.
#
#  DESCRIPTION:
#	CMake を使用するにあたって必要な前処理を行なう。
#
#	(1) RunSwig を実行するにあたり、必要となるスクリプトファイル等がビルド
#	    ディレクトリにないと実行できないので、予めコピーする。
#
#	(2) ビルドディレクトリが <dst_dir> となることで各種の相対パスが変わって
#	    くる。実行パスについては絶対パス化することで回避できるが、.i ファイ
#	    ルには相対パスが直接記述されているため、パスの書き換えを行わないと
#	    従来のシステムとは共存できない (CMakeしか使わないのであれば.iファイ
#	    ルを直接編集してしまえばよい）。
#
# ==============================================================================
#  Version:
#	Ver 1.0	 2019/02/28 F.Kanehori	First release version.
# ==============================================================================
version = 1.0

import sys
import os
import glob
import shutil
import subprocess
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = os.path.basename(sys.argv[0]).split('.')[0]

# ---------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options] src-dir dst-dir\n'\
	+ '\tsrc-dir: src directory on the original source tree\n'\
	+ '\tdst-dir: src directory on the build tree'
parser = OptionParser(usage = usage)
parser.add_option('-v', '--verbose', dest='verbose',
			action='count', default=0,
                        help='set verbose count')
parser.add_option('-V', '--version', dest='version',
			action='store_true', default=False,
                        help='show version')

# ----------------------------------------------------------------------
#  Process for command line
#
(options, args) = parser.parse_args()
if options.version:
        print('%s: Version %s' % (prog, version))
        sys.exit(0)
if len(args) != 2:
	print('%s: incorrect number of arguments\n' % prog)
	subprocess.Popen('python %s.py -h' % prog, shell=True).wait()
	sys.exit(1)

srcdir = args[0].replace('\\', '/')
dstdir = args[1].replace('\\', '/')
verbose = options.verbose
if verbose:
	print('src-dir: %s' % srcdir)
	print('dst-dir: %s' % dstdir)

# ----------------------------------------------------------------------
#  Directories
#
RS_srcdir = '%s/RunSwig' % srcdir
FO_srcdir = '%s/Foundation' % srcdir
FW_srcdir = '%s/Framework' % srcdir
RS_dstdir = '%s/RunSwig' % dstdir
FO_dstdir = '%s/Foundation' % dstdir
FW_dstdir = '%s/Framework' % dstdir

# ----------------------------------------------------------------------
#  Methods
#
def expand_filenames(names):
	fnames = []
	for name in names:
		fnames.extend(glob.glob(name))
	return fnames

# ----------------------------------------------------------------------
#  Main process
#
cwd = os.getcwd()

# (1) RunSwig にあるスクリプトファイル等を作業場所にコピーする。
#
os.chdir(RS_srcdir)
fnames = expand_filenames(['*.py', '*.bat', '*.projs', 'Makefile*'])
for fname in fnames:
	if verbose:
		print('  copy %s -> %s' % (fname, RS_dstdir))
	shutil.copy(fname, RS_dstdir)
#
pythonlib = 'pythonlib'
dstdir = '%s/%s' % (RS_dstdir, pythonlib)
#os.makedirs(dstdir, exist_ok=True)	## ??
if not os.path.exists(dstdir):
	os.mkdir(dstdir)
os.chdir(pythonlib)
fnames = expand_filenames(['*.py'])
for fname in fnames:
	if verbose:
		print('  copy %s/%s -> %s' % (pythonlib, fname, dstdir))
	shutil.copy(fname, dstdir)

# (2) Foundation にあるスクリプトファイルを作業場所にコピーする
#     Foundation にある.iファイルを変更して作業場所に書き出す
#
os.chdir(FO_srcdir)
for fname in ['RunSwig.py', 'ScilabSwig.py']:
	if verbose:
		print('  copy %s -> %s' % (fname, FO_dstdir))
	shutil.copy(fname, FO_dstdir)

cmnd = 'python %s/replace.py' % RS_srcdir
args = '-o %s/Scilab.i Scilab.i ../../include=../../../include' % FO_dstdir
stat = subprocess.Popen('%s %s' % (cmnd, args)).wait()
if stat != 0:
	print('%s: Error: rewrite "Scilab.i" failed' % prog)
	sys.exit(1)

# (3) Framework にあるスクリプトファイルを作業場所にコピーする
#
os.chdir(FW_srcdir)
for fname in ['RunSwigFramework.py']:
	if verbose:
		print('  copy %s -> %s' % (fname, FW_dstdir))
	shutil.copy(fname, FW_dstdir)

#  以上
#
os.chdir(cwd)
sys.exit(0)

# end: PrepForCMake.py

#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  SYNOPSIS:
#	python PrepForCMake.py src-dir bld-dir
#	arguments:
#	    src-dir:	'src' directory path on the original source tree.
#	    bld-dir:	'src' directory path on the build tree.
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
#	Ver 1.0	 2019/03/05 F.Kanehori	First release version.
#	Ver 1.1	 2019/03/07 F.Kanehori	Change .i file rewrite method.
# ==============================================================================
version = 1.1

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
usage = 'Usage: %prog [options] src-dir bld-dir\n'\
	+ '\tsrc-dir: src directory on the original source tree\n'\
	+ '\tbld-dir: src directory on the build tree'
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
blddir = args[1].replace('\\', '/')
verbose = options.verbose
if verbose:
	print('src-dir: %s' % srcdir)
	print('bld-dir: %s' % blddir)

# ----------------------------------------------------------------------
#  Directories
#
RS_srcdir = '%s/RunSwig' % srcdir
FO_srcdir = '%s/Foundation' % srcdir
FW_srcdir = '%s/Framework' % srcdir
RS_blddir = '%s/RunSwig' % blddir
FO_blddir = '%s/Foundation' % blddir
FW_blddir = '%s/Framework' % blddir

# ----------------------------------------------------------------------
#  Methods
#
def expand_filenames(names):
	fnames = []
	for name in names:
		fnames.extend(glob.glob(name))
	return fnames

def is_newer(file_to_check, compared_with, use_mtime=True):
	if not os.path.exists(file_to_check):
		print('%s: "%s" does not exist' % (prog, file_to_check))
		return None
	if not os.path.exists(compared_with):
		return True
	#
	st1 = os.stat(file_to_check)
	st2 = os.stat(compared_with)
	time1 = st1.st_mtime_ns if use_mtime else st1.st_ctime_ns
	time2 = st2.st_mtime_ns if use_mtime else st2.st_ctime_ns
	if time1 <= time2:
		if verbose:
			print('  %s is older ... not copy' % file_to_check)
		return False
	return True

def copy_if_newer(fname, blddir, add_info=None):
	if not is_newer(fname, '%s/%s' % (blddir, fname)):
		return
	if verbose:
		src = '%s/%s' % (add_info, fname) if add_info else fname
		print('  copy %s -> %s' % (src, blddir))
	shutil.copy(fname, blddir)

def find_relative_dir(blddir):
	cwd = blddir.replace(os.sep, '/').split('/')
	reldir = ''
	while cwd != []:
		if cwd[-1] == 'core':
			reldir += 'include'
			break
		reldir += '../'
		cwd = cwd[:-1]
	return reldir

# ----------------------------------------------------------------------
#  Main process
#
cwd = os.getcwd()

# (1) RunSwig にあるスクリプトファイル等を作業場所にコピーする。
#
os.chdir(RS_srcdir)
fnames = expand_filenames(['*.py', '*.bat', '*.projs', 'Makefile*'])
for fname in fnames:
	copy_if_newer(fname, RS_blddir)
#
pythonlib = 'pythonlib'
blddir = '%s/%s' % (RS_blddir, pythonlib)
#os.makedirs(blddir, exist_ok=True)	## ??
if not os.path.exists(blddir):
	os.mkdir(blddir)
os.chdir(pythonlib)
fnames = expand_filenames(['*.py'])
for fname in fnames:
	copy_if_newer(fname, blddir, pythonlib)

# (2) Foundation にあるスクリプトファイルを作業場所にコピーする
#     Foundation にある.iファイルを変更して作業場所に書き出す
#
if not os.path.exists(FO_blddir):
	os.makedirs(FO_blddir)
os.chdir(FO_srcdir)
for fname in ['RunSwig.py', 'ScilabSwig.py']:
	copy_if_newer(fname, FO_blddir)

if is_newer("Scilab.i", '%s/Scilab.i' % FO_blddir):
	reldir = find_relative_dir(FO_blddir)
	cmnd = 'python %s/replace.py' % RS_srcdir
	#args = '-o %s/Scilab.i Scilab.i ../../include=../../../include' % FO_blddir
	args = '-o %s/Scilab.i Scilab.i ../../include=%s' % (FO_blddir, reldir)
	stat = subprocess.Popen('%s %s' % (cmnd, args)).wait()
	if stat != 0:
		print('%s: Error: rewrite "Scilab.i" failed' % prog)
		sys.exit(1)

# (3) Framework にあるスクリプトファイルを作業場所にコピーする
#
if not os.path.exists(FW_blddir):
	os.makedirs(FW_blddir)
os.chdir(FW_srcdir)
for fname in ['RunSwigFramework.py']:
	copy_if_newer(fname, FW_blddir)

#  以上
#
os.chdir(cwd)
sys.exit(0)

# end: PrepForCMake.py

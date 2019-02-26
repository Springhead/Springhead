#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  SYNOPSIS:
#	python PrepForCMake.py
#
#  DESCRIPTION:
#	CMake を使用するにあたって必要な前処理を行なう。
#
#	(1) RunSwig を実行するにあたり、必要となるスクリプトファイル等が build
#	    ディレクトリにないと実行できないので、予めコピーする。
#
#	(2) Build ディレクトリが src/build となることで各種の相対パスが異なって
#	    くる。実行パスについては絶対パス化することで回避できるが、.i ファイ
#	    ルには相対パスが直接記述されているため、パスの書き換えを行わないと
#	    従来のシステムとは共存できない (CMakeしか使わないのであれば.iファイ
#	    ルを直接編集してしまえばよい）。
#
# ==============================================================================
#  Version:
#	Ver 1.0	 2019/02/26 F.Kanehori	First release version.
# ==============================================================================
version = 1.0

import sys
import os
import glob
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].replace(os.sep, '/').split('/')[-1].split('.')[0]

# ----------------------------------------------------------------------
#  Import Springhead python library.
#
from FindSprPath import *
spr_path = FindSprPath(prog)
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from Error import *
from Util import *
from Proc import *
from FileOp import *

# ----------------------------------------------------------------------
#  Directories
#
sprtop = spr_path.abspath()
##bindir = spr_path.relpath('bin')
srcdir = spr_path.abspath('src')
RSdir = '%s/%s' % (srcdir, 'RunSwig')
FOdir = '%s/%s' % (srcdir, 'Foundation')
FWdir = '%s/%s' % (srcdir, 'Framework')

# ---------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options]'
parser = OptionParser(usage = usage)
parser.add_option('-b', '--build-dir', dest='build_dir',
			action='store', default='build',
                        help='build (working) directory')
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

build_dir = options.build_dir
verbose = options.verbose

# ----------------------------------------------------------------------
#  Directories
#
blddir = '%s/%s' % (srcdir, build_dir)
RS_blddir = '%s/%s' % (blddir, 'RunSwig')
FO_blddir = '%s/%s' % (blddir, 'Foundation')
FW_blddir = '%s/%s' % (blddir, 'Framework')

# ----------------------------------------------------------------------
#  Main process
#
cwd = os.getcwd()
Fop = FileOp(verbose=0)

# (1) RunSwig にあるスクリプトファイル等を作業場所にコピーする。
#
os.chdir(RSdir)
Fop.cp('*.py', RS_blddir)
Fop.cp('*.bat', RS_blddir)
Fop.cp('*.projs', RS_blddir)
Fop.cp('Makefile*', RS_blddir)
#
pythonlib = 'pythonlib'
Fop.makedirs(pythonlib, exist_ok=True)
os.chdir(pythonlib)
Fop.cp('*', '%s/%s' % (RS_blddir, pythonlib))
os.chdir(cwd)

# (2) Foundation にあるスクリプトファイルを作業場所にコピーする
#     Foundation にある.iファイルを変更して作業場所に書き出す
#
os.chdir(FOdir)
Fop.cp('RunSwig.py', FO_blddir)
Fop.cp('ScilabSwig.py', FO_blddir)
cmnd = 'python %s/replace.py' % RSdir
args = '-o %s/Scilab.i Scilab.i ../../include=../../../include' % FO_blddir
stat = Proc().execute('%s %s' % (cmnd, args)).wait()
if stat != 0:
	Error(prog).abort('rewrite "Scilab.i" failed')

# (3) Framework にあるスクリプトファイルを作業場所にコピーする
#
os.chdir(FWdir)
Fop.cp('RunSwigFramework.py', FW_blddir)

#  以上
#
os.chdir(cwd)
sys.exit(0)

# end: PrepForCMake.py

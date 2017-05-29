#!/usr/local/bin/python3.4
# -*- coding: utf-8 -*-
# ==============================================================================
#  FILE:
#	RunSwigFramework.py
#
#  SYNOPSIS:
#	python RunSwigFramework.py
#
#  DESCRIPTION:
#	以下のファイルを生成するために ShiftJIS の世界で swig を実行する.
#	生成するファイル：
#	    include/SprFWOldSpringheadDecl.hpp
#	    src/FWOldSpringheadDecl.hpp
#	    src/FWOldSpringheadStub.cpp
#	作業ディレクトリ：
#	    Springhead2 の直下に swigtemp というディレクトリを作り使用する.
#
# ==============================================================================
#  Version:
#	Ver 1.0	 2017/05/10 F.Kanehori	Windows batch file から移植.
# ==============================================================================
version = 1.0
debug = True

import sys
import os
import glob
from optparse import OptionParser

#  Import Springhead2 python library.
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
python_version = 34
verbose = 1 if debug else 0
dry_run = 1 if debug else 0

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
incdir = '%s/%s' % (spr2top, 'include')
srcdir = '%s/%s' % (spr2top, 'src')
bindir = '%s/%s' % (spr2top, 'bin')
pythondir = '%s/Python%s' % (bindir, python_version)
swigdir = '%s/swig' % bindir
foundation_dir = '%s/%s' % (srcdir, 'Foundation')
framework_dir = '%s/%s' % (srcdir, 'Framework')

# ----------------------------------------------------------------------
#  Scripts
#
pythonexe = 'python%s' % (python_version if unix else '')
python = '%s/%s' % (pythondir, pythonexe)
nkf = '%s/nkf' % bindir
swig = 'swig'
make = 'make' if unix else 'nmake'
runswig_foundation = '%s %s/RunSwig.py' % (python, foundation_dir)

# ----------------------------------------------------------------------
#  Files and etc.
#
module = 'FWOldSpringhead'
interfacefile = '%s.i' % module		# in src/Foundation
makefile = '%sStub.mak.txt' % module	# in src/Foundation
stubcpp = '%sStub.cpp' % module		# in src/Framework

# ----------------------------------------------------------------------
#  Local method
# ---------------------------------------------------------------------
def output(fname, lines):
	fobj = TextFio(fname, 'w', encoding='utf8')
	if fobj.open() < 0:
		E.print(fobj.error())
	if fobj.writelines(lines, '\n') < 0:
		E.print(fobj.error())
	fobj.close()


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
parser.add_option('-d', '--dry_run',
			dest='dry_run', action='store_true', default=False,
			help='dry_run (for debug)')
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
if len(args) != 0:
	parser.error("incorrect number of arguments")

clean	= options.clean
verbose	= options.verbose
dry_run	= options.dry_run

# ----------------------------------------------------------------------
#  src/Foundation へ移って RunSwig を実行する.
#
cmd = '%s Framework Foundation' % runswig_foundation
status = U.exec(cmd, shell=True, dry_run=dry_run)
if status != 0:
	E.print('%s failed (%d)' % (runswig_foundation, status))

# ----------------------------------------------------------------------
#  swigtemp 下に SJIS world を作る.
#
swigtmp = '%s/swigtemp' % spr2top
tmp_inc = '%s/include' % swigtmp
tmp_src = '%s/src' % swigtmp

#  Create directory if not exists.
incdir_names = ['Base', 'Framework']
srcdir_names = ['Foundation', 'Framework']
tmp_incdirs = list(map(lambda x: '%s/%s' % (tmp_inc, x), incdir_names))
tmp_srcdirs = list(map(lambda x: '%s/%s' % (tmp_src, x), srcdir_names))
for dir in tmp_incdirs:
	if not os.path.exists(dir):
		if verbose: print('creating %s' % dir)
		os.makedirs(dir)
for dir in tmp_srcdirs:
	if not os.path.exists(dir):
		if verbose: print('creating %s' % dir)
		os.makedirs(dir)

#  Convert kanji code into SJIS.
incf_names = ['Springhead.h', 'Base/Env.h', 'Base/BaseDebug.h']
srcf_names = ['Foundation/UTTypeDesc.h', 'Framework/FWOldSpringheadNodeHandler.h']
for file in incf_names:
	cmnd = '%s -s -O %s/%s %s/include/%s' % (nkf, incdir, file, swigtmp, file)
	cmnd = U.pathconv(cmnd)
	status = Util.exec(cmnd, shell=True, verbose=verbose, dry_run=dry_run)
	if status != 0:
		E.print('"%s" failed (%d)' % (U.pathconv(cmnd, 'unix'), status))
for file in srcf_names:
	cmnd = '%s -s -O %s/%s %s/src/%s' % (nkf, srcdir, file, swigtmp, file)
	cmnd = U.pathconv(cmnd)
	status = Util.exec(cmnd, shell=True, verbose=verbose, dry_run=dry_run)
	if status != 0:
		E.print('"%s" failed (%d)' % (U.pathconv(cmnd, 'unix'), status))

# ----------------------------------------------------------------------
#  ここからは swigtemp/src/Foundation に移って作業する.
#
os.chdir('%s/Foundation' % tmp_src)
if verbose:
	print('  chdir to %s' % U.pathconv(os.getcwd(), 'unix'))

# ----------------------------------------------------------------------
#  swig のインターフェイスファイルを作成する.
#
srcimp = '%s/src/Framework/FWOldSpringheadNodeHandler.h' % swigtmp
srcimpdep = '%s/Framework/FWOldSpringheadNodeHandler.h' % srcdir
swigtmp_rel = Util.pathconv(os.path.relpath(swigtmp), 'unix')

print('src files: %s' % srcimp)
lines = []
lines.append('# Do not edit. %s will update this file.' % prog)
lines.append('%%module %s' % module)
for file in incf_names:
	lines.append('%%include "%s/%s"' % (incdir, file))
for file in srcf_names:
	lines.append('%%include "%s/%s"' % (srcdir, file))
if verbose:
	path = '%s/%s' % (os.getcwd(), interfacefile)
	print('  creating "%s"' % U.pathconv(path, 'unix'))
output(interfacefile, lines)

# ----------------------------------------------------------------------
#  makefile を作成する.
#
swigargs = '-I../%s/Lib' % swigdir
swigargs += ' -spr -w312,325,401,402 -DSWIG_OLDNODEHANDLER -c++'
cp = 'cp' if unix else 'copy'
rm = 'rm' if unix else 'del'
quiet = '>/dev/null 2>&1' if unix else '>NUL 2>&1'
lines = []
lines.append('# Do not edit. %s will update this file.' % prog)
lines.append('all:\t../../../src/Framework/%sStub.cpp' % module)
lines.append('../../../src/Framework/%sStub.cpp:\t../%s' % (module, srcimpdep))
lines.append('\t../%s/%s %s %s' % (swigdir, swig, swigargs, interfacefile))
lines.append('\t%s Spr%sDecl.hpp ../../../include/%s %s' % (cp, module, module, quiet))
lines.append('\t%s %sStub.cpp ../../../src/Framework %s' % (cp, module, quiet))
lines.append('\t%s %sDecl.hpp ../../../src/Framework %s' % (cp, module, quiet))
lines.append('')
lines.append('clean:\t')
lines.append('\t-%s -f ../../../src/Framework/%sStub.cpp %s' % (rm, module, quiet))
lines.append('\t-%s -f ../../../include/%s %s' % (rm, module, quiet))
lines.append('\t-%s -f ../../../src/Framework/%sStub.cpp %s' % (rm, module, quiet))
lines.append('\t-%s -f ../../../src/Framework/%sDecl.hpp %s' % (rm, module, quiet))
if verbose:
	path = '%s/%s' % (os.getcwd(), makefile)
	print('  creating "%s"' % U.pathconv(path, 'unix'))
lines = U.pathconv(lines)
output(makefile, lines)

# ----------------------------------------------------------------------
#  make を実行する.
#
cmd = '%s -f %s' % (make, makefile)
if clean:
	cmd += ' clean'
status = U.exec(cmd, shell=True, dry_run=dry_run)
if status != 0:
	E.print('%s failed (%d)' % (make, status))

# ----------------------------------------------------------------------
#  処理終了.
#
sys.exit(0)

# end: RunSwigFramework.py

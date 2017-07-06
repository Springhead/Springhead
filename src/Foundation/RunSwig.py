#!/usr/local/bin/python3.4
# -*- coding: utf-8 -*-
# ==============================================================================
#  FILE:
#	RunSwig.py
#
#  SYNOPSIS:
#	python RunSwig.py module target [target]...
#
#  DESCRIPTION:
#	指定されたモジュールについて, swig を実行するための makefile を作成し,
#	make を実行する.
#
# ==============================================================================
#  Version:
#	Ver 1.0	 2017/04/24 F.Kanehori	Windows batch file から移植.
#	Ver 1.0a 2017/06/29 F.Kanehori	Add messages.
# ==============================================================================
version = 1.0

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
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
python_version = 34

# ----------------------------------------------------------------------
#  Globals
#
U = Util()
E = Error(prog)
unix = U.is_unix()

# ----------------------------------------------------------------------
#  Directories
#
spr2top = U.pathconv(os.path.relpath(spr2), 'unix')
incdir = '%s/%s' % (spr2top, 'include')
srcdir = '%s/%s' % (spr2top, 'src')
bindir = '%s/%s' % (spr2top, 'bin')
swigdir = '%s/%s' % (bindir, 'swig')
pythondir = '%s/Python%s' % (bindir, python_version)

incdir_rel = U.pathconv(os.path.relpath(incdir), 'unix')
srcdir_rel = U.pathconv(os.path.relpath(srcdir), 'unix')

# ----------------------------------------------------------------------
#  Scripts
#
pythonexe = 'python%s' % (python_version if unix else '')
python = '%s/%s' % (pythondir, pythonexe)
make = 'make' if unix else 'nmake'
swig = 'swig'

# ----------------------------------------------------------------------
#  Paths
#
addpath = os.pathsep.join([bindir, swigdir])

# ----------------------------------------------------------------------
#  Main process
# ----------------------------------------------------------------------
#  オプションの定義
#
usage = 'Usage: %prog [options] module [projct [project]..]'
parser = OptionParser(usage = usage)
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
if len(args) < 1:
	parser.error("incorrect number of arguments")

module = args[0]
target_list = args[::-1]

verbose	= options.verbose
dry_run	= options.dry_run

# ----------------------------------------------------------------------
#  Files
#
interfacefile = '%s.i' % module
makefile = '%sStub.mak.txt' % module
stubfile = '%s/%s/%sStub.cpp' % (srcdir, module, module)

# ----------------------------------------------------------------------
#  ヘッダファイル情報を収集する.
#
incf_names = ['Springhead.h', 'Base/Env.h', 'Base/BaseDebug.h']
srcf_names = ['Foundation/UTTypeDesc.h']
auxdep_inc = list(map(lambda x: '%s/%s' % (incdir, x), incf_names))
auxdep_src = list(map(lambda x: '%s/%s' % (srcdir, x), srcf_names))
auxdep = copy.deepcopy(auxdep_inc)
auxdep.extend(auxdep_src)

srcinf = []
srcimp = []
srcinfdep = []
srcimpdep = []
for target in target_list:
	srcinf.extend(glob.glob('%s/%s/*.h' % (incdir_rel, target)))
	srcimp.extend(glob.glob('%s/%s/*.h' % (srcdir_rel, target)))
	srcinfdep.extend(glob.glob('%s/%s/*.h' % (incdir_rel, target)))
	srcimpdep.extend(glob.glob('%s/%s/*.h' % (srcdir_rel, target)))
srcinf = U.pathconv(srcinf, 'unix')
srcimp = U.pathconv(srcimp, 'unix')
srcinfdep = U.pathconv(srcinfdep, 'unix')
srcimpdep = U.pathconv(srcimpdep, 'unix')
if verbose:
	print('srcinf: %s' % srcinf)
	print('srcimp: %s' % srcimp)
	print('srcinfdep: %s' % srcinfdep)
	print('srcimpdep: %s' % srcimpdep)

# ----------------------------------------------------------------------
#  インターフェイスファイルを生成する.
#
lines = []
lines.append('#\tDo not edit. RunSwig.py will update this file.')
lines.append('%%module %s' % module)
lines.append('#define DOUBLECOLON ::')
for fname in auxdep_inc:
	lines.append('%%include "%s"' % fname)
for fname in srcinf:
	lines.append('%%include "%s"' % fname)
for fname in auxdep_src:
	lines.append('%%include "%s"' % fname)
for fname in srcimp:
	lines.append('%%include "%s"' % fname)
#
def output(fname, lines):
	if verbose:
		print('%s:' % fname)
		for line in lines:
			print('  %s' % line)
	fobj = TextFio(fname, 'w', encoding='utf8')
	if fobj.open() < 0:
		E.print(fobj.error())
	if fobj.writelines(lines, '\n') < 0:
		E.print(fobj.error())
	fobj.close()
#
path = '%s/%s' % (os.getcwd(), interfacefile)
print('    *** %s: creating "%s"' % (prog, U.pathconv(path, 'unix')))
output(interfacefile, lines)

# ----------------------------------------------------------------------
#  makefile を生成する.
#
lines = []
lines.append('#\tDo not edit. RunSwig.py will update this file.')
lines.append('all:\t%s' % stubfile)
line = '%s:\t' % stubfile
line += ' '.join(auxdep)
line += ' ' + ' '.join(srcinfdep)
line += ' ' + ' '.join(srcimpdep)
lines.append(line)
line = '\t%s/%s -I%s/Lib' % (swigdir, swig, swigdir)
line += ' -spr -w305,312,319,325,401,402'
line += ' -DSWIG_%s -c++ %s' % (module, interfacefile)
lines.append(line)
#
path = '%s/%s' % (os.getcwd(), makefile)
print('    *** %s: creating "%s"' % (prog, U.pathconv(path, 'unix')))
lines = U.pathconv(lines)
output(makefile, lines)

# ----------------------------------------------------------------------
#  make を実行する.
#
cmd = '%s -f %s' % (make, U.pathconv(makefile))
print('    *** %s: execute %s' % (prog, cmd))
status = U.exec(cmd, addpath=addpath, shell=True, dry_run=dry_run)
if status != 0:
	E.print('%s failed (%d)' % (make, status))

sys.exit(0)

# end: RunSwig.py

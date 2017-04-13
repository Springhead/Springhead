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
#	Ver 1.0	 2017/04/13 F.Kanehori	Windows batch file から移植.
# ==============================================================================
version = 1.0

import sys
import os
import glob
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
python_version = 34
debug = True

# ----------------------------------------------------------------------
#  Globals
#
U = Util()
unix = U.is_unix()

# ----------------------------------------------------------------------
#  Process for command line
#
(options, args) = parser.parse_args()
module = args[0]
target_list = args[1:][::-1]

# ----------------------------------------------------------------------
#  Directories
#
spr2top = U.pathconv(os.path.relpath(spr2), 'unix')
bindir = '%s/%s' % (spr2top, 'bin')
incdir = '%s/%s' % (spr2top, 'include')
swigdir = '%s/%s' % (bindir, 'swig')
pythondir = '%s/Python%s' % (bindir, python_version)

incdir_rel = U.pathconv(os.relpath(incdir), 'unix')
srcdir_rel = U.pathconv(os.relpath(srcdir), 'unix')

# ----------------------------------------------------------------------
#  Scripts
#
pythonexe = 'python%s' % (python_version if unix else '')
python = '%s/%s' % (pythondir, pythonexe)
make = 'make' if unix else 'nmake'

# ----------------------------------------------------------------------
#  Files
#
modulefile = '%s.i' % module
makefile = '%s/%s' % (module, 'Stub.mak.txt')
stubfile = '%s/%s/%sStub.cpp' % (srcdir, module, module)

# ----------------------------------------------------------------------
#  Paths
#
addpath = os.pathsep.join([bindir, swigdir])

# ----------------------------------------------------------------------
#  Main process
# ----------------------------------------------------------------------

#  Gather header file information.
#
auxdep_list1 = ['%s/Springhead.h' % incdir,
		'%s/Base/Env.h' % incdir,
		'%s/Base/BaseDebug.h' % incdir ]
auxdep_list2 = ['%s/Foundation/UTTypeDesc.h' % srcdir ]
auxdep = ' '.join(auxdep_list1.extend(auxdep_list2))

srcinf = []
srcimp = []
srcinfdep = []
srcimpdep = []
for target in target_list:
	srcinf.extend(glob.glob('%s/%s/*.h' % (incdir_rel, target)))
	srcimp.extend(glob.glob('%s/%s/*.h' % (srcdir_rel, target)))
	srcinfdep.extend(glob.glob('%s/%s/*.h' % (incdir_rel, target)))
	srcimpdep.extend(glob.glob('%s/%s/*.h' % (srcdir_rel, target)))

#  Make swig ".i" file
#
lines = []
lines.append('#\tDo not edit. RunSwig.py will update this file.')
lines.append('%%module %s' % module)
lines.append('#define DOUBLECOLON ::')
for fname in auxdep_list1:
	lines.append('%%include "%s"' % fname)
for fname in srcinf:
	lines.append('%%include "%s"' % fname)
for fname in auxdep_list2:
	lines.append('%%include "%s"' % fname)
for fname in srcimp:
	lines.append('%%include "%s"' % fname)
#
def fileout(fname):
	fobj = TextFio(fname, 'w', encoding='utf8')
	if fobj.open() < 0:
		E.print(fobj.error())
	if fobj.writelines(lines, '\n') < 0:
		E.print(fobj.error())
	fobj.close()
#
fileout(modulefile)

#  Make makefile.
#
lines = []
lines.append('#\tDo not edit. RunSwig.py will update this file.')
lines.append('echo all:\t%s' % stubfile)
for fname in auxdep:
	lines.append('%%include "%s"' % fname)
for fname in srcinfdep:
	lines.append('%%include "%s"' % fname)
for fname in srcimpdep:
	lines.append('%%include "%s"' % fname)
args = ['-spr', '-w305,312,319,325,401,402']
args.append('-DSWIG_%s' % module)
args.append('-c++')
args.append(modulefile)
lines.append(' '.join(args))
#
fileout(stubfile)

#  Execute make.
#
cmd = '%s -f %s' % (make, U.pathconv(makefile))
U.exec(cmd, addpath=addpath, shell=True, dry_run=debug)

sys.exit(0)

# end: RunSwig.py

#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  SYNOPSIS:
#	python make_manager [options]
#
#	対象とするプロジェクトに関するオプション：
#	    -A	  対象となるすべてのプロジェクトについて, 以降の引数で示された
#		  処理を実行する.
#
#	個別のプロジェクトの処理に関するオプション：
#	    -d	  "Makefile.swig" を削除する.
#	    -c	  "Makefile.swig" を生成する. 既にあったら何もしない.
#	    -t	  "Makefile.swig.tmp" を作成する (テンポラリファイル作成モード).
#	    -r	  "Makefile.swig.tmp" があれば, それを改名して "Makefile"とする.
#
#	    テンポラリファイル作成モード： make の実行中に"Makefile.swig"を
#	    書き換えてしまうのを回避するためのモード.
#
#  DESCRIPTION:
#	各プロジェクトで実行する make の制御を行なう.
#
# ==============================================================================
#  Version:
#     Ver 1.0	 2017/04/13 F.Kanehori	Windows batch file から移植.
#     Ver 1.1	 2017/04/17 F.Kanehori	Suppress warnig message.
#     Ver 1.2	 2017/07/24 F.Kanehori	Python executable directory moved.
#     Ver 1.3	 2017/09/04 F.Kanehori	New python library に対応.
#     Ver 1.4	 2017/10/11 F.Kanehori	起動するpythonを引数化.
#     Ver 1.5	 2017/11/08 F.Kanehori	Python library path の変更.
#     Ver 1.6	 2017/11/29 F.Kanehori	Python library path の変更.
#     Ver 1.7	 2018/07/03 F.Kanehori	空白を含むユーザ名に対応.
#     Ver 1.8	 2019/02/21 F.Kanehori	Cmake環境に対応.
#     Ver 1.9	 2019/04/01 F.Kanehori	Python library path 検索方法変更.
#     Ver 1.10	 2019/09/18 F.Kanehori	Cmakeが生成したファイルの後始末を追加.
# ==============================================================================
version = 1.10
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
from FindSprPath import *
spr_path = FindSprPath(prog)
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from TextFio import *
from Proc import *
from FileOp import *
from Error import *

# ----------------------------------------------------------------------
#  Globals
#
proc = Proc()
f_op = FileOp()

# ----------------------------------------------------------------------
#  Files
#
makefile = 'makefile.swig'		# name of target makefile
tempfile = makefile + '.tmp'		# temporary file name
projfile = 'do_swigall.projs'		# project dependency definition 
one_file = 'do_swigone.projs'

# ----------------------------------------------------------------------
#  Helper methods.
#

#  Verbose print.
#
def vprint(msg, level=0):
	if verbose > level:
		print(msg)

#  Create makeifle.
#
def create(fname, proj, dept):
	if os.path.exists(fname):
		#Error(prog).warn('file "%s" already exists.' % fname)
		return

	#  Generate makefile body.
	flag = ' -v' if verbose else ''
	cmnd = '%s%s' % (createmkf, flag)
	args = '%s %s %s' % (fname, proj, dept)
	#print('create_mkf.py%s %s' % (flag, args))
	proc.execute('%s %s' % (cmnd, args), shell=True)
	proc.wait()

#  Do the job for one project.
#
def do_process(proj, dept):
	# proj:	    Project name.
	# dept:	    Dependent projects.

	#  Option '-d': Delete makefile.
	if options.delete:
		if os.path.exists(makefile):
			vprint('    *** %s: removing "%s"' % (proj, makefile))
			f_op.rm(makefile)
		if os.path.exists(one_file):
			vprint('    *** %s: removing "%s"' % (proj, one_file))
			f_op.rm(one_file)
		int_file = '%s.i' % proj
		if os.path.exists(int_file):
			vprint('    *** %s: removing "%s"' % (proj, int_file))
			f_op.rm(int_file)
		stb_file = '%sStub.cpp' % proj
		if os.path.exists(stb_file):
			vprint('    *** %s: removing "%s"' % (proj, stb_file))
			f_op.rm(stb_file)
		hpp_file = '%sDecl.hpp' % proj
		if os.path.exists(hpp_file):
			vprint('    *** %s: removing "%s"' % (proj, hpp_file))
			f_op.rm(hpp_file)

	#  Option '-c': Create makefile.
	if options.create:
		print('    *** %s: creating "%s"' % (proj, makefile))
		create(makefile, proj, dept)

	#  Option '-t': Create temporary makefile.
	if options.maketmp:
		print('    *** %s: creating "%s"' % (proj, tempfile))
		create(tempfile, proj, dept)

	#  Option '-r': Rename temporary makefile to makefile.
	if options.rename:
		print('    *** %s: renaming "%s -> %s"' % (proj, tempfile, makefile))
		if not os.path.exists(tempfile):
			if os.path.exists(makefile):
				# OK, nothing to do.
				return
		f_op.mv(tempfile, makefile)

# ---------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options]'
parser = OptionParser(usage = usage)
parser.add_option('-A', '--all',
			dest='all', action='store_true', default=False,
			help='repeat for all projects')
parser.add_option('-d', '--delete',
			dest='delete', action='store_true', default=False,
			help='delete makefile')
parser.add_option('-c', '--create',
			dest='create', action='store_true', default=False,
			help='create makefile')
parser.add_option('-r', '--rename',
			dest='rename', action='store_true', default=False,
			help='rename temporary-makefile to makefile')
parser.add_option('-t', '--maketmp',
			dest='maketmp', action='store_true', default=False,
			help='create temporary-makefile')
parser.add_option('-D', '--debug',
			dest='debug', action='store_true', default=False,
			help='for debug')
parser.add_option('-P', '--python',
                        dest='python', action='store', default='python',
                        help='python command name')
parser.add_option('-v', '--verbose',
			dest='verbose', action='count', default=0,
			help='set verbose count')
parser.add_option('-V', '--version',
			dest='version', action='store_true', default=False,
			help='show version')

# ----------------------------------------------------------------------
#  Process for command line
#
(options, args) = parser.parse_args()
if options.version:
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
debug_projs = ''
if len(args) > 0:
	if options.debug:
		debug_projs = list(map(lambda x: x.lower(), args))
	else:
		parser.error("incorrect number of arguments")

verbose = options.verbose
flags = []
if options.all:	    flags.append('-A')
if options.delete:  flags.append('-d')
if options.create:  flags.append('-c')
if options.maketmp: flags.append('-t')
if options.rename:  flags.append('-r')

# ----------------------------------------------------------------------
#  Directories
#	Relative path cause problem when using CMake build environment.
#	Because finding path is done under CMake build environment; e.g.
#	src/build/RunSwig; but script is actually executed at original
#	file tree; src/RunSwig etc.
#

from FindSprPath import *
spr_path = FindSprPath(prog)
sprtop = spr_path.abspath()
##bindir = spr_path.relpath('bin')
##srcdir = spr_path.relpath('src')
bindir = spr_path.abspath('bin')
srcdir = spr_path.abspath('src')
etcdir = '%s/%s' % (srcdir, 'RunSwig')
runswigdir = '%s/%s' % (srcdir, 'RunSwig')

if verbose:
	print('  sprtop:    %s' % sprtop)
	print('  srcdir:    %s' % srcdir)
	print('  bindir:    %s' % bindir)
	print('  makefile:  %s' % makefile)
	print('  tempfile:  %s' % tempfile)
	print('  projfile:  %s' % projfile)
	print('  one_file:  %s' % one_file)
	print('  flags:     %s' % ' '.join(flags))
	if options.debug:
		print('  projs (for debug) -> %s' % debug_projs)
	print()

# ----------------------------------------------------------------------
#  Scripts
#
if options.python:
	python = options.python
createmkf = '%s %s/create_mkf.py -P %s' % (python, runswigdir, python)

# ----------------------------------------------------------------------
#  Main process
# ----------------------------------------------------------------------

#  Read project dependency definition file.
#
fio = TextFio("%s/%s" % (etcdir, projfile))
if fio.open() < 0:
	Error(prog).error(fio.error())
lines = fio.read()
fio.close()

#  Remove cmake generated dummy files.
#
projs = []
for line in lines:
	proj = line.split()
	if (len(proj) > 0):
		projs.append(proj[0])
cwd = os.getcwd()
for proj in projs:
	os.chdir('../%s' % proj)
	fio = TextFio('RunSwig_gen_files.txt')
	if fio.open() < 0:
		Error(prog).error(fio.error())
	tmp_lines = fio.read()
	fio.close()
	for line in tmp_lines:
		for fname in line.split():
			if os.path.exists(fname) and os.path.getsize(fname) == 0:
				msg = '.. removing %s (cmake generated dummy file)'
				print(msg % fname)
				os.remove(fname)
	os.chdir(cwd)

#  Do the job.
#
curr_proj = os.getcwd().split(os.sep)[-1].lower()
for line in lines:
	vprint('Def: [%s]' % line, 1)
	fields = line.split()
	if len(fields) < 1:
		continue
	proj = fields[0]
	dept = fields[1] if len(fields) > 1 else None
	vprint('proj: %s <- %s' % (proj, dept), 1)

	#  Change to target directory.
	cwd = os.getcwd()
	target_dir = '%s/%s' % (srcdir, proj)
	if options.debug:
		if proj.lower() in debug_projs:
			curr_proj = proj.lower()
	else:
		os.chdir(target_dir)

	if options.all or proj.lower() == curr_proj:
		#  Create dependencies definition file for one project.
		if options.create or options.maketmp:
			fio = TextFio(one_file, 'w')
			if fio.open() < 0:
				Error(prog).error(fio.error())
			line = '%s %s' % (proj, dept)
			vprint('creating one file: "%s" [%s]' % (one_file, line))
			fio.writelines([line])
			fio.close()
		#  Do process.
		if trace:
			print('calling do_process: %s' % ' '.join(flags))
		do_process(proj, dept)

	#  Return to original directory.
	if not options.debug:
		os.chdir(cwd)

if trace:
	print('LEAVE: %s' % prog)
	sys.stdout.flush()
sys.exit(0)

# end: make_manager.py

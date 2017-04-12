#!/usr/bin/env python
# -*- coding: utf-8 -*-
# ==============================================================================
#  FILE:
#	make_manager.py
#
#  SYNOPSIS:
#	python make_manager [-A] [-d] [-c] [-t] [-r]
#
#	対象とするプロジェクトに関する引数：
#	    -A	  対象となるすべてのプロジェクトについて, 以降の引数で示された
#		  処理を実行する.
#
#	個別のプロジェクトの処理に関する引数：
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
#	Ver 1.0	 2017/04/12 F.Kanehori	Windows batch file から移植.
# ==============================================================================
version = 1.0

import sys
import os
import glob
from optparse import OptionParser

sys.path.append('../../bin/test')
from TextFio import *
from Util import *
from Error import *

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split('\\')[-1].split('.')[0]
python_version = 34

# ----------------------------------------------------------------------
#  Globals
#
E = Error(prog)
unix = Util.is_unix()
path_sep = '/' if unix else '\\'

# ----------------------------------------------------------------------
#  Directories
#
spr2top = '../..'
srcdir = '%s/%s' % (spr2top, 'src')
bindir = '%s/%s' % (spr2top, 'bin')
pythondir = '%s/Python%s' % (bindir, python_version)
etcdir = '%s/%s' % (srcdir, 'RunSwig')
runswigdir = '%s/%s' % (srcdir, 'RunSwig')

# ----------------------------------------------------------------------
#  Scripts
#
pythonexe = 'python%s' % (python_version if Util.is_unix() else '')
python = '%s/%s' % (pythondir, pythonexe)
createmkf = '%s %s/create_mkf.py' % (python, runswigdir)

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

#  Create makeifle.
#
def create(fname, proj, dept):
	if os.path.exists(fname):
		E.print('file "%s" already exists.' % fname, prompt='Warning')
		return

	#  Generate makefile body.
	flag = ' -v' if verbose else ''
	cmnd = '%s%s' % (createmkf, flag)
	args = '%s %s %s' % (fname, proj, dept)
	Util.exec('%s %s' % (cmnd, args), shell=True, verbose=0)

#  Do the job for one project.
#
def do_process(proj, dept):
	# proj:	    Project name.
	# dept:	    Dependent projects.
	print()
	print('    *** %s ***' % proj)

	#  Option '-d': Delete makefile.
	if options.delete:
		if os.path.exists(makefile):
			if verbose:
				print('removing "%s"' % makefile)
			Util.rm(makefile)

	#  Option '-c': Create makefile.
	if options.create:
		if verbose:
			print('creating "%s"' % makefile)
		create(makefile, proj, dept)

	#  Option '-t': Create temporary makefile.
	if options.maketmp:
		if verbose:
			print('creating "%s"' % tempfile)
		create(tempfile, proj, dept)

	#  Option '-r': Rename temporary makefile to makefile.
	if options.rename:
		if verbose:
			print('renaming "%s" -> "%s"' % (tempfile, makefile))
		Util.mv(tempfile, makefile)

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
if len(args) > 0:
	if options.debug:
		debug_projs = list(map(lambda x: x.lower(), args))
	else:
		parser.error("incorrect number of arguments")

verbose = options.verbose
if verbose:
	print('  spr2top:   %s' % spr2top)
	print('  srcdir:    %s' % srcdir)
	print('  bindir:    %s' % bindir)
	print('  pythondir: %s' % pythondir)
	print('  makefile:  %s' % makefile)
	print('  tempfile:  %s' % tempfile)
	print('  projfile:  %s' % projfile)
	print('  one_file:  %s' % one_file)
	flags = []
	if options.all:	    flags.append('-A')
	if options.delete:  flags.append('-d')
	if options.create:  flags.append('-c')
	if options.maketmp: flags.append('-t')
	if options.rename:  flags.append('-r')
	print('  flags:     %s' % ' '.join(flags))
	print('  python:    %s/%s' % (pythondir, python))
	if options.debug:
		print('  projs (for debug) -> %s' % debug_projs)
	print()

# ----------------------------------------------------------------------
#  Main process
# ----------------------------------------------------------------------

#  Read project dependency definition file.
#
fio = TextFio('%s/%s' % (etcdir, projfile))
if fio.open() < 0:
	E.print(fio.error())
lines = fio.read()
fio.close()

#  Do the job.
#
curr_proj = os.getcwd().split(path_sep)[-1].lower()
for line in lines:
	if verbose > 1:
		print('Def: [%s]' % line)
	fields = line.split()
	proj = fields[0]
	dept = fields[1] if len(fields) > 1 else None
	if verbose > 1:
		print('proj: %s <- %s' % (proj, dept))

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
				E.print(fio.error())
			line = '%s %s' % (proj, dept)
			if verbose:
				print('creating one file: "%s" [%s]' % (one_file, line))
			fio.writelines([line])
			fio.close()
		#  Do process.
		do_process(proj, dept)

	#  Return to original directory.
	if not options.debug:
		os.chdir(cwd)

sys.exit(0)

# end: make_manager.py

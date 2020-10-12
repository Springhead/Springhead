#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	python setup.py [-f (--force) | -c (--check)]
#	options:
#	    -f:		無条件に再セットアップを実行する。
#	    -c:		セットアップファイルの検査のみを実行する。
#
#  DESCRIPTION:
#	Springhead のビルド環境をチェックする。
#	step 1:		Python のバージョンをチェックする。
#	step 2:		セットアップファイルが存在したら何もしない。
#	step 3:		必要なバイナリの存在を調べ、それらの実行パスを
#			セットアップファイル("setup.conf")に記録する。
#	step 4:		swigをmakeする。
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2020/10/08 F.Kanehori	First version.
# ======================================================================
version = 1.0

import sys
import os
import re
import glob
import codecs
import subprocess
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
progpath = sys.argv[0]
PIPE = subprocess.PIPE
DEVNULL = subprocess.DEVNULL

# ----------------------------------------------------------------------
#  Globals
#
setup_file = './setup.conf'
paths = {}

# ----------------------------------------------------------------------
#  Programs to check.
#
required = ['devenv', 'make', 'nkf']

# ----------------------------------------------------------------------
#  Helper methods
# ----------------------------------------------------------------------
#  コマンドを実行してその出力を得る
#
def execute(cmnd, timeout=None):
	proc = subprocess.Popen(cmnd, stdout=PIPE,
				      stderr=DEVNULL, shell=True)
	try:
		out, err = proc.communicate(timeout=timeout)
		status = proc.returncode
	except subprocess.TimeoutExpired:
		proc.kill()
		out, err = proc.communicate()
		status = 1
	encoding = os.device_encoding(1)
	if encoding is None:
		encoding = 'cp932'
	out = out.decode(encoding) if out else None
	return status, out

#  Change path separators.
#
def upath(path):
	return path.replace(os.sep, '/')

#  Error process.
#
def error(msg):
	sys.stderr.write('%s: Error: %s\n' % (prog, msg))
def abort(msg, exitcode=1):
	error(msg)
	sys.exit(exitcode)

#  Show usage.
#
def print_usage():
	print()
	cmnd = 'python %s --help' % progpath
	subprocess.Popen(cmnd, shell=True).wait()
	sys.exit(1)

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options] texmain'
parser = OptionParser(usage = usage)
#
parser.add_option('-c', '--check', dest='check',
			action='store_true', default=False,
			help='check setup-file contents')
parser.add_option('-f', '--force', dest='force',
			action='store_true', default=False,
			help='force rewrite setup-file')
parser.add_option('-o', '--old-version', dest='old_version',
			action='store_true', default=False,
			help='invoke python version 2.7')
parser.add_option('-v', '--verbose', dest='verbose',
			action='count', default=0,
			help='set verbose mode')
parser.add_option('-V', '--version', dest='version',
			action='store_true', default=False,
			help='show version')
#
(options, args) = parser.parse_args()
if options.version:
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
#
check = options.check
force = options.force
verbose = options.verbose
#
if len(args) != 0:
	error('incorrect number of arguments')
	print_usage()
#
if verbose:
	if (check): print('option -c')
	if (force): print('option -f')

# ----------------------------------------------------------------------
#  step 1
#	Python のバージョンをチェックする。
#	python のメジャーバージョンは 3 以上でなければならない。
#
(major, minor, micro, release, serial) = sys.version_info
print('python version: %s.%s.%s' % (major, minor, micro))
if major < 3:
	abort('python version 3 or greater is required.')
cmnd = 'where python'
stat, out = execute(cmnd)
if stat != 0:
	abort("can't get python's path")
out = out.split()[0]
if verbose:
	print('python path: %s' %  out.replace(os.sep, '/'))
paths['python'] = out

# ----------------------------------------------------------------------
#  step 2
#	セットアップファイルが存在したら何もしない。
#
if os.path.exists(setup_file) and not force:
	print('setup file ("%s") exists.' % upath(setup_file))
	print('done')
	sys.exit()

# ----------------------------------------------------------------------
#  step 3
#	必要なバイナリの存在を調べ、それらの実行パスをセットアップ
#	ファイル("setup.conf")に記録する。
#
for prog in required:
	cmnd = 'where %s' % prog
	stat, out = execute(cmnd)
	if stat != 0:
		if verbose:
			print('    %s: NOT FOUND' % prog)
		paths[prog] = 'NOT FOUND'
		continue
	out = out.split()[0]
	if verbose:
		print('%s path: %s' %  (prog, out.replace(os.sep, '/')))
	paths[prog] = out

print('found paths:')
tmplist = required
tmplist.insert(0, 'python')
for prog in tmplist:
	print('    %-6s -> %s' % (prog, upath(paths[prog])))

# ----------------------------------------------------------------------
#  step 4
#	swigをmakeする。
#




#  終了
#
print('done')
sys.exit(0)

# end: SlnConv.py

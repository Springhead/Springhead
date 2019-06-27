#!/python
#####!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	python GUID_check.py [options]
#	options
#	    -b dir	Sprcify build(working) directory name.
#	    -t dir	Specify SPRTOP directory.
#
#  DESCRIPTION:
#	Springhead.sln に記載されている GUID の整合性をチェックする。
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2019/06/20 F.Kanehori	First version.
# ======================================================================
version = 1.0

import sys
import os
import re
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]

# ----------------------------------------------------------------------
#  Helper methods
#
def grep(patterns, fname):
	lines = read_file(fname)
	if lines is None:
		return None
	#
	for line in lines:
		match = False
		for patt in patterns:
			m = re.search(patt, line)
			if m:
				match = True
				break
		if match:
			return m.group(1)
	return None

def read_file(fname, encoding='utf-8'):
	try:
		f = open(fname, 'r', encoding=encoding)
	except Exception:
		print('Error: file open error: "%s"' % fname)
		return None
	try:
		lines = f.readlines()
	except Exception as e:
		print('Error: %s: read error: (%s)' % (fname, e))
		return None
	f.close()
	return lines

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options]'
parser = OptionParser(usage = usage)
#
parser.add_option('-b', '--blddir', dest='blddir',
			action='store', default='build',
			help='build (working) directory name [default: %default]',
			metavar='DIR')
parser.add_option('-p', '--projs', dest='projs',
			action='append', default=None,
			help='projects to be checked [default: all]',
			metavar='PROJS')
parser.add_option('-s', '--slnname', dest='slnname',
			action='store', default='Springhead',
			help='solution name [default: %default]',
			metavar='SOLUTION')
parser.add_option('-t', '--topdir', dest='topdir',
			action='store', default=None,
			help='Springhead top directory [default: %CD%]',
			metavar='DIR')
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
slnname = options.slnname
projs = options.projs
topdir = options.topdir
blddir = options.blddir
verbose = options.verbose

if projs is None:
	projs = ['Base', 'Collision', 'Creature', 'FileIO', 'Foundation',
		 'Framework', 'Graphics', 'HumanInterface', 'Physics', 'RunSwig']
if topdir == None:
	topdir = os.getcwd().replace(os.sep, '/')

topdir = topdir.replace(os.sep, '/')
blddir = blddir.replace(os.sep, '/')
slndir = '%s/%s' % (topdir, blddir)

if verbose:
	print('topdir:  %s' % topdir)
	print('blddir:  %s' % blddir)
	print('slndir:  %s' % slndir)
	print('slnname: %s' % slnname)
	print('projs:   %s' % projs)

# ----------------------------------------------------------------------
#  Main program
#
cwd = os.getcwd()
if not os.path.exists(slndir):
	print('Error: No such directory ("%s")' % slndir)
	sys.exit(1)

for proj in projs:
	patt = ['Project\("{.+}"\) = "%s",.*, "{(.+)}"' % proj]
	sln_id = grep(patt, '%s/%s.sln' % (slndir, slnname))
	if sln_id is None:
		#print('Error: grep: %s.sln failed' % slnname)
		#sys.exit(1)
		continue
	#print('%16s: %s' % (proj, sln_id))

	os.chdir('%s/%s' % (slndir, proj))
	patt = ['<ProjectGuid>{(.+)}</ProjectGuid>']
	vcx_id = grep(patt, '%s.vcxproj' % proj)
	if vcx_id is None:
		print('Error: grep: %s.vcxproj failed' % proj)
		sys.exit(1)
	#print('%16s: %s' % (proj, vcx_id))
	os.chdir(cwd)

	print('%16s: %s - %s %s' % (proj, sln_id, vcx_id, sln_id == vcx_id))

sys.exit(0)

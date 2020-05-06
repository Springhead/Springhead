#!/usr/bin/env python
# -*- coding: utf-8 -*-
# ======================================================================
#  FILE:
#	log_append.py
#
#  DESCRIPTION:
#	Append lines from stdin to given file.
#
#  VERSION:
#	Ver 1.0  2016/06/20 F.Kanehori	First release version
#	Ver 1.01 2017/08/07 F.Kanehori	Use os.sep instead of '\\'.
# ======================================================================
version = 1.01
import sys
import os
from optparse import OptionParser

sys.path.append('../../bin/test')
from TextFio import *
from Error import *

# ----------------------------------------------------------------------
#  Constants and globaly used variables
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
E = Error(prog)

# ----------------------------------------------------------------------
#  Process for command line
#
usage = 'Usage: %prog outfile'
parser = OptionParser(usage = usage)
parser.add_option('-v', '--verbose',
			dest='verbose', action='count', default=0,
			help='set verbose mode')
parser.add_option('-V', '--version',
			dest='version', action='store_true', default=False,
			help='show version')
(options, args) = parser.parse_args()
#
if options.version:
	E.print('Version %s' % version, prompt=None)
	sys.exit(0)
#
if len(args) != 1:
	E.print("incorrect number of arguments")

# output file name
outfile = args[0]

# ----------------------------------------------------------------------
#  Open output file
#
f = TextFio(outfile, 'a')
if f.open() < 0:
	E.print(f.error())
	
# ----------------------------------------------------------------------
#  Copy from stdin to outfile
#
while True:
	line = sys.stdin.read()
	if line == '':
		break
	f.writeline(line)
f.close()

sys.exit(0)

# dnd: log_append.py

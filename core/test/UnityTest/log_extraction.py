#!/usr/local/bin/python
# ==============================================================================
#  SYNOPSIS:
#       log_extraction [options]
#           
#  DESCRIPTION:
#	Extract compiling warning/error log lines from unity build log file.
#
#  VERSION:
#       Ver 1.0  2016/06/20 F.Kanehori  First version
#       Ver 1.01 2016/06/22 F.Kanehori  Bug fixed: version print
#	Ver 1.02 2017/08/07 F.Kanehori	Use os.sep instead of '\\'.
# ==============================================================================
version = 1.02
import os
import sys
from optparse import OptionParser
import re

#-------------------------------------------------------------------------------
def verbose(msg, level=0):
	if options.verbose > level:
		sys.stderr.write(msg + '\n')

def error(msg):
	sys.stderr.write('%s: %s\n' % (script, msg))

def is_new(str, list):
	for s in list:
		if s == str:
			return False
	list.append(str)
	return True

#-------------------------------------------------------------------------------
# Options
#
usage = "Usage: %prog [options] file"
parser = OptionParser(usage = usage)
parser.add_option('-v', '--verbose', action="count", dest='verbose',
                    default=0, help='set verbose mode')
parser.add_option('-V', '--version', action="store_true", dest='version',
                    help='show version')
(options, args) = parser.parse_args()
#
if options.version:
	parser.print_help()
	sys.exit(0)
script = sys.argv[0].split(os.sep)[-1].split('.')[0]

#-------------------------------------------------------------------------------
# Patterns to search
#
str_tgt = '^-target: (.+)\.unity$'
str_bgn = '^-----CompilerOutput:'
str_end = '^-----EndCompilerOutput---------------'
#
pattern_tgt = re.compile(str_tgt)
pattern_bgn = re.compile(str_bgn)
pattern_end = re.compile(str_end)

#-------------------------------------------------------------------------------
# Main process
#
if len(args) != 1:
	parser.error("incorrect number of arguments")
	sys.exit(-1)

ifname = args[0];
already_found = []
with open(ifname, 'r') as ifile:
	numlines = 0
	count = 0
	no_more = False
	include = False
	line = ifile.readline()
	while (line):
		numlines += 1
		include_one = False
		verbose(line.rstrip(), 1)
		if not no_more:
			if pattern_tgt.match(line) is not None:
				include_one = True
			elif pattern_bgn.match(line) is not None:
				if is_new(line, already_found):
					include = True
			elif pattern_end.match(line) is not None:
				if include:
					include_one = True
				include = False
			if include or include_one:
				print('%d: %s' % (numlines, line.rstrip()))
				count += 1
		# end not no_more
		line = ifile.readline()

verbose('%s: %d/%d lines extracted' % (script, count, numlines))
sys.exit(count)

#end: replace.py

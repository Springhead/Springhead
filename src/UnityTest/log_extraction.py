#! C:/Python34/python
# ==============================================================================
#  SYNOPSIS:
#       log_extraction [options]
#           
#  DESCRIPTION:
#	Extract compiling warning/error log lines from unity build log file.
#
#  VERSION:
#       Ver 1.0  2016/06/16 F.Kanehori  First release
# ==============================================================================
version = 1.0
import os
import sys
from optparse import OptionParser
import re
#import types

#-------------------------------------------------------------------------------
def verbose(msg, level=0):
	if options.verbose > level:
		sys.stderr.write(msg + '\n')

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
script = sys.argv[0].split('\\')[-1].split('.')[0]

#-------------------------------------------------------------------------------
# Constants
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
			m = pattern_tgt.match(line)
			if m:
				include_one = True
				no_more = True
			m = pattern_bgn.match(line)
			if m:
				include = True
			m = pattern_end.match(line)
			if m:
				include = False
				include_one = True
			if include or include_one:
				print(line.rstrip())
				count += 1
		# end not no_more
		line = ifile.readline()

verbose('%s: %d/%d lines extracted' % (script, count, numlines))
sys.exit(count)

#end: replace.py

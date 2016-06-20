#! C:/Python34/python
# ==============================================================================
#  SYNOPSIS:
#	replace [options] infile outfile
#	options:
#	    -p PATTERN		specify replace pattern (from=to)
#	    -v			set verbose mode
#
#  DESCRIPTION:
#	Replace specified pattern(from) in the file to new pattern(to).
#	Currently:
#	    only one replace pattern can be specified
#	    only one pattern per one line will be replaced
#	    replacement will be applied to all lines in the file
#
#  VERSION:
#       Ver 1.0  2016/06/16 F.Kanehori	First version
# ==============================================================================
version = 1.0
import os
import sys
from optparse import OptionParser
import types

#-------------------------------------------------------------------------------
# Constants
#
encoding = 'utf-8'

#-------------------------------------------------------------------------------
# Options
#
usage = "Usage: %prog [options] infile outfile"
parser = OptionParser(usage = usage)
parser.add_option('-p', '--pattern', action="store", dest='pattern',
                    help='replace pattern (from=to)')
parser.add_option('-v', '--verbose', action="count", dest='verbose',
                    default=0, help='set verbose mode')
parser.add_option('-V', '--version', action="store_true", dest='version',
                    help='show version')
(options, args) = parser.parse_args()
#
script = sys.argv[0].split('\\')[-1].split('.')[0]
verbose = options.verbose
if options.version:
	print('%s: Version %s' % (script, version))
	sys.exit(0)

#-------------------------------------------------------------------------------
def replace(ifname, ofname):
	with open(ofname, 'w') as ofile:
		with open(ifname, 'r') as ifile:
			line = ifile.readline()
			count = 1
			while (line):
				replaced = line.replace(pat_fm, pat_to)
				ofile.write(replaced)
				if verbose > 0 and replaced != line:
					print(fixed(count, 4) + line.rstrip())
					print("   => " + replaced.rstrip())
				elif verbose > 1:
					print(fixed(count, 4) + replaced.rstrip())
				line = ifile.readline()
				count += 1
	return 0

def fixed(num, width):
	string = str(num)
	length = len(string)
	if length < width:
		return ' ' * (width-length) + string + ': '
	return string + ': '
	
#-------------------------------------------------------------------------------
# Main process
#
status = 0
if len(args) != 2:
	parser.error("incorrect number of arguments")
	sys.exit(-1)
if not isinstance(options.pattern, str):
	parser.error('pattern not specified')
	sys.exit(-1)

pattern = options.pattern.split('=')
pat_fm = pattern[0]
pat_to = pattern[1]

ifname = args[0];
ofname = args[1];
if verbose > 0:
	sys.stderr.write(script + ':\n')
	sys.stderr.write('    ifile: ' + ifname + '\n')
	sys.stderr.write('    ofile: ' + ofname + '\n')
	sys.stderr.write('    pattern from: [' + pat_fm + ']\n') 
	sys.stderr.write('    pattern to:   [' + pat_to + ']\n') 

status = replace(ifname, ofname)
sys.exit(status)

#end: replace.py

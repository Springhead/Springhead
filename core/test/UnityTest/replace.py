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
#       Ver 2.0  2016/06/23 F.Kanehori	Command syntax changed
#					Multiple pattern replace implemented
#       Ver 2.0a 2016/07/07 F.Kanehori	Help message revised
# ==============================================================================
version = '2.0'
import os
import sys
from optparse import OptionParser
import types

# ------------------------------------------------------------------------------
def replace(ifname, ofname, patterns, sep):
	pat = []
	for pattern in patterns:
		p = pattern.split(options.sep)
		pat.append({'fm': p[0], 'to': p[1]})
	#
	with open(ofname, 'w') as ofile:
		with open(ifname, 'r') as ifile:
			line = ifile.readline()
			count = 1
			while (line):
				replaced = line
				for p in pat:
					fm = p['fm']
					to = p['to']
					replaced = replaced.replace(fm, to)
				ofile.write(replaced)
				if verbose > 0 and replaced != line:
					print(fixed(count, 4) + line.rstrip())
					print("   => " + replaced.rstrip())
				elif options.verbose > 1:
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

# ------------------------------------------------------------------------------
#  Process for command line
#
usage = "Usage: %prog [options] infile outfile from=to [from=to].."
parser = OptionParser(usage = usage)
parser.add_option('-s', '--separator', action="store", dest='sep', default='=',
                    help='pattern separator [default: %default]')
parser.add_option('-v', '--verbose', action="count", dest='verbose',
                    default=0, help='set verbose mode')
parser.add_option('-V', '--version', action="store_true", dest='version',
                    help='show version')
(options, args) = parser.parse_args()
verbose = options.verbose
#
script = sys.argv[0].split('\\')[-1].split('.')[0]
if options.version:
	print('%s: Version %s' % (script, version))
	sys.exit(0)
#
if len(args) < 3:
	parser.error("incorrect number of arguments")
	sys.exit(-1)
ifname = args[0];
ofname = args[1];
patterns = args[2:]
if verbose:
	print(script + ':')
	print('    ifile: ' + ifname)
	print('    ofile: ' + ofname)
	for pattern in patterns:
		if not options.sep in pattern:
			parser.error('invalid pattern specified [' + pattern + ']')
		p = pattern.split(options.sep)
		print('    pattern from: [' + p[0] + ']') 
		print('    pattern to:   [' + p[1] + ']') 
		
# ------------------------------------------------------------------------------
#  Main process
#
status = replace(ifname, ofname, patterns, options.sep)
sys.exit(status)

#end: replace.py

#!/usr/local/bin/python
# ==============================================================================
#  SYNOPSIS:
#	replace [options] infile from=to [from=to]..
#	options:
#	    -o outfile		Output file name (default: stdout).
#	    -s separator	Delimiter of the pattern (default: '=').
#	    -v			Set verbose mode.
#
#  DESCRIPTION:
#	Find the pattern <from> in the file and replace them to <to>.
#
#  VERSION:
#       Ver 1.0  2016/06/16 F.Kanehori	First version
#       Ver 2.0  2016/06/23 F.Kanehori	Command syntax changed
#					Multiple pattern replace implemented
#       Ver 2.1	 2019/02/26 F.Kanehori	Move 'outfile' arg to option.
# ==============================================================================
version = '2.1'

import os
import sys
from optparse import OptionParser
import types

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].replace(os.sep, '/').split('/')[-1].split('.')[0]

# ----------------------------------------------------------------------
#  Methods
#
def replace(infile, outfile, patterns, sep):
	pat = []
	for pattern in patterns:
		p = pattern.split(options.sep)
		pat.append({'fm': p[0], 'to': p[1]})
	#
	try:
		ofile = sys.stdout if outfile == '-' else open(outfile, 'w') 
	except OSError:
		print("%s: Error: can't open file %s" % (prog, outfile))
		return -1

	with open(infile, 'r') as ifile:
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
	if outfile == '-':
		ofile.close()
	return 0

def fixed(num, width):
	string = str(num)
	length = len(string)
	if length < width:
		return ' ' * (width-length) + string + ': '
	return string + ': '

# ------------------------------------------------------------------------------
#  Options
#
usage = "Usage: %prog [options] infile from=to [from=to].."
parser = OptionParser(usage = usage)
parser.add_option('-o', '--outfile', dest='outfile',
		    action="store", default='-',
                    help='output file name [default: %default]')
parser.add_option('-s', '--separator', dest='sep',
		    action="store", default='=',
                    help='pattern separator [default: %default]')
parser.add_option('-v', '--verbose', dest='verbose',
                    action="count", default=0,
		    help='set verbose mode')
parser.add_option('-V', '--version', dest='version',
		    action="store_true", default=False,
                    help='show version')

# ------------------------------------------------------------------------------
#  Process for command line
#
(options, args) = parser.parse_args()
if options.version:
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
if len(args) < 2:
	parser.error("incorrect number of arguments")
	sys.exit(-1)

# get options and input file name
infile = args[0];
patterns = args[1:]
outfile = options.outfile
verbose = options.verbose

if verbose:
	print('%s:' % prog)
	print('    ifile: ' + infile)
	print('    ofile: ' + outfile)
	for pattern in patterns:
		if not options.sep in pattern:
			parser.error('invalid pattern specified [' + pattern + ']')
		p = pattern.split(options.sep)
		print('    pattern from: [' + p[0] + ']') 
		print('    pattern to:   [' + p[1] + ']') 
		
# ------------------------------------------------------------------------------
#  Main process
#
status = replace(infile, outfile, patterns, options.sep)
sys.exit(status)

#end: replace.py

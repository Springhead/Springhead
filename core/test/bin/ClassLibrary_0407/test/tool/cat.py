import sys
import os
from optparse import OptionParser

usage = 'Usage: %prog [options]'
parser = OptionParser(usage = usage)
parser.add_option('-n', '--number', action='store_true',
			dest='number', default=False,
			help='add line number (starting from 1)')
(options, args) = parser.parse_args()

argc = len(args)
if argc > 1:
	#parser.error('too many file names')
	parser.error('Sorry, file input is not implemented yet')

count = 0
while True:
	try:
		line = input()
	except:
		break
	count += 1
	if options.number:
		print('%6d %s' % (count, line))
	else:
		print(line)

sys.exit()

#!/usr/bin/env python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	MakeReport [-o csvfile] logfile
#	    csvfile:	Output(csv) file name.
#	    logfile:	Input log file name.
#
#  DESCRIPTION:
#	Make test report file as csv format.  Test report must be
#	created by 'BatchTest' or 'BatchTestControl' script.
#	Report file can be viewed by 'Excel'.
#
#  VERSION:
#	Ver 1.0  2016/11/17 F.Kanehori	First release version.
#	Ver 2.0  2017/09/21 F.Kanehori	Log file format changed.
#	Ver 2.1  2017/09/28 F.Kanehori	Add separator for tests/Samples.
#	Ver 2.2  2017/11/16 F.Kanehori	Python library path の変更.
#	Ver 2.3  2017/11/30 F.Kanehori	Python library path の変更.
# ======================================================================
version = '2.3'

import sys
import os
import re
import copy
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
#
PROJ = 0	# = [ PRJ1, PRJ2, PRJ3 ]
USE_CS = 1	# = [ True, False ]
EXPD = 2	# = [ W32, X64 ]
TEST = 3	# = [ BLD=[...], RUN=[...] ]
#
PRJ1 = 0	# for PROJ (project info): directory hierarchy: upper
PRJ2 = 1	# for PROJ (project info): directory hierarchy: middle
PRJ3 = 2	# for PROJ (project info): directory hierarchy: lower
W32  = 0	# for platform: Win32
X64  = 1	# for platform: x64
BLD  = 0	# for execution stage: build
RUN  = 1	# for execution stage: run

# ----------------------------------------------------------------------
#  Import Springhead python library.
#
sys.path.append('../../src/RunSwig')
from FindSprPath import *
spr_path = FindSprPath(prog)
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from TextFio import *

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options] logfile'
parser = OptionParser(usage = usage)
parser.add_option('-o', '--output',
			dest='outfile', default=None,
			help='output file path', metavar='FILE')
parser.add_option('-v', '--verbose',
			dest='verbose', action='count', default=0,
			help='set verbose mode')
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
if len(args) != 1:
	parser.error("incorrect number of arguments")

# get options and input file name
outfile = options.outfile if options.outfile else sys.stdout
verbose = options.verbose
logfile = args[0]

if verbose:
	outname = outfile if outfile != sys.stdout else 'stdout'
	print('logfile: %s' % logfile)
	print('outfile: %s' % outname)

# ----------------------------------------------------------------------
#  Read log data.
#
f = TextFio(logfile)
if f.open() < 0:
	print('%s: %s' % (prog, f.error()))
	sys.exit(-1)
f.read()
lines = f.lineinfo()
f.close()

# ----------------------------------------------------------------------
#  Abbreviations for run status.
#
def abbrev_sub(string, find_str):
	return string.find(find_str) >= 0

def abbrev(stat):
	if abbrev_sub(stat, 'Need Intervention'):   return '**NI**'
	if abbrev_sub(stat, 'Assertion faild'):	    return 'Assert'
	if abbrev_sub(stat, 'interrunpted:'):	    return 'INTR'
	if abbrev_sub(stat, 'Access Violation'):    return 'AccV'
	if abbrev_sub(stat, 'SEH:'):		    return '*SEH*'
	return stat

# ----------------------------------------------------------------------
#  Set correct field by configuration.
#
def reflect_configs(data, configs):
	refd = {'Debug': ' ', 'Release': ' ', 'Trace': ' '}
	for  n in range(len(configs)):
		key = configs[n]
		refd[key] = data[n]
	return [refd['Debug'], refd['Release'], refd['Trace']]

# ----------------------------------------------------------------------
#  Analyze data lines.
#
patt = 'VS (\d+), Revision (\d+), ClosedSrc ([^\s]+), Configs (.+)'
m = re.match(patt, lines[0][1])
if m:
	toolset,revision,cs,configs \
		= m.group(1), m.group(2), m.group(3), m.group(4)
configs = configs.split(',')
#
proj = []	# for project info
expd = []	# for expected result
test = []	# for test result
result = []	# [ [proj,expd,test]... ]

for numb,data in lines[1:]:
	if verbose > 1:
		print('%3d: %s' % (numb, data))
	#
	if len(data) == 0:
		continue
	line = (numb-2) % 5	# line index within one test record
	if line == 0:
		# test path
		m = re.match('.+/src/(\w+)/(\w+)/([&\w-]+):', data)
		if m:
			proj = [m.group(1), m.group(2), m.group(3)]
		else:
			m = re.match('.+/src/(\w+)/([&\w-]+):', data)
			if m:
				proj = [m.group(1), m.group(2), '']
			else:
				msg = 'bad test path: line %d: %s' % (numb, data)
				print('%s: %s' % (prog, msg))
				sys.exit(-1)
		if verbose:
			print('  %s' % '/'.join(proj))
	#
	elif line == 1:
		# use_closed_src
		m = re.match('.+:\s(.+)', data)
		if m is None:
			msg = 'bad use_closed_src: line %d: %s' % (numb, data)
			print('%s: %s' % (prog, msg))
			sys.exit(-1)
		use_cs = m.group(1)
		if verbose > 1:
			print('  use_closed_src: %s' % use_cs)
	#
	elif line == 2:
		# expected status
		m = re.match('.+build:\s*([^,]+),\s*run:\s*(.+)', data)
		if m is None:
			msg = 'bad expected status: line %d: %s' % (numb, data)
			print('%s: %s' % (prog, msg))
			sys.exit(-1)
		expd = [m.group(1), m.group(2)]
		if verbose > 1:
			print('  expected: [%s, %s]' % (expd[W32], expd[X64]))
	else:
		indx = BLD if line == 3 else RUN
		step = 'build' if indx == BLD else 'run  '
		patt = '\s*%s:\s*Win32:\s*\((.+)\):\s*x64:\s*\((.+)\)' % step
		m = re.match(patt, data)
		if m is None:
			msg = 'bad test result: line %d: %s' % (numb, data)
			print('%s: %s' % (prog, msg))
			sys.exit(-1)
		list_w32 = list(map(lambda x: x.strip(), m.group(1).split(',')))
		list_x64 = list(map(lambda x: x.strip(), m.group(2).split(',')))
		list_w32 = reflect_configs(list_w32, configs)
		list_x64 = reflect_configs(list_x64, configs)
		if indx == BLD:
			test = [[None, None], [None, None]]
		test[W32][indx] = list(map(lambda x: x.strip(), list_w32))
		test[X64][indx] = list(map(lambda x: x.strip(), list_x64))
		if verbose > 1:
			indx_s = 'BLD' if indx == BLD else 'RUN'
			print('  W32[%s]: %s,  X64[%s]: %s' %
				(indx_s, test[W32][indx], indx_s, test[X64][indx]))
	#
	if line == 4:
		r = [proj, use_cs, expd, test]
		result.append(copy.deepcopy(r))
	# end for

# ----------------------------------------------------------------------
#  Generate csv data.
#
configs = 'Debug,Release,Trace'
commas = ',' * configs.count(',')
#
obuf = []
obuf.append('Rev,%s,,,Use,x86%s,x64%s,' % (toolset, commas, commas))
obuf.append('%s,,,,ClosedSrc,%s,%s,0' % (revision, configs, configs))
#
prev = ['', '', '']
proj = ['', '', '']
dirsave = 'tests'
#
for r in result:
	# separator line between tests and Samples
	if dirsave and r[PROJ][PRJ1] != dirsave:
		tbuf = ',,,,,,,,,,,'
		obuf.append(tbuf)
		dirsave = r[PROJ][PRJ1]

	# project name
	proj = r[PROJ]
	use_cs = r[USE_CS]
	for n in range(3):
		tmp = proj[n]
		if tmp == prev[n]:
			proj[n] = ''
		prev[n] = tmp
		
	tbuf = ',%s,%s,%s,' % (proj[0], proj[1], proj[2])
	tbuf += '○,' if use_cs == 'True' else '×,'

	# test results
	test = r[TEST]
	for plat in [W32, X64]:
		#
		stat = test[plat]
		for n in range(len(stat[0])):
			bstat, rstat = stat[0][n], stat[1][n]
			if bstat == 'OK':
				rstat = abbrev(rstat)
				tbuf += '%s,' % rstat
			elif bstat == '':
				tbuf += ','
			else:
				tbuf += 'Build NG,'
	
	# expected result
	tbuf += r[EXPD][RUN]

	#
	if verbose > 1:
		print(tbuf)
	obuf.append(tbuf)

# ----------------------------------------------------------------------
#  Write out to file.
#
f = TextFio(outfile, 'w', encoding='shift_jis', verbose=verbose)
if f.open() < 0:
	print('%s: %s' % (prog, f.error()))
	sys.exit(-1)
f.set_lines(obuf)
f.writelines()
f.close()

# ----------------------------------------------------------------------
#  End of process.
#
if verbose:
	print('%s: done' % prog)
sys.exit(0)

# end: MakeReport.py

#!/usr/bin/env python
# -*- coding: utf-8 -*-
# =============================================================================
#  SYNOPSIS:
#	BatchTestControl [-t toolset] [-d testdir] [-i inctopdir]
#			 [-l logfile] [-f ctlfile] [-s section]
#			 [-c use_cs] [-u unuse_cs]
#			 [-r rebuild] [-C config] [-E erase]
#			 [-P platform] [-N no_serialize]
#			 [-T timeout] [-D dry_run] [-v verbose]
#	arguments:
#	    toolset:	Visual Stuido Version (default: '14.0').
#	    testdir:	Test directory (relative to SpringheadTop) (str).
#	    inctopdir:	True:  Test start at 'testdir'.
#			False: Test start at subdirs of 'testdir'.
#	    logfile:	Test result log file (str).
#			MakeReport script needs this log file.
#	    ctlfile:	Test control file name (str).
#	    section:	Test Control section name (str).
#	    use_cs:	Test only "UseClosedSrc is True" solutions.
#	    unuse_cs:	Test only "UseClosedSrc is False" solutions.
#	    rebuild:	Force rebuild (project only) (bool).
#	    config:	Build configuration ('Debug'|'Release'|...).
#	    erase:	Erase serialized file before test starts.
#	    platform:	Build platform ('Win32'|'x64').
#	    no_serialize:
#			No serialization on interruption.
#	    timeout:	Time out in seconds (int) (default: no time out).
#	    dry_run:	Show commands but not execute.
#	    verbose:	Verbose mode level (0: silent).
#
#  DESCRIPTION:
#	Fall back test for Springhead libraries.
#	All platforms and configurations are tested for all solutions.
#	Solutions are found under following directories:
#		$SpringheadTop/src/tests
#		$SpringheadTop/src/Samples
#
#  VERSION:
#	Ver 1.0  2016/11/16 F.Kanehori	First version.
#	Ver 1.1  2017/01/11 F.Kanehori	Use VersionControlSystem class.
#	Ver 2.0  2017/08/10 F.Kanehori	DailybuildControl -> ControlParams.
#	Ver 2.1  2017/09/14 F.Kanehori	Python library revised.
#	Ver 2.2  2017/09/27 F.Kanehori	Log file format changed.
#					Introduce serialization facility.
#	Ver 2.3  2017/11/16 F.Kanehori	Python library path ÇÃïœçX.
#	Ver 2.4  2017/11/30 F.Kanehori	Python library path ÇÃïœçX.
#	Ver 2.5  2017/12/07 F.Kanehori	Interface ÇÃå©íºÇµ.
# =============================================================================
version = '2.5'

import sys
import os
import glob
import pickle
from optparse import OptionParser
from VisualStudio import *
from SpringheadTest import *
from VersionControlSystem import *

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
max_loglines = 10

# ----------------------------------------------------------------------
#  Import Springhead python library.
#
sys.path.append('../../src/RunSwig')
from FindSprPath import *
spr_path = FindSprPath(prog)
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from FileOp import *
from TextFio import *
from Error import *
from Util import *
from Proc import *

# ----------------------------------------------------------------------
#  File names
#
serialize_fname_results = 'serialized.results'
serialize_fname_visited = 'serialized.visited'

# ----------------------------------------------------------------------
#  Log output class
#
class Log(TextFio):
	def __init__(self, prog, path):
		self.prog = prog
		self.path = path
		self.opened = False
		if path is not None:
			super().__init__(path, 'a', 'sjis')

	def open(self):
		if self.path is None:
			return 0
		if super().open() < 0:
			Error(self.prog).abort(self.error())
		self.truncate()
		self.opened = True
		return 0

	def put(self, buff):
		print(buff)
		if self.opened:
			self.writelines(buff)

	def close(self):
		if self.opened:
			super().close()
		self.opened = False

# ------------------------------------------------------
#  Get current source revision.
#
def vcs_revision():
	url = 'http://springhead.info/spr2/Springhead/trunk/'
	topdir = '../..'
	vcs = VersionControlSystem('Subversion', {'url': url})
	return vcs.revision(topdir)	# [returncode, revision, errstr]

# ------------------------------------------------------
#  Serialize/desierialize object.
#
def serialize(obj, fname):
	f = Fio(fname, 'wb')
	if f.open() < 0:
		print(f.error())
		return -1
	try:
		pickle.dump(obj, f.obj)
	except pickle.PickleError as err:
		print(err)
		f.close()
		return -1
	f.close()
	return 0

def deserialize(fname):
	f = Fio(fname, 'rb')
	if f.open() < 0:
		print(f.error())
		return None
	try:
		obj = pickle.load(f.obj)
	except pickle.PickleError as err:
		print(err)
		f.close()
		return None
	f.close()
	return obj

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options]'
parser = OptionParser(usage = usage)
parser.add_option('-t', '--toolset',
			dest='toolset', default='14.0',
			help='Visual Studio version [default: %default]')
parser.add_option('-d', '--directory',
			dest='directories', action='append', default=None,
			help='test directoy (relative to Springhead top dir)',
			metavar='DIR')
parser.add_option('-i', '--inc-topdir',
			dest='inc_topdir', action='store_true', default=False,
			help='include top directory to test [default: %default]')
parser.add_option('-f', '--control-file',
			dest='ctlfile', default='test.control',
			help='test control file [default: %default]', metavar='FILE')
parser.add_option('-s', '--control-section',
			dest='section', default=None,
			help='test control section', metavar='SECTION')
parser.add_option('-l', '--log',
			dest='log', default=None,
			help='output result to file', metavar='FILE')
parser.add_option('-c', '--use-closed-src',
			dest='use_closed_src', action='store_true', default=False,
			help='test only "UseClosedSrc is True" solutions')
parser.add_option('-u', '--unuse-closed-src',
			dest='unuse_closed_src', action='store_true', default=False,
			help='test only "UseClosedSrc is False" solutions')
parser.add_option('-r', '--force-rebuild',
			dest='force_rebuild', action='store_true', default=False,
			help='force rebuild')
parser.add_option('-C', '--config',
			dest='configs', action='append', default=None,
			help='configurations to be tested', metavar='CONFIG')
parser.add_option('-E', '--erase-serialized',
			dest='erase_serialized', action='store_true', default=False,
			help='erase serialized file before test starts')
parser.add_option('-P', '--platform',
			dest='platforms', action='append', default=None,
			help='platforms to be tested', metavar='PLATFORM')
parser.add_option('-N', '--no-serialize',
			dest='serialize', action='store_false', default=True,
			help='no serialization on interruption')
parser.add_option('-T', '--timeout',
			dest='timeout', type='int', default=0,
			help='set timeout (0 for not set) [default: %default]',
			metavar='SEC')
parser.add_option('-D', '--dry-run',
			dest='dry_run', action='store_true', default=False,
			help='set dry-run mode [default: %default]')
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
if len(args) > 0:
	parser.error("incorrect number of arguments")

# get options
toolset = options.toolset
closed_src_control = ControlParams.AUTO
csc_str = 'auto'
if options.use_closed_src:
	closed_src_control = ControlParams.USE
	csc_str = 'use'
if options.unuse_closed_src:
	closed_src_control = ControlParams.UNUSE
	csc_str = 'unuse'
force_rebuild = options.force_rebuild
dry_run = options.dry_run
verbose = options.verbose

if verbose:
	print('toolset:            %s' % toolset)
	print('closed_src_control: %s' % closed_src_control)
	print('force_rebuild:      %s' % force_rebuild)
	print('dry-run:            %s' % dry_run)
	print('serialization:      %s' % serialize)
	print('erase-serialized:   %s' % options.erase_serialized)

# ----------------------------------------------------------------------
#  Create Visual Studio object
#
vs = VisualStudio(toolset, verbose)
vs.set_dry_run(dry_run)
vs_version = vs.get('version')

# ----------------------------------------------------------------------
#  Test parameters
#
platforms = [ 'Win32', 'x64' ]
configs = [ 'Debug', 'Release', 'Trace' ]
directories = [ 'src/tests', 'src/Samples' ]

inc_topdir = options.inc_topdir
if options.configs:
	configs = options.configs
if options.platforms:
	platforms = options.platforms
if options.directories:
	directories = options.directories

if verbose:
	print('configs:        %s' % configs)
	print('platforms:      %s' % platforms)
	print('directory:      %s' % Util.upath(directories))
	print('incldue topdir: %s' % str(inc_topdir))

# ----------------------------------------------------------------------
#  Create SpringheadTest object
#
test = SpringheadTest(vs, options.ctlfile, options.section,
			closed_src_control, verbose, dry_run)
test.set_max_loglines(max_loglines)
test.set_override_control('exe', True, True)
test.set_force_rebuild(force_rebuild)
test.set_progress_report()
if options.timeout > 0:
	test.set_override_control('timeout', None, options.timeout)

# ----------------------------------------------------------------------
#  Prepare log file
#
log = Log(prog, options.log)
if log.open() < 0:
	sys.exit(-1)

# ----------------------------------------------------------------------
#  Test Go
#
cwd = os.getcwd()

# erase serialied data file if specifiled
fname_results = Util.upath('%s/%s' % (cwd, serialize_fname_results))
fname_visited = Util.upath('%s/%s' % (cwd, serialize_fname_visited))
if options.erase_serialized:
	fop = FileOp()
	fop.rm(fname_results)
	fop.rm(fname_visited)
# initialize results and visited
results = None
visited = None
if options.serialize:
	if os.path.exists(fname_results) and os.path.exists(fname_visited):
		# read from serialized files
		results = deserialize(fname_results)
		visited = deserialize(fname_visited)
		if results is None:
			Error(prog).abort('can\'t read "%s"' % fname_results)
		if visited is None:
			Error(prog).abort('can\'t read "%s"' % fname_visited)
		# remove serialized files
		print('read serialized file: "%s"' % fname_results)
		print('read serialized file: "%s"' % fname_visited)
		fop = FileOp()
		fop.rm(fname_results)
		fop.rm(fname_visited)

# test go!
results, visited = test.traverse(Util.pathconv(directories), platforms,
				configs, include_topdir=inc_topdir,
				results=results, visited=visited)

# if interrupted
if KEYINTR.is_interrupted() and options.serialize:
	# serialize objects (results and visited)
	rc1 = serialize(results, fname_results)
	rc2 = serialize(visited, fname_visited)
	if rc1 == 0:
		print('write  \'results\' to file "%s"' % fname_results)
	if rc2 == 0:
		print('write  \'visited\' to file "%s"' % fname_visited)

os.chdir(cwd)

if results is None or visited is None:
	Error(prog).abort('test failed')

# ----------------------------------------------------------------------
#  Make report
#
c_error = ControlParams.ERROR	# for short
c_build = ControlParams.BUILD
c_run   = ControlParams.RUN

# test environment
vs_name = vs.get_toolset()[1].split()[-1]
returncode, revision, err = vcs_revision()
if returncode != 0:
	Error(prog).print('can not get current revision', exitcode=0)
	Error(prog).abort(err)
environ = 'VS %s, Revision %s, ClosedSrc %s, Configs %s' \
		% (vs_name, revision, csc_str, ','.join(configs))
print('')
print('Test Environment:')
log.put('%s' % environ)

print('')
print('Test Results:')
for n in range(len(visited)):
	dir = visited[n]
	log.put('%s:' % Util.upath(dir))
	if not dir in results:
		continue
	if verbose > 1:
		print('results[c_error]: %s' % results[dir][c_error])
		print('results[c_build]: %s' % results[dir][c_build])
		print('results[c_run]:   %s' % results[dir][c_run])
	if results[dir][c_error] is not None:
		log.put('  %s' % str(results[dir][c_error]))
		continue
	if results[dir][c_build] is None or results[dir][c_run] is None:
		log.put("** pan: results has broken **")
		continue

	# return status
	step = ['error', 'build', 'run  ']
	for n in [c_build, c_run]:
		r = results[dir][n]
		report = []
		for p in platforms:
			if not p in r:
				continue
			codes = []
			for c in configs:
				if not c in r[p]:
					continue
				code_str = '%s: %s' % (c, str(r[p][c]))
				codes.append(code_str)
			# end configs
			report.append('%s: (%s)' % (p, ', '.join(codes)))
		# end platforms
		print('  %s:  %s' % (step[n], ',  '.join(report)))

	# judgement
	expected = [None, 0, int(results[dir][c_run]['expected'])]
	timeout = ' (timeout)' if expected[c_run] == Proc.ETIME else ''
	format = '  use_closed_src: %s'
	log.put(format % results[dir][c_build]['use_closed_src'])
	format = '  expected status: build: %d, run: %d%s'
	log.put(format % (expected[c_build], expected[c_run], timeout))
	for n in [c_build, c_run]:
		r = results[dir][n]
		if expected[n] is None:
			expected[n] = 0
		report = []
		for p in platforms:
			if not p in r:
				continue
			judges = []
			for c in configs:
				if not c in r[p]:
					continue
				judge = 'OK' if r[p][c] == expected[n] else 'NG'
				if expected[n] == 0 and r[p][c] == Proc.ETIME:
					judge = 'timeout'
				elif r[p][c] == Proc.ENEEDHELP:
					judge = '** Need Intervention **'
				elif r[p][c] == Proc.ECANCELED:
					judge = '^C'
				elif SEH.is_seh(r[p][c]):
					judge = SEH.seh_str(r[p][c])
				elif r[p][c] == Proc.EINTR:
					judge = 'interrupted'
				elif r[p][c] == 3:
					judge = 'Assertion faild (maybe)'
				elif r[p][c] is None:
					judge = '--'
				judges.append(judge)
			# end configs
			report.append('%s: (%s)' % (p, ', '.join(judges)))
		# end platforms
		log.put('  %s:  %s' % (step[n], ':  '.join(report)))

log.close()
sys.exit(0)

# end: BatchTestControl.py

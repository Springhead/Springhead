#!/usr/bin/env python
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:
#	ControlParams
#	    Test control parameter manager class.
#	    Should be called from where a solution file exists.
#
#  INITIALIZER:
#	obj = ControlParams(fname, section=None, verbose=0)
#	arguments:
#	    fname:	Test control file name (str).
#	    section:	Test control section name (str).
#	    verbose:	Verbose level (0: silent) (int).
#
#  METHODS:
#	set(ControlParams.PLATFORM, str)
#	set(ControlParams.CONFIG, str)
#	set(ControlParams.USE_CLOSED_SRC_FLAG, str)
#	set(ControlParams.RECOMPILE, bool)
#	set(ControlParams.LOG_OUTPUT, bool, bool)	# (build, run)
#	set(ControlParams.DRYRUN, bool)
#	    Set or override the control parameter.
#	    PLATFORM:	    Set build option (platform).
#	    CONFIG:	    Set build option (config).
#	    USE_CLOSED_SRC_FLAG:
#			    Set usage of closed source files.
#	    RECOMPILE:	    Force recompile by removing binary files.
#	    LOG_OUTPUT:	    Generate log file or not.
#	    DRYRUN:	    Set/unset dry run flag.
#
#	value = get(key)
#	    Get value associated with the key.
#	    arguments:
#		key:	    See below for aveilable keys defined (const).
#	    returns:	    Value associated with the key (obj).
#
#	msg = has_error()
#	    returns:	    Error message if error has occurred so far.
#			    None if recorded no error.
#
#	info()
#	    Print information of this instance.
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2016/10/05 F.Kanehori	First version.
#	Ver 2.0  2017/08/10 F.Kanehori	Name changed: script, param file.
#	Ver 3.0  2017/09/13 F.Kanehori	Python library revised.
#	Ver 4.0  2018/01/11 F.Kanehori	‘S‘Ì‚ÌŒ©’¼‚µ.
# ======================================================================
import sys
import os

# local python library
#
sys.path.append('../../src/RunSwig')
from FindSprPath import *
spr_path = FindSprPath('ControlParams')
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from Error import *
from Util import *
from FileOp import *
from KvFile import *

class ControlParams:

	#  Keywords
	#
	EXCLUDE		    = 1
	DECEND		    = 2
	ALIAS		    = 3
	BUILD		    = 11
	USE_CLOSE_SRC	    = 12
	CPPMACRO	    = 13
	OUTDIR		    = 14	# Directory where executable to put.
	BUILD_LOG	    = 15	# Build log file path
	BUILD_ERRLOG	    = 16
	RUN		    = 21
	TIMEOUT		    = 22
	EXPECTED	    = 23
	PIPE_PROCESS	    = 24
	KILL_PROCESS	    = 25
	NEED_INTERVENTION   = 26
	RUN_LOG		    = 27
	RUN_ERRLOG	    = 28
	LOG_CONTROL	    = 31

	"""
	SOLUTION	    = 1		# Name of the solution.
	PLATFORM	    = 3		# Platform name (e.g. '14.0'). (*)
	CONFIG		    = 4		# Configuration name.
	LOGFILES	    = 6		# Log file path [BLD, RUN].
	EXCLUDE		    = 7		# Exclude this directory form the test.
	USE_CLOSED_SRC	    = 8		# Use closed source or not.
	CPPMACRO	    = 9		# Macro definition for cpp preprocessor.
	PIPEPROCESS	    = 10	# Command line of the process whose stdout
					# is piped to test process.
	TIMEOUT		    = 11	# Runtime timeout value in seconds .
	EXPECTED	    = 12	# Expected run result (status code).
	NEEDINTERVENTION    = 13	# Need manual intervention at runtime.
	KILLPROCESS	    = 14	# Need kill process explicitly.
	BUILDLOG	    = 15	# Log file name to put all build logs.
	BUILDERRLOG	    = 16	# Log file name to put all build error lines.
	#
	SOLUTION_DIR	    = 51	# Process base directory (abs path).
	CLEAN		    = 52	# Force recompile by removing binaries.
	CS_CONTROL	    = 53	# Closed source usage (AUTO, USE or UNUSE).
	CS_CTRL_HEADER	    = 54	# Header file name for closed-src control.
	EXE_CONTROLS	    = 55	# Execution control [BLD, RUN].
	LOG_CONTROLS	    = 56	# Log output control [BLD, RUN].
	LIBDIR		    = 54	# Directory where related binaries are.
	"""

	# list indices
	ERR	= 0		#
	BLD	= 1		#
	RUN	= 2

	# closed source control
	AUTO	= 0		# change header file accoding to control file
	USE	= 1		# test solutions only with UseClosedSrc=True
	UNUSE	= 2		# test solutions only with UseClosedSrc=False

	# set function code
	DRYRUN	= 0

	#  Class initializer
	#
	def __init__(self, ccver, fname, section, verbose=0):
		self.clsname = self.__class__.__name__
		self.version = 4.0
		#
		self.solution = None
		self.error = None
		self.dry_run = False
		self.verbose = verbose

		# directories
		self.dir = {}
		self.dir['solution'] = os.getcwd()
		self.dir['output'] = None
		self.dir['log'] = 'log'
		self.dir['lib'] = {}
		self.dir['lib']['win32'] = None
		self.dir['lib']['win64'] = None

		# control file names
		self.ctlfile = {}
		"""
		self.ctlfile['solution'] = 'dailybuild.alias'
		self.ctlfile['output'] = 'dailybuild.outdir'
		self.ctlfile['timeout'] = 'dailybuild.timeout'
		self.ctlfile['all'] = fname
		"""
		self.ctlfile['name'] = fname
		self.ctlfile['section'] = section

		# header file names
		self.header = {}
		self.header['closed_src_header'] = 'UseClosedSrcOrNot.h'

		# controls
		self.control = {}
		self.control['exclude'] = False
		self.control['use_closed_src'] = False
		self.control['cs_control'] = self.AUTO
		self.control['exe'] = {self.BLD: True, self.RUN: True}
		self.control['log'] = {self.BLD: True, self.RUN: True}
		self.control['pipeprocess'] = None
		self.control['timeout'] = None
		self.control['expected'] = None
		self.control['cppmacro'] = None
		self.control['needintervention'] = None
		self.control['killprocess'] = None

		# file paths
		self.path = {}
		self.path['solution'] = None
		self.path['output'] = None
		self.path['log'] = {self.BLD: None, self:RUN: None}

		# build options
		self.opts = {}
		self.opts['platform'] = None
		self.opts['config'] = None
		self.opts['clean'] = False

		# compiler
		self.cc = {}
		self.cc['version'] = ccver

		#
		self.solution = self.dir['solution'].split(os.sep)[-1]
		self.__read_control_params(self.ctlfile['name'], self.ctlfile['section'])
		self.path['solution'] = self.solution + '.sln'
		libdir = '%s/../dependency/lib' % spr_path.abspath('core')
		libdir = Util.pathconv(os.path.abspath(libdir))
		"""
		libdir = self.__find_directory('lib')
		"""
		self.dir['lib']['win32'] = libdir + os.sep + 'win32'
		self.dir['lib']['win64'] = libdir + os.sep + 'win64'
		#
		if self.verbose > 2:
			self.info()
##@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#	set(ControlParams.CONFIGS, platform, config)
#	set(ControlParams.RECOMPILE, flag=True)
#	set(ControlParams.LOGFILES, comp=None, run=None)
#	set(ControlParams.USE_CLOSED_SRC_FLAG, usage)
#	set(ControlParams.DRYRUN, flag=True)

	#  Set build options and generate executable/log file name.
	#
	def build_option(self, platform, config):
		self.opts['platform'] = platform
		self.opts['config'] = config
		self.__set_output_dir()
		self.__make_outfile_name()
		self.__make_logfile_name()
		if self.verbose > 1:
			self.info()

	#  Force 'rebuild' instead of normal 'build'.
	#
	def force_rebuild(self, clean):
		self.opts['clean'] = clean

	#  Make log file or not.  Initial value is True for both.
	#
	def log_option(self, build, run):
		self.control['log'][self.BLD] = build
		self.control['log'][self.RUN] = run

	#  Use closed soruce or not.
	#
	def closed_src_control(self, control):
		if isinstance(control, str):
			conv = {'use': self.USE, 'unuse': self.UNUSE,
				'auto': self.AUTO}
			if control in conv:
				control = conv[control]
			else:
				control = self.AUTO
		self.control['cs_control'] = control

	"""
	#  Override execution control flags got from control file.
	#
	def override_exe_control(self, build, run):
		self.control['exe'][self.BUILD] = build
		self.control['exe'][self.RUN] = run
	"""
	#  Get the value associated with the key.
	#
	def get(self, key):
		if key == self.SOLUTION:	return self.solution
		elif key == self.SOLUTION_DIR:	return self.dir['solution']
		elif key == self.OUTDIR:	return self.dir['output']
		elif key == self.LIBDIR:	return self.dir['lib']
		elif key == self.PLATFORM:	return self.opts['platform']
		elif key == self.CONFIG:	return self.opts['config']
		elif key == self.CLEAN:		return self.opts['clean']
		elif key == self.OUTFILE:	return self.path['output']
		elif key == self.LOGFILES:	return self.path['log']
		elif key == self.CS_CTRL_HEADER:return self.header['closed_src_header']
		elif key == self.EXCLUDE:	return self.control['exclude']
		elif key == self.USE_CLOSED_SRC:return self.control['use_closed_src']
		elif key == self.CS_CONTROL:	return self.control['cs_control']
		elif key == self.CPPMACRO:	return self.control['cppmacro']
		elif key == self.EXE_CONTROLS:	return self.control['exe']
		elif key == self.LOG_CONTROLS:	return self.control['log']
		elif key == self.PIPEPROCESS:	return self.control['pipeprocess']
		elif key == self.TIMEOUT:	return self.control['timeout']
		elif key == self.EXPECTED:	return self.control['expected']
		elif key == self.NEEDINTERVENTION:\
						return self.control['needintervention']
		elif key == self.KILLPROCESS:	return self.control['killprocess']
		return None

	#  Returns True if setup process failed.
	#
	def has_error(self):
		return self.error

	#  Print information of this instance.
	#
	def info(self):
		print('ControlParams:')
		print('  solution:   %s' % self.solution)
		print('  directories')
		print('    solution: %s' % Util.upath(self.dir['solution']))
		print('    output:   %s' % Util.upath(self.dir['output']))
		print('    log:      %s' % Util.upath(self.dir['log']))
		print('    bin:      %s' % Util.upath(self.dir['lib']))
		print('  controls')
		print('    exe:      %s' % self.control['exe'])
		print('    log:      %s' % self.control['log'])
		print('    pipe-proc %s' % self.control['pipeprocess'])
		print('    expected: %s' % self.control['expected'])
		print('    timeout:  %s' % self.control['timeout'])
		print('    intervene:%s' % self.control['needintervention'])
		print('    killproc: %s' % self.control['killprocess'])
		print('  paths')
		print('    solution: %s' % Util.upath(self.path['solution']))
		print('    output:   %s' % Util.upath(self.path['output']))
		print('    log:      %s' % Util.upath(self.path['log']))
		print('  build options')
		print('    platform: %s' % self.opts['platform'])
		print('    config:   %s' % self.opts['config'])
		print('    clean:    %s' % str(self.opts['clean']))
		print('  Visual Studio')
		print('    version:  %s' % str(self.cc.vs['version']))

	#  Set various parameters.
	#
	def set(self, func, arg1):
		if func == self.DRYRUN:
			if arg1 is None:
				arg1 = True
			self.dry_run = arg1
		else:
			msg = 'set: invalid function number'
			Error(self.clsname).print(msg)

	# --------------------------------------------------------------
	#  For class private use
	# --------------------------------------------------------------

	#  Read test control parameter file.
	#  Control file is searched and read from current directory to
	#  go up until 'tests' or 'Samples' directory has found.
	#  As for the same key, lower level file has priority.
	#
	def __read_control_params(self, fname, section):
		# fname:	Control parameter file name.
		# section:	Control parameter section name.
		cwd = self.dir['solution'].split(os.sep)
		top = len(cwd)
		for n in range(len(cwd)):
			if cwd[n] in ['tests', 'Samples']:
				top = n
		for n in range(top, len(cwd)):
			up = len(cwd) - n - 1
			prefix = ''
			for n in range(up):
				prefix += '..' + os.sep
			if os.path.exists(prefix + fname):
				kvf = KvFile(prefix + fname)
				section = self.ctlfile['section']
				if kvf.read() > 0:
					self.__set_control_values(kvf)
		if self.verbose > 1:
			cs_control_str = ['AUTO', 'USE', 'UNUSE']
			print('\ncontrol_params:')
			print('  Exclude        %s' % self.control['exclude'])
			print('  SolutionAlias  %s' % self.solution)
			print('  OutputDir      %s' % self.dir['output'])
			print('  UseClosedSrc   %s' % self.control['use_closed_src'])
			print('  CS control     %s' %
					cs_control_str[self.control['cs_control']])
			print('  CppMacro       %s' % self.control['cppmacro'])
			print('  Build          %s' % self.control['exe'][self.BLD])
			print('  Run            %s' % self.control['exe'][self.RUN])
			print('  Timeout        %s' % self.control['timeout'])
			print('  ExpectedStatus %s' % self.control['expected'])
			print('  NeedIntervene  %s' % self.control['needintervention'])
			print('  KillProcess    %s' % self.control['killprocess'])

	#  Set control value (for all key-value pairs).
	#
	def __set_control_values(self, kvf):
		# kvf:		KvFile class object.
		section = self.ctlfile['section']
		for key in kvf.keys(section):
			self.__set_control_value(key.lower(),
						 kvf.get(key, section=section))

	#  Set control value (for one key-value pair).
	#
	def __set_control_value(self, key, value):
		# key:		Control key.
		# value:	Control value to set.
		if key == 'exclude':
			self.control['exclude'] = value
		elif key == 'solutionalias':
			self.solution = value
		elif key == 'outputdir':
			self.dir['output'] = value
		elif key == 'useclosedsrc':
			self.control['use_closed_src'] = value
		elif key == 'cppmacro':
			self.control['cppmacro'] = value
		elif key == 'dontbuild':
			self.control['exe'][self.BLD] = not value
		elif key == 'dontrun':
			self.control['exe'][self.RUN] = not value
		elif key == 'pipeprocess':
			self.control['pipeprocess'] = value
		elif key == 'timeout':
			self.control['timeout'] = value
		elif key == 'expectedstatus':
			self.control['expected'] = value
		elif key == 'needintervention':
			self.control['needintervention'] = value
		elif key == 'killprocess':
			self.control['killprocess'] = value

	#  Set output directory name.
	#  Directory name is composed from toolset version, platform name,
	#  configuration name and pythin version.  These parameters can be
	#  specified by macro '$TOOLSET', '$PLATFORM', '$CONFIGURATION' in
	#  the control file (key: 'OutputDir').
	#
	def __set_output_dir(self):
		# default output directory is determined by build options
		vsv = self.cc.vs['version']
		platform = self.opts['platform']
		config = self.opts['config']
		self.dir['output'] = os.sep.join([vsv, platform, config])
		#
		if not os.path.exists(self.ctlfile['output']):
			return
		# output directory is taken from the file
		line = self.__read_one_line(self.ctlfile['output'])
		if line is not None:
			line = line.replace('$TOOLSET', vsv)
			line = line.replace('$PLATFORM', platform)
			line = line.replace('$CONFIGURATION', config)
			self.dir['output']= line.replace('/', '\\')

	#  Generate output file name.
	#
	def __make_outfile_name(self):
		path = self.dir['output'] + os.sep + self.solution + '.exe'
		self.path['output'] = path

	#  Generate log file name.
	#
	def __make_logfile_name(self):
		platform = self.opts['platform']
		config = self.opts['config']
		base = self.dir['log']
		base += os.sep + self.solution + '_' + self.cc.vs['version']
		base += '_' + platform + '_' + config
		self.path['log'][self.BLD] = base + '_build.log'
		self.path['log'][self.RUN] = base + '_run.log'

	"""
	#  Find directory having 'src' or 'test' as one of direct child
	#  directories, and returns it with specified 'dir' appended.
	#
	def __find_directory(self, dir):
		# dir:		Directory name to append.
		dirs = self.dir['solution'].split(os.sep)
		for n in range(len(dirs)):
			if dirs[n] == 'src' or dirs[n] == 'test':
				break
		if n == len(dirs)-1:
			self.__error("can't find %s directory" % dir)
			return None
		return os.sep.join(dirs[0:n]) + os.sep + dir
	"""

	#  Replace macro definition in the file specified by 'path'.
	#
	def __replace(self, path, fm, to):
		# path:		Path of the header file.
		# fm:		Search pattern.
		# to:		Replace pattern.
		if path is None or not os.path.exists(path):
			return
		if self.verbose:
			print('  replace: pattern: [%s] -> [%s]' % (fm, to))
		try:
			# read header file
			lines = []
			with open(path, 'r') as ifile:
				for line in ifile:
					lines.append(line.strip())
			count = 0
			replaced = []
			for line in lines:
				if fm in line:
					line.replace(fm, to)
					count += 1
				replaced.append(line)
			ifile.close()
			if count == 0:
				if self.verbose:
					print('  no need to replace')
					print()
				return

			# create tmp file
			tmpfname = path + '.tmp'
			with open(tmpfname, 'w') as ofile:
				for line in lines:
					ofile.write(line + '\n')
			ofile.close()

			# rename files
			fop = FileOp(verbose=self.verbose, dry_run=self.dry_run)
			fop.mv(path, path + '.org')
			fop.mv(tmpfname, path)
			if self.dry_run: print()

		except IOError as err:
			self.__error('%s' % (err))

	#  Read one line data form 'path'.  Comment lines are ignored.
	#
	def __read_one_line(self, path):
		lines = []
		try:
			with open(path, 'r') as ifile:
				for line in ifile:
					lines.append(line.strip())
		except IOError as err:
			# can't read file
			self.__error('%s' % (err))
			return None
		lines = self.__skip_comment(lines, '#')
		if lines is None or len(lines) == 0:
			return None
		if self.verbose:
			print('  read: [%s]' % lines[0])
		return lines[0]

	#  soon will be obsoleted
	#
	def __skip_comment(self, lines, prefix):
		if lines is None: return
		skipped = []
		for line in lines:
			line = line.strip()
			if line != '' and line[:len(prefix)] != prefix:
				skipped.append(line)
		return skipped

	#  Print error message to stderr.
	#
	def __error(self, msg):
		# msg:		Error message to print.
		self.error = '%s: %s' % (self.clsname, msg)

	#  Make unix like path representation
	#
	def __unixpath(self, path):
		# path:		Path to convert.
		return Util.pathconv(path, 'unix')


# ----------------------------------------------------------------------
#  Test main
# ----------------------------------------------------------------------
if __name__ == '__main__':

	from optparse import OptionParser
	from SpringheadTest import *
	from VisualStudio import *
	from Proc import *

	usage = 'Usage: %prog [options] directory'
	parser = OptionParser(usage = usage)
	parser.add_option('-t', '--toolset', dest='toolset',
			default='14.0',
			help='Visual Studio version [default: %default]')
	parser.add_option('-c', '--closed_src_control',
			dest='cs_control', default='auto',
			metavar='CTRL',
			help="closed src control " +
				"('use', 'unuse' or 'auto'(default))")
	parser.add_option('-6', '--x64',
			dest='x64', action='store_true',
			default=False, help='test only on x64 platform')
	parser.add_option('-8', '--x86',
			dest='x86', action='store_true',
			default=False, help='test only on x86 platform')
	parser.add_option('-D', '--dry-run',
			dest='dry_run', action='store_true',
			default=False, help='set dry-run mode')
	parser.add_option('-v', '--verbose',
			dest='verbose', action='count',
			default=0, help='set verbose mode')
	parser.add_option('-w', '--vs-verbose',
			dest='vs_verbose', action='count',
			default=0, help='set VisualStudio verbose mode')
	(options, args) = parser.parse_args()
	if len(args) != 1:
		Proc().exec('python ControlParams.py -h').wait()
		sys.exit(-1)
	if options.cs_control not in ['use', 'unuse', 'auto']:
		print("Error: -c: must be one of 'use', 'unuse' or 'auto'")
		sys.exit(-1)
	if not options.x86 and not options.x64:
		options.x86 = True
		options.x64 = True
	testdir = args[0]

	putlog = [True, 64]		# [build, run]
	runlog_lines = 10

	def build(platform, config, execute):
		if execute == False:
			return 1
		ctl.log_option(putlog[0], putlog[1])
		ctl.build_option(platform, config)
		if not ctl.has_error():
			info(ctl)
		vs.set_outdir(ctl.get(ctl.OUTDIR), clean=True)
		vs.set_log(ctl.get(ctl.LOGFILES)[ctl.BLD])
		status = vs.build(platform, ctl.get(ctl.CONFIG))
		print('build result: %d' % status)
		return status

	def run(platform, ctl, addbase, verbose):
		exefile = ctl.get(ctl.OUTFILE)
		logfile = ctl.get(ctl.LOGFILES)[ctl.RUN]
		if platform == 'Win32':
			addpath = '%s/win32' % addbase
		else:
			addpath = '%s/win64' % addbase
		pipeprocess = ctl.get(ctl.PIPEPROCESS)
		timeout = ctl.get(ctl.TIMEOUT)
		print('PIPE: %s' % pipeprocess)
		print('TIME: %s' % timeout)
		if pipeprocess:
			toolpath = spr_topdir + '/bin/test'
			proc1 = Proc(verbose, options.dry_run)
			proc2 = Proc(verbose, options.dry_run)
			proc1.exec(exefile, addpath=addpath,
				   stdin=Proc.PIPE, stdout=logfile)
			proc2.exec(pipeprocess, addpath=toolpath,
				   stdout=Proc.PIPE)
			status = proc1.wait(timeout)
			proc2.wait()
		else:
			proc1 = Proc(verbose, options.dry_run)
			proc1.exec(exefile, addpath=addpath, stdout=logfile)
			status = proc1.wait(timeout)
		head(logfile, runlog_lines)
		augmsg = '(timeout)' if status == Proc.ETIME else ''
		print('run result: %d %s' % (status, augmsg))

	def info(ctl):
		cs_control_str = ['AUTO', 'USE', 'UNUSE']
		print('ControlParams')
		print('  solution:       %s' % ctl.get(ctl.SOLUTION))
		print('  solution_dir:   %s' % Util.upath(ctl.get(ctl.SOLUTION_DIR)))
		print('  use_closed_src: %s' % ctl.get(ctl.USE_CLOSED_SRC))
		print('  cs_control:     %s' %
				cs_control_str[ctl.get(ctl.CS_CONTROL)])
		print('  outfile:        %s' % Util.upath(ctl.get(ctl.OUTFILE)))
		print('  logfiles        %s' % Util.upath(ctl.get(ctl.LOGFILES)))
		print('  binary_dir:     %s' % Util.upath(ctl.get(ctl.LIBDIR)))
		print('  exe_controls:   %s' % ctl.get(ctl.EXE_CONTROLS))
		print('  log_controls:   %s' % ctl.get(ctl.LOG_CONTROLS))
		print('  timeout:        %s' % ctl.get(ctl.TIMEOUT))
		print('  pipeprocess:    %s' % ctl.get(ctl.PIPEPROCESS))
		print()

	def head(file, maxlines):
		if not os.path.exists(file):
			return 
		with open(file) as f:
			line = f.readline()
			while line and maxlines > 0:
				print(line.strip())
				line = f.readline()
				maxlines -= 1

	def find_dir(name):
		dirs = os.getcwd().split(os.sep)
		found = False
		for n in range(len(dirs)):
			if dirs[n] == name:
				found = True
				break
		return os.sep.join(dirs[0:n]) if found else None

	cwd = os.getcwd()
	os.chdir(Util.pathconv(testdir))
	print('cwd: %s' % Util.upath(os.getcwd()))

	print('Visual Studio')
	vs = VisualStudio(options.toolset, options.vs_verbose)
	print('  %s' % vs.get('version'))
	ctl = ControlParams(vs.get('version'), options.verbose)
	vs.solution(ctl.get(ctl.SOLUTION))
	if vs.has_error() or ctl.has_error():
		os.chdir(cwd)
		sys.exit(-1)

	vs.set_dry_run(options.dry_run)
	ctl.set_dry_run(options.dry_run)

	ctl.closed_src_control(options.cs_control)

	timeout = 5
	verbose = 1

	spr_topdir = find_dir('test')
	if spr_topdir is None:
		spr_topdir = find_dir('src')
	if spr_topdir is None:
		print('%s: Error: can not find Spr top directory' % ctl.clsname)
		sys.exit(-1)
	print('top dir: %s' % Util.upath(spr_topdir))
	addbase = '%s/../libs/bin' % spr_topdir

	# Win32
	status = build('Win32', 'Debug', options.x86)
	if status == 0:
		run('Win32', ctl, addbase, verbose)
	if options.x86 and options.x64:
		input('Hit any key')
		print('')

	# x64
	status = build('x64', 'Release', options.x64)
	if status == 0:
		run('x64', ctl, addbase, verbose)

	os.chdir(cwd)
	sys.exit(0)

# end: ControlParams.py

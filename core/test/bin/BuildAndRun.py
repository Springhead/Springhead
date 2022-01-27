#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:
#	BuildAndRun:
#	    Build the solution and/or run the program.
#
#  INITIALIZER:
#	obj = BuildAndRun(ctl, ccver, verbose=0, dry_run=False)
#	arguments:
#	    ctl:	Instance of ControlParams class.
#	    ccver:	C-compiler version (str).
#			    Windows: Visual Studio version (str).
#			    unix:    gcc version (dummy).
#	    verbose:	Verbose level (int) (0: silent).
#	    dry_run:	Show command but do not execute (bool).
#
#  METHODS:
#	stat = build(dirpath, slnfile, platform, config, outfile,
#			logfile, errlogfile, force=False)
#	    Change directory to 'dirpath' temporarily and build the
#	    solution.  'Outfile'/'logfile' should be absolute path or
#	    relative path to 'dirpath'.
#	    arguments:
#	        dirpath:    Directory path where 'slnfile' exists.
#		slnfile:    Solution/make file name.
#	        platform:   Target platform ('x86' or 'x64').
#		config:	    Compiler option.
#			        Windows: 'Debug'/'Release/Trace' (config)
#			        unix:	 '-g'/'-O2' (any option is OK)
#		outfile:    Output binary file path.
#			    NOTE: Leaf file name is dummy for Windows.
#		logfile:    Log file path (log will be appended).
#		errlogfile: Error log file path (log will be appended).
#		force:	    Force recompile (bool).
#	    returns:	 0: OK
#			-1: compile error.
#
#	stat = run(dirpath, exefile, args, logfile,
#			addpath=None, timeout=None, pipeprocess=None)
#	    Execute the program.
#	    arguments:
#		dirpath:    Directory path where prigram executes.
#		exefile:    Executable file path.
#		args:	    Run time arguments.
#		logfile:    Log file path (log will be appended).
#		errlogfile: Error log file path (log will be appended).
#		addpath:    Additional path to execute program.
#		timeout:    Time out value in sec (num).
#		pipeproc:   Process name whose output is piped to
#			    the program 'exefile'.
#	    returns:	 0: OK
#			-1: compile error.
#
#	msg = error()
#	    Check if something wrong has occured in setup process.
#	    returns:	Error message if error has occurred so far.
#			None if recorded no error.
#
# ----------------------------------------------------------------------
#  VERSION:
#     Ver 1.0	 2018/02/08 F.Kanehori	First version.
#     Ver 1.1	 2018/03/15 F.Kanehori	Bug fixed (for unix).
#     Ver 1.2	 2018/08/07 F.Kanehori	Execute binary directly (unix).
#     Ver 1.3	 2019/08/07 F.Kanehori	Add parameter to initializer.
#     Ver 1.3.1	 2021/07/15 F.Kanehori	libdir 取得方式変更.
#     Ver 1.3.2	 2021/12/20 F.Kanehori	CMAKE_BLDDIR を反映
# ======================================================================
import sys
import os
import re
from stat import *

from ControlParams import *
from CMake import *
from VisualStudio import *

# ----------------------------------------------------------------------
#  このスクリプトは ".../core/test/bin" に置く	
#
ScriptFileDir = os.sep.join(os.path.abspath(__file__).split(os.sep)[:-1])
prog = sys.argv[0].replace('/', os.sep).split(os.sep)[-1].split('.')[0]
TopDir = '/'.join(ScriptFileDir.split(os.sep)[:-3])

# ----------------------------------------------------------------------
#  Springhead python library の導入
#
libdir = '%s/core/src/RunSwig/pythonlib' % TopDir
sys.path.append(libdir)
from Error import *
from Util import *
from Proc import *
from TextFio import *
from FileOp import *
from ConstDefs import *

class BuildAndRun:

	#  Class initializer.
	#
	def __init__(self, ctl, ccver, verbose=0, dry_run=False):
		self.clsname = self.__class__.__name__
		self.version = '1.3.1'
		#
		self.ctl = ctl
		self.ccver = ccver
		self.dry_run = dry_run
		self.verbose = verbose
		#
		self.use_cmake = ctl.get(CFK.USE_CMAKE)
		self.encoding = 'utf-8' if Util.is_unix() else 'cp932'
		self.errmsg = None

	#  Compile the solution.
	#
	def build(self, dirpath, slnfile, platform, config,
			outfile, logfile, errlogfile, force=False):
		self.dirpath = dirpath
		self.slnfile = slnfile
		self.platform = platform	# used by run()
		self.config = config		# used by run()
		if self.verbose:
			print('build solution')
			print('  dirpath: %s' % dirpath)
			print('  slnfile: %s' % slnfile)
			print('  config:  %s' % config)
			print('  outfile: %s' % outfile)
			print('  force:   %s' % force)
			print('  logfile: %s' % logfile)
			print('  errlog:  %s' % errlogfile)
		self.verbose = 0

		# go to target directory.
		if dirpath:
			dirsave = self.__chdir('build', dirpath)
		if self.verbose:
			cwd = Util.upath(os.getcwd())
			print('build: in directory "%s"' % cwd)

		# prepare log file (append mode).
		logf = self.__open_log(logfile, 'a', RST.BLD)
		errlogf = self.__open_log(errlogfile, 'a', RST.BLD)

		# call compiler
		args = [slnfile, platform, config, outfile, force]
		if Util.is_unix():
			stat, loginfo = self.__build_u(args)
		else:
			stat, loginfo = self.__build_w(args)

		# merge log data
		cmnd, data = loginfo
		errors = self.__select_errors(data, RST.BLD)
		self.__merge_log(2, data, logf, cmnd, RST.BLD)
		self.__merge_log(1, errors, errlogf, cmnd, RST.ERR)

		# return to previous directory
		if dirpath:
			os.chdir(dirsave)
		return stat
				
	#  Execute the program.
	#	Windows:  Invoke 'exefile' directly.
	#	unix:	  Invoke through 'make -f exefile test'.
	#		  In this case, 'exefile' should be a makefile.
	#
	def run(self, dirpath, exefile, args, logfile, errlogfile,
			addpath=None, timeout=None, pipeprocess=None):
		if self.verbose:
			print('run program')
			print('  dirpath: %s' % dirpath)
			print('  exefile: %s' % exefile)
			print('  args:    %s' % args)
			print('  logfile: %s' % logfile)
			print('  errlog:  %s' % errlogfile)
			print('  addpath: %s' % addpath)
			print('  timeout: %s' % timeout)
			print('  pipe:    %s' % pipeprocess)

		# go to target directory.
		if dirpath:
			dirsave = self.__chdir('run', dirpath)
		if self.verbose:
			cwd = Util.upath(os.getcwd())
			print('run: in directory "%s"' % cwd)
		if Util.is_windows():
			if exefile[-4:] != '.exe':
				exefile = '%s.exe' % exefile
		if self.verbose:
			print('  exefile: %s' % exefile)
		if not os.path.exists(exefile):
			print('run: no such file "%s"' % exefile)
			return -1
		
		# use following temporary log file.
		if Util.is_unix():
			config = self.config.replace('-', '')
			ccver = ''
		else:
			config = self.config
			ccver = '_%s' % self.ccver
		tmplogdir = 'log'
		tmplog = '%s/%s%s_%s_%s_run.log' % \
			(tmplogdir, self.clsname, ccver, self.platform, config)
		os.makedirs(tmplogdir, exist_ok=True)

		# prepare log file (append mode).
		logf = self.__open_log(logfile, 'a', RST.RUN)
		errlogf = self.__open_log(errlogfile, 'a', RST.RUN)

		# execute program.
		shell= True if Util.is_unix() else False
		if pipeprocess:
			# proc1: target program.
			# proc2: generate input stream to feed proc1.
			proc1 = Proc(verbose=self.verbose, dry_run=self.dry_run)
			proc2 = Proc(verbose=self.verbose, dry_run=self.dry_run)
			proc1.execute('%s %s' % (exefile, args),
				      shell=shell, addpath=addpath,
				      stdin=Proc.PIPE, stdout=tmplog,
				      stderr=Proc.STDOUT)
			proc2.execute(pipeprocess,
				      shell=shell, addpath=addpath,
				      stdout=Proc.PIPE)
			stat = proc1.wait(timeout)
			proc2.wait()
		else:
			# proc1: target program.
			proc1 = Proc(verbose=self.verbose, dry_run=self.dry_run)
			proc1.execute('%s %s' % (exefile, args),
				   addpath=addpath, shell=shell,
				   stdout=tmplog, stderr=Proc.STDOUT)
			stat = proc1.wait(timeout)

		# merge log info.
		cmnd = '%s %s' % (exefile, args)
		errors = self.__select_errors(tmplog, RST.RUN)
		self.__merge_log(2, tmplog, logf, cmnd, RST.RUN)
		self.__merge_log(1, errors, errlogf, cmnd, RST.ERR)

		if dirpath:
			os.chdir(dirsave)
		return stat

	#  Check if some error has occured so far.
	#
	def error(self):
		return self.errmsg


	# --------------------------------------------------------------
	#  For class private use
	# --------------------------------------------------------------

	#  Change directory.
	#
	def __chdir(self, func, path):
		dirsave = Util.upath(os.getcwd())
		try:
			os.chdir(path)
		except:
			msg = "%s: chdir failed (%s)" % (func, path)
			Error(self.clsname).error(msg)
		return dirsave

	#  Take directory part of the path.
	#
	def __dirpart(self, path):
		dpart = '/'.join(Util.upath(path).split('/')[:-1])
		if dpart == '':
			dpart = '.'
		return dpart

	#  Open log file.
	#
	def __open_log(self, fname, fmode, step):
		# arguments:
		#   fname:	Log file path.
		#   fmode:	File open mode ('r', 'w' or 'a').
		#   prefix:	Error message prefix (str).
		# returns:	File object.

		if self.dry_run:
			print('  open log file "%s"' % fname)
			return None
		if fname is None:
			return None

		logdir = self.__dirpart(fname)
		if logdir != '':
			os.makedirs(logdir, exist_ok=True)
		logf = TextFio(fname, mode=fmode, encoding=self.encoding)
		if logf.open() < 0:
			msg = 'build' if step == RST.BLD else 'run'
			msg += ': open error: "%s"' % fname
			Error(self.clsname).error(msg)
			logf = None
		return logf

	#  Call compiler (for unix).
	#
	def __build_u(self, args):
		# arguments:
		#   args:	Parameters to compiler (list).
		opt_flags = {
			'Debug':	'OPTS=-g',
			'Release':	'OPTS=-O2',
			'Trace':	'OPTS=-O2',
			None:		''
		}
		[slnfile, platform, config, outfile, force] = args
		if config not in opt_flags:
			config = None
		tmplogdir = 'log'
		tmplog = '%s/%s_%s_%s_build.log' % \
			(tmplogdir, self.clsname, self.platform, self.config)
		os.makedirs(tmplogdir, exist_ok=True)
		FileOp().touch(tmplog)

		if (self.use_cmake):
			cwd = os.getcwd()
			build_dir = self.ctl.get(CFK.CMAKE_BLDDIR)
			if not build_dir:
				build_dir = 'build'
			os.chdir(build_dir)
			target = 'all'
		else:
			target = 'compile'
		cmnd = 'make -f %s %s' % (slnfile, target)
		args = opt_flags[config]
		proc = Proc(verbose=self.verbose, dry_run=self.dry_run)
		proc.execute('%s %s' % (cmnd, args), shell=True,
				stdout=tmplog, stderr=Proc.STDOUT)
		stat = proc.wait()
		if (self.use_cmake):
			os.chdir(cwd)

		cmnd = '%s %s' % (cmnd, args)
		loginfo = [cmnd, tmplog]
		return stat, loginfo

	#  Call compiler (for Windows).
	#
	def __build_w(self, args):
		# arguments:
		#   args:	Parameters to compiler (list).
		[slnfile, platform, config, outfile, force] = args

		outdir = self.__dirpart(outfile).replace('x86', 'Win32')
		tmplog = 'log/%s_%s_%s_%s_build.log' % \
			(self.clsname, self.ccver, self.platform, self.config)
		FileOp().rm(tmplog)

		if self.verbose > 1:
			print('build solution (Windows)')
			print('  slnfile: %s' % slnfile)
			print('  config:  %s' % config)
			print('  outdir:  %s' % outdir)
			print('  tmplog:  %s' % tmplog)
			print('  force:   %s' % force)

		vs = VisualStudio(self.ccver, self.verbose)
		vs.solution(slnfile)
		vs.set(VisualStudio.OUTDIR, outdir, force)
		vs.set(VisualStudio.LOGFILE, tmplog)
		vs.set(VisualStudio.DRYRUN, self.dry_run)
		if vs.error():
			Error(self.clsname).abort(vs.error())
		stat = vs.build(platform, config)

		cmnd = vs.get(VisualStudio.COMMAND)
		loginfo = [cmnd, tmplog]
		return stat, loginfo

	def __select_errors(self, fname, step):
		# arguments:
		#   fname:	Log file name (str).
		#   step:	Execute step (RST.BLD or RST.RUN).
		# returns:	List of error messages (str[]).

		fsize = os.stat(fname).st_size
		fobj = TextFio(fname, size=fsize)
		if fobj.open() < 0:
			msg = 'build' if step == RST.BLD else 'run'
			msg += '_s: open error: "%s"' % fname
			Error(self.clsname).error(msg)
		lines = fobj.read()
		fobj.close()

		patt = re.compile(' error', re.I)
		errors = []
		for line in lines:
			if patt.search(line):
				errors.append(line)
		return errors

	def __merge_log(self, kind, data, logf, cmnd, step):
		# arguments:
		#   kind:	Kind of process.
		#		    1: Next arg 'data' is a list of log data.
		#		    2: Read log data from the file named 'data'.
		#   data:	See above.
		#   logf:	Log file object.
		#   step:	Execution step (RST.BLD or RST.RUN).
		#   cmnd:	Command string to be executed (str).

		if logf is None:
			return

		# make header part
		cwd = self.dirpath if self.dirpath else os.getcwd()
		cwd = Util.upath(cwd).split('/')
		head1 = '*** %s: %s ***' % (cwd[-2], cwd[-1])
		head2 = '%% %s' % cmnd
		if step == RST.BLD:
			title = [head1, head2, '']
		else:
			title = [head1]

		# merge
		if kind == 1:
			# data is a list of string
			logf.writelines(title)
			logf.writelines(data)
		elif kind == 2:
			# data is the file name to be read
			fsize = os.stat(data).st_size
			tmpf = TextFio(data, size=fsize)
			if tmpf.open() < 0:
				msg = 'build' if step == RST.BLD else 'run'
				msg += '_w: open error: "%s"' % (name, data)
				Error(self.clsname).error(msg)
			else:
				logf.writelines(title)
				lines = tmpf.read()	
				logf.writelines(lines)
				tmpf.close()
		else:
			msg = 'merge_log: bad kind: %s' % kind
			Error(self.clsname).abort(msg)
		logf.close()


# ----------------------------------------------------------------------
#  Test main
# ----------------------------------------------------------------------
if __name__ == '__main__':
	from optparse import OptionParser

	usage = 'Usage: %prog [options] directory'
	parser = OptionParser(usage = usage)
	parser.add_option('-t', '--toolset', dest='toolset',
			default='14.0',
			help='Visual Studio version [default: %default]')
	parser.add_option('-c', '--config', dest='config',
			default='Release',
			help='configuration [default: %default]')
	parser.add_option('-p', '--platform', dest='platform',
			default='x64',
			help='platform [default: %default]')
	parser.add_option('-s', '--solution', dest='solution',
			default=None,
			help='solution file name')
	parser.add_option('-f', '--force', dest='force',
			action='store_true', default=None,
			help='force recompile')
	parser.add_option('-C', '--clean', dest='clean',
			action='store_true', default=None,
			help='clean up log file before test')
	parser.add_option('-T', '--timeout', dest='timeout',
			default=None,
			help='time out value in sec')
	parser.add_option('-D', '--dry-run', dest='dry_run',
			action='store_true', default=False,
			help='set dry-run mode')
	parser.add_option('-v', '--verbose', dest='verbose',
			action='count', default=0,
			help='set verbose mode')
	(options, args) = parser.parse_args()

	if len(args) != 1:
		prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
		Proc().execute('python %s.py -h' % prog).wait()
		sys.exit(-1)
	if not options.toolset:
		prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
		Proc().execute('python %s.py -h' % prog).wait()
		sys.exit(-1)
	directory = args[0]
	toolset = options.toolset
	config = options.config
	platform = options.platform
	solution = options.solution
	force = options.force
	clean = options.clean
	timeout = options.timeout
	dry_run = options.dry_run
	verbose = options.verbose

	if not os.path.exists(directory):
		try_dir = '../../src/' + directory
		if not os.path.exists(try_dir):
			Error('test').abort('no such directory: %s' % directory)
		directory = try_dir
	if not solution:
		solution = directory.split('/')[-1]
	bldlog = '%s/BuildAndRun_test_build.log' % os.getcwd()
	runlog = '%s/BuildAndRun_test_run.log' % os.getcwd()
	if clean:
		FileOp().rm(bldlog)
		FileOp().rm(runlog)

	print('test start')
	print('  directory: %s' % directory)
	print('  solution:  %s' % solution)
	print('  toolset:   %s' % toolset)
	print('  platform:  %s' % platform)
	print('  config:    %s' % config)
	print('  force:     %s' % force)
	print('  timeout:   %s' % timeout)
	print('  dry_run:   %s' % dry_run)
		
	# Windows
	if Util.is_windows():
		if solution:
			if solution[-4:] != '.sln':
				solution += '%s.sln' % toolset
		bar = BuildAndRun(toolset, verbose, dry_run)
		stat = bar.build(directory,
				solution,
				platform,
				config,
				'%s/%s/%s/%s.exe' % \
					(toolset, platform, config, solution[:-4]),
				bldlog,
				force)
		print('build: stat %d' % stat)
		if stat != 0:
			print(bar.error())
		if stat == 0:
			outbase = solution[:-4].replace(toolset, '')
			outpath = '%s/%s/%s/%s/%s.exe' % \
				(directory, toolset, platform, config, outbase),
			print('outpath: %s' % outpath)
			stat = bar.run(directory,
				Util.pathconv(outpath),
				'',
				runlog,
				timeout)
			print('run: stat %d' % stat)

	sys.exit(0)

# end: BuildAndRun.py

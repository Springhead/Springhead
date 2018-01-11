#!/usr/bin/env python
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:
#	BuildAndRun:
#	    Build the solution and/or run the program.
#
#  INITIALIZER:
#	obj = BuildAndRun(ccver, arch, verbose=0, dry_run=False)
#	arguments:
#	    ccver:	C-compiler version (str).
#			    Windows: Visual Studio version (str).
#			    unix:    gcc version (dummy).
#	    arch:	Target architecture ('x86' or 'x64').
#	    verbose:	Verbose level (int) (0: silent).
#	    dry_run:	Show command but do not execute (bool).
#
#  METHODS:
#	stat = build(dirpath, slnfile, opts, outfile, logfile, force=False)
#	    Change directory to 'dirpath' temporarily and build the
#	    solution.  'Outfile'/'logfile' should be absolute path or
#	    relative path to 'dirpath'.
#	    arguments:
#	    dirpath:    Directory path where 'slnfile' exists.
#		slnfile:    Solution/make file name.
#		opts:	    Compiler option.
#			        Windows: 'Debug'/'Release'
#			        unix:	 '-g'/'-O2' (any option is OK)
#		outfile:    Output binary file path.
#			    NOTE: Leaf file name is dummy for Windows.
#		logfile:    Log file path (log will be appended).
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
#		addpath:    Additional path to execute program.
#		timeout:    Time out value in sec (num).
#		pipeproc:   Process name whose output is piped to
#			    the program 'exefile'.
#	    returns:	 0: OK
#			-1: compile error.
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2018/01/11 F.Kanehori	First version.
# ======================================================================
import sys
import os
from VisualStudio import *

# local python library
#
sys.path.append('../../src/RunSwig')
from FindSprPath import *
spr_path = FindSprPath('BuildAndRun')
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from Error import *
from Util import *
from Proc import *
from TextFio import *
from FileOp import *

class BuildAndRun:

	#  Class initializer.
	#
	def __init__(self, ccver, arch, verbose=0, dry_run=False):
		self.clsname = self.__class__.__name__
		self.version = 1.0
		#
		self.ccver = ccver
		self.arch = arch
		self.dry_run = dry_run
		self.verbose = verbose
		#
		self.E = Error(self.clsname)

	#  Compile the solution.
	#
	def build(self, dirpath, slnfile, opts, outfile, logfile, force=False):
		self.config = opts
		if self.verbose:
			print('build solution')
			print('  dirpath: %s' % dirpath)
			print('  slnfile: %s' % slnfile)
			print('  opts:    %s' % opts)
			print('  outfile: %s' % outfile)
			print('  force:   %s' % force)
			print('  logfile: %s' % logfile)

		# go to target directory.
		dirsave = self.__chdir('build', dirpath)
		if self.verbose:
			cwd = Util.upath(os.getcwd())
			print('build: in directory "%s"' % cwd)

		# prepare log file (append mode).
		logf = self.__open_log(logfile, 'a', 'build')

		# call compiler
		self.args = [slnfile, opts, outfile, force]
		if Util.is_unix():
			stat = self.__build_u(logf)
		else:
			tmpdir = self.__dirpart(logfile)
			stat = self.__build_w(logf, tmpdir)
		if logf:
			logf.close()
		self.args = list(map(lambda x:0, self.args))

		os.chdir(dirsave)
		return stat
				
	#  Execute the program.
	#
	def run(self, dirpath, exefile, args, logfile,
			addpath=None, timeout=None, pipeprocess=None):
		if self.verbose:
			print('run program')
			print('  dirpath: %s' % dirpath)
			print('  exefile: %s' % exefile)
			print('  args:    %s' % args)
			print('  logfile: %s' % logfile)
			print('  addpath: %s' % addpath)
			print('  timeout: %s' % timeout)
			print('  pipe:    %s' % pipeprocess)

		# go to target directory.
		dirsave = self.__chdir('run', dirpath)
		if self.verbose:
			cwd = Util.upath(os.getcwd())
			print('run: in directory "%s"' % cwd)

		if Util.is_windows():
			arch = 'x64' if self.arch == 'x64' else 'Win32'
			bindir = '%s/%s/%s' % (self.ccver, arch, self.config)
			exefile = '%s/%s' % (bindir, exefile)
		if self.verbose:
			print('  exefile: %s' % exefile)
		if not os.path.exists(exefile):
			print('fun: no such file "%s"' % exefile)
			return -1
		
		# use following temporary log file.
		tmpdir = self.__dirpart(logfile)
		if Util.is_unix():
			config = config.replace('-', '')
		tmplog = 'log/%s_%s_%s_%s_run.log' % \
				(self.clsname, self.ccver, self.arch, self.config)

		# prepare log file (append mode).
		logf = self.__open_log(logfile, 'a', 'go')

		# execute program.
		if pipeprocess:
			proc1 = Proc(self.verbose, self.dry_run)
			proc2 = Proc(self.verbose, self.dry_run)
			proc1.exec('%s %s' % (exefile, args),
				   addpath=addpath, stdin=Proc.PIPE,
				   stdout=tmplog, stderr=Proc.STDOUT)
			proc2.exec(pipeprocess, addpath=addpath,
				   stdout=Proc.PIPE)
			proc2.wait()
			stat = proc1.wait(timeout)
		else:
			proc1 = Proc(self.verbose, self.dry_run)
			proc1.exec('%s %s' % (exefile, args),
				stdout=tmplog, stderr=Proc.STDOUT)
			stat = proc1.wait(timeout)

		# merge log info.
		if logf:
			tmpf = TextFio(tmplog)
			if tmpf.open() < 0:
				msg = '__run: open error: "%s"' % tmplog
				self.E.print(msg, alive=True)
			else:
				lines = tmpf.read()	
				logf.writelines(lines)
				tmpf.close()
			logf.close()

		os.chdir(dirsave)
		return stat


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
			self.E.print(msg, alive=True)
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
	def __open_log(self, fname, fmode, prefix):
		# arguments:
		#   logfile:	Log file path.
		# returns:	Log file object.

		if self.dry_run:
			print('  open log file "%s"' % fname)
			return None

		logdir = self.__dirpart(fname)
		if logdir != '':
			os.makedirs(logdir, exist_ok=True)
		logf = TextFio(fname, mode=fmode)
		if logf.open() < 0:
			msg = '%s: open error: "%s"' % (prefix, fname)
			self.E.print(msg, alive=True)
			logf = None
		return logf

	#  Call compiler (for unix).
	#
	def __build_u(self, logf):
		# arguments:
		#   logf:	Log file object.
		#   self.args:	Other parameters.
		[slnfile, opts, outfile, force] = self.args

		cmnd = 'make -f %s' % slnfile
		args = '%s -o %s' % (opts, outfile)
		proc = Proc(self.verbose, self.dry_run)
		proc.exec('%s %s' % (cmnd, args),
				stdout=Proc.PIPE, stderr=Proc.STDOUT)
		stat = proc.wait()
		out, err = proc.output()
		if logf:
			logf.witelines(out)
		return stat

	#  Call compiler (for Windows).
	#
	def __build_w(self, logf, tmpdir):
		# arguments:
		#   logf:	Log file object.
		#   tmpdir:	Temporary directory for logging.
		#   self.args:	Other parameters.
		[slnfile, opts, outfile, force] = self.args

		outdir = self.__dirpart(outfile).replace('x86', 'Win32')
		tmplog = 'log/%s_%s_%s_%s_build.log' % \
				(self.clsname, self.ccver, self.arch, config)
		FileOp().rm(tmplog)
		if self.verbose > 1:
			print('build solution (Windows)')
			print('  slnfile: %s' % slnfile)
			print('  opts:    %s' % opts)
			print('  outdir:  %s' % outdir)
			print('  errlog:  %s' % errlog)
			print('  tmplog:  %s' % tmplog)
			print('  force:   %s' % force)

		vs = VisualStudio(self.ccver, self.verbose)
		vs.solution(slnfile)
		vs.set(VisualStudio.OUTDIR, outdir, force)
		vs.set(VisualStudio.LOGFILE, tmplog)
		vs.set(VisualStudio.DRYRUN, self.dry_run)
		if vs.has_error():
			self.E.print(vs.has_error())
		stat = vs.build(self.arch, opts)
		if logf:
			tmpf = TextFio(tmplog)
			if tmpf.open() < 0:
				msg = '__build_w: open error: "%s"' % tmplog
				self.E.print(msg, alive=True)
			else:
				lines = tmpf.read()	
				logf.writelines(lines)
				tmpf.close()
		return stat

# ----------------------------------------------------------------------
#  Test main
# ----------------------------------------------------------------------
if __name__ == '__main__':
	from optparse import OptionParser

	usage = 'Usage: %prog [options]'
	parser = OptionParser(usage = usage)
	parser.add_option('-d', '--directory', dest='directory',
			default=None,
			help='solution directory', metavar='DIR')
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

	directory = options.directory
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
			Error('test').print('no such directory: %s' % directory)
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
				solution += '.sln'
		bar = BuildAndRun(toolset, platform, verbose, dry_run)
		stat = bar.build(directory,
				solution,
				config,
				'%s/%s/%s/%s.exe' % \
					(toolset, platform, config, solution[:-4]),
				bldlog,
				force)
		print('build: stat %d' % stat)
		if stat == 0:
			stat = bar.run(directory,
				solution[:-4] + '.exe',
				'',
				runlog,
				timeout)
			print('run: stat %d' % stat)

	sys.exit(0)

# end: BuildAndRun.py

#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:
#	VisualStudio:
#	    Wrapper class for invoking Visual Studio from command line.
#
#  INITIALIZER:
#	obj = VisualStudio(toolset, verbose=0)
#	arguments:
#	    toolset:	Platform toolset.
#			e.g. '14.0', '14', 'v140', '2015'
#	    verbose:	Verbose level (int) (0: silent).
#
#  METHODS:
#	solution(name) 
#	    Set solution file name.
#	    arguments:
#	    name:	Solution name (str). Solution file name is
#			determined by 'name' and Visual Studio version.
#
#	set(VisualStudio.OUTDIR, dname, clean=True)
#	set(VisualStudio.LOGFILE, fname)
#	set(VisualStudio.DRYRUN, flag)
#	    OUTDIR:	Set output directory and clean flag.  If clean
#			flag is True, output directory will be cleaned
#			before build (i.e. rebuild).
#	    LOGFILE:	Set log file path.  All output to stdoout and
#			stderr will be redirected to this file.
#	    DRYRUN:	Set/unset dry run flag.  If this flag is True
#			show command but do not execute it (for debug).
#	    argumnets:
#		dname:	    Output directory name (str).
#		clean:	    Clean flag (bool).
#		fname:	    Log file name (str).
#		flag:	    Dry_run flag (bool).
#
#	msg = error()
#	    Check if something wrong has occured in setup process.
#	    returns:	Error message if error has occurred so far.
#			None if recorded no error.
#
#	status = build(platform, config)
#	    Set up build environment and do build (self.__exec()).
#	    arguments:
#		platform:   'x86', 'x64'
#		config:	    'Debug', 'Release', 'Trace'
#	    returns:	Build status code (int).
#
#	string = get(VisualStudio.VERSION)
#	string = get(VisualStudio.TOOLSET)
#	    VERSION:	Return Visual Studio version (str).
#			e.g. '14.0'
#	    TOOLSET:	Return Tool set name and VS name (str[]).
#			e.g. ['140', 'Visual Studio 2015']
#
#	info()
#	    Print setup information of this instance.
#
# ----------------------------------------------------------------------
#  VERSION:
#     Ver 1.00   2016/10/06 F.Kanehori	First version.
#     Ver 1.01   2016/11/09 F.Kanehori	Add method: get_toolset.
#     Ver 1.02   2017/09/07 F.Kanehori	Python library revised.
#     Ver 1.03   2017/09/14 F.Kanehori	Change return value: error().
#     Ver 1.04   2017/11/16 F.Kanehori	Python library path 変更.
#     Ver 1.05   2017/11/30 F.Kanehori	Python library path 変更.
#     Ver 2.00   2018/02/07 F.Kanehori	全体の見直し.
#     Ver 2.01   2018/12/25 F.Kanehori	Visual Studio 2017 に対応.
#     Ver 2.02   2020/06/11 F.Kanehori	devenv 探索方式変更.
# ======================================================================
#from __future__ import print_function
import sys
import os

# local python library
#
sys.path.append('../../src/RunSwig')
from FindSprPath import *
spr_path = FindSprPath('VisualStudio')
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from Error import *
from Util import *
from Proc import *
from FileOp import *

class VisualStudio:

	#  Class constants.
	#
	VERSION		= 1
	TOOLSET		= 2
	DRYRUN		= 3
	OUTDIR		= 4
	LOGFILE		= 5
	COMMAND		= 6

	#  Class initializer.
	#
	def __init__(self, toolset, verbose=0):
		self.clsname = self.__class__.__name__
		self.version = 2.2
		#
		self.verbose = verbose
		pts, vsv, vsn = self.__get_vsinfo(toolset)
		self.pf_toolset = pts	# e.g. v140
		self.vs_version = vsv	# e.g. 14.0
		self.vs_name = vsn	# e.g. Visual Studio 2015
		self.vs_path = None
		#
		self.devenv_found = None
		self.solution_name = None
		self.solution_dir = os.getcwd()
		self.solution_file = None
		self.platform = None
		self.config = None
		self.outdir = None
		self.clean = False
		self.logfile = None
		self.dry_run = False
		#
		self.cmnd = None
		self.errmsg = None
		#
		self.vs_path = self.__get_vs_path(self.vs_version)

	#  Set solution file name.
	#
	def solution(self, name):
		if self.vs_version is None:
			return
		vs_version = self.vs_version
		if name[-4:] == '.sln':	name = name[0:-4]
		# solution file name includes VS version.
		self.solution_name = name
		#### self.solution_file = name + vs_version + '.sln'
		self.solution_file = name + '.sln'
		solution_path = '%s/%s' % (self.solution_dir, self.solution_file)
		if not os.path.exists(solution_path):
			# no such solution file
			self.solution_name = None

	#  Set various parameters to Visual Studio.
	#
	def set(self, func, arg1, arg2=True):
		if func == self.OUTDIR:
			self.__need_arg(1, arg1)
			self.__type_check(1, arg1, str)
			self.outdir = Util.upath(arg1)
			self.clean = arg2

		elif func == self.LOGFILE:
			self.__need_arg(1, arg1)
			self.__type_check(1, arg1, str)
			self.logfile = Util.upath(arg1)

		elif func == self.DRYRUN:
			self.dry_run = arg1
		else:
			msg = 'set: invalid function specified: %s' % str(func)
			Error(self.clsname).abort(msg)

	def __need_arg(self, num, arg):
		if arg is None:
			msg = 'set: argument required: arg%d' % num
			Error(self.clsname).abort(msg)

	def __type_check(self, num, arg, type):
		if not isinstance(arg, type):
			msg = 'set: invalid argument: arg%d' % num
			Error(self.clsname).abort(msg)

	#  Build solution.
	#
	def build(self, platform, config):
		# check if Visual Studio is available.
		#
		status = -1
		if self.vs_path is None:
			msg = self.vs_name + ' (' + self.pf_toolset + ')'
			self.errmsg = 'devenv not found: ' + msg
			print()
			Error(self.clsname).abort(self.errmsg)
			return status

		# prepare log directory
		#
		if self.logfile is not None:
			logdir = '/'.join(self.logfile.split('/')[:-1])
			os.makedirs(logdir, exist_ok=True)
		# do build
		#
		if platform == 'x86':
			platform = 'Win32'
		self.platform = platform
		self.config = config
		status = self.__build()
		if status != 0:
			self.errmsg = 'build failed (%d)' % status
		#
		return status

	#  Get verious information about VS.
	#
	def get(self, kind):
		if kind == self.VERSION:
			return self.vs_version
		elif kind == self.TOOLSET:
			return self.pf_toolset, self.vs_name
		elif kind == self.COMMAND:
			return self.cmnd
		msg = 'set: invalid argument'
		Error(self.clsname).abort(msg)

	#  Print setup information.
	#
	def info(self):
		print('  toolset   : %s (%s)' % (self.vs_name, self.pf_toolset))
		print('  vs_version: %s' % self.vs_version)
		print('  VS path   : %s' % self.vs_path)
		print('  solution  : %s' % self.solution_name)
		print('  directory : %s' % self.solution_dir)

	#  Check if some error has occured so far.
	#
	def error(self):
		msg = None
		if self.vs_version is None:
			msg = 'Visual Studio version is not specified'
		elif self.devenv_found is None:
			msg = self.errmsg
		elif self.solution_name is None:
			msg = 'file not found: "%s"' % self.solution_file
		if msg is not None:
			msg = '%s: %s' % (self.clsname, msg)
		return msg


	# --------------------------------------------------------------
	#  For class private use
	# --------------------------------------------------------------

	#  Get Visual Studio version information.
	#
	def __get_vsinfo(self, toolset):
		# arguments:
		#   toolset:	Toolset specifier (one of follows).
		# returns:
		#   Version representing triplet.

		pts, vsv, vsn = (None, None, None)
		if toolset in ['14.0', '14', 'v140', '2015']:
			pts, vsv, vsn = ('v140', '14.0', 'Visual Studio 2015')
		elif toolset in ['15.0', '15', 'v141', '2017']:
			pts, vsv, vsn = ('v141', '15.0', 'Visual Studio 2017')
		#
		if pts is None:
			self.errmsg = 'invalid platform toolset: %s' % (toolset)
		if self.verbose and pts is not None:
			print('  platform toolset: %s (%s)' % (vsn, pts))
			print('  VisualStudio ver: %s' % (vsv))
		return pts, vsv, vsn

	#  Get path of Visual Studio executable (i.e. devenv).
	#
	def __get_vs_path(self, version):
		# arguments:
		#   version:	Visual Studio version. Must have the format
		#		that self.__get_vsinfo() returns.
		# returns:	Path of devenv.exe.

		if version is None:
			# bad VS version
			return None
		devenv = 'devenv.exe'
		devenvpath = None
		# try 'where' command first
		proc = Proc(verbose=0).execute('where %s' % devenv,
					       stdout=Proc.PIPE, shell=True)
		stat, out, err = proc.output();
		if stat == 0:
			out = out.split('\n')[0].strip()
			devenvpath = '/'.join(out.split(os.sep)[:-1])

		# try default install directory
		elif self.vs_version <= '14.0':
			devenvpath = 'C:/Program Files (x86)/Microsoft Visual Studio '
			devenvpath += version
			devenvpath += '/Common7/IDE'
		else:
			sys_base = 'C:/Program Files (x86)/Microsoft Visual Studio'
			devenvpath = sys_base \
				   + '/%s' % self.vs_name.split()[2] \
				   + '/Community/Common7/IDE'
		#if self.verbose:
		#	print('  devenv path: %s' % devenvpath)
		if devenvpath is None \
		   or not os.path.exists(Util.pathconv('%s/%s' % (devenvpath, devenv))):
			self.errmsg = 'devenv not found'
			return None
		#
		print('devenvpath: %s' % devenvpath)
		sys.stdout.flush()
		if devenvpath is not None:
			devenv_found = True
		return devenvpath

	#  Build solution.
	#
	def __build(self):
		# returns:	Build status.

		# build option
		build_opt = '"%s|%s"' % (self.config, self.platform)
		if self.verbose:
			print('  build option: %s' % build_opt)

		# delete all files in outdir before build (rebuild)
		if self.clean:
			fop = FileOp(verbose=self.verbose, dry_run=self.dry_run)
			status = fop.rm(self.outdir)
			status = fop.rm(self.logfile)
			if status != 0 and not self.dry_run:
				self.errmsg = 'file deletion failed (%d)' % status
				return status

		# build
		cmnd = 'devenv %s /build %s' % (self.solution_file, build_opt)
		self.cmnd = cmnd	# for log output
		cmnd += ' /Out %s' % Util.pathconv(self.logfile)
		proc = Proc(verbose=self.verbose, dry_run=self.dry_run)
		proc.execute(cmnd, addpath=Util.pathconv(self.vs_path))
		status = proc.wait()
		if status != 0 and self.verbose > 1:
			self.__show_error(self.logfile)
		if self.verbose:
			print('  result is %d' % status)
		return status

	#  Print build error information in the log file.
	#
	def __show_error(self, file):
		# file:		Pathof build log file.
		if not os.path.exists(file):
			return
		# extract lines containing 'error' from file
		try:
			f = open(file, 'r')
		except IOError as err:
			return None
		for line in f:
			if 'error' in line or 'Error' in line:
				print(line.strip())
		f.close()


# ----------------------------------------------------------------------
#  Test main
# ----------------------------------------------------------------------
if __name__ == '__main__':
	from optparse import OptionParser

	usage = 'Usage: %prog [options]'
	parser = OptionParser(usage = usage)
	parser.add_option('-d', '--directory',
			dest='directory', default=None,
			help='directoy where solution file exists', metavar='DIR')
	parser.add_option('-t', '--toolset', dest='toolset',
			default='14.0',
			help='Visual Studio version [default: %default]')
	parser.add_option('-D', '--dry-run',
			dest='dry_run', action='store_true',
			default=False, help='set dry-run mode')
	parser.add_option('-v', '--verbose',
			dest='verbose', action='count',
			default=0, help='set verbose mode')
	(options, args) = parser.parse_args()

	vs_version= '14.0'
	if options.directory is None:
		def __vs(ver):
			verbose = 0
			print('%s =>' % ver)
			vs = VisualStudio(ver, verbose)
			#vs.solution('test/Quaternion')
			vs.solution('test/test')
			if not vs.error():
				vs.info()
		print('-- normal case --')
		__vs(vs_version)
		print('')
		print('-- error case --')
		__vs('11.0')
	else:
		cwd = os.getcwd()
		os.chdir(options.directory)
		toolset = options.toolset
		dry_run = options.dry_run
		verbose = options.verbose
		
		print('Visual Studio')
		vs = VisualStudio(toolset, verbose)
		solution_name = os.getcwd().split(os.sep)[-1:][0]
		print(solution_name)
		vs.solution(solution_name + options.toolset)
		if vs.error():
			os.chdir(cwd)
			sys.exit(-1)
		if verbose: print('')

		def test(platform, config):
			outdir = os.sep.join([vs_version, platform, config])
			logfile = 'log/' + vs.clsname + '_' + toolset + '_'
			logfile += platform + '_' + config + '_' + 'build.log'
			vs.set(vs.OUTDIR, outdir)
			vs.set(vs.LOGFILE, logfile)
			vs.set(vs.DRYRUN, dry_run)	# default: False

			print('=> (%s, %s)' % (platform, config))
			status = vs.build(platform, config)
			print('status: %d' % status)
			return status

		status = test('x86', 'Debug')
		if status != 0:
			input('Hit any key')
		print('')
		status = test('x64', 'Release')

		os.chdir(cwd)
	# end if
	sys.exit(0)

# end: VisualStudio.py

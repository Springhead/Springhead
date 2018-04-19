#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:
#	Traverse:
#	    Traverse and test specified tree recursively.
#
#  INITIALIZER:
#	obj = Traverse(result, csc, control, section,
#			toolset, platforms, configs, csusage, rebuild,
#			timeout, report=True, audit=False,
#			dry_run=False, verbose=0)
#	arguments:
#	    result:	TestResult class object (obj).
#	    csc:	ClosedSrcControl class object (obj).
#	    control:	Test control file name (str).
#	    section:	Test section name (str).
#	    toolset:	C-compiler version.
#			    Windows: Visual Studio version (str).
#			    unix:    gcc version (dummy).
#	    platforms:	Platforms to be tested (PLATS).
#	    conifgs:	Configurations to be tested (CONFS).
#	    csusage:	Closed source usage (CSU).
#	    rebuild:	Force rebuild (bool).
#	    timeout:	Time out value in sec (0: not set).
#	    report:	Enable progress report (bool).
#	    dry_run:	Show commnad but not execute (bool).
#	    verbose:	Verbose mode level (0: silent).
#
#  METHODS:
#	traverse(top)
#	    Start travers.
#	    arguments:
#	        top:	Top directory name (str).
#	    returns:	    0: succ, -1: fail
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2018/02/26 F.Kanehori	First version.
#	Ver 1.01 2018/03/14 F.Kanehori	Dealt with new Error class.
#	Ver 1.1  2018/03/15 F.Kanehori	Bug fixed (for unix).
#	Ver 1.11 2018/03/28 F.Kanehori	Bug fixed (for unix).
#	Ver 1.12 2018/04/19 F.Kanehori	Special trap introduced.
# ======================================================================
import sys
import os

from ControlParams import *
from BuildAndRun import *
from KeyInterruption import *
from StructuredException import *

# local python library
#
sys.path.append('../../src/RunSwig')
from FindSprPath import *
spr_path = FindSprPath('Traverse')
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from Error import *
from Util import *
from Proc import *
from TextFio import *
from FileOp import *

class Traverse:

	#  Class initializer.
	#
	def __init__(self, testid,
			result, csc, control, section,
			toolset, platforms, configs, csusage, rebuild,
			timeout, report=True, audit=False,
			dry_run=False, verbose=0):
		self.clsname = self.__class__.__name__
		self.version = 1.1
		#
		self.testid = testid
		self.result = result
		self.csc = csc
		self.control = control
		self.section = section
		self.toolset = toolset
		self.platforms = self.__sort_platforms(platforms)
		self.configs = self.__sort_configs(configs)
		self.csusage = csusage
		self.rebuild = rebuild
		self.timeout = timeout
		self.report = report
		self.audit = audit
		self.dry_run = dry_run
		self.verbose = verbose
		#
		# this is local convention
		if control == 'dailybuild.control':
			self.is_dailybuild = True
		else:
			self.is_dailybuild = False
		self.trap_enabled = False	#### special trap ####
		#
		self.encoding = 'utf-8' if Util.is_unix() else 'cp932'
		self.once = True
		self.trace = True
		self.fop = FileOp()

	#  Compile the solution.
	#
	def traverse(self, top):
		#### special trap for manual test ####
		skips = os.getenv('SPR_SKIP')
		if self.trap_enabled and top.split('/')[-1] in skips:
			print('skip: %s' % top)
			return 0
		#### end trap ####
		if not os.path.isdir(top):
			msg = 'not a directory: %s' % top
			Error(self.clsname).error(msg)
			return 0
	
		# go to test directory
		dirsave = self.__chdir(top)
		cwd = Util.upath(os.getcwd())
		
		# read control file
		ctl = ControlParams(self.control, self.section, verbose=self.verbose)
		if ctl.error():
			Error(self.clsname).error(ctl.error())
			return -1
		if self.once:
			self.__init_log(ctl.get(CFK.BUILD_LOG), RST.BLD)
			self.__init_log(ctl.get(CFK.BUILD_ERR_LOG), RST.BLD, RST.ERR)
			self.__init_log(ctl.get(CFK.RUN_LOG), RST.RUN)
			self.__init_log(ctl.get(CFK.RUN_ERR_LOG), RST.RUN, RST.ERR)
			self.once = False
			print()

		# check test condition
		is_cand = self.__is_candidate_dir(cwd)
		exclude = ctl.get(CFK.EXCLUDE, False)
		has_sln = self.__has_solution_file(ctl, cwd, self.toolset)
		descend = ctl.get(CFK.DESCEND) and is_cand and not exclude
		do_this = is_cand and not exclude and has_sln
		#
		interrupted = False
		stat = 0
		if do_this:
			if self.audit:
				print('ENTER: %s' % cwd)
			if self.verbose:
				ctl.info()
			stat = self.process(cwd, ctl)
			if stat == Proc.ECANCELED:
				interrupted = True
		elif self.audit:
			if not is_cand: msg = 'not a candidate dir'
			if exclude:	msg = 'exclude condition'
			if not has_sln: msg = 'has no solution file'
			print('skip: -%s (%s)' % (cwd, msg))

		# process for all subdirectories
		if descend and not interrupted:
			for item in sorted(os.listdir(cwd)):
				if not os.path.isdir(item):
					continue
				if not self.__is_candidate_dir(item):
					continue
				subdir = '%s/%s' % (cwd, item)
				stat = self.traverse(subdir)
				if stat == Proc.ECANCELED:
					break

		# all done for this directory and decsendants.
		if self.audit and do_this:
			print('LEAVE: %s' % cwd)
		if dirsave:
			os.chdir(dirsave)
		return stat

	#  Process for one solution file.
	#
	def process(self, cwd, ctl):
		if not os.path.isdir(cwd):
			msg = 'not a directory: %s' % cwd
			Error(self.clsname).error(msg)
		#
		slnfile = self.__solution_file_name(ctl, cwd, self.toolset)
		if self.verbose > 1:
			print('solution file: %s' % slnfile)

		# closed source control
		if self.csusage == CSU.AUTO:
			tmp_u = ctl.get(CFK.USE_CLOSED_SRC)
			usage = CSU.USE if tmp_u else CSU.UNUSE
		else:
			usage = self.csusage
		self.csc.set_usage(usage)

		# process this directory
		name = self.__report_1('\t', True, False)
		bar = BuildAndRun(self.toolset, self.verbose, self.dry_run)
		if bar.error():
			self.__report_1(bar.error(), False, True)
			self.result.set_info(name, TST.ERR, bar.error())
			return -1
		#
		self.result.set_info(name, RST.EXP, ctl.get(CFK.EXPECTED))
		stat = 0
		for platform in self.platforms:
			# need build?
			if not ctl.get(CFK.BUILD):
				continue
			#
			if platform != self.platforms[0]:
				self.__report('  ', None, False)
			self.__report(' %s:' % platform, None, False)
			self.platform = platform

			stat = 0
			for config in self.configs:
				#
				# Build (compile and link)
				#
				self.__report('%s' % config, '.build', False)
				self.config = config
				outpath = self.__make_outpath(ctl, slnfile)
				stat = bar.build(None,
						slnfile,
						platform,
						config,
						outpath,
						ctl.get(CFK.BUILD_LOG),
						ctl.get(CFK.BUILD_ERR_LOG),
						self.rebuild)
				if KEYINTR.is_interrupted():
					KEYINTR.set_interrupted(False)
					stat = Proc.ECANCELED
				self.result.set_result(name, RST.BLD,
						platform, config,
						stat)
				#
				self.__report(None, '.', False, False)
				if self.verbose:
					print(self.__status_str(RST.BLD, stat))
				if stat != 0:
					self.__report(None, None, False, True)
					if stat == Proc.ECANCELED:
						print('interrupted')
						break
					if self.verbose:
						print('build error (%d)' % stat)
					continue
				#
				# Run
				#
				if not ctl.get(CFK.RUN):
					self.__report(',', '(skip). ', False)
					continue
				if ctl.get(CFK.INTERVENTION):
					self.__report(',', 'intervention. ', False)
					continue
				self.__report(None, 'run', False)
				#
				addpath = self.__runtime_addpath(ctl, platform)
				if Util.is_unix():
					outpath = slnfile
				stat = bar.run(None,
						outpath,
						'',	# no args
						ctl.get(CFK.RUN_LOG),
						ctl.get(CFK.RUN_ERR_LOG),
						addpath,
						ctl.get(CFK.TIMEOUT, default=0),
						ctl.get(CFK.PIPE_PROCESS))
				kill_proc = ctl.get(CFK.KILL_PROCESS)
				if kill_proc:
					Proc().kill(image=kill_proc)
				if KEYINTR.is_interrupted():
					KEYINTR.set_interrupted(False)
					stat = Proc.ECANCELED
				self.result.set_result(name, RST.RUN,
						platform, config,
						stat)
				if stat == Proc.ECANCELED:
					self.__report(None, None, False, True)
					if self.verbose:
						print('interrupted')
					break
				#
				if self.verbose:
					print(self.__status_str(RST.RUN, stat))
				if stat == Proc.ETIME:
					self.__report('', '(timedout)', False, False)
				self.__report(',', ' (rc %s).' % stat, False, False)

			# end config
			if stat == Proc.ECANCELED:
				break
		# end platform

		self.__report_1(None, False, True)
		return stat


	# --------------------------------------------------------------
	#  For class private use
	# --------------------------------------------------------------

	#  Sort platform and configuration names.
	#
	def __sort_platforms(self, platforms):
		# arguments:
		#   platforms:	A list of platform names ([str]).
		# returns:	Sorted list ([str]).

		names = []
		for x in PLATS:
			if x in platforms:
				names.append(x)
		return names

	def __sort_configs(self, configs):
		# arguments:
		#   configs:	A list of configuration names ([str]).
		# returns:	Sorted list ([str]).

		names = []
		for x in CONFS:
			if x in configs:
				names.append(x)
		return names

	#  Initialize log file.
	#
	def __init_log(self, path, step, errlog=None):
		# arguments:
		#   path:	Log file path (str).
		#   step:	Execution step (RST.BLD or RST.RUN).
		#   errlog:	Error log or not (RST.ERR or None).

		if path is None:
			return None

		# file header info
		testids = { TESTID.STUB: 'スタブ',
			    TESTID.TESTS: '',
			    TESTID.SAMPLES: 'サンプル',
			    TESTID.OTHER: 'その他' }
		steps = { RST.BLD: 'ビルド', RST.RUN: '実行' }
		datestr = '日付 : %s' % Util.date()
		err = 'エラー' if errlog == RST.ERR else ''
		header = '--- %s%s%sのログ ---' % \
			(testids[self.testid], steps[step], err)

		# write header
		f = TextFio(path, 'w', encoding=self.encoding)
		if f.open() < 0:
			msg = '__init_log: open error "%s"', path
			Error(self.clsname).error(msg)
			return
		f.writeline(datestr)
		f.writeline(header)
		f.writeline('')
		f.close()
		print('init logfile: "%s"' % path)

	#  Change directory.
	#
	def __chdir(self, path):
		# arguments:
		#   path:	Directory path to change (str).
		# returns:	Previous directory path (str).

		abspath = os.path.abspath(path)
		cwd = os.getcwd()
		if abspath == cwd:
			return None
		try:
			os.chdir(abspath)
		except:
			msg = 'chdir failed (%s)' % path
			Error(self.clsname).error(msg)
		return Util.upath(cwd)

	#  Is this directory a test candidate?
	#  This method is intended to eliminate unwilling directories
	#  from directory traverse.
	#
	def __is_candidate_dir(self, dir):
		# arguments:
		#   dir:	Directory name to be checked (str).
		# returns:	Reason for non-candidate (str).
		#		None if this directory is candidate.

		leaf = Util.upath(dir).split('/')[-1]

		# directory listed below or whose name begins with '.'
		# are not target directory.
		excludes = ['Template', 'log', self.toolset]
		if leaf[0] == '.' or leaf in excludes:
			return False
		return True

	#  Make solution file name.
	#
	def __solution_file_name(self, ctl, dir, ccver):
		# arguments:
		#   ctl:	ControlParams object (obj).
		#   dir:	Directory name (str).
		#   ccver:	C-compiler version (dummy for unix) (str).
		# returns:	Solution file name (str).

		leaf = Util.upath(dir).split('/')[-1]
		alias = ctl.get(CFK.SOLUTION_ALIAS)
		solution = alias if alias else leaf
		if Util.is_unix():
			fname = solution
		else:
			fname = '%s%s.sln' % (solution, ccver)
		return fname

	#  Has solution file?
	#
	def __has_solution_file(self, ctl, dir, ccver):
		# arguments:
		#   ctl:	ControlParams object (obj).
		#   dir:	Directory name (str).
		#   ccver:	C-compiler version (dummy for unix) (str).
		# returns:	Has solution file or not (bool).

		slnfile = self.__solution_file_name(ctl, dir, ccver)
		return os.path.exists(slnfile)

	#  Make output binary path.
	#
	def __make_outpath(self, ctl, slnfile):
		# arguments:
		#   ctl:	ControlParams object (obj).
		#   slnfile:	Solution file name (str).
		# returns:	Output directory path (str).

		outdir = ctl.get(CFK.OUTPUT_DIR)
		if outdir:
			outdir = outdir.replace('$TOOLSET', self.toolset)
			outdir = outdir.replace('$PLATFORM', self.platform)
			outdir = outdir.replace('$CONFIGURATION', self.config)
			outdir = outdir.replace('x86', 'Win32')
		else:
			outdir = '.'
		binary = ctl.get(CFK.BINARY_OUT, default=slnfile)
		binary = binary.replace('.sln', '')
		binary = binary.replace(self.toolset, '')
		outpath = '%s/%s' % (os.path.abspath(outdir), binary)
		return Util.upath(outpath)

	#  Make runtime additional path.
	#
	def __runtime_addpath(self, ctl, platform):
		# arguments:
		#   ctl:	ControlParams object (obj).
		#   platform:	Platform name ('x86' or 'x64')
		# returns:	Additional path (str).

		adds = []
		if ctl.get(CFK.ADD_PATH):
			adds.append(ctl.get(CFK.ADD_PATH))
		if Util.is_unix():
			delim = ':'
		else:
			delim = ';'
			bintop = '%s/dependency/bin' % spr_path.abspath()
			if platform == 'x86' or platform.lower() == 'win32':
				adds.append('%s/win32' % bintop)
			if platform == 'x64' or platform.lower() == 'win64':
				adds.append('%s/win64' % bintop)
				adds.append('%s/win32' % bintop)
		return delim.join(adds)

	#  Print report (with newline control).
	#
	def __report(self, msg_l, msg_s, header=True, newline=False):
		# arguments:
		#   msg_l:	Report message (long version) (str).
		#   msg_s:	Report message (short version) (str).
		#   header:	Print header of not (bool).
		#   newline:	Print newline code or not (bool).
		# returns:	Test name (str).

		if not self.report:
			return
		is_long = len(self.platforms) + len(self.configs) > 2
		if is_long and not msg_l:
			return
		elif not is_long and not msg_s:
			return
		msg = msg_l if is_long else msg_s
		self.__report_1(msg, header, newline)

	def __report_1(self, msg, header=True, newline=False):
		# arguments:
		#   msg:	Report message (str).
		#   header:	Print header of not (bool).
		#   newline:	Print newline code or not (bool).
		# returns:	Test name (str).

		if not self.report:
			return
		tmp = os.getcwd().split(os.sep)
		name = tmp
		for n in range(len(tmp)):
			if tmp[n] == 'core':
				name = '/'.join(tmp[n+2:])
		if name == '':
			name = tmp[-1]
		if header:
			sys.stdout.write('%s:\t' % name.replace('/', ': '))
		if msg:
			sys.stdout.write(msg)
		if newline:
			sys.stdout.write('\n')
		sys.stdout.flush()
		return name

	#  Error code and description.
	#
	def __status_str(self, step, status):
		# arguments:
		#   step:	Execution step (RST.BLD or RST.RUN).
		#   status:	Status code (Proc.const).
		# returns:	Message string associated with the code.
		if status == Proc.ECANCELED:
			s = '%d (keyboard interrupt)' % status
		elif status == Proc.ETIME:
			s = '%d (timed out)' % status
		elif SEH.is_seh(status):
			seh_code = SEH.seh_code(status)
			seh_str = SEH.seh_str(status)
			s = '%d (%s) %s' % (status, seh_code, seh_str)
		elif status == Proc.EINTR:
			s = '%d (unknown interruption)' % status
		else:
			name = 'build' if step == RST.BLD else 'run'
			s = '%d' % status
		return '  %s result is: %s' % (name, s)

# end: Traverse.py

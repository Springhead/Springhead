#!/usr/bin/env python
# -*- coding: utf-8 -*-
# =============================================================================
#  CLASS:
#	SpringheadTest
#	    Build-and-Run class for Springhead test.
#
#  INITIALIZER:
#	obj = SpringheadTest(cc, control_file, control_section,
#			     closed_src_control=0, verbose=0, dry_run=False)
#	arguments:
#	    cc:		Compiler class object (obj).
#			in case of Windows: Visual Studio class object.
#			in case of unix: Cc class object.
#	    control_file:
#			Test control file name ('dailybuild.control') (str).
#	    control_section:
#			Test control section name ('dailybuild') (str).
#	    closed_src_control:
#			Usage of closed source (0:auto, 1:use, 2:unuse).
#	    verbose:	Verbose level (0:silent) (int).
#	    dry_run:	Dry_run control (bool).
#
#  METHODS:
#	set_preprocessor_macro(macro, inherit=True)
#	    Replace preprocessor macro to specified one in build process.
#	    Original macro is saved before build, and revived after.
#	    arguments:
#		macro:	    Macro definition like unix's -D option (str).
#		inherit:    Inherit macros from parent or preprocessor's
#			    default settings.
#
#	set(SpringheadTest.CC_MACRO, name, value=None)
#	set(SpringheadTest.MAX_LOGLINES, num_lines)
#	set(SpringheadTest.OVERRIDE, kind, comp=None, run=None)
#	set(SpringheadTest.RECOMPILE, flag=True)
#	set(SpringheadTest.PROGRESS_REPORT, flag=True)
#	set(SpringheadTest.USE_CLOSED_SRC_FLAG, usage)
#	    CC_MACOR:		Set preprocessor macro.
#	    MAX_LOGLINES:	Set max number of lines to runtime log file.
#	    OVERRIDE:		Override control specified by control-file.
#	    RECOMPILE:		Force recompile by removeing binary file.
#	    PROGRESS_REPORT:	Enable progress report flag.
#	    argumnets:
#		name:	    Macro name (str).
#		value:	    Macro value (str).
#		num_lines:  Number of log lines (int).
#		kind:	    'exe':	    Override execution control.
#			    'timeout':  Override timeout control.
#		comp:	    Override value for compile stage (bool).
#		run:	    Override value for run stage (bool).
#		flag:	    Override value (bool).
#		usage:	    ControlParams.AUTO:	Obey control file.
#			    ControlParams.USE:	Force to use.
#			    ControlParams.UNUSE:	Force to unuse.
"""
#	set_max_loglines(max_loglines)
#	    Set maximum number of lines to runtime log file.
#	    arguments:
#		max_loglines:	Number of lines.
#
#	set_override_control(key, build, run)
#	    Override execution control specified by control file.
#	    arguments:
#		key:	    'exe': Override execution control.
#			    'log': Override log output control.
#		build,run:  Override value (bool).
#
#	set_force_rebuild(flag)
#	    Clearing .exe and .obj's before build to force rebuild.
#	    arguments:
#		flag:	    Force rebuild or not (bool).
#
#	set_progress_report(flag=True)
#	    Set progress report flag.  If this flag is set, progress
#	    of the test is reported to the console regardless of the
#	    verbose flag.
#	    arguments:
#		flag:	    Set verbose or not (bool).
"""
#
#	apply_use_closed_src(flag)
#	    Change use_closed_src condition by rewriting the macro
#	    definition in the header file.  Header file path will be
#	    obtained by ControlParams.get(ControlParams.CSU_HEADER).
#	    arguments:
#		flag:	    Usage of closed source (int) (1:use, 2:unuse).
#
#	results,visted = traverse(dirs, platforms, configs, topdir=False)
#	    Traverse diretory tree and compile-and-run targets.
#	    arguments:
#		dirs:	    Directories to be traversed.
#		platforms:  Platforms to be tested.
#		configs:    Configurations to be built.
#		topdir:	    Include directory 'dirs' itself or not (bool).
#			    Subdirectories under 'dirs' are always processed.
#	    returns:	Test results as follows:
#		result[ERR]:			 error info
#		result[BLD][use_closed_src]:	 use closed src or not
#		result[BLD][platform][config]:	 build status
#		result[RUN  ][platform][config]: run status
#		result[RUN  ][expected]:	 expected status
#	    		    where ERR/BLD/RUN are the constants belongs
#			    to the class ControlParmas.
#		visited:    List of directories where test actually did.
#
#	value = get(key)
#	    Wrapper for ControlParams.get() method.
#	    arguments:
#		key:	    Key to search.
#	    returns:
#		value:	    Value associated with key.
#
# -----------------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2016/11/17 F.Kanehori	First version.
#	Ver 1.1  2017/01/11 F.Kanehori	OK for ControlParams V1.2.
#	Ver 1.2  2017/01/12 F.Kanehori	Rename: set_force_rebuild().
#	Ver 1.3  2017/09/13 F.Kanehori	Correspond to repository reconstruction.
#	Ver 1.4  2017/09/21 F.Kanehori	Log file format changed.
#					Introduce serialization facility.
#	Ver 1.5  2017/11/16 F.Kanehori	Python library path ÇÃïœçX.
#	Ver 1.6  2017/11/30 F.Kanehori	Python library path ÇÃïœçX.
#	Ver 2.0  2017/12/11 F.Kanehori	Interface ÇÃå©íºÇµ.
# =============================================================================
import sys
import os
import re
import signal
import glob
import codecs
from ControlParams import *
from VisualStudio import *
from StructuredException import *

# local python library
#
sys.path.append('../../src/RunSwig')
from FindSprPath import *
spr_path = FindSprPath('SpringheadTest')
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from Util import *
from Proc import *
from FileOp import *
from TextFio import *

# ----------------------------------------------------------------------
#  Keyboard interruption handling class.
# ----------------------------------------------------------------------
class KEYINTR(Exception):
	interrupted = False

	@staticmethod
	def set_interrupted(flag=True):
		KEYINTR.interrupted = flag
	@staticmethod
	def is_interrupted():
		return KEYINTR.interrupted
	@staticmethod
	def what(self):
		return 'KEYINTR'

	@staticmethod
	def handler(signum, frame):
		print()
		print('Signal handler called with signal', signum)
		sys.stdout.flush()
		KEYINTR.set_interrupted()

# ----------------------------------------------------------------------
#  class SpringheadTest
# ----------------------------------------------------------------------
class SpringheadTest:

	def __init__(self, cc,
		     control_file='test.control', control_section=None,
		     use_closed_src=ControlParams.AUTO,
		     verbose=0, dry_run=False):
		self.clsname = self.__class__.__name__
		self.version = 1.7
		#
		self.cc = cc		# Visual Studio agent
		self.ctl = ControlParams(cc.get_version(),
					 control_file,
					 control_section,
					 verbose)
					# Test control parameters
		self.use_closed_src = use_closed_src
		self.control_file = control_file
		self.control_section = control_section
		self.cc_macro = None
		self.putlog = [True, True]	# [build, run]
		self.max_loglines = 0
		self.recompile = False
		self.override = {}
		self.override['exe'] = [None, None, None]
		self.override['log'] = [None, None, None]
		self.override['timeout'] = [None, None, None]
		self.spr_topdir = None
		#
		self.use_closed_src_cond = None
		self.progress_report = False
		self.verbose = verbose
		self.dry_run = dry_run
		#
		self.PPDEF = 'PreprocessorDefinitions'
		self.tmp = {}

"""
	#  Replace preprocessor macro to specified one in build process.
	#  Original macro is saved before build, and revived after.
	#
	def set_preprocessor_macro(self, macro, inherit=True):
		self.cc_macro = macro
		if inherit:
			self.cl_macro += ';%%(%s)' % self.PPDEF
"""

	#  Set various parameters to Visual Studio.
	#
	def set(self, func, arg1, arg2, arg3):
		if arg1 is None:
			msg = 'set: argument required: arg1'
			Error(self.clsname).print(msg)

		if func == self.CC_MACRO:
			if is not instance(arg1, list):
				arg1 = [arg1]
			if Util.is_unix():
				arg1 = list(map(lambda x: '-D %s' % x, arg1)
				macro = ' '.join(arg1)
			else:
				arg1.append('%%(%s)' % self.PPDEF)
				macro = ';'.join(arg1)
			self.cc_macro = marco

		elif func == self.MAX_LOGLINES:
			if not isinstance(arg1, int):
				msg = 'set: invalid argument: arg1'
				Error(self.clsname).print(msg)
			self.max_loglines = arg1

		elif func == self.OVERRIDE:
			ok_kind = ['exe', 'timeout']
			if arg1 not in ok_kind:
				msg = 'set: invalid argument: arg1'
				Error(self.clsname).print(msg)
			if arg2:
				self.override[arg1][self.BLD] = arg2
			if arg2:
				self.override[arg1][self.RUN] = arg2

		elif func == self.RECOMPILE:
			if arg1 is None:
				arg1 = True
			self.recompile = arg1

		elif func == self.PROGRESS_REPORT:
			if arg1 is None:
				arg1 = True
			self.progress_report = arg1

		elif func == self.USE_CLOSED_SRC_FLAG:

		else:
			msg = 'set: invalid function number'
			Error(self.clsname).print(msg)

"""
	#  Set maximum number of lines to runtime log file.
	#
	def set_max_loglines(self, max_loglines):
		self.max_loglines = max_loglines

	#  Override execution control specified by control file.
	#
	def set_override_control(self, key, build, run):
		self.override_control[key][ControlParams.BLD] = build
		self.override_control[key][ControlParams.RUN] = run

	#  Clearing .exe and .obj's before build to force rebuild.
	#
	def set_force_rebuild(self, flag):
		self.force_rebuild = flag

	#  Set progress report flag.
	#
	def set_progress_report(self, flag=True):
		self.progress_report = flag
"""

	#  Change use_closed_src condition by rewriting the macro
	#  definition in the header file.
	#
	def apply_use_closed_src(self, flag):
		if flag == self.use_closed_src_cond:
			return
		incdir = spr_path.abspath('inc')
		"""
		hdir = self.__find_dir('src')
		if hdir is None:
			hdir = self.__find_dir('test')
		"""
		hfile = self.ctl.get(self.ctl.CS_CTRL_HEADER)
		##hpath = Util.pathconv('%s/include/%s' % (hdir, hfile))
		hpath = Util.pathconv('%s/%s' % (incdir, hfile))
		f = TextFio(hpath, encoding='utf8')
		if f.open() < 0:
			print('%s' % f.error())
			return
		lines = f.read()
		f.close()
		#
		patt_def = '#define\s+USE_CLOSED_SRC'
		patt_udf = '#undef\s+USE_CLOSED_SRC'
		search = patt_udf if flag else patt_def
		str_fm = '#undef'  if flag else '#define'
		str_to = '#define' if flag else '#undef'
		patterns = [str_fm, str_to, search]
		replaced, count = f.replace(patterns, lines)
		if count > 0:
			f = TextFio(hpath, 'w', encoding='utf8')
			if f.open() < 0:
				return
			f.writelines(replaced)
			if self.verbose:
				msg = 'USE_CLOSED_SRC changed'
				print('%s: [%s] -> [%s], file: "%s"'
					% (msg, str_fm, str_to, Util.upath(hpath)))
		self.use_closed_src_cond = flag

	#  Traverse diretory tree and buil-and-run solutions.
	#
	def traverse(self, dirs, platforms, configs,
			topdir=False, results=None, visited=None):
		coredir = spr_path.abspath('core')
		self.spr_topdir = coredir
		"""
		spr_topdir = self.__find_dir('test')
		if spr_topdir is None:
			spr_topdir = self.__find_dir('src')
		self.spr_topdir = spr_topdir
		"""
		if self.verbose:
			print('Springhead top: %s' % Util.upath(spr_topdir))
		abs_dirs = [os.sep.join([spr_topdir, x]) for x in dirs]
		#
		if results is None: results = {}
		if visited is None: visited = []

		# traverse start.
		signal.signal(signal.SIGINT, KEYINTR.handler)
		for root in abs_dirs:
			err = self.__traverse_sub(root, platforms, configs,
					topdir, results, visited)
		return results, visited

	#  Wrapper of ControlParams.get() method.
	#
	def get(self, key):
		if self.ctl is None:
			return None
		return ctl.get(key)

	# --------------------------------------------------------------
	#  For class private use
	# --------------------------------------------------------------

	#  Traverse sub method (recursively called).
	#
	def __traverse_sub(self, root, platforms, configs,
				topdir, results, visited):
		# root:		Test top directory.
		# platforms:	Platforms to be tested.
		# configs:	Configurations to be built.
		# include_optdir:
		#		Include top directory or not.
		# results:	Dictionary to set test result.
		# visited:	List to set tested directories.
		if self.verbose > 1:
			method = sys._getframe().f_code.co_name
			print('enter: %s(%s)' % (method, Util.upath(root)))

		# process for subdirectories
		for dir in os.listdir(root):
			path = os.sep.join([root, dir])
			if not self.__is_candidate_dir(path):
				if self.verbose > 1:
					print('recurse: not directory: %s' % dir)
				continue
			if topdir and not self.__has_solution_file(path):
				if self.verbose > 1:
					print('recurse: no solution-f: %s' % dir)
				continue
			err = self.__traverse_sub(path, platforms,
					configs, True, results, visited)
			if err < 0:
				return err

		# end of subdirectories
		if root in visited:
			print('(already visited): %s' % Util.upath(root))
			return 0
		err = 0
		if topdir and self.__has_solution_file(root):
			result, err = self.__build(root, platforms, configs)
			if not KEYINTR.is_interrupted():
				results[root] = result
				if err != 1:	# use_closed_src agree
					visited.append(root)
		# 
		if self.verbose > 1:
			print('leave: %s(%s)' % (method, Util.upath(root)))
		return err

	#  Build and run for solution in specified directory (dir).
	#  All platforms and configurations will be tried.
	#  Results are returned as a following structure.
	#	result[CTL.ERR]:		   error info
	#	result[CTL.BLD][platform][config]: build result
	#	result[CTL.RUN][platform][config]: run result
	#	result[CTL.RUN][expected]:	   expected status
	#	where CTL stands for ControlParams.
	#
	def __build(self, dir, platforms, configs):
		# arguments:
		#   dir:	Directory in where test will be done.
		#   platforms:	Platforms to be tested.
		#   configs:	Configurations to be built.
		# returns:
		#   result:	Compile/run result.
		#   status:	0: normal, 1: skipped, -1: error
	
		# initialize
		result = self.__init_result(platforms, configs)

		# go to the test directory
		os.chdir(dir)
		progress_report = self.progress_report or self.verbose
		progress_report_only = self.progress_report and not self.verbose
		if progress_report:
			sys.stdout.write(Util.upath(os.getcwd()) + '\t')
			sys.stdout.flush()

		# take account test control values
		ctl = ControlParams(self.cc.get_version(),
				self.control_file,
				self.control_section, self.verbose)

		# exclude
		if ctl.get(ctl.EXCLUDE):
			msg = 'by exclude condition'
			if progress_report:
				print('  skip: %s' % msg)
			if self.verbose > 1:
				print('leave: %s' % msg)
			return result, 1

		# solution
		self.cc.solution(ctl.get(ctl.SOLUTION))
		if self.cc.has_error() or ctl.has_error():
			msg_cc = self.cc.has_error()
			msg_ctr = ctl.has_error()
			msg = msg_cc if msg_cc else msg_ctl
			if progress_report:
				print('  Error: %s' % msg)
			if self.verbose > 1:
				print('leave: %s' % msg)
			result[ctl.ERR] = msg
			return result, -1
	
		# use closed source or not
		ctl.closed_src_control(self.use_closed_src)
		use_closed_src = ctl.get(ctl.USE_CLOSED_SRC)
		closed_src_control = ctl.get(ctl.CS_CONTROL)
		if ((use_closed_src and closed_src_control == ctl.UNUSE) or
		    (not use_closed_src and closed_src_control == ctl.USE)):
			msg = 'by USE_CLOSED_SRC condition contradiction'
			if progress_report:
				print('  skip: %s' % msg)
			if self.verbose > 1:
				print('leave: %s' % msg)
			return result, 1
		flag = True if use_closed_src else False
		self.apply_use_closed_src(flag)

		# pipe process
		pipeprocess = ctl.get(ctl.PIPEPROCESS)
		binpath = None
		"""
		if pipeprocess:
			binpath = self.spr_topdir + Util.pathconv('/bin/test')
			# Replace macro '$(BIN)' in pipeprocess to binpath.
			pipeprocess = pipeprocess.replace('$(BIN)', binpath)
		"""

		# timeout
		timeout = ctl.get(ctl.TIMEOUT)
		if self.override['timeout'][ctl.RUN] is not None:
			timeout = self.override['timeout'][ctl.RUN]

		# force recompile
		ctl.recompile(self.recompile)

		# log output
		ctl.log_option(self.putlog[0], self.putlog[1])

		# expected result status
		expected = ctl.get(ctl.EXPECTED)
		result[ctl.RUN]['expected'] = expected if expected else 0

		# edit project file if 'CppMacro' control is defined
		cpp_macro = ctl.get(ctl.CPPMACRO)
		if cpp_macro is not None:
			status, msg = self.__define_cpp_macro(ctl, cpp_macro)
			if status != 0:
				if progress_report:
					print('  Error: %s' % msg)
				if self.verbose > 1:
					print('leave: %s' % msg)
				result[ctl.ERR] = msg
				return result, -1

		# for all platforms
		for platform in platforms:
			if progress_report_only:
				sys.stdout.write('  %s:' % platform)
				sys.stdout.flush()

			# additional path for 'run'
			addpath = '%s;' % binpath if binpath else ''
			if platform.lower() == 'x64':
				addpath += self.spr_topdir + '/bin/win64;'
				addpath += self.spr_topdir + '/../dependency/bin/win64;'
			addpath += self.spr_topdir + '/bin/win32;'
			addpath += self.spr_topdir + '/../dependency/bin/win32'
			addpth = Util.pathconv(addpath)

			# for all configurations
			for config in configs:
				# compile
				if self.__if_skip(ctl, ctl.BLD):
					if self.verbose: print('  skip build')
					continue
				status = self.__compile(ctl, platform, config, result,
						progress_report, progress_report_only)
				if status < 0:
					return result, status
				if status == 1:
					continue
				# run
				if self.__if_skip(ctl, ctl.RUN):
					if ctl.get(ctl.NEEDINTERVENTION):
						status = Proc.ENEEDHELP
						result[ctl.RUN][platform][config] = status
					if self.verbose: print('  skip run')
					continue
				status = self.__run(ctl, platform, config, result,
						addpath, pipeprocess, timeout)
				if status < 0:
					return result, status
			# end of configs
		# end of platforms
		if progress_report_only:
			print()
		if cpp_macro:
			self.__revive_cpp_macro()
		return result, 0

	#  Compile.
	#
	def __compile(self, ctl, platform, config, result,
			    progress_report, progress_report_only):
		# arguments:
		#   ctl:	Instance of ControlParams class.
		#   platform:	Test platform.
		#   config:	Build configuration.
		#   result:	Object to set test result.
		#   progress_report:
		#   progress_report_only:
		#		Control variables for progress report.
		# returns:	0: normal, 1: compile error, -2: interrupted
		#		others: other error

		ctl.build_option(platform, config)
		if ctl.has_error():
			msg = 'ctl has error'
			if progress_report:
				print('  Error: %s' % msg)
			if self.verbose > 1:
				print('leave: %s' % msg, 1)
			result[ctl.ERR] = msg
			return -2
		self.cc.set_outdir(ctl.get(ctl.OUTDIR), ctl.get(ctl.CLEAN))
		self.cc.set_log(ctl.get(ctl.LOGFILES)[ctl.BLD])
		conf = ctl.get(ctl.CONFIG)
		if progress_report_only:
			sys.stdout.write(' %s' % conf)
			sys.stdout.flush()
		elif self.verbose:
			print('[%s, %s]' % (platform, config))
		status = self.cc.build(platform, conf)
		if KEYINTR.is_interrupted():
			#KEYINTR.set_interrupted(False)
			status = Proc.ECANCELED
		if self.verbose:
			print('  build result: %s' % self.__status_str(status))
		result[ctl.BLD][platform][config] = status
		result[ctl.BLD]['use_closed_src'] = ctl.get(ctl.USE_CLOSED_SRC)
		if status != 0:
			if status == Proc.ECANCELED:
				return -2
			return 1
		return 0

	#  Run.
	#
	def __run(self, ctl, platform, config, result,
			addpath, pipeprocess, timeout):
		# arguments:
		#   ctl:	Instance of ControlParams class.
		#   platform:	Test platform.
		#   config:	Build configuration.
		#   result:	Object to set test result.
		#   addpath:	Path to add for execution.
		#   pipeprocess:
		#		Process to generate test input data.
		#   timeout:	Timeout value for test program.
		# returns:	0: normal, -2: interrupted

		exefile = ctl.get(ctl.OUTFILE)
		if not os.path.exists(exefile):
			return 0
		if self.verbose and addpath is not None:
			print('  addpath: %s' % addpath)
		logfile = ctl.get(ctl.LOGFILES)[ctl.RUN]
		cmnd = exefile
		if pipeprocess:
			toolpath = self.spr_topdir + '/bin/test'
			proc1 = Proc(self.verbose, self.dry_run)
			proc2 = Proc(self.verbose, self.dry_run)
			proc1.exec(cmnd, addpath=addpath,
				   stdin=Proc.PIPE, stdout=logfile)
			proc2.exec(pipeprocess, addpath=toolpath,
				   stdout=Proc.PIPE)
			status = proc1.wait(timeout)
			proc2.wait()
		else:
			proc1 = Proc(self.verbose, self.dry_run)
			proc1.exec(cmnd, addpath=addpath, stdout=logfile)
			status = proc1.wait(timeout)
		#
		"""
		proc_to_kill = ctl.get(ctl.KILLPROCESS)
		if proc_to_kill:
			proc1.kill(image=proc_to_kill)
		"""
		if KEYINTR.is_interrupted():
			#KEYINTR.set_interrupted(False)
			status = Proc.ECANCELED
		if self.verbose:
			self.__head(logfile, self.max_loglines)
			print('  run result: %s' % self.__status_str(status))
		result[ctl.RUN][platform][config] = status
		if status == Proc.ECANCELED:
			return -2
		return 0

	#  Replace project file (.vcxproj) to define CPP macro.
	#
	def __define_cpp_macro(self, ctl, cpp_macro):
		# arguments:
		#   ctl:	ControlParams class object.
		#   cpp_macro:	Macro definition.
		# returns:	Status and error message.

		if self.verbose:
			print()
		cc_version = self.cc.get_version()
		proj_dir = ctl.get(ctl.SOLUTION_DIR)
		proj_file = ctl.get(ctl.SOLUTION) + cc_version + '.vcxproj'
		proj_save = proj_file + '.org'
		proj_path = proj_dir + os.sep + proj_file
		if not os.path.exists(proj_path):
			proj_file = ctl.get(ctl.SOLUTION) + '_' + cc_version + '.vcxproj'
			proj_path = proj_dir + os.sep + proj_file
			if not os.path.exists(proj_path):
				msg = 'project file not found: %s' % proj_path
				return -1, msg

		# save original project file
		fop = FileOp(verbose=self.verbose, dry_run=self.dry_run)
		if not os.path.exists(proj_save):
			status = fop.mv(proj_path, proj_save)
			if status != 0:
				msg = '  Error: mv %s -> %s' % (proj_path, proj_save)
				return status, msg
			if self.verbose:
				print('  save project file to "%s"' % proj_save) 
		self.tmp['proj_dir'] = proj_dir
		self.tmp['proj_file'] = proj_file
		self.tmp['proj_save'] = proj_save

		# read original project file
		ifname = proj_dir + os.sep + proj_save
		orgf = TextFio(ifname)
		if orgf.open() < 0:
			msg = orgf.error()
			return -1 , msg
		encoding = orgf.get_encoding()
		if encoding is None:
			msg = 'unexpected file encoding: %s' % proj_file
			return -1 , msg
		lines = orgf.read()
		orgf.close()
		if self.verbose:
			print('  %s: %d lines read' % (ifname, len(lines)))

		# replace tag value
		tag = 'PreprocessorDefinitions'
		pat = '(\W*)<%s>(.*)</%s>' % (tag, tag)
		replaced = []
		for line in lines:
			m = re.search(pat, line)
			if m:
				line = '%s<%s>%s;%s</%s>\n' % \
					(m.group(1), tag, cpp_macro, m.group(2), tag)
				if self.verbose > 1:
					print('  %s' % line.strip())
			replaced.append(line)

		ofname = proj_dir + '\\' + proj_file
		newf = TextFio(ofname, mode='w', encoding=encoding)
		if newf.open() < 0:
			msg = orgf.error()
			return -1 , msg
		newf.writelines(replaced)
		newf.close()
		if self.verbose:
			print('  %s: %d lines written' % (ofname, len(replaced)))

		return 0, None

	#  Revive project file.
	#
	def __revive_cpp_macro(self):

		proj_dir = self.tmp['proj_dir']
		proj_file = proj_dir + os.sep + self.tmp['proj_file']
		proj_save = proj_dir + os.sep + self.tmp['proj_save']
		proj_junk = proj_file + '.junk'
		fop = FileOp(verbose=self.verbose, dry_run=self.dry_run)

		# do nothing if saved file does not exist
		if not os.path.exists(proj_save):
			return

		# rename ".vcxproj" to ".vcxproj.junk"
		status = fop.mv(proj_file, proj_junk)
		if status != 0:
			print('  Error: mv %s -> %s' % (proj_file, proj_junk))
			return
		# rename ".vcxproj.save" to ".vcxproj"
		status = fop.mv(proj_save, proj_file)
		if status != 0:
			print('  Error: mv %s -> %s' % (proj_save, proj_file))
			return
		# delete ".vcxproj.junk"
		status = fop.rm(proj_junk)
		if status != 0:
			print('  Error: rm %s' % proj_junk)
			return
	
		if self.verbose:
			print('  revive project file: "%s"' % proj_file) 

	#  Initialize result structure.
	#
	def __init_result(self, platforms, configs):
		# arguments:
		#   platforms:	List of platforms (str).
		#   configs:	List of configurations (str).
		# returns:	Initialized result list (obj).

		result = [None, {}, {}]
		for n in [ControlParams.BLD, ControlParams.RUN]:
			result[n] = {}
			for p in platforms:
				result[n][p] = {}
				for c in configs:
					result[n][p][c] = None
		result[ControlParams.RUN]['expected'] = None
		return result

	"""
	#  Traverse upwards to find named dirctory.
	#
	def __find_dir(self, name):
		# arguments:
		#   name	Directory name to find.
		# returns:	Absolute path of just one upper level
		#		directory which specified by 'name'.

		dirs = os.getcwd().split('\\')
		found = False
		for n in range(len(dirs)):
			if dirs[n] == name:
				found = True
				break
		return os.sep.join(dirs[0:n]) if found else None
	"""

	#  Is this directory a test candidate?
	#  This method is intended to eliminate unwilling directories
	#  from directory traverse.
	#
	def __is_candidate_dir(self, dir):
		# arguments:
		#   dir:	Directory name to be checked (str).
		# returns:	Candidate or not (bool).

		if not os.path.isdir(dir):
			return False
		leaf = dir.split(os.sep)[-1]
		# these files are exceptions
		if leaf[0] == '.' or leaf == 'Template':
			return False
		return True

	#  Does this directory have solution file?
	#
	def __has_solution_file(self, dir):
		# arguments:
		#   dir:	Directory name to be checked (str).
		# returns:	Yes or no (bool).

		slns = glob.glob(dir + os.sep + '*.sln')
		if len(slns) == 0:
			return False
		return True

	#  Should we really execute? (check execution control)
	#
	def __if_skip(self, ctl, index):
		# arguments:
		#   ctl:	ControlParams class object.
		#   index:	Stage to be checked (ctl.BLD or ctl.RUN).
		# returns:	Should skip or not (bool).

		need_intervention = ctl.get(ctl.NEEDINTERVENTION)
		if need_intervention and index == ctl.RUN:
			return True
		control = ctl.get(ctl.EXE_CONTROLS)[index]
		override = self.override['exe'][index]
		do = override if override is not None else control
		return not do

	#  Print some first lines from the file.
	#
	def __head(self, fname, maxlines):
		# arguments:
		#   file:	File path (str).
		#   maxlines:	Maximum lines to be printed (int).

		if not os.path.exists(fname):
			return 
		f = TextFio(fname)
		if f.open() < 0:
			return
		lines = f.read(maxlines)
		f.close()
		for line in lines:
			print(line)

	#  Error code and description.
	#
	def __status_str(self, status):
		# arguments:
		#   status:	Status code (int).
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
			s = '%d' % status
		return s

# end: SpringheadTest.py

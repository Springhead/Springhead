#!/usr/local/bin/python3.4
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:	Proc(self, verbose=0, dry_run=False)
#
#  CONSTANTS:
#	STDOUT, PIPE, NULL
#	EINTR, EINVAL, ETIME, EPROTO, ECANCELED, ENEEDHELP
#
#  METHODS:
#	proc = exec(self, args, stdin=None, stdout=None, stderr=None,
#		    shell=False, env=None, addpath=None, append=False)
#	status = wait(self, timeout=None)
#	kill(self, pid=None, image=None, verbose=0)
#	out, err = output(self)
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2017/08/19 F.Kanehori	First version.
#					Separated from class Util.
#	Ver 1.1  2017/09/14 F.Kanehori	Bug fixed.
#	Ver 1.11 2017/10/07 F.Kanehori	Dos intrinsic commands OK.
#	Ver 1.12 2017/10/13 F.Kanehori	Set default encoding.
#	Ver 1.13 2018/01/11 F.Kanehori	wait(): Enable dry_run.
#	Ver 1.14 2018/02/21 F.Kanehori	Set dummy object to Proc.proc
#					when dry_run flag specified.
#	Ver 1.15 2018/03/12 F.Kanehori	Now OK for doxygen.
# ======================================================================
import sys
import os
import subprocess
import signal
import copy
sys.path.append('/usr/local/lib')
from Util import *

##  Process handling class (Wrapper class for 'subprocess' module).
#
class Proc:
	#  Class constants
	#
	STDOUT	  = subprocess.STDOUT
	PIPE	  = subprocess.PIPE
	NULL	  = subprocess.DEVNULL
	#
	##  Interrupted by system call.
	EINTR	  = -4
	##  Invalid argument.
	EINVAL	  = -22
	##  Time expired.
	ETIME	  = -62
	##  Protocol error.
	EPROTO	  = -71
	##  Operation canceled.
	ECANCELED = -125
	##  Need intervention.
	ENEEDHELP = -99999

	##  The initializer.
	#   @param verbose	Verbose level (0: silent) (int).
	#   @param dry_run	Show command but do not execute it (bool).
	#
	def __init__(self, verbose=0, dry_run=False):
		self.clsname = self.__class__.__name__
		self.version = 1.14
		#
		self.verbose = verbose
		self.dry_run = dry_run
		#
		self.pipe = [None, None, None]
		self.fd = [None, None, None]
		self.proc = None
		self.pid = None
		self.creationflags = 0

	##  Execute program.
	#   @param args		Command and its arguments (str or str[]).
	#   @param stdin	File object, file name or pipe (obj or str).
	#   @param stdout	File object, file name or pipe (obj or str).
	#   @param stderr	File object, file name or pipe (obj or str).
	#   @param shell	Set True when executing DOS command,
	#			or pipe is used for process input/output (bool).
	#   @param env		Environment for new process (dict).
	#   @param addpath	Additional path to prepend env['PATH'] (str).
	#   @param append	Set output redirect file open mode to 'append'
	#			(bool).
	#   @returns		Self object.
	#
	def exec(self, args,
		       stdin=None, stdout=None, stderr=None,
		       shell=False, env=None, addpath=None,
		       append=False):
		#
		self.pipe[0] = stdin	# redirect argument
		self.pipe[1] = stdout
		self.pipe[2] = stderr
		self.status = 0
		
		# conversion of path in args
		if isinstance(args, str):
			args = args.split()
		args = ' '.join(args)

		# prepare output redirection file
		rmode = 'r'			# open mode for input
		wmode = 'a' if append else 'w'	# open mode for output
		self.fd[0] = self.__open(self.pipe[0], rmode, self.dry_run)
		self.fd[1] = self.__open(self.pipe[1], wmode, self.dry_run)
		self.fd[2] = self.__open(self.pipe[2], wmode, self.dry_run)

		# prepare environment
		new_env, org_env = self.__set_environment(env, addpath)
		self.org_env = org_env

		# execute command
		if self.dry_run or self.verbose:
			a = ['<', '>', '2>']
			redirect = ''
			for n in range(3):
				if self.fd[n]:
					name = self.__dev_name(self.fd[n])
					redirect += ' (%s %s)' % (a[n], name)
			print('  EXEC: %s%s' % (args, redirect))
			if addpath:
				print('        addpath: %s' % Util.upath(addpath))
		if self.dry_run:
			class dummy:
				stdin = 0
				stdout = 1
				stderr = 2
			self.proc = dummy()	# dummy!
			return self
		if self.verbose > 1:
			print('args to Popen')
			print('  args: %s' % args)
			print('  creationflags: %s' % self.creationflags)
			print('  stdin/out/err: %s' % self.pipe)
			print('  fd: %s' % self.fd)
			print('  env: %s' % new_env)
			print('  shell: %s' % shell)
		self.proc = subprocess.Popen(args,
				stdin=self.fd[0],
				stdout=self.fd[1],
				stderr=self.fd[2],
				creationflags=self.creationflags,
				env=new_env,
				shell=shell)
		self.pid = self.proc.pid
		if self.verbose:
			print('  (pid: %d)' % self.pid)
		return self

	##  Wait for process termination then return termination code.
	#   @param timeout	Time out value in seconds (int).
	#   @returns		Process termination code (int).
	#
	def wait(self, timeout=None):
		if self.dry_run:
			self.status = 0
			return self.status
		#
		if self.proc is None:
			return Proc.EINVAL
		#
		if timeout and self.verbose:
			print('  TIMEOUT: %s sec' % timeout)
		try:
			timeoutval = None
			if timeout:
				timeoutval = int(timeout)
			status = self.proc.wait(timeoutval)

		except KeyboardInterrupt:
			if self.verbose:
				print('  KeyboardInterrupt')
			status = Proc.ECANCELED
		except subprocess.TimeoutExpired:
			if self.verbose:
				print('  TimeoutExpired')
			status = Proc.ETIME
		except subprocess.SubprocessError as err:
			if self.verbose:
				print('  SubprocessError: %s' % err)
			status = Proc.EINTR

		# make sure to temrminate process
		if self.proc.poll() is None:
			if self.verbose:
				pid = self.proc.pid
				print('  kill process (pid %d)' % pid)
			#os.kill(proc.pid, signal.CTRL_BREAK_EVENT)
			self.proc.terminate()

		# cleanup
		self.__close(self.fd[0], self.pipe[0])
		self.__close(self.fd[1], self.pipe[1])
		self.__close(self.fd[2], self.pipe[2])
		self.__revive_envirnment(self.org_env)

		# only lower 16 bits are meaningful
		self.status = self.__s16(status)
		return self.status

	##  Kill specified process.
	#   @param pid		Process-ID to kill (int).
	#   @param image	Process image name to kill (str).
	#   @param verbose	Verbose level (0: silent) (int).
	#
	#   CAUTION:
	#   @n	It's extreamly dangerous to kill by 'image'
	#	because there may be the process(es) having the same
	#	process image name which you never want to kill.
	#
	def kill(self, pid=None, image=None, verbose=0):
		if pid is None and image is None:
			pid = self.pid
			if self.verbose:
				print('  kill: pid: %d' % pid)
		tasklist = self.__tasklist()

		# find task(s) to kill
		tasks = []
		for task in tasklist:
			if pid == int(task[0]):
				pair = [pid, task[1]]
				if self.verbose:
					print('  kill: found %s' % pair)
				tasks.append(pair)
			elif image == task[1]:
				pair = [int(task[0]), image]
				if self.verbose:
					print('  kill: found %s' % pair)
				tasks.append(pair)
		tasks.reverse()

		# kill
		for task in tasks:
			pid = task[0]
			image = task[1]
			if self.verbose:
				print('  kill %s (pid %d)' % (image, pid))
			try:
				os.kill(pid, signal.SIGTERM)
			except Exception as e:
				if self.verbose:
					print('  %s' % e)

	##  Get both stdout and stderr output from the process.
	#   @returns		out, err
	#   @n out:		Output string got from stdout stream (str).
	#   @n err:		Output string got from stderr stream (str).
	#
	def output(self):
		if self.dry_run:
			return None, None
		if self.proc is None:
			if self.verbose:
				print('  invalid process')
			return None, None
		if self.pipe[1] != Proc.PIPE and self.pipe[2] != Proc.PIPE:
			if self.verbose:
				print('  output is no redirected')
			return None, None
		#
		out, err = self.proc.communicate()
		encoding = os.device_encoding(1)
		if encoding is None:
			encoding = 'UTF-8' if Util.is_unix() else 'cp932'
		out = out.decode(encoding) if out else None
		err = err.decode(encoding) if err else None
		return out, err

	# --------------------------------------------------------------
	#  For class private use
	# --------------------------------------------------------------

	##  Convert zero-extended-16bit-signed-int into 32bit-signed-int.
	#   @param value	32-bit-unsigend-int value (int).
	#   @n			upper 16 bits:  all zeors
	#   @n			lower 16 bits:  16-bit-signed-int value
	#   @returns		32-bit-signed-int value (int).
	#
	def __s16(self, value):
		# arguments:

		return -(value & 0b1000000000000000) | (value & 0b0111111111111111)

	##  Get current task list.
	#   @returns		List of [pid, task_name]
	#
	def __tasklist(self):
		cmnd = 'ps a' if Util.is_unix() else 'tasklist'
		proc = Proc()
		proc.exec(cmnd, stdout=Proc.PIPE, shell=True)
		out, err = proc.output()
		proc.wait()
		outlist = out.replace('\r', '').split('\n')
		tasks = []
		for line in outlist:
			items = line.split()
			if Util.is_unix():
				if len(items) < 5: continue
				if items[0] == 'PID': continue
				tasks.append([items[0], items[4]])
			else:
				if len(items) != 6: continue
				tasks.append([items[1], items[0]])
		#print(tasks)
		return tasks

	##  Set new enviroment variables.
	#   @param env		Enviroment to set (dict).
	#   @param addpath	Path to prepend env['PATH'] (str).
	#   @returns		new_env, org_env
	#   @n new_env:		New environment (dict).
	#   @n org_env:		Old environment (dict).
	#
	def __set_environment(self, env, addpath):
		if self.dry_run:
			return None, None
		if env is None and addpath is None:
			return None, None
		org_env = copy.deepcopy(os.environ)
		new_env = os.environ if env is None else env
		if addpath is not None:
			new_env['PATH'] = addpath + ';' + new_env['PATH']
		return new_env, org_env

	##  Revive old enviroment variables set by self.__set_environment().
	#   @param org_env	Environment to revive (dict).
	#
	def __revive_envirnment(self, org_env):
		if org_env is not None:
			os.environ = copy.deepcopy(org_env)

	##  Get file object.
	#   @param file		File name string or file object.
	#   @param mode		File open mode (str).
	#   @param dry_run	Show command but do not execute it (bool).
	#   @retval obj		File object if dry_run is False.
	#   @retval None	If dry_run is True.
	#
	def __open(self, file, mode, dry_run):
		if self.dry_run:
			return None
		if not isinstance(file, str):
			return file
		try:
			f = open(file, mode)
		except IOError as err:
			f = None
		return f

	##  Release file object.
	#   @param object	File object returned by self.__open().
	#   @param file		The same argument passed to self.__open().
	#
	def __close(self, object, file):
		if isinstance(file, str):
			object.close()

	##  Device name for verbose message.
	#   @param dev		Device descriptor (int).
	#   @returns		Device name (str).
	#
	def __dev_name(self, dev):
		if dev == -1:	return 'pipe'
		if dev == -2:	return 'stdout'
		if dev == -3:	return '/dev/null'
		return dev

# end: Proc.py

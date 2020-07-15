#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:	Proc(self, verbose=0, dry_run=False)
#
#  CONSTANTS:
#	STDOUT, PIPE, NULL
#	EINTR, EINVAL, ETIME, EPROTO, ECANCELED, ENEEDHELP
#
#  METHODS:
#	proc = execute(self, args, stdin=None, stdout=None, stderr=None,
#		       shell=False, env=None, addpath=None, append=False)
#	status = wait(self, timeout=None)
#	kill(self, pid=None, image=None, verbose=0)
#	status, out, err = output(self, timeout=None)
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0   2017/08/19 F.Kanehori	First version.
#					Separated from class Util.
#	Ver 1.1   2017/09/14 F.Kanehori	Bug fixed.
#	Ver 1.11  2017/10/07 F.Kanehori	Dos intrinsic commands OK.
#	Ver 1.12  2017/10/13 F.Kanehori	Set default encoding.
#	Ver 1.13  2018/01/11 F.Kanehori	wait(): Enable dry_run.
#	Ver 1.14  2018/02/21 F.Kanehori	Set dummy object to Proc.proc
#					when dry_run flag specified.
#	Ver 1.15  2018/03/12 F.Kanehori	Now OK for doxygen.
#	Ver 1.2   2018/03/14 F.Kanehori	Change: exec() -> execute().
#	Ver 1.3   2018/03/19 F.Kanehori	Change interface: output()
#	Ver 1.31  2018/03/22 F.Kanehori	Bug fixed.
#	Ver 1.32  2018/04/05 F.Kanehori	Bug fixed (kill at timeout).
#	Ver 1.33  2018/06/28 F.Kanehori	Fixes spaces in homedir.
#	Ver 1.34  2020/07/15 F.Kanehori	output(): Change decode strategy.
# ======================================================================
import sys
import os
import subprocess
import signal
import copy
import re
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
		self.version = 1.34
		self.out_encoding = self.__system_encoding()
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
	#   @n		Should be followed by self.wait() or self.output().
	#   @n		e.g.
	#   @n		rc = Proc(cmnd, ...).wait(...) or
	#   @n		rc, out, err = Proc(cmnd, ...).output(...)
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
	def execute(self, args,
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
		args = self.__space_in_homedir(args)

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
				start_new_session=True,
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
			if Util.is_unix():
				os.killpg(self.proc.pid, signal.SIGTERM)
			else:
				os.kill(self.proc.pid, signal.SIGTERM)

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
	#   @param timeout	Time out value in seconds (int).
	#   @param out_encoding	Encoding of output stream (str).
	#   @returns		rc, out, err
	#   @n status:		Process termination code (int).
	#   @n out:		Output string got from stdout stream (str).
	#   @n err:		Output string got from stderr stream (str).
	#
	def output(self, timeout=None, out_encoding=None):
		if self.dry_run:
			return 0, None, None
		if self.proc is None:
			if self.verbose:
				print('  invalid process')
			return 1, None, None
		if self.pipe[1] != Proc.PIPE and self.pipe[2] != Proc.PIPE:
			if self.verbose:
				print('  output is not redirected')
			print('  output is not redirected')
			return 0, None, None

		# change encoding to decodable one
		# only if nkf available and out_encoding is decodable.
		if out_encoding is None:
			out_encoding = self.out_encoding
		if self.__has_nkf() and self.__is_decodable(out_encoding):
			# change encoding if we can use nkf
			enc_utf8 = ['cp65001', 'utf8', 'utf-8', 'utf_8']
			enc_sjis = ['cp932', 'sjis', 'shift-jis']
			nkf_arg = '-w'	# set 'utf8' as default encoding
			if out_encoding in enc_utf8: nkf_arg = '-w'
			if out_encoding in enc_sjis: nkf_arg = '-s'
			cmnd = 'nkf %s' % nkf_arg
			proc = subprocess.Popen(cmnd,
					stdin=self.proc.stdout,
					stdout=subprocess.PIPE,
					shell=True)
			stat = self.proc.wait()
			self.proc = proc

		# get output
		try:
			out, err = self.proc.communicate(timeout=timeout)
			status = self.proc.returncode
		except subprocess.TimeoutExpired:
			self.proc.kill()
			out, err = self.proc.communicate()
			status = Proc.ETIME
		#print('status: %d' % status)
		#print('out: [%s]' % out)
		#print('err: [%s]' % err)
		out = out.decode(out_encoding) if out else None
		err = err.decode(out_encoding) if err else None

		# cleanup
		self.__close(self.fd[0], self.pipe[0])
		self.__close(self.fd[1], self.pipe[1])
		self.__close(self.fd[2], self.pipe[2])
		self.__revive_envirnment(self.org_env)

		# only lower 16 bits are meaningful
		self.status = self.__s16(status)
		return self.status, out, err

	# --------------------------------------------------------------
	#  For class private use
	# --------------------------------------------------------------

	##  Double quote path in args if home dir name has space(s).
	#   @param args		command args (str).
	#   @returns		Replaced args if needed (str).
	#
	def __space_in_homedir(self, args):
		new_args = args
		homedir = Util.upath(os.path.expanduser('~'))
		if args.find(homedir) >= 0 and homedir.find(' ') >= 0:
			tmp_home = homedir.replace(' ', '@')
			tmp_repl = args.replace(homedir, tmp_home)
			org_list = tmp_repl.split()
			new_list = []
			for item in org_list:
				if item.find('@') >= 0:
					item = '"%s"' % item.replace('@', ' ')
				new_list.append(item)
			new_args = ' '.join(new_list)
			#print('==> args replaced to [%s]' % new_args)
		return new_args

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
		proc.execute(cmnd, stdout=Proc.PIPE, shell=True)
		stat, out, err = proc.output()
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
		if isinstance(file, str) and object:
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

	##  Check if system has 'nkf' command or not.
	#   @returns	True if 'nkf' command exists, False if else.
	#
	def __has_nkf(self):
		cmnd = 'which nkf' if Util.is_unix() else 'where nkf'
		rc = subprocess.Popen(cmnd, stdout=subprocess.DEVNULL,
					    shell=True).wait()
		return rc == 0

	##  Get system encoding name.
	#   @returns		System encoding name (str).
	#
	def __system_encoding(self):
		encoding = os.device_encoding(1)
		#print('os.device_encoding(1) returns [%s]' % encoding)
		if encoding is None:
			stdout_info = str(sys.stdout)
			#print('sys.stdout returns [%s]' % stdout_info)
			m = re.search("encoding='(.+)'", stdout_info)
			if m:
				encoding = m.group(1)
			else:
				encoding = 'utf-8'
				#print('assume %s as default encoding' % encoding)
		#print('encoding: %s' % encoding)
		return encoding

	##  Check if speicfied encoding is decodable.
	#   @param encoding	Encoding name (str).
	#   @returns		Decodable or not (bool).
	#
	def __is_decodable(self, encoding):
		test_str = b'test'
		try:
			test_str.decode(encoding)
			can_decode = True
		except:
			can_decode = False
		return can_decode

# end: Proc.py

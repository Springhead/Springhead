#!/usr/local/bin/python3.6
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:
#	FileOp
#	    Class for file operation functions.
#
#  INITIALIZER:
#	obj = FileOp(info=0, dry_run=False, verbose=0)
#	  arguments:
#	    info:	Show operation information (silent if 0) (int).
#	    dry_run:	Show command but do not execute it (bool).
#	    verbose:	Verbose level (silent if 0) (int).
#
#  METHODS:
#	status = cp(src, dst, follow_symlinks)
#	status = mv(src, dst)
#	status = rm(path, recurse=False, use_shutil=True, idle_time=0)
#	    OS independent unix like copy/move/remove command.
#	  arguments:
#	    src:	Source file path (cp and mv).
#	    dst:	Destination file path (cp and mv).
#	    path:	File path (rm).
#	    recurse:	Remove all subdirectories and directory itself.
#	    use_shutil:	Use shutil.rmtree() to remove directory or not.
#			Some files can not removed by shutil.rmtree().
#			In that case, set this argument False and use
#			os.unlink() and os.rmdir().
#	    idle_time:	When use_shutil is False, removing directory
#			'path' just after removing all of its contents
#			may cause 'path' remains and inaccessible.
#			This argument i(in second) set idleing time
#			before removing 'path' directory itself.
#	  returns:	0: succ, 1: fail.
#
#	touch(path, mode=0o666, no_create=False)
#	    Method version of 'touch' command.
#	    If the file exists, change its access time.
#	    Otherwise, create a empty file unless 'no_create' is True.
#	  arguments:
#	    Create new file with file mode 'mode'.
#	    path:	File path to touch.
#	    mode:	File mode.
#	    no_create:	Do not create a file even the case.
#	  returns:	-1: Can't create file when 'no_create' is False.
#			 0: Otherwise.
#
#	string = ls(path, sort='name', show='mtime')
#	    Get given file information.
#	  arguments:
#	    path:	File path to get/print info.
#	    sort:	Sort key (str).
#			One of 'name', 'ctime', 'mtime' or 'atime'.
#	    show:	Time to show (str).
#			One of 'ctime', 'mtime' or 'atime'.
#	  returns:	File information (str).
#			Empty string if path does not exist.
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2017/08/20 F.Kanehori	First version.
#	Ver 1.1  2017/09/11 F.Kanehori	Aargument 'dry_run' added.
#	Ver 1.2  2017/09/11 F.Kanehori	Implement move() for unix.
#	Ver 1.3  2017/10/23 F.Kanehori	Add argument to ls().
#	Ver 1.4  2018/03/01 F.Kanehori	Rewrite cp/mv/rm using shutil.
# ======================================================================
import sys
import os
import datetime
import stat
import glob
import shutil
from pathlib import Path
from time import sleep

sys.path.append('/usr/local/lib')
from Proc import *
from Util import *
from Error import *

class FileOp:
	#  These are class instance methods.
	#
	def __init__(self, info=0, dry_run=False, verbose=0):
		self.clsname = self.__class__.__name__
		self.version = 1.4
		#
		self.info = info
		self.dry_run = dry_run
		self.verbose = verbose

	#  Copy
	#
	def cp(self, src, dst, follow_symlinks=True):
		u_src = Util.upath(src)
		u_dst = Util.upath(dst)
		a_dst = os.path.abspath(dst)
		if self.dry_run or self.info:
			print('cp: %s -> %s' % (u_src, u_dst))
			if self.dry_run:
				return 0
		#
		plist = u_src.split('/')
		src_pdir = '/'.join(plist[:-1])
		src_leaf = plist[-1]
		need_chdir = len(plist) > 1
		if need_chdir:
			cwd = os.getcwd()
			os.chdir(src_pdir)
		#
		rc = 0
		for name in glob.glob(src_leaf):
			u_name = Util.upath(name)
			rc = self.__cp(name, a_dst, follow_symlinks)
		#
		if need_chdir:
			os.chdir(cwd)
		return rc

	#  Move
	#
	def mv(self, src, dst):
		msg = 'mv: %s -> %s' % (Util.upath(src), Util.upath(dst))
		if self.dry_run or self.info:
			print(msg)
			if self.dry_run:
				return 0
		#
		try:
			shutil.move(src, dst)
			rc = 0
		except OSError as why:
			prog = '%s: mv' % self.clsname
			msg = str(why)
			Error(prog).print(msg, alive=True)
			rc = 1
			#
		return rc

	#  Remove
	#
	def rm(self, path, recurse=False, use_shutil=True, idle_time=0):
		msg = 'rm: %s' % Util.upath(path)
		if self.dry_run or self.info:
			print(msg)
			if self.dry_run:
				return 0
		#
		rc = 0
		for name in glob.glob(path):
			u_name = Util.upath(name)
			rc = self.__rm(u_name, use_shutil, idle_time)
		#
		return rc

	#  Touch command.
	#
	def touch(self, path, mode=0o666, no_create=False):
		flags = os.O_APPEND
		if not no_create:
			flags |= os.O_CREAT
		try:
			fd = os.open(path, flags=flags, mode=mode)
		except:
			if no_create:
				return 0
			return -1
		#
		ns_atime, ns_mtime = self.__time_to_set()
		if not isinstance(fd, int):
			fd = fd.fileno()
		fn = fd if os.utime in os.supports_fd else path
		ns = (ns_atime, ns_mtime)
		os.utime(fn, ns=ns)
		os.close(fd)
		return 0

	#  Ls like command.
	#
	def ls(self, path, sort='name', show='mtime'):
		if os.path.isdir(path):
			path += '/*'
		filelist = glob.glob(path)
		if len(filelist) == 0:
			return 'total 0'
		if len(filelist) == 1:
			return self.__ls_one(filelist[0], show)
		lslist = []
		if path[-2:] == '/*':
			lslist.append(self.__ls_one(path[:-2], show))
		for f in filelist:
			if os.path.isdir(f):
				lslist.append(self.__ls_one(f, show))
				sublist = self.ls(f)
				if not sublist:
					continue
				if sublist[0:5] == 'total':
					continue
				if isinstance(sublist, str):
					lslist.append(sublist)
				else:
					lslist.extend(sublist)
			else:
				lslist.append(self.__ls_one(f, show))
		return self.__ls_sort(lslist, sort)

	# --------------------------------------------------------------
	#  For class private use
	# --------------------------------------------------------------

	#  Copy files and directories.
	#
	def __cp(self, src, dst, follow_symlinks=True):
		# argument:
		#   src:	Soruce path (file or directory).
		#   dst:	Destination path (file or direcoty).
		#   follow_symlinks:
		#		When False and src is symbolic link,
		#		make symbolic link instead of copying
		#		body of src.
		# returns:	0: succ, 1: fail

		if self.verbose:
			print('__cp: %s -> %s' % (src, dst))
		#
		try:
			if self.verbose:
				print('    cwd: %s' % Util.upath(os.getcwd()))
				print('    src: %s' % src)
				print('    dst: %s' % dst)
			if os.path.isdir(src):
				if os.path.exists(dst):
					prog = '%s: cp' % self.clsname
					msg = '"%s" exists' % dst
					Error(prog).print(msg, alive=True)
					return 1
				if self.verbose:
					print('    shutil.copytree()')
				shutil.copytree(src, dst, follow_symlinks)
			else:
				if self.verbose:
					print('  shutil.copy2()')
				shutil.copy2(src, dst)
			rc = 0
		except OSError as why:
			prog = '%s: cp' % self.clsname
			Error(prog).print(str(why), alive=True)
			rc = 1
		#
		return rc

	#  Remove files and directories.
	#
	def __rm(self, path, use_shutil, idle_time):
		# argument:
		#   path:	File or directory path to remove.
		# returns:	0: succ, 1: fail
		
		try:
			if os.path.isdir(path):
				if use_shutil:
					shutil.rmtree(path, ignore_errors=True)
				else:
					rc = self.__remove_all(path)
					if Util.is_windows() and idle_time > 0:
						sleep(idle_time)  # Kludge
					os.rmdir(path)
			else:
				os.remove(path)
			rc = 0
		except OSError as why:
			prog = '%s: rm' % self.clsname
			Error(prog).print(str(why), alive=True)
			rc = 1
		return rc

	#  Remove all files/directories under specified directory.
	#
	def __remove_all(self, path):
		# argument:
		#   path:	Directory path to remove its contents.
		# returns:	0: succ, 1: fail

		another_drive = False
		if Util.is_windows():
			t_drive = os.path.abspath(path)[0]
			c_drive = os.getcwd()[0]
			another_drive = t_drive != c_drive
		#print('remove_all: %s (another drive: %s)' % (path, another_drive))
		if another_drive:
			# os.walk() seemes to work on current drive only.
			dlist = Util.upath(path).split('/')
			wrkdir = '/'.join(dlist[:-1])
			path = dlist[-1]
			cwd = os.getcwd()
			os.chdir(wrkdir)
		#
		#print('remove_all: path: %s' % path)
		try:
			for root, dirs, files in os.walk(path, topdown=False):
				for name in files:
					fpath = '%s/%s' % (root, name)
					os.chmod(fpath, stat.S_IWRITE)
					os.unlink(fpath)
				for name in dirs:
					dpath = '%s/%s' % (root, name)
					os.rmdir(dpath)
			rc = 0
		except OSError as why:
			prog = '%s: rm' % self.clsname
			msg = str(why)
			Error(prog).print(msg, alive=True)
			rc = 1
		#
		if another_drive:
			os.chdir(cwd)

		return rc

	#  Get current time (for touch method).
	#
	def __time_to_set(self):
		# argument:
		#   verbose	Verbose option.
		# returns:	Time stamp.

		d = datetime.datetime.today()
		CC, YY, MM, DD = 20, d.year%100, d.month, d.day
		hh, mm, ss, ms = d.hour, d.minute, d.second, d.microsecond
		if self.verbose > 1:
			fmt = '  current time: %s' % datetime_format
			print(fmt % (CC, YY, MM, DD, hh, mm, ss, ms))
		yy = CC * 100 + YY
		dt = datetime.datetime(yy, MM, DD, hh, mm, ss, ms)
		ts = int(dt.timestamp() * 1000000 + 0.0000005) * 1000
		return ts, ts

	#  Ls for one file.
	#
	def __ls_one(self, path, show='mtime'):
		# argument:
		#   path	File path.
		# returns:	Ls string.

		try:
			fstat = os.stat(path)
		except:
			return ''
		fmode = stat.filemode(fstat.st_mode)
		fname = self.__add_modifier(path, fmode)
		nlink = fstat.st_nlink
		fsize = fstat.st_size
		if show == 'mtime': timemode = fstat.st_mtime
		if show == 'ctime': timemode = fstat.st_ctime
		if show == 'atime': timemode = fstat.st_atime
		dt = datetime.datetime.fromtimestamp(int(timemode))
		ftime = dt.strftime('%Y-%m-%d %H:%M')
		fmt = '{0} {1} {2:>8} {3} {4}'
		ls = fmt.format(fmode, nlink, fsize, ftime, fname)
		return Util.upath(ls)

	def __add_modifier(self, path, mode):
		# argument:
		#   path	File path.
		#   mode	File mode.
		# returns:	Modifier added file path.

		if mode[0] == 'd': return path + '/'
		if mode[3] == 'x': return path + '*'
		if mode[6] == 'x': return path + '*'
		if mode[9] == 'x': return path + '*'
		if os.path.islink(path):
			linkedfile = os.readlink(path)
			if os.path.isabs(linkedfile):
				filetostat = linkedfile
			else:
				dirname = os.path.dirname(path)
				filetostat = '%s/%s' % (dirname, linkedfile)
			linkedstat = os.stat(filetostat)
			linkedmode = stat.filemode(linkedstat.st_mode)
			info = self.__add_modifier(linkedfile, linkedmode)
			return '%s -> %s' % (path, info)
		return path

	def __ls_sort(self, lslist, sort):
		# argument:
		#   lslist	Unsorted file list.
		#   sort	Sort key.
		# returns:	Sorted file list.

		if sort == 'name':
			return sorted(lslist, key=lambda f: f.split()[5])
		return sorted(lslist, key=lambda f: f.split()[3]+f.split()[4])

# end: FileOp.py

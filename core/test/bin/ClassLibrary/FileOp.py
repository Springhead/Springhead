#!/usr/local/bin/python3.6
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:	FileOp(info=0, dry_run=False, verbose=0)
#
#  METHODS:
#	status = cp(src, dst, follow_symlinks)
#	status = mv(src, dst)
#	status = rm(path, recurse=False, use_shutil=True, idle_time=0)
#	mkdir(path, mode=0o777, dir_fd=None)
#	rmdir(path, dir_fd=None)
#	makedirs(path, mode=0o777, exist_ok=False)
#	touch(path, mode=0o666, no_create=False)
#	string = ls(path, sort='name', show='mtime')
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2017/08/20 F.Kanehori	First version.
#	Ver 1.1  2017/09/11 F.Kanehori	Aargument 'dry_run' added.
#	Ver 1.2  2017/09/11 F.Kanehori	Implement move() for unix.
#	Ver 1.3  2017/10/23 F.Kanehori	Add argument to ls().
#	Ver 1.4  2018/03/01 F.Kanehori	Rewrite cp/mv/rm using shutil.
#	Ver 1.5  2018/03/05 F.Kanehori	Add mkdir()/rmdir()/makedirs().
#	Ver 1.51 2018/03/09 F.Kanehori	Now OK for doxygen.
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

##  File operation support class.
#
class FileOp:
	##  The initializer.
	#   @param info		Print operation to perform or not (bool).
	#   @param dry_run	Show command but do not execute it (bool).
	#   @param verbose	Verbose level (0: silent) (int).
	#
	def __init__(self, info=0, dry_run=False, verbose=0):
		self.clsname = self.__class__.__name__
		self.version = 1.51
		#
		self.info = info
		self.dry_run = dry_run
		self.verbose = verbose

	##  Unix like copy (cp) command.
	#   @param src		Source file path (str).
	#   @param dst		Destination file path (str).
	#   @param follow_symlinks
	#			When False and argument 'src' is symbolic link,
	#			make new symbolic link instead of copying file
	#			body linked by 'src'.
	#   @retval 0		successful
	#   @retval 1		failure
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

	##  Unix like move (mv) command.
	#   @param src		Source file path (str).
	#   @param dst		Destination file path (str).
	#   @retval 0		successful
	#   @retval 1		failure
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

	##  Unix like remove (rm) command.
	#   @param path		File path (str).
	#   @param recurse	Remove all subdirectories and directory itself.
	#   @param use_shutil	Use shutil.rmtree() to remove directory or not.
	#   @n			Some files can not removed by shutil.rmtree().
	#			In that case, set this argument False
	#			and use os.unlink() and os.rmdir() instead.
	#   @param idle_time	Set idling time in second.
	#
	#   NOTE
	#   @n			When use_shutil is False, removing directory
	#			'path' just after removing all of its contents
	#			may cause 'path' remains and inaccessible.
	#			If the case, set 'idle_time' to wait before
	#			removing the 'path' directory itself.
	#   @retval 0		successful
	#   @retval 1		failure
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

	##  Make new directory (Wrapper of os.mkdir())
	#   @param path		New directory path to create (str).
	#   @param mode		Access mode of the directory (int).
	#   @param dir_fd	File descriptor open to a directory or None.
	#   @n			If dir_fd is not None, path should be relative;
	#			path will then be relative to that directory.
	#
	def mkdir(self, path, mode=0o777, dir_fd=None):
		if self.dry_run or self.info:
			print('mkdir: %s' % Util.upath(path))
			if self.dry_run:
				return 0
		os.mkdir(path, mode=mode, dir_fd=dir_fd)

	##  Remove directory (Wrapper of os.rmdir())
	#   @param path		Directory path to remove (str).
	#   @param dir_fd	File descriptor open to a directory, or None.
	#   @n			If dir_fd is not None, path should be relative;
	#			path will then be relative to that directory.
	#
	def rmdir(self, path, dir_fd=None):
		if self.dry_run or self.info:
			print('rmdir: %s' % Util.upath(path))
			if self.dry_run:
				return 0
		os.rmdir(path, dir_fd=dir_fd)

	##  Make new directory recursively (Wrapper of os.makedirs())
	#   @param path		New directory path to create (str).
	#   @param mode		Access mode of the directory (int).
	#   @param exist_ok	If exist_ok is False and target directory
	#			already exist, exception OSError is raised.
	#
	def makedirs(self, path, mode=0o777, exist_ok=False):
		if self.dry_run or self.info:
			print('makedirs: %s' % Util.upath(path))
			if self.dry_run:
				return 0
		os.makedirs(path, mode=mode, exist_ok=exist_ok)

	##  Change file access/modification time.
	#   @param path		File path (str).
	#   @param mode		File creation mode (int).
	#   @param no_create	If no_create is False and file does not exist,
	#   @n			new file will be created with specified 'mode'.
	#   @retval 0		successful
	#   @retval 1		failure
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
			return 1
		#
		ns_atime, ns_mtime = self.__time_to_set()
		if not isinstance(fd, int):
			fd = fd.fileno()
		fn = fd if os.utime in os.supports_fd else path
		ns = (ns_atime, ns_mtime)
		os.utime(fn, ns=ns)
		os.close(fd)
		return 0

	##  List contents of directory.
	#   @param path		Path to show list (str).
	#   @param sort		Sort key (str).
	#   @n			If soret is 'name', list is sorted by file name.
	#			Otherwise, sorted by date and time.
	#   @param show		Specify time kind showed in the list.
	#   @n			'mtime': show last modification time.
	#   @n			'ctime': show last status change time.
	#   @n			'atime': show last access time.
	#   @returns		List of file informations ([str]).
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

	##  Copy files and directories (Helper method of self.cp()).
	#   @param src		Source file path (str).
	#   @param dst		Destination file path (str).
	#   @param follow_symlinks
	#   @n			When False and argument 'src' is symbolic link,
	#			make new symbolic link instead of copying file
	#			body linked by 'src'.
	#   @retval 0		successful
	#   @retval 1		failure
	#
	def __cp(self, src, dst, follow_symlinks=True):
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

	##  Remove files and directories (Helper method of self.rm()).
	#   @param path		File path (str).
	#   @param use_shutil	Use shutil.rmtree() to remove directory or not.
	#   @n			See self.rm() for detail.
	#   @param idle_time	Set idling time in second.
	#   @n			See self.rm() for detail.
	#   @retval 0		successful
	#   @retval 1		failure
	#
	def __rm(self, path, use_shutil, idle_time):
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

	##  Remove all files/directories under specified directory.
	#   @param path		Directory path to remove its contents (str).
	#   @retval 0		successful
	#   @retval 1		failure
	#
	def __remove_all(self, path):
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

	##  Get current time (for touch method).
	#   @retval		Last access time (in msec).
	#   @retval		Last modification time (in msec).
	#
	#		This method returns the same time as atime and mtime.
	#
	def __time_to_set(self):
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

	##  List information for one file (Helper method of ls()).
	#   @param path		File path to show (str).
	#   @param show		Specify time kind showed in the list.
	#   @n			'mtime': show last modification time.
	#   @n			'ctime': show last status change time.
	#   @n			'atime': show last access time.
	#   @returns		File information (str).
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

	##  Add file modifier to given path string.
	#   @param path		Path string.
	#   @param mode		File permission string.
	#   @returns		Modifier added path string.
	#
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

	##  Sort ls list by specified field.
	#   @param lslist	File list ([str]).
	#   @n			List format is;
	#   @n			<perm> <link> <size> <date> <time> <name>
	#   @param sort		Sort key.
	#   @n			If 'name', list is sorted by file name.
	#			Otherwise, sorted by date and time.
	#   @returns		Sorted file list ([str]).
	#
	def __ls_sort(self, lslist, sort):
		if sort == 'name':
			return sorted(lslist, key=lambda f: f.split()[5])
		return sorted(lslist, key=lambda f: f.split()[3]+f.split()[4])

# end: FileOp.py

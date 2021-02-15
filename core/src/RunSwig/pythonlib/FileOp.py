#!/usr/local/bin/python
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
#	[string] = ls(path, sort='name', show='mtime', backtrack=True)
#	[infos] in tree_walk(path, recurse=True, backtrack=False)
#	    infos = [[isdir, root, name, stat], ...]
#
# ----------------------------------------------------------------------
#  VERSION:
#     Ver 1.00   2017/08/20 F.Kanehori	First version.
#     Ver 1.01   2017/09/11 F.Kanehori	Aargument 'dry_run' added.
#     Ver 1.02   2017/09/11 F.Kanehori	Implement move() for unix.
#     Ver 1.03   2017/10/23 F.Kanehori	Add argument to ls().
#     Ver 1.04   2018/03/01 F.Kanehori	Rewrite cp/mv/rm using shutil.
#     Ver 1.05   2018/03/05 F.Kanehori	Add mkdir()/rmdir()/makedirs().
#     Ver 1.06   2018/03/14 F.Kanehori	Dealt with new Error class.
#     Ver 1.07   2018/04/19 F.Kanehori	New edition of ls().
#     Ver 1.08   2021/02/04 F.Kanehori	Can run on python 2.7.
# ======================================================================
from __future__ import print_function
import sys
import os
import datetime
import stat
import glob
import shutil
import math
import re
#from pathlib import Path
from time import sleep
from stat import *

sys.path.append('/usr/local/lib')
from Util import *
from Error import *
if Util.is_unix():
	import pwd
	import grp

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
		self.version = 1.08
		#
		self.info = info
		self.dry_run = dry_run
		self.verbose = verbose
		#
		self.major = sys.version_info[0]

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
		u_src = Util.upath(src)
		u_dst = Util.upath(dst)
		if self.dry_run or self.info:
			print('mv: %s -> %s' % (u_src, u_dst))
			if self.dry_run:
				return 0
		#
		rc = 0
		try:
			shutil.move(u_src, u_dst)
		except OSError as why:
			prog = '%s: mv' % self.clsname
			msg = str(why)
			Error(prog).error(msg)
			rc = 1
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
		u_path = Util.upath(path)
		if self.dry_run or self.info:
			print('rm: %s' % u_path)
			if self.dry_run:
				return 0
		#
		rc = 0
		for name in glob.glob(path):
			u_name = Util.upath(name)
			rc = self.__rm(u_name, use_shutil, idle_time)
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
			#fd = os.open(path, flags=flags, mode=mode)
			fd = os.open(path, flags, mode)
		except:
			if no_create:
				return 0
			return 1
		#
		if not isinstance(fd, int):
			fd = fd.fileno()
		ns_atime, ns_mtime = self.__time_to_set()
		#ns = (ns_atime, ns_mtime)
		if self.major >= 3:
			fn = fd if os.utime in os.supports_fd else path
			ns = (ns_atime, ns_mtime)
			os.utime(fn, ns=ns)
		else:
			fn = path
			ns_atime = (ns_atime // 1000000000) - 9*60*60
			ns_mtime = (ns_mtime // 1000000000) - 9*60*60
			ns = (ns_atime, ns_mtime)
			os.utime(fn, ns)
		os.close(fd)
		return 0

	##  List contents of directory.
	#   @param path		Path to show list (str or [str]).
	#   @param sort		Sort key (str).
	#   @n			If key is 'name', sorted by file name.
	#   @n			Otherwise, sorted by date and time.
	#   @param show		Specify time kind showed in the list.
	#   @n			'mtime': show last modification time.
	#   @n			'ctime': show last status change time.
	#   @n			'atime': show last access time.
	#   @param backtrack	Use backtrack (depth first) search (bool).
	#   @n			This implies recurse be True.
	#   @returns		List of file informations ([str]).
	#
	def ls(self, path, sort='name', show='mtime', backtrack=True):
		if path is None or path is '':
			path = '.'
		dcount = self.__ls_ftype_count(path, os.path.isdir)
		fcount = self.__ls_ftype_count(path, os.path.isfile)
		filefirst = dcount > 0 and fcount > 0
		blocksize = 1024
		#
		lslist = []
		for items in self.tree_walk(path, backtrack=True):
			if sort != 'name':
				self.__ls_sort(items, sort)
			total = self.__ls_total_size(items, blocksize)
			lslist.append('total %dK' % total)
			for item in items:
				lslist.append(self.__ls_edit(item, show))
		#
		return lslist

	##  Yields file information of given directory.
	#   @param path		Path to yield iterator (str).
	#   @param recurse	Walk directories recursively (bool).
	#   @param backtrack	Use backtrack (depth first) search (bool).
	#   @n			This implies recurse be True.
	#   @returns		Iterator of file infromation dictionary.
	#   @n			Keys are:
	#   @n			'isdir': Is directory or not (bool).
	#   @n			'root':	 Root directory path (str).
	#   @n			'name':	 File name (str).
	#   @n			'stat':	 File stat information (obj).
	#
	def tree_walk(self, path, recurse=False, backtrack=False):
		if backtrack:
			recurse = True
		if isinstance(path, str):
			path = [path]
		#
		pathlist = []
		for name in path:
			exp1 = os.path.expandvars(name)
			exp2 = os.path.expanduser(exp1)
			abspath = os.path.abspath(exp2)
			if os.path.isdir(abspath):
				tmplist = glob.glob('%s/*' % abspath)
				pathlist.extend(sorted(Util.upath(tmplist)))
			else:
				tmplist = glob.glob(abspath)
				pathlist.extend(sorted(Util.upath(tmplist)))
		#
		result = []
		dnames = []
		for name in pathlist:
			name = Util.upath(name).replace('//', '/')
			if os.path.isdir(name):
				info = self.__ls_info(True, name, None)
				result.append(info)
				if backtrack:
					itr = self.tree_walk(name, True, True)
					for r in itr:
						result.extend(r)
				else:
					dnames.append(name)
			else:
				dname = os.path.dirname(name)
				bname = os.path.basename(name)
				info = self.__ls_info(False, dname, bname)
				result.append(info)
		yield result
		#
		for name in dnames:
			tree_walk(name, True, False)


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
		rc = 0
		try:
			if self.verbose:
				print('    cwd: %s' % Util.upath(os.getcwd()))
				print('    src: %s' % Util.upath(src))
				print('    dst: %s' % Util.upath(dst))
			if os.path.isdir(src):
				if os.path.exists(dst):
					prog = '%s: cp' % self.clsname
					msg = '"%s" exists' % dst
					Error(prog).error(msg)
					return 1
				if self.verbose:
					print('    shutil.copytree()')
				shutil.copytree(src, dst, follow_symlinks)
			else:
				if self.verbose:
					print('  shutil.copy2()')
				shutil.copy2(src, dst)
		except OSError as why:
			prog = '%s: cp' % self.clsname
			Error(prog).error(str(why))
			rc = 1
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
		if self.verbose:
			print('__rm: %s' % path)
		try:
			if os.path.isdir(path) and not os.path.islink(path):
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
			Error(prog).error(str(why))
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
		rc = 0
		for root, dirs, files in os.walk(path, topdown=False):
			try:
				for name in files:
					fpath = '%s/%s' % (root, name)
					if os.path.islink(fpath):
						os.remove(fpath)
					else:
						os.chmod(fpath, stat.S_IWRITE)
						os.unlink(fpath)
				for name in dirs:
					dpath = '%s/%s' % (root, name)
					os.rmdir(dpath)
			except OSError as why:
				m = re.match('\[Errno (\d+)\].*', str(why))
				prog = '%s: rm' % self.clsname
				msg = str(why)
				Error(prog).error(msg)
				rc = 1
		#
		if another_drive:
			os.chdir(cwd)
		return rc

	##  Get current time (for touch method).
	#   @retval atime	Last access time (in msec).
	#   @retval mtime	Last modification time (in msec).
	#   @n
	#   NOTE: This method returns the same time as atime and mtime.
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
		if self.major >= 3:
			ts = int(dt.timestamp() * 1000000 + 0.0000005) * 1000
		else:
			dt1 = (dt - datetime.datetime(1970,1,1)).total_seconds()
			dt2 = datetime.timedelta(milliseconds=1).total_seconds()
			dt = dt1 // dt2
			ts = int(dt * 1000000 + 0.0000005)
		#print("{:,}".format(ts))
		return ts, ts

	##  Count satisfying given condition in the list.
	#   @param path		File path(s) (str or [str]).
	#   @param func		Function object to test condition.
	#   @returns		Number of files to satisfy given condition.
	#
	def __ls_ftype_count(self, path, func):
		if isinstance(path, str):
			path = [path]
		pathlist = []
		for p in path:
			pathlist.extend(glob.glob(p))
		return len(list(filter(lambda x: func(x), pathlist)))

	##  Sort by time.
	#   @param items	File info items ([dict]).
	#   @retunrs		Sorted items ([dict]).
	#
	def __ls_sort(self, items, sort):
		if sort == 'mtime':
			items.sort(key = lambda item: item['stat'].st_mtime)
		elif sort == 'ctime':
			items.sort(key = lambda item: item['stat'].st_ctime)
		elif sort == 'atime':
			items.sort(key = lambda item: item['stat'].st_atime)

	##  Caluculate total block size.
	#   @param items	File info items ([dict]).
	#   @param blocksize	Inode block size in bytes (int).
	#   @returns		Number of blocks (int).
	#
	def __ls_total_size(self, items, blocksize):
		size = 0
		for item in items:
			s = item['stat'].st_size
			size += math.floor((s + blocksize - 1) / blocksize)
		return size

	##  Edit file info.
	#   @param item		File info item (dict).
	#   @param show		Which time mode to show (str).
	#   @n			'mtime': Show last modification time.
	#   @n			'ctime': Show file creation time.
	#   @n			'atime': Show last access time.
	#   @returns		File info line (str).
	#
	def __ls_edit(self, info, show):
		root = info['root']
		name = info['name']
		fname = root if info['isdir'] else '%s/%s' % (root, name)
		fstat = info['stat']
		if fstat is None:
			return fname	# no stat info available
		#fmode = stat.filemode(fstat.st_mode)
		fmode = self.__filemode(fstat.st_mode)
		fdrive = os.path.splitdrive(fname)[0].lower()
		cdrive = os.path.splitdrive(os.getcwd())[0].lower()
		if fdrive == cdrive:
			fname = os.path.relpath(fname)
		fname = self.__add_modifier(fname, fmode)
		nlink = fstat.st_nlink
		if Util.is_unix():
			uid = pwd.getpwuid(fstat.st_uid).pw_name
			gid = grp.getgrgid(fstat.st_gid).gr_name
		else:
			uid = os.getlogin()
			gid = fstat.st_gid
		fsize = fstat.st_size
		if show == 'mtime': timemode = fstat.st_mtime
		if show == 'ctime': timemode = fstat.st_ctime
		if show == 'atime': timemode = fstat.st_atime
		dt = datetime.datetime.fromtimestamp(int(timemode))
		ftime = dt.strftime('%Y-%m-%d %H:%M')
		fmt = '{0} {1} {2} {3} {4:>10} {5} {6}'
		ls = fmt.format(fmode, nlink, uid, gid, fsize, ftime, fname)
		return Util.upath(ls)

	##  Add file modifier to given path string.
	#   @param path		File path (str).
	#   @param mode		File permission (str).
	#   @returns		Modifier added path (str).
	#
	def __add_modifier(self, path, mode):
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
			#linkedmode = stat.filemode(linkedstat.st_mode)
			linkedmode = self.__filemode(linkedstat.st_mode)
			info = self.__add_modifier(linkedfile, linkedmode)
			return '%s -> %s' % (path, info)
		return path

	##  Set file information.
	#   @param isdir	Is directory or not (bool).
	#   @param root		Directory (part of the file) path (str).
	#   @param name		File leaf name (str) or None (directory).
	#   @returns		File info object (dict).
	#
	def __ls_info(self, isdir, root, name):
		info = {'isdir': isdir, 'root': root, 'name': name}
		fname = root if isdir else '%s/%s' % (root, name)
		try:
			os.stat_float_times(True)
			fstat = os.stat(fname)
			info['stat'] = fstat
		except:
			info['stat'] = None
		return info

	##  Wrapper function of stat.filemode
	#   @param mode		File mode
	#   @returns		File mode string in '-rwxrwxrwx' format.
	#
	def __filemode(self, st_mode):
		if self.major >= 3:
			# version 3 or later
			return stat.filemode(st_mode)
		#
		# for older version (2.7)
		#
		fmode = 'd' if S_ISDIR(st_mode) else '-'
		fmode += 'r' if (st_mode & S_IRUSR) else '-'
		fmode += 'w' if (st_mode & S_IWUSR) else '-'
		fmode += 'x' if (st_mode & S_IXUSR) else '-'
		fmode += 'r' if (st_mode & S_IRGRP) else '-'
		fmode += 'w' if (st_mode & S_IWGRP) else '-'
		fmode += 'x' if (st_mode & S_IXGRP) else '-'
		fmode += 'r' if (st_mode & S_IROTH) else '-'
		fmode += 'w' if (st_mode & S_IWOTH) else '-'
		fmode += 'x' if (st_mode & S_IXOTH) else '-'
		return fmode

# end: FileOp.py

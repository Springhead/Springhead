#!/usr/local/bin/python3.6
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:
#	FileOp
#	    Class for file operation functions.
#
#  INITIALIZER:
#	obj = FileOp(info=0, verbose=0, dry_run=False)
#	  arguments:
#	    info:	Show operation information (silent if 0) (int).
#	    verbose:	Verbose level (silent if 0) (int).
#	    dry_run:	Show command but do not execute it (bool).
#
#  METHODS:
#	status = cp(fm_path, to_path, force=False)
#	status = mv(fm_path, to_path, force=False)
#	status = rm(path, force=False)
#	    OS independent copy/move/remove command.
#	  arguments:
#	    fm_path:	Source file path to copy.
#	    to_path:	Destination file path to copy.
#	    path:	File path to move (rename) or remove.
#	    force:	Force operation if True.
#	  returns:	Process return code.
#
#	touch(path, mode=0o666, no_create=False)
#	    Method version of 'touch' command.
#	    If specified file exists, change its access time.
#	    If specified file does not exit and 'no_create' is False,
#	  arguments:
#	    create new file with file mode 'mode'.
#	    path:	File path to touch.
#	    mode:	File mode.
#	    no_create:	Do not create a file even the case.
#	  returns:	Always 0.
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
#	Ver 1.31 2017/12/07 F.Kanehori	Add spaces to dry_run message.
# ======================================================================
import sys
import os
import datetime
import stat
import subprocess
import glob
sys.path.append('/usr/local/lib')
from Proc import *
from Util import *

class FileOp:
	#  These are class instance methods.
	#
	def __init__(self, info=0, verbose=0, dry_run=False):
		self.clsname = self.__class__.__name__
		self.version = 1.3
		#
		self.info = info
		self.verbose = verbose
		self.dry_run = dry_run

	#  File copy command.
	#
	def cp(self, fm_path, to_path, force=False):
		fos = self.__fileop(self.info, self.verbose, self.dry_run)
		return fos.copy(fm_path, to_path, force)

	#  Unix like file move command.
	#
	def mv(self, fm_path, to_path, force=False):
		fos = self.__fileop(self.info, self.verbose, self.dry_run)
		if Util.is_unix():
			rc = fos.move(fm_path, to_path, force)
		else:
			fm_dir, fm_leaf = self.__split_path(fm_path)
			to_dir, to_leaf = self.__split_path(to_path)
			if fm_dir == to_dir:
				rc = fos.rename(fm_path, to_leaf, force)
			else:
				rc = fos.copy(fm_path, to_path, force)
				if rc == 0:
					info = fos.set_info(0)
					rc = fos.remove(fm_path, force)
					fos.set_info(info)
		return rc

	#  File remove command.
	#
	def rm(self, path, force=False):
		fos = self.__fileop(self.info, self.verbose, self.dry_run)
		if os.path.isdir(path):
			path += '/*'
		filelist = glob.glob(path)
		rc = 0
		for f in filelist:
			if os.path.isdir(f):
				continue
			rc = fos.remove(f, force)
			if rc != 0:
				break
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
			return ''
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

	class __fileop:
		def __init__(self, info, verbose, dry_run):
			self.info = info
			self.verbose = verbose
			self.dry_run = dry_run

		def copy(self, fm_path, to_path, force):
			cmnd = 'cp' if Util.is_unix() else 'copy /V'
			if force:
				args = self.__select('-f', '/Y')
				cmnd += ' %s' % args
			return self.__exec(cmnd, fm_path, to_path)

		def rename(self, fm_path, to_path, force):
			#  This applies to Windows only.
			cmnd = 'rename'
			return self.__exec(cmnd, fm_path, to_path)

		def move(self, fm_path, to_path, force):
			#  This applies to unix only.
			cmnd = 'mv'
			if force:
				args = '-f'
				cmnd += ' %s' % args
			return self.__exec(cmnd, fm_path, to_path)

		def remove(self, path, force):
			cmnd = 'rm' if Util.is_unix() else 'del'
			if force:
				args = self.__select('-f', '/F /Q')
				cmnd += ' %s' % args
			return self.__exec(cmnd, path)

		def __select(self, unix, win):
			return unix if Util.is_unix() else win

		def __exec(self, cmnd, path1, path2=None):
			if self.dry_run:
				msg = '  %s %s' % (cmnd, Util.pathconv(path1))
				if path2:
					msg += ' ' + Util.pathconv(path2)
				print(msg)
				return 0
			if self.info:
				if cmnd[0:2] == 'rm' or cmnd[0:3] == 'del':
					msg = "removed `%s'" % path1
				else:
					msg = "`%s' -> `%s'" % (path1, path2)
				print(Util.upath(msg))
			args = [cmnd]
			args.append(Util.pathconv(path1))
			if path2:
				args.append(Util.pathconv(path2))
			#
			null = Proc.NULL
			proc = Proc(self.verbose)
			proc.exec(args, stdout=null, stderr=null, shell=True)
			return proc.wait()

		def set_info(self, info):
			old_info = self.info
			self.info = info
			return old_info

	#  Split file path into directory part and file leaf part.
	#
	def __split_path(self, path):
		# argument:
		#   path	File path.
		# returns:	Directory part (str) and leaf name (str).

		abspath = os.path.abspath(path)
		parts = Util.upath(abspath).split('/')
		dpart = parts[:len(parts)-1]
		fpart = parts[-1]
		return '/'.join(dpart), fpart

	#  Get current time (for touch method).
	#
	def __time_to_set(self):
		# argument:
		#   verbose	Verbose option.
		# returns:	Time stamp.

		d = datetime.today()
		CC, YY, MM, DD = 20, d.year%100, d.month, d.day
		hh, mm, ss, ms = d.hour, d.minute, d.second, d.microsecond
		if self.verbose > 1:
			fmt = '  current time: %s' % datetime_format
			print(fmt % (CC, YY, MM, DD, hh, mm, ss, ms))
		yy = CC * 100 + YY
		dt = datetime(yy, MM, DD, hh, mm, ss, ms)
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
		dt = datetime.fromtimestamp(int(timemode))
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

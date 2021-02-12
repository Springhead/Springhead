#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:	Util()
#
#  METHODS:
#	bool = is_unix()
#	bool = is_windows()
#	string = pathconv(path, to=None)
#	string = upath(path)
#	string = wpath(path)
#	string = date()
#	string = time()
#	string = now()
#
#	makedirs(path[, mode])
#
# ----------------------------------------------------------------------
#  VERSION:
#     Ver 1.00   2016/10/13 F.Kanehori	Release version.
#     Ver 2.00   2017/08/19 F.Kanehori	Some methods are moved to
#					Proc class and FileOp class.
#     Ver 2.01   2017/09/11 F.Kanehori	Add alias method for pathconv().
#     Ver 2.02   2018/02/19 F.Kanehori	Change default date/time format.
#     Ver 2.02.1 2018/03/08 F.Kanehori	Now OK for doxygen.
#     Ver 2.02.2 2020/11/01 F.Kanehori	Fix comment.
#     Ver 2.03   2021/02/04 F.Kanehori	Can run on python 2.7.
# ======================================================================
#from __future__ import print_function
import sys
import os
import datetime

##  Miscelaneous utility method class.
#
class Util:
	##  The initializer.
	#
	def __init__(self):
		self.clsname = self.__class__.__name__
		self.version = 2.03
	##  Determine if currently running under unix.
	#   Class instance version of Util.is_unix().
	#   @returns		True if yes, False otherwise.
	def is_unix(self): return Util.is_unix()
	##  Determine if currently running under Windws.
	#   Class instance version of Util.is_windows().
	#   @returns		True if yes, False otherwise.
	def is_windows(self): return Util.is_windows()
	##  Change path separators according to given OS name 'to'.
	#   Class instance version of Util.pathconv().
	#   @returns		Separator replaced string.
	def pathconv(self, **keywords): return Util.pathconv(**keywords)
	##  Get current date string.
	#   Class instance version of Util.date().
	#   @returns		Path string.
	def date(self, **keywords): return Util.date(**keywords)
	##  Get current time string.
	#   Class instance version of Util.time().
	#   @returns		Time string.
	def time(self, **keywords): return Util.time(**keywords)
	##  Get current date and time string.
	#   Class instance version of Util.now().
	#   @returns		Date and time string.
	def now(self, **keywords): return Util.now(**keywords)
	##  Wrapper of makefirs (version 3 and after).
	#   Class instance version of Util.makefies().
	#   @returns		none
	def makedirs(self, **keywords): return Util.makedirs(**keywords)

	##  Determine if currently running under unix.
	#   @returns		True if yes, False otherwise.
	#
	@staticmethod
	def is_unix():
		return os.name == 'posix'

	##  Determine if currently running under Windws.
	#   @returns		True if yes, False otherwise.
	#
	@staticmethod
	def is_windows():
		return os.name == 'nt'

	##  Change path separators according to given OS name 'to'.
	#   @param path		Path string to convert.
	#   @param to		Target OS name. One of follows;
	#			'unix', 'linux', 'posix',
	#			'windows, 'win', 'nt'.
	#   @returns		Separator replaced string.
	#
	@staticmethod
	def pathconv(path, to=None):
		if path is None:
			to = os.name	# assume current os
		elif isinstance(path, list):
			return list(map(lambda x: Util.pathconv(x, to), path))
		elif isinstance(path, dict):
			d = {}
			for k in path:
				d[k] = Util.pathconv(path[k], to)
			return d
		elif not isinstance(path, str):
			return path
		sep_u = ['\\', '/']
		sep_w = sep_u[::-1]
		sep = sep_u if Util.is_unix() else sep_w
		if to is not None:
			if to.lower() in ['unix', 'linux', 'posix']:
				sep = sep_u
			if to.lower() in ['windows', 'win', 'nt']:
				sep = sep_w
		return path.replace(sep[0], sep[1])

	##  Aliases for pathconv(path, to='unix').
	#   @param path		Path string to convert.
	#   @returns		Path string using '/' as path separator.
	#
	@staticmethod
	def upath(path):
		return Util.pathconv(path, 'unix')

	##  Aliases for pathconv(path, to='windows').
	#   @param path		Path string to convert.
	#   @returns		Path string using '\\\\' as path separator.
	#
	@staticmethod
	def wpath(path):
		return Util.pathconv(path, 'windows')

	##  Get current date string.
	#   @param format	Date string format.
	#   @returns		Date string.
	#
	#			Default format is 'yyyy-mm-dd'.
	#
	@staticmethod
	def date(format='%Y-%m-%d'):
		return Util.now(format)

	##  Get current time string.
	#   @param format	Time string format.
	#   @returns		Time string.
	#
	#			Default format is 'hh:mm:ss'.
	#
	@staticmethod
	def time(format='%H:%M:%S'):
		return Util.now(format)

	##  Get current date and time string.
	#   @param format	Date and time string format.
	#   @returns		Date and time string.
	#
	#			Default format is 'yyyy-mm-dd hh:mm:ss'.
	#
	@staticmethod
	def now(format='%Y-%m-%d %H:%M:%S'):
		return datetime.datetime.now().strftime(format)

	##  Wrapper of makefirs (version 3 and after).
	#   @param name		Directory name to create.
	#   @param mode		Permission mode of the directory.
	#   @param exist_ok	Do not raise exception if directory
	#			already exists.
	#   @returns		none
	#
	@staticmethod
	def makedirs(name, mode=0o777, exist_ok=False):
		exists = os.path.exists(name) and os.path.isdir(name)
		if exists and exist_ok:
			return
		os.makedirs(name, mode)

# end: Util.py

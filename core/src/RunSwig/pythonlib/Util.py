#!/usr/local/bin/python3.4
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:
#	Util
#	    Class for miscelaneous utility methods.
#	    Both 'member' and 'static' methods are available.
#
#  METHODS:
#	bool = is_unix()
#	bool = is_windows()
#	    Judges if running under unix or Windows.
#	    returns:	True or False
#
#	string = pathconv(path, to=None)
#	    Change path separators according to given OS name 'to'.
#	    If OS name is not given, current OS is assumed.
#	  arguments:
#	    path:	Original path string.
#	    to:		Destination OS name. One of follows;
#			    'unix', 'linux', 'posix',
#			    'windows', 'win', 'nt'
#	    returns:	Converted path string.
#
#	string = upath(path)
#	    Alias for pathconv(path, 'unix') 
#	string = wpath(path)
#	    Alias for pathconv(path, 'windows') 
#
#	string = date()
#	    Returns date string (format='%Y-%m-%d'). ['yyyy-mm-dd'].
#
#	string = time()
#	    Returns time string (format='%H:%M:%S'). ['hh:mm:ss']
#
#	string = now()
#	    Returns date and time string (format='%Y-%m-%d %H:%M:%S').
#					['yyyy-mm-dd hh:mm:ss']
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2016/10/13 F.Kanehori	Release version.
#	Ver 2.0  2017/08/19 F.Kanehori	Some methods are moved to
#					Proc class and FileOp class.
#	Ver 2.1  2017/09/11 F.Kanehori	Add alias method for pathconv().
#	Ver 2.2  2018/02/19 F.Kanehori	Change default date/time format.
# ======================================================================
import sys
import os
import datetime

class Util:
	#  These are class instance methods.
	#
	def __init__(self):
		self.clsname = self.__class__.__name__
		self.version = 2.1
	def is_unix(self): return Util.is_unix()
	def is_windows(self): return Util.is_windows()
	def pathconv(self, **keywords): return Util.pathconv(**keywords)
	def date(self, **keywords): return Util.date(**keywords)
	def time(self, **keywords): return Util.time(**keywords)
	def now(self, **keywords): return Util.now(**keywords)

	#  Judges if running under unix or Windows.
	#
	@staticmethod
	def is_unix():
		return os.name == 'posix'

	@staticmethod
	def is_windows():
		return os.name == 'nt'

	#  Change directory separators (slashes and back-slashes).
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

	#  Aliases for pathconv().
	#
	@staticmethod
	def upath(path):
		return Util.pathconv(path, 'unix')
	@staticmethod
	def wpath(path):
		return Util.pathconv(path, 'windows')

	#  Get current date and/or time string.
	#
	@staticmethod
	def date(format='%Y-%m-%d'):
		return Util.now(format)

	@staticmethod
	def time(format='%H:%M:%S'):
		return Util.now(format)

	@staticmethod
	def now(format='%Y-%m-%d %H:%M:%S'):
		return datetime.datetime.now().strftime(format)

# end: Util.py

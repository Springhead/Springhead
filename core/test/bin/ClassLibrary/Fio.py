#!/usr/local/bin/python3.4
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:	Fio(path, mode='r', verbose=0)
#
#  METHODS:
#	status = open(encoding=None)
#	close()
#	errmsg = error()
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2016/11/06 F.Kanehori	Release version.
#	Ver 1.1  2017/01/11 F.Kanehori	Rewrite method: open().
#	Ver 1.2  2017/01/12 F.Kanehori	Now sys.std{in|out|err} is OK.
#	Ver 2.0  2017/04/10 F.Kanehori	Ported to unix.
#	Ver 2.01 2017/09/08 F.Kanehori	Some subtle change.
#	Ver 2.02 2018/03/07 F.Kanehori	Now OK for doxygen.
# ======================================================================
import sys
import os
import io

##  Base class for file input/output.
#
class Fio:
	##  The initializer.
	#   @param path		File path to open (str)
	#			or system stream object
	#			    (sys.stdin|sys.stdout|sys.stderr).
	#   @param mode		File open mode
	#			    ('r'|'w'|'a' with optional 'b').
	#   @param verbose	Verbose level (0: silent) (int).
	#
	####def __init__(self, path, mode='r', disp_mode=None, verbose=0):
	def __init__(self, path, mode='r', verbose=0):
		self.clsname = self.__class__.__name__
		self.version = 2.01
		#
		self.sys_stream = self.__is_system_stream(path)
		self.path = path
		self.name = self.__stream_name(path)
		self.mode = mode
		####self.disp_mode = disp_mode
		self.verbose = verbose
		#
		self.obj = None		# file object
		self.opened = False
		self.errmsg = None

	##  Open the file.  Argument 'encoding' is effective only
	#   for text file write mode.
	#   @param encoding	Character encoding (text file, write open only).
	#			One of;
	#			    'ascii' (or 'us-ascii'),
	#			    'utf-8' (or 'utf_8', 'utf8'),
	#			    'cp932' (or 'shift_jis', 'sjis'),
	#			    'utf-16' (or 'unicode', 'utf16'),
	#			    'iso-2022-jp' (or 'jis'),
	#			    'euc-jp' (or 'euc').
	#   @returns		0: suaccess, -1: failure
	#
	def open(self, encoding=None):
		if self.sys_stream:
			self.opened = True
			self.obj = self.path
			return 0
		#
		path = self.path
		mode = self.mode
		if self.opened:
			self.close()
			self.opened = False
		try:
			self.obj = open(path, mode, encoding=encoding)
			self.opened = True
			status = 0
		except IOError as err:
			msg = 'file open error: "%s" (%s)\n%s' \
				% (self.path, self.__mode_str(), err)
			self.errmsg = msg
			self.opened = False
			status = -1
		return status

	##  Close the file.
	#
	def close(self):
		if self.opened:
			self.obj.close()
		self.obj = None
		self.pened = False
		self.errmsg = None

	##  Return error message (most recent one only).
	#   @returns		Error message if aney (str).
	#
	def error(self):
		return self.errmsg

	# --------------------------------------------------------------
	#  For class private use
	# --------------------------------------------------------------

	#  Check if system stream.
	#
	def __is_system_stream(self, stream):
		# arguments:
		#   stream:	Stream object to cjeck.
		# returns:	True if yes, otherwise False.

		return isinstance(stream, io.TextIOWrapper)

	##  Return stream name string.
	#   @param strem	Stream object.
	#   @returns		Stream name string.
	#
	def __stream_name(self, stream):
		if isinstance(stream, str):
			name = os.path.basename(stream)
		elif isinstance(stream, io.TextIOWrapper):
			name = stream.name
		else:
			name = 'unknown'
		return name

	##  Return readable file mode.
	#   @returns		File mode string.
	#
	def __mode_str(self):
		modes = { 'r':	'read',
			  'w':	'write',
			  'a':	'append',
			  'rb':	'read binary',
			  'wb':	'write binary',
			  'ab':	'append binary',
			  'invalid': 'invalid mode: %s' % self.mode
			}		      
		####mode = self.disp_mode if self.disp_mode else self.mode
		key = self.mode if self.mode in modes else 'invalid'
		return modes[key]

# end: Fio.py

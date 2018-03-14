#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:
#	ControlParams
#	    Test control parameter management class.
#	    Should be called from where a solution file exists.
#
#  INITIALIZER:
#	obj = ControlParams(fname, section=None, verbose=0)
#	arguments:
#	    fname:	Test control file name (str).
#	    section:	Test control section name (str).
#	    verbose:	Verbose level (0: silent) (int).
#
#  METHODS:
#	value = get(key, override=None, default=None)
#	    Get value associated with the key.  If arg 'override' is
#	    not None, return it instead of defined value in the test
#	    control file.
#	    arguments:
#		key:	    See below for aveilable keys defined (const).
#		override:   Always return this if specified (obj).
#		default:    Return this if key is not defined (obj).
#	    returns:	    Value associated with the key (obj).
#
#	msg = error()
#	    Check if something wrong has occured in setup process.
#	    returns:	    Error message if error has occurred so far.
#			    None if recorded no error.
#
#	info()
#	    Print information of this instance.
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2016/10/05 F.Kanehori	First version.
#	Ver 2.0  2017/08/10 F.Kanehori	Name changed: script, param file.
#	Ver 3.0  2017/09/13 F.Kanehori	Python library revised.
#	Ver 4.0  2018/02/26 F.Kanehori	全体の見直し.
#	Ver 4.01 2018/03/14 F.Kanehori	Dealt with new Error class.
# ======================================================================
import sys
import os

# local python library
#
sys.path.append('../../src/RunSwig')
from FindSprPath import *
spr_path = FindSprPath('ControlParams')
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from KvFile import *

from ConstDefs import *

class ControlParams:
	#  Class initializer
	#
	def __init__(self, fname, section=None, verbose=0):
		self.clsname = self.__class__.__name__
		self.version = 4.01
		#
		self.fname = fname
		self.section = section
		self.verbose = verbose
		#
		self.control = {}
		self.__set_default_path(self.control)
		self.__read_control_file(fname, section)
		self.errmsg = None

	#  Get value associated with the key.
	#  
	def get(self, key, override=None, default=None):
		if override:
			return override
		if key not in self.control.keys():
			return default
		return self.control[key]

	#  Check if some error has occured so far.
	#
	def error(self):
		msg = None
		if self.errmsg:
			msg = self.errmsg
		if msg is not None:
			msg = '%s: %s' % (self.clsname, msg)
		return msg

	#  Print information of this instance.
	#
	def info(self):
		print('ControlParams ("%s"):' % self.fname)
		for key in sorted(self.control.keys()):
			keyname = '%s:' % key
			print('  %-16s %s' % (keyname, self.get(key)))


	# --------------------------------------------------------------
	#  For class private use
	# --------------------------------------------------------------

	#  Read control file.
	#  Control file is searched and read from current directory and
	#  go upward until 'core' directory found.  This go up process
	#  is prevented by the special value "** DO NOT INHERIT **".
	#  As for the same key, lower level file has priority.
	#
	def __read_control_file(self, fname, section):
		# arguments:
		#   fname:	Control file name.
		#   section:	Control section name.

		cwd = os.getcwd().split(os.sep)
		siz = len(cwd)
		top = siz
		for n in range(siz):
			if cwd[n] in ['core']:
				top = n + 1
		for n in range(top, siz):
			prefix = '../' * (siz - n - 1)
			if os.path.exists(prefix + fname):
				path = Util.upath(os.path.abspath(prefix + fname))
				if self.verbose:
					print('read: %s' % path)
				kvf = KvFile(path, verbose=0)
				num_sections = kvf.read(self.control)
				self.__set_control_values(kvf, section)
				"""
				##kvf.show(1, section='Windows')
				print('read: %s' % path)
				print('SECTION: %s' % section)
				print('KEYS(): %s' % kvf.keys())
				print('KEYS(section): %s' % kvf.keys(section))
				for k in sorted(self.control.keys()):
					print('++ %-20s = %s' % (k, self.control[k]))
				"""

	#  Set control value (for all key-value pairs).
	#
	def __set_control_values(self, kvf, section):
		# arguments:
		#   kvf:	KvFile class object.
		#   section:	Section name (str).

		for key in kvf.keys(section):
			value = kvf.get(key, section=section)
			if value == '** DO NOT INHERIT **':
				del self.control[key]
				continue
			self.control[key] = value

	#  Set default control value.
	#
	def __set_default_path(self, dic):
		# arguments:
		#   dic:	Dictionary to set.

		spr_path = FindSprPath(self.clsname)
		dic['SprTop'] = spr_path.abspath()
		dic['SprTest'] = spr_path.abspath('test')


# ----------------------------------------------------------------------
#  Test main
# ----------------------------------------------------------------------
if __name__ == '__main__':

	from optparse import OptionParser
	from Proc import *
	from Error import *

	usage = 'Usage: %prog [options] file section'
	parser = OptionParser(usage = usage)
	parser.add_option('-v', '--verbose',
			dest='verbose', action='count',
			default=0, help='set verbose mode')
	(options, args) = parser.parse_args()
	if len(args) != 2:
		Proc().execute('python ControlParams.py -h').wait()
		sys.exit(-1)

	fname = args[0]
	section = args[1]

	ctl = ControlParams(fname, section)
	if ctl.error():
		Error().abort(ctl.error())
	ctl.info()

	sys.exit(0)

# end: ControlParams.py

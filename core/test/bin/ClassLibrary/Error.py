#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:	Error(prog, out=sys.stderr, verbose=0)
#
#  METHODS:
#	put(msg, prompt='Error', exitcode=-1, alive=False)
#	info(msg)
#	warn(msg)
#	error(msg, prompt='Error'):
#	abort(msg, prompt='Error', exitcode=-1)
#
# ----------------------------------------------------------------------
#  VERSION:
#     Ver 1.00   2016/11/06 F.Kanehori	First version.
#     Ver 2.00   2017/04/10 F.Kanehori	Ported to unix.
#     Ver 2.01   2017/08/15 F.Kanehori	Change test case message.
#     Ver 2.01   2017/09/11 F.Kanehori	Update print().
#     Ver 2.02   2017/09/13 F.Kanehori	Add abort().
#     Ver 2.03   2018/01/10 F.Kanehori	Add arg 'alive' to print().
#     Ver 2.03.1 2018/03/09 F.Kanehori	Now OK for doxygen.
#     Ver 3.00   2018/03/14 F.Kanehori	Change method name (print->put).
#					Add error(), warn(), info().
#     Ver 3.00.1 2018/03/19 F.Kanehori	Documentation.
#     Ver 3.01   2021/02/04 F.Kanehori	Can run on python 2.7.
# ======================================================================
#from __future__ import print_function
import sys

##  Error handling class.
#
class Error:
	##  The initializer.
	#   @param prog		Program name to place in message (str).
	#   @param out		Output stream (obj).
	#   @param verbose	Verbose level (0: silent) (int).
	#
	def __init__(self, prog, out=sys.stderr, verbose=0):
		self.clsname = self.__class__.__name__
		self.version = 3.01
		#
		self.prog = prog
		self.out  = out
		self.verbose = verbose
		self.to_die = ['Error', 'Abort', 'Fatal', 'Pan', 'Panic']
		#

	##  Print error message to output stream self.out.
	#   @param msg		Message to print (str).
	#   @n			Output format: 	'<prog>:[ <prompt>:] <msg>'.
	#   @param prompt	Prompt string or None.
	#   @n			If 'prompt' is one of 'Error', 'Fatal',
	#			'Abort' or 'Pan' (or 'Panic'), program will
	#			be terminated with given exitcode.
	#   @n			Argument 'alive' cancels this behaviour.
	#   @param exitcode	Exit code (int).
	#   @param alive	Force return to caller (bool).
	#
	def put(self, msg, prompt='Error', exitcode=-1, alive=False):
		die = False
		#if prompt in ['Error', 'Fatal', 'Abort', 'Pan', 'Panic']:
		if prompt in self.to_die:
			die = True if not alive else False
		str1 = '%s: ' % self.prog if self.prog else ''
		str2 = '%s: ' % prompt if prompt else ''
		msg_str = '%s%s%s' % (str1, str2, msg)
		if alive and self.verbose and die:
			msg_str += ' -> exit code %d' % exitcode
		#
		print(msg_str, file=self.out)
		self.out.flush()
		if die and not alive:
			sys.exit(exitcode)

	##  Print information message.
	#   @n	Abbreviation for:
	#	self.put(msg, prompt=None, alive=True)
	#   @n	NOTE:
	#   @n	Always output to sys.stdout nevertheless out parameter
	#	to the initializer.
	#   @param msg		Information message (str).
	#
	def info(self, msg):
		__out_save = self.out
		self.out = sys.stdout
		self.put(msg, prompt=None, alive=True)
		self.out = __out_save

	##  Print warning message.
	#   @n	Abbreviation for:
	#	self.put(msg, prompt='Warning', alive=True)
	#   @param msg		Warning message (str).
	#
	def warn(self, msg):
		self.put(msg, prompt='Warning', alive=True)

	##  Print error message.
	#   @n	Abbreviation for:
	#	self.put(msg, prompt='Error', alive=True)
	#   @n	Control always return to the caller.
	#   @param msg		Error message (str).
	#   @param prompt	See self.put().
	#
	def error(self, msg, prompt='Error'):
		self.put(msg, prompt=prompt, alive=True)

	##  Print error message and die.
	#   @n	Abbreviation for:
	#	self.put(msg, prompt='Error', exitcode=-1, alive=False)
	#   @n	Prompt should be one of
	#	'Error', 'Fatal', 'Abort', 'Pan' or 'Panic'.
	#	If other prompt is given, use 'Error' as default one.
	#   @n	Control never return to the caller.
	#   @param msg		Error message (str).
	#   @param prompt	See self.put().
	#   @param exitcode	See self.put().
	#
	def abort(self, msg, prompt='Error', exitcode=-1):
		if prompt not in self.to_die:
			prompt = 'Error'
		self.put(msg, prompt=prompt, exitcode=exitcode)

# end: Error.py

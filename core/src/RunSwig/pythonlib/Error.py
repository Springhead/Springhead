#!/usr/local/bin/python3.4
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:	Error(prog, out=sys.stderr, verbose=0)
#
#  METHODS:
#	print(msg, prompt='Error', exitcode=-1, alive=False)
#	abort(msg, prompt='Error', exitcode=-1)
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2016/11/06 F.Kanehori	First version.
#	Ver 2.0  2017/04/10 F.Kanehori	Ported to unix.
#	Ver 2.01 2017/08/15 F.Kanehori	Change test case message.
#	Ver 2.1  2017/09/11 F.Kanehori	Update print().
#	Ver 2.2  2017/09/13 F.Kanehori	Add abort().
#	Ver 2.3  2018/01/10 F.Kanehori	Add arg 'alive' to print().
#	Ver 2.21 2018/03/09 F.Kanehori	Now OK for doxygen.
# ======================================================================
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
		self.version = 2.3
		#
		self.prog = prog
		self.out  = out
		self.verbose = verbose
		#

	##  Print error message to output stream self.out.
	#   @param msg		Error message to print (str).
	#   @n			Output format: 	'<prog>:[ <prompt>:] <msg>'.
	#   @param prompt	Prompt string or None.
	#   @n			If 'prompt' is one of 'Error', 'Fatal',
	#			'Abort' or 'Pan', program will be terminated
	#			with given exitcode.
	#   @n			Argument 'alive' cancels this behaviour.
	#   @param exitcode	Exit code (int).
	#   @param alive	Force return to caller (bool).
	#
	def print(self, msg, prompt='Error', exitcode=-1, alive=False):
		die = False
		if prompt in ['Error', 'Fatal', 'Abort', 'Pan']:
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

	##  Print error message and die (Synonym for self.print()).
	#   NOTE
	#   @n	If you do not want to ceise program, you should not call
	#	this method to avoid confusion arises from method name!
	#   @param msg		Error message (str).
	#   @param prompt	See self.print().
	#   @param exitcode	See self.print().
	#
	def abort(self, msg, prompt='Error', exitcode=-1):
		self.print(msg, prompt, exitcode)

	# --------------------------------------------------------------
	#  For class private use
	# --------------------------------------------------------------

# end: Error.py

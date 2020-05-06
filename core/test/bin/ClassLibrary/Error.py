#!/usr/local/bin/python3.4
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:
#	Error
#	    Error handling wrapper class.
#
#  INITIALIZER:
#	obj = Error(prog, out=sys.stderr, verbose=0)
#	  arguments:
#	    prog:	Program name to precede message (str).
#	    out:	Output stream (obj).
#	    verose:	Verbose mode level (int; 0: silence).
#
#  METHODS:
#	print(msg, prompt='Error', exitcode=-1)
#	    Print error message to output stream.  If prompt is one of
#	    follows, program will be terminated with given exitcode.
#	  arguments:
#	    msg:	Error message to print (str).
#			Message format is;
#			    '<prog>:[ <prompt>:] <msg>'
#	    prompt:	Prompt string (str) or None.
#			    'Error'|'Fatal'|'Abort'|'Pan':
#				     Program terminates with exitcode.
#			    None:    Prompt is not printed.
#			    others:  Program will continue.
#	    exitcode:	Exit code (int).
#
#	abort(msg, prompt='Error', exitcode=-1)
#	    Synonym for print() method.
#	    If you do not want to ceise program, you should not call
#	    this method to avoid confusion arises from method name!
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2016/11/06 F.Kanehori	First version.
#	Ver 2.0  2017/04/10 F.Kanehori	Ported to unix.
#	Ver 2.01 2017/08/15 F.Kanehori	Change test case message.
#	Ver 2.1  2017/09/11 F.Kanehori	Update print().
#	Ver 2.2  2017/09/13 F.Kanehori	Add abort().
# ======================================================================
import sys

class Error:
	#  Class initializer
	#
	def __init__(self, prog, out=sys.stderr, verbose=0):
		self.clsname = self.__class__.__name__
		self.version = 2.2
		#
		self.prog = prog
		self.out  = out
		self.verbose = verbose
		#
		self.testcase = False

	#  Print message.
	#
	def print(self, msg, prompt='Error', exitcode=-1, die=False):
		if exitcode != 0:
			if prompt in ['Error', 'Fatal', 'Abort', 'Pan']:
				die = True
		str1 = '%s: ' % self.prog if self.prog else ''
		str2 = '%s: ' % prompt if prompt else ''
		msg_str = '%s%s%s' % (str1, str2, msg)
		if self.testcase and die:
			msg_str += '-> exit code %d' % exitcode
		#
		print(msg_str, file=self.out)
		self.out.flush()
		if die and self.testcase is False:
			sys.exit(exitcode)

	#  Print message and die.
	#
	def abort(self, msg, prompt='Error', exitcode=-1):
		self.print(msg, prompt, exitcode, die=True)

	#  For debug only.
	#
	def set_testcase(self, testcase):
		self.testcase = testcase

	# --------------------------------------------------------------
	#  For class private use
	# --------------------------------------------------------------

# end: Error.py

#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:
#	KEYINTR
#	    Keyboard interruption handling class.
#	    No initializer (static method only).
#
#  METHODS:
#	set_interrupted(flag=True)
#	    Set interruption flag.
#	    arguments:
#		flag:	    True or False.
#
#	is_interrupted()
#	    Tell if interruption flag is set or not.
#	    arguments:	    None
#	    returns:	    True if interrupt flag is set. False otherwise.
#
#	what()
#	    Returns the reason of interruption.
#	    arguments:	    None
#	    returns:	    The reason of interruption (str).
#
#	handler(signum, frame)
#	    Interruption handler.
#	    arguments:
#		signum:	    Signal number (int).
#		frame:	    Stack frame information (obj).
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2018/02/08 F.Kanehori	First version.
# ======================================================================
import sys

class KEYINTR(Exception):
	interrupted = False

	#  Set interruption flag.
	#
	@staticmethod
	def set_interrupted(flag=True):
		KEYINTR.interrupted = flag

	#  Tell if interruption flag is set or not.
	#
	@staticmethod
	def is_interrupted():
		return KEYINTR.interrupted

	#  Returns the reason of interruption.
	#
	@staticmethod
	def what(self):
		return 'KEYINTR'

	#  Interruption handler.
	#
	@staticmethod
	def handler(signum, frame):
		print()
		print('Signal handler called with signal', signum)
		sys.stdout.flush()
		KEYINTR.set_interrupted()

# end: KeyInterruption.py

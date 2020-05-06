#!/usr/bin/env python
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:
#	SEH
#	    Interruptions and exceptios handling class.
#
#  INITIALIZER:
#	obj = SEH()
#
#  METHODS:
#	is_seh(code)
#	  arguments:
#	    code:	Exception code (int).
#			See "dailybuild_SEH_Handler.h".
#	  returns:	Structured Exception or not (bool).
#
#	seh_str(code)
#	  arguments:
#	    code:	Exception code (int).
#	  returns:	Windows error code in hex form (str).
#
#	seh_code(code):
#	  arguments:
#	    code:	Exception code (int).
#	  returns:	Description of this exception (str).
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2016/09/29 F.Kanehori	First version.
#	Ver 1.01 2017/09/11 F.Kanehori	Comment added.
# ======================================================================

# ----------------------------------------------------------------------
#  Structured exception adapter class.
# ----------------------------------------------------------------------
class SEH:
	code_def = [
		[ -9002, '0xC0000002', 'Datatype Misalignment' ],
		[ -9005, '0xC0000005', 'Access Violation' ],
		[ -9008, '0xC0000008', 'Invalid Handle' ],
		[ -9140, '0xC000008C', 'Array Bounds Exceeded' ],
		[ -9148, '0xC0000094', 'Integer Divide by Zero' ],
		[ -9149, '0xC0000095', 'Integer Overflow' ],
		[ -9253, '0xC00000FD', 'Stack Overflow' ],
		[ -9404, '0xC0000194', 'Possible Deadlock' ],
		[ -9903, '0x80000003', 'Breakpoint' ],
		[ -9908, '0x40010008', 'Debug Control Break' ]
	]

	@staticmethod
	def is_seh(code):
		max_code = -9000
		#min_code = -9999
		if code is None:
			return False
		#if min_code <= int(code) and int(code) <= max_code:
		if int(code) <= max_code:
			return True
		return False

	@staticmethod
	def seh_str(code):
		return SEH.__seh_str_sub(code)[1]

	@staticmethod
	def seh_code(code):
		return SEH.__seh_str_sub(code)[0]

	@staticmethod
	def __seh_str_sub(code):
		if code is None:
			return '0', '--'
		if not SEH.is_seh(code):
			return '0', 'not SEH'
		for n in range(len(SEH.code_def)):
			if code == SEH.code_def[n][0]:
				return SEH.code_def[n][1], SEH.code_def[n][2]
		code_str = hex(-(code + 10000)).upper().replace('0X', '0x')
		if code_str == '0x7E':
			return code_str, 'SEH: System Thread Exception Not Handled'
		return code_str, 'SEH: Unknown Exception: %s' % code_str

# end: StructuredException.py

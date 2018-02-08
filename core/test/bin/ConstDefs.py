#!/usr/bin/env python
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:
#	ConstDefs
#	    Definition of constants used througho out this system.
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2018/02/07 F.Kanehori	First version.
# ======================================================================
from enum import Enum

class CFK():		#  Control File Keywords.
	SPRTOP		= 'SprTop'
	SPRTEST		= 'SprTest'
	EXCLUDE		= 'Exclude'
	DESCEND		= 'Descend'
	SOLUTION_ALIAS	= 'SolutionAlias'
	BUILD		= 'Build'
	USE_CLOSED_SRC	= 'UseClosedSrc'
	CPP_MACRO	= 'CppMacro'
	BUILD_LOG	= 'BuildLog'
	BUILD_ERR_LOG	= 'BuildErrLog'
	RUN		= 'Run'
	OUTPUT_DIR	= 'OutputDir'
	BINARY_OUT	= 'BinaryOut'
	TIMEOUT		= 'Timeout'
	EXPECTED	= 'Expeted'
	ADD_PATH	= 'AddPath'
	PIPE_PROCESS	= 'PipeProcess'
	KILL_PROCESS	= 'KillProcess'
	RUN_LOG		= 'RunLog'
	RUN_ERR_LOG	= 'RunErrLog'
	INTERVENTION	= 'Intervention'

class RST(Enum):		#  ReSulT category.
	ERR		= 0	#    error iinformation
	SKP		= 1	#    skip information
	BLD		= 2	#    build result
	RUN		= 3	#    run result
	EXP		= 4	#    expected result status

class CSU(Enum):		#  Closed Srouce Usage.
	AUTO		= 0	#    change header file accoding to control file
	USE		= 1	#    test solutions only with UseClosedSrc=True
	UNUSE		= 2	#    test solutions only with UseClosedSrc=False

class TESTID(Enum):		#  Test ID.
	STUB		= 0	#     Stub build.
	TESTS		= 1	#     src/tests.
	SAMPLES		= 2	#     src/Samples
	OTHER		= 3	#     others

#  List of platform names.
#
PLATS	= [ 'x86', 'x64' ]

#  List of configuration names.
#
CONFS	= [ 'Debug', 'Release', 'Trace' ]


# ----------------------------------------------------------------------
#  Test main
# ----------------------------------------------------------------------
if __name__ == '__main__':
	import sys

	def print_list(cls):
		for name, member in cls.__members__.items():
			value = member.value
			print('  %-24s%s (%s)' % (member, name, value))

	print('-- Control file keywords --')
	print_list(CFK)
	print()
	
	print('-- Result log indices --')
	print_list(RST)
	print()

	print('-- Closed src usage --')
	print_list(CSU)
	print()

	print('-- List of platform names  --')
	print('  %s' % ', '.join(PLATS))
	print()

	print('-- List of configuration names  --')
	print('  %s' % ', '.join(CONFS))

	sys.exit(0)

# end: ConstDefs.py

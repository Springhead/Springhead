#!/usr/local/bin/python3.4
# -*- coding: utf-8 -*-
# ==============================================================================
#  FILE:
#	FindSprPath.py
#
#  CLASS:
#	FindSprPath:  Find Springhead top path (absolute and relative).
#
#  INITIALIZER:
#	obj = FindSprPath(prog, verbose=0)
#	    prog:	Program name to precede message (str).
#	    verbose:	Verbose mode level (0: silence).
#
#  METHODS:
#	path = abspath(select=None)
#	    Get absolute path of specified directory.
#	    select:	Directory selector.
#			    None:	Springhead top directory.
#			    'core'	<top>/core directory.
#			    'bin'	<core>/bin directory.
#			    'inc'	<core>/include directory.
#			    'src'	<core>/src directory.
#			    'test'	<core>/test directory.
#			    'pythonlib'	<src>/RunSwig directory.
#			    'buildtool'	buildtool directory.
#	    returns:	Absolute path of selected directory.
#	
#	path = relpath(select=None)
#	    Get relative path (to cwd) of specified directory.
#	    select:	Directory selector.
#			(Same that of abspath() above)
#	    returns:	Relative path of selected directory.
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2017/07/27 F.Kanehori	First version.
#	Ver 1.1  2017/10/05 F.Kanehori	Change serach logic.
#	Ver 1.2  2017/10/19 F.Kanehori	Add test case (non-exist).
#	Ver 1.3  2017/11/08 F.Kanehori	Add 'buildtool' to selector.
#	Ver 1.4  2017/11/29 F.Kanehori	Add 'pythonlib' to selector.
#	Ver 1.41 2018/03/09 F.Kanehori	Bug fixed.
# ----------------------------------------------------------------------
import sys
import os

class FindSprPath:
	#  Class initializer
	#
	def __init__(self, prog, verbose=0):
		self.prog = prog
		self.version = 1.4
		self.verbose = verbose
		#
		cwd = os.getcwd().split(os.sep)[::-1]
		lookup = 'core'
		top = None
		for n in range(len(cwd)):
			if not cwd[n].startswith(lookup): continue
			top = '/'.join(cwd[::-1][0:len(cwd)-n-1])
			break
		if 'top' not in vars():
			self.__err('can\'t find directory "%s"' % lookup)
			self.__err('cwd is "%s"' % os.getcwd())
		self.top = top

	#  Get absolute path.
	#
	def abspath(self, select=None):
		if select is None:
			return self.top
		elif select is 'core':
			return '%s/core' % self.top
		elif select is 'bin':
			return '%s/core/bin' % self.top
		elif select is 'inc':
			return '%s/core/include' % self.top
		elif select is 'src':
			return '%s/core/src' % self.top
		elif select is 'test':
			return '%s/core/test' % self.top
		elif select is 'pythonlib':
			return '%s/core/src/RunSwig/pythonlib' % self.top
		elif select is 'buildtool':
			return '%s/buildtool' % self.top
		print('%s: bad selector "%s"' % (self.prog, select), file=sys.stderr)
		return None

	#  Get relative path.
	#
	def relpath(self, select=None):
		abs_path = self.abspath(select)
		if abs_path is None:
			return None
		return os.path.relpath(abs_path).replace(os.sep, '/')

	# --------------------------------------------------------------
	#  For class private use
	# --------------------------------------------------------------

	#  Put error message to stderr.
	#
	def __err(self, msg):
		cname = self.__class__.__name__
		print('%s: %s' % (cname, msg), file=sys.stderr)


# ======================================================================
#  Self test code.
#
if __name__ == '__main__':
	verbose = 0
	obj = FindSprPath('TEST', verbose=verbose)

	print('test start at\n\t%s' % os.getcwd().replace(os.sep, '/'))

	selectors = ['top', 'core', 'bin', 'inc', 'src', 'test', 'NonExist']

	print()
	for sel in selectors:
		print('%s:\t%s' % (sel, obj.abspath(None if sel == 'top' else sel)))
	print()
	for sel in selectors:
		print('%s:\t%s' % (sel, obj.relpath(None if sel == 'top' else sel)))

	sys.exit(0)

# end: FindSprPath.py

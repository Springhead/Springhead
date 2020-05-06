#!/usr/bin/env python
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:
#	VersionControlSystem:
#		Wrapper class to get some information like revision
#		number from source code version control system.
#		Currently available systems are:
#		  'Subversion'	    Apache Subversion system
#		  'GitHub'	    GitHub system
#
#  INITIALIZER:
#	obj = VersionControlSystem(system, args=None, verbose=0)
#	arguments:
#	    system:	Version control system name.
#			'Subversion'	Apache Subversion system
#			'GitHub'	GitHub system
#	    args:	Atguments to get system information (dic).
#			  'url':	Url of the repository.
#	    verbose:	Verbose level (int) (0: silent).
#	    
#  METHODS:
#	code, rev, err = revision()
#	    Get revision number string.
#	    arguments:
#	      code:	    0: success, other: failure (int).
#	      rev:	    Revision (str).
#	      err:	    Error message got from stderr (str).
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2017/01/11 F.Kanehori	First version (Subversion only).
#	Ver 1.1  2017/09/13 F.Kanehori	Python library revised.
#	Ver 1.2  2017/11/16 F.Kanehori	Python library path ÇÃïœçX.
#	Ver 1.3  2017/12/07 F.Kanehori	GitHub î≈é¿ëï.
# ======================================================================
import sys
import os
import re

# local python library
#
sys.path.append('../../src/RunSwig')
from FindSprPath import *
spr_path = FindSprPath('VersionControlSystem')
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from Util import *
from Proc import *

# ----------------------------------------------------------------------
#  Class VersionControlSystem
#
class VersionControlSystem:

	#  Class initializer.
	#
	def __init__(self, system, args=None, verbose=0):
		self.clsname = self.__class__.__name__
		self.version = 1.3
		#
		self.system = system
		self.verbose = verbose
		#
		if system == 'Subversion':
			self.obj = self.Subversion(args['url'], verbose)
		elif system == 'GitHub':
			self.obj = self.GitHub(args['url'], verbose)
		#
		if verbose:
			print('VersionControlSystem: %s' % system)

	#  Get revision number.
	#
	def revision(self, dir=None):
		if dir == None:
			revision = self.obj.revision()
		else:
			cwd = os.getcwd()
			os.chdir(dir)
			revision = self.obj.revision()
			os.chdir(cwd)
		return revision

	#  Subclass: Subversion
	#
	class Subversion:
		def __init__(self, url, verbose):
			self.url = url
			self.verbose = verbose

		def revision(self):
			url = self.url
			cmnd = 'svn info'
			proc = Proc(verbose=self.verbose)
			proc.exec(cmnd, stdout=Proc.PIPE, stderr=Proc.STDOUT)
			status = proc.wait()
			out, err = proc.output()
			#
			revision = "can't get current revision"
			if status == 0:
				lines = out.split('\n')
				pattern = 'Last Changed Rev:\s+(\d+).*'
				for s in lines:
					m = re.match(pattern, s)
					if m:
						revision = m.group(1)
			return status, revision, err

	#  Subclass: GitHub
	#
	class GitHub:
		def __init__(self, url, verbose):
			self.url = url
			self.verbose = verbose

		def revision(self):
			url = self.url
			cmnd1 = 'git log --abbrev-commit --oneline --max-count=1'
			cmnd2 = 'nkf -s'
			proc1 = Proc(verbose=self.verbose)	# git
			proc2 = Proc(verbose=self.verbose)	# nkf
			proc1.exec(cmnd1, stdout=Proc.PIPE, stderr=Proc.STDOUT)
			proc2.exec(cmnd2, stdin=proc1.proc.stdout,
					  stdout=Proc.PIPE, stderr=Proc.STDOUT)
			status2 = proc2.wait()
			status1 = proc1.wait()
			status = status1 + status2
			out, err = proc2.output()
			#
			revision = "can't get current revision"
			if status == 0:
				pattern = '(^[0-9a-f]+\s)'
				m = re.match(pattern, out)
				if m:
					revision = m.group(1)
			return status, revision, err

# ----------------------------------------------------------------------
#  Test main
# ----------------------------------------------------------------------
if __name__ == '__main__':

	verbose = 0
	topdir_svn = '../../..'
	topdir_git = '../../../../Git-Springhead/Springhead'

	def test(sys, args, topdir, verbose):
		vcs = VersionControlSystem(sys, args, verbose)
		code, rev, err = vcs.revision(topdir)
		if code == 0:
			print('%s: revision: %s' % (sys, rev))
		else:
			print('%s: Error: %s (%s)' % (sys, rev, err))

	system = 'Subversion'
	args = {'url': 'http://springhead.info/spr2/Springhead/trunk/'}
	test(system, args, topdir_svn, verbose)

	system = 'GitHub'
	args = {'url': 'http://github.com/sprphys/Springhead/'}
	test(system, args, topdir_git, verbose)

	sys.exit(0)

# end: VersionControlSystem.py

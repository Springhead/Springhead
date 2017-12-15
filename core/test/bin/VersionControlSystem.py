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
#	    returns:
#		code:	    0: success, other: failure (int).
#		rev:	    Revision (str).
#		err:	    Error message got from stderr (str).
#
#	short_id, full_id, date = revision_info(commit_id='HEAD')
#	    Get commit information.
#	    arguments:
#		commit_id:  Commit short name to retrieve (str).
#			    If None, get HEAD information.
#	    returns:
#		short_id:   Commit id in short form (str).
#		full_id:    Commit id in full form (str).
#		date:	    Date of commit in 'YYYY-MM-DD' from (str).
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2017/01/11 F.Kanehori	First version (Subversion only).
#	Ver 1.1  2017/09/13 F.Kanehori	Python library revised.
#	Ver 1.2  2017/11/16 F.Kanehori	Python library path ‚Ì•ÏX.
#	Ver 1.3  2017/12/07 F.Kanehori	GitHub ”ÅŽÀ‘•.
# ======================================================================
import sys
import os
import re
import datetime

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

	#  Get revision information.
	#
	def revision_info(self, dir=None, revision='HEAD'):
		if dir:
			cwd = os.getcwd()
			os.chdir(dir)
		if system == 'Subversion':
			r = -1
			s = 'NOT IMPLEMENTED'
			f = None
			d = None
		elif system == 'GitHub':
			r, s, f, d = self.obj.revision_info(revision)
		if dir:
			os.chdir(cwd)
		return r, s, f, d

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
			cmnd = 'git log --abbrev-commit --oneline --max-count=1'
			status, out, err = self.__exec(url, cmnd)
			#
			revision = "can't get current revision"
			if status == 0:
				pattern = '(^[0-9a-f]+\s)'
				m = re.match(pattern, out)
				if m:
					revision = m.group(1)
			return status, revision, err

		def revision_info(self, commit_id='HEAD'):
			print('REV-INFO (%s)' % commit_id)
			url = self.url

			# Get commit-id of HEAD.
			if commit_id == 'HEAD':
				cmnd = 'git show'
				status, out, err = self.__exec(url, cmnd)
				if status != 0:
					msg = "can't get HEAD info"
					return status, msg, None, None
				line = out.split('\\')[0]
				pattern = 'commit\s(\w+)'
				m = re.match(pattern, line)
				if m:
					commit_id = m.group(1)
					if self.verbose == 0:
						print('HEAD -> %s' % commit_id)

			cmnd = 'git log'
			########cmnd = 'git show %s' % commit_id
			status, out, err = self.__exec(url, cmnd)
			if status != 0:
				msg = "can't get revision info"
				return status, msg, None, None
			#
			"""
			findstr = commit_id if commit_id != 'HEAD' else '.+'
			lines = out.split('\\')[0]
			for line in lines.split('\n'):
				print('LINE: [%s]' % line)	#############
				pattern = 'commit\s+(%s)' % findstr
				print('PATTERN: [%s]' % pattern)	#############
				m = re.match(pattern, line)
				if m:
					print('MATCH(1)')	#############
					full_id = m.group(1)
					short_id = full_id[:7]
					continue
				pattern = 'Date:\s+(.+)'
				m = re.match(pattern, line)
				if m:
					print('MATCH(2)')	#############
					ifmt = '%a %b %d %H:%M:%S %Y %z'
					ofmt = '%Y-%m-%d'
					mstr = m.group(1)
					dt = datetime.strptime(mstr, ifmt)
					date = dt.strftime(ofmt)
					break
			return status, short_id, full_id, date
			"""
			return 0, "x", "y", "z"

		def __exec(self, url, cmnd):
			cmnd1 = cmnd
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
			return status, out, err

# ----------------------------------------------------------------------
#  Test main
# ----------------------------------------------------------------------
if __name__ == '__main__':

	verbose = 0
	topdir_svn = '../../..'
	topdir_git = '../../../../Springhead'

	def test(sys, args, topdir, verbose):
		vcs = VersionControlSystem(sys, args, verbose)
		code, rev, err = vcs.revision(topdir)
		if code == 0:
			print('%s: revision: %s' % (sys, rev))
		else:
			print('%s: Error: %s (%s)' % (sys, rev, err))
		if sys == 'Subversion':
			return
		#
		r, s, f, d = vcs.revision_info(topdir)
		print('default(%d): %s, %s, %s' % (r, s, f, d))
		r, s, f, d = vcs.revision_info(topdir, '5a314')
		print('+5a314+(%d): %s, %s, %s' % (r, s, f, d))

	system = 'Subversion'
	args = {'url': 'http://springhead.info/spr2/Springhead/trunk/'}
	test(system, args, topdir_svn, verbose)

	system = 'GitHub'
	args = {'url': 'http://github.com/sprphys/Springhead/'}
	test(system, args, topdir_git, verbose)

	sys.exit(0)

# end: VersionControlSystem.py

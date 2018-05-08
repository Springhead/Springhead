#!/usr/local/bin/python
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
#	obj = VersionControlSystem(system, url, wrkdir='.', verbose=0)
#	arguments:
#	    system:	Version control system name.
#			'Subversion'	Apache Subversion system
#			'GitHub'	GitHub system
#			'haselab'	git.haselab.net system
#	    url:	Url of git server.
#	    wrkdir:	Directory to perform action (Git directory).
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
#	info = revision_info(revision='HEAD')
#	    Get commit information.
#	    arguments:
#		revision:   Subversion: revision number to retrieve (str).
#			    Git: Commit short name to retrieve (str).
#			    If 'all', get all commit info.
#	    returns:
#		'all':	    List of triplets.
#		other:	    Triplet [short-id, long-id, date]
#				short_id:   Short form commit id (str).
#				full_id:    Long form commit id (str).
#				date:	    Commit date ('YYYY-MM-DD').
#			    [] if can not get information.
#
#	content = get_file_content(path, revision)
#	    Get file contents of specified revision.
#	    arguments:
#		path:	    File path relative to repository top dir (str).
#		revision:   Subversion: revision number (str).
#			    Git: Commit id (short form will do) (str).
#	    returns:	    File contents (str).
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2017/01/11 F.Kanehori	First version (Subversion only).
#	Ver 1.1  2017/09/13 F.Kanehori	Python library revised.
#	Ver 1.2  2017/11/16 F.Kanehori	Python library path の変更.
#	Ver 1.3  2017/12/20 F.Kanehori	GitHub 版実装.
#	Ver 1.4  2018/01/18 F.Kanehori	Add get_file_content().
#	Ver 1.41 2018/02/19 F.Kanehori	Bug fixed.
#	Ver 1.42 2018/03/14 F.Kanehori	Dealt with new Proc class.
#	Ver 1.43 2018/03/19 F.Kanehori	Dealt with Proc.output() change.
#	Ver 1.5  2018/05/08 F.Kanehori	Code reviewd.
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
	def __init__(self, system, url, wrkdir='.', verbose=0):
		self.clsname = self.__class__.__name__
		self.version = 1.5
		#
		self.system = system
		self.url = Util.upath(url)
		self.wrkdir = Util.upath(wrkdir)
		self.verbose = verbose
		#
		if system == 'Subversion':
			self.obj = self.Subversion(url, verbose)
		elif system == 'GitHub' or system == 'haselab':
			self.obj = self.GitHub(url, verbose)
		#
		if verbose:
			print('VersionControlSystem: %s' % system)
			print('  connecting to "%s"' % self.url)
			print('  working dir:  "%s"' % self.wrkdir)

	#  Get revision number.
	#
	def revision(self):
		self.__pushd()
		revision = self.obj.revision()
		self.__popd()
		return revision

	#  Get revision information.
	#
	def revision_info(self, revision='HEAD'):
		self.__pushd()
		revisions = self.obj.revision_info(revision)
		self.__popd()
		return revisions

	#  Get file contents of specified revision.
	#
	def get_file_content(self, path, revision):
		self.__pushd()
		content = self.obj.get_file_content(path, revision)
		self.__popd()
		return content

	# --------------------------------------------------------------
	#  Local helper methods.
	#
	def __pushd(self):
		cwd = Util.upath(os.getcwd())
		if self.wrkdir in [None, cwd]:
			self.dirsave = None
		else:
			self.dirsave = cwd
			os.chdir(self.wrkdir)

	def __popd(self):
		if self.dirsave:
			os.chdir(self.dirsave)
			self.dirsave = None

	# ==============================================================
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
			proc.execute(cmnd, stdout=Proc.PIPE, stderr=Proc.STDOUT)
			status, out, err = proc.output()
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

		def revision_info(self, revision='HEAD'):
			# sorry - not implemented yet
			return []

		def get_file_content(self, path, revision):
			# sorry - not implemented yet
			return None

	# ==============================================================
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
			rev = None
			err = "can't get current revision"
			if status == 0:
				pattern = '(^[0-9a-f]+\s)'
				m = re.match(pattern, out)
				if m:
					rev = m.group(1)
					err = None
			return status, rev, err

		def revision_info(self, commit_id='HEAD'):
			url = self.url
			cmnd = 'git log'
			status, out, err = self.__exec(url, cmnd)
			if status != 0:
				return []
			#
			infos = []
			for lines in out.split('\\'):
				for line in lines.split('\n'):
					pattern = 'commit\s+([0-9a-f]+)'
					m = re.match(pattern, line)
					if m:
						long_id = m.group(1)
						short_id = long_id[:7]
						continue
					pattern = 'Date:\s+(.+)'
					m = re.match(pattern, line)
					if not m:
						continue
					ifmt = '%a %b %d %H:%M:%S %Y %z'
					ofmt = '%Y-%m%d,%H:%M:%S'
					mstr = m.group(1)
					dt = datetime.datetime.strptime(mstr, ifmt)
					date = dt.strftime(ofmt)
					info = [short_id, long_id, date]
					if commit_id in [short_id, 'HEAD']:
						return info
					infos.append(info)
			return infos

		def get_file_content(self, path, commit_id):
			url = self.url
			contents = []
			cmnd = 'git show %s:%s' % (commit_id, path)
			status, out, err = self.__exec(url, cmnd)
			if status != 0:
				return None
			return out

		def __exec(self, url, cmnd):
			cmnd1 = cmnd
			cmnd2 = 'nkf -s'
			proc1 = Proc(verbose=self.verbose)	# git
			proc2 = Proc(verbose=self.verbose)	# nkf
			proc1.execute(cmnd1, stdout=Proc.PIPE, stderr=Proc.STDOUT)
			proc2.execute(cmnd2, stdin=proc1.proc.stdout,
					     stdout=Proc.PIPE, stderr=Proc.STDOUT)
			status, out, err = proc2.output()
			return status, out, err

# ----------------------------------------------------------------------
#  Test main
#
#  ** CAUTION **
#	This test program is used by DailyBuild task.
#	    "TestAllGit.bat" and "MakeReport{Git|SVN}.bat"
#	So do not DELETE nor MODIFY this program.
# ----------------------------------------------------------------------
from optparse import OptionParser
from Error import *
if __name__ == '__main__':
	prog = sys.argv[0].split(os.sep)[-1].split('.')[0]

	repository_def = {
	    'Subversion': {
		'url': 'http://springhead.info/spr2/Springhead/trunk/',
		'dir': '../../..'
	    },
	    'GitHub': {
		'url': 'http://github.com/sprphys/Springhead/',
		'dir': '../../../../Springhead'
	    },
	    'haselab': {
		'url': 'http://git.haselab.net/DailyBuild/Result/',
		'dir': '../../../../DailyBuildResult/Result'
	    }
	}

	def print_usage():
		print()
		cmnd = 'python %s.py --help' % prog
		shell = False if Util.is_unix() else True
		Proc().execute(cmnd, shell=shell).wait()
		sys.exit(1)
	
	# --------------------------------------------------------------
	usage = 'Usage: %prog [options] {HEAD | all | commit-id}'
	parser = OptionParser(usage = usage)
	#
	parser.add_option('-S', '--subversion', dest='subversion',
				action='store_true', default=False,
				help='use Subversion')
	parser.add_option('-G', '--github', dest='github',
				action='store_true', default=False,
				help='use GitHub')
	parser.add_option('-H', '--haselab', dest='haselab',
				action='store_true', default=False,
				help='use git.haselab.net')
	parser.add_option('-f', '--fname', dest='fname',
				action='store', default=None,
				help='get file content')
	parser.add_option('-v', '--verbose', dest='verbose',
				action='count', default=0,
				help='set verbose mode')
	parser.add_option('-V', '--version', dest='version',
				action='store_true', default=False,
				help='show version')
	#
	(options, args) = parser.parse_args()
	if options.version:
		version = VersionControlSystem('', '').version
		print('%s: Version %s' % (prog, version))
		sys.exit(0)
	if len(args) != 1:
		Error(prog).error("incorrect number of arguments")
		print_usage()
	repo_sub = options.subversion
	repo_git = options.github
	repo_hlb = options.haselab
	verbose = options.verbose
	revision = args[0]

	repo_count = 0
	if repo_sub: repo_count += 1
	if repo_git: repo_count += 1
	if repo_hlb: repo_count += 1
	if repo_count > 1:
		print('invalid combination of arguments (-s, -g and -h)')
		print(usage)
		sys.exit(-1)
	if repo_count == 0:
		print('one of -s, -g or -h required')
		print(usage)
		sys.exit(-1)

	if repo_sub: system = 'Subversion'
	if repo_git: system = 'GitHub'
	if repo_hlb: system = 'haselab'

	# --------------------------------------------------------------
	def test(system, url, wrkdir, verbose):
		vcs = VersionControlSystem(system, url, wrkdir, verbose)
		code, rev, err = vcs.revision()
		if code == 0:
			print('%s: revision: %s' % (system, rev))
		else:
			print('%s: Error: %s (%s)' % (system, rev, err))
		if system == 'Subversion':
			return
		#
		revisions = vcs.revision_info()
		print('default: %s' % revisions)
		revisions = vcs.revision_info('7813b94')
		print('7813b94: %s' % revisions)
		revisions = vcs.revision_info('all')
		print('all:')
		for rev in revisions:
			print('         %s' % rev)

	def info(system, args, wrkdir, commit_id, out=True):
		vcs = VersionControlSystem(system, url, wrkdir, verbose)
		revs = vcs.revision_info(commit_id)
		if out:
			if commit_id != 'all':
				revs = [revs]
			for rev in revs:
				print('%s,%s,%s' % (rev[0], rev[1], rev[2]))
		return revs

	def contents(wrkdir, fname, rev):
		vcs = VersionControlSystem(system, url, wrkdir, verbose)
		contents = vcs.get_file_content(fname, rev[0])
		print('--[%s,%s,%s]--' % (rev[0], rev[1], rev[2]))
		if contents is None:
			return
		if isinstance(contents, str):
			print(contents.replace('\r', ''))

	# --------------------------------------------------------------
	repository = repository_def[system]
	url = repository['url']
	wrkdir = repository['dir']

	if system == 'Subversion':
		test(system, url, srkdir, verbose)

	else:
		if options.fname:
			revs = info(system, url, wrkdir, revision, out=False)
			if not isinstance(revs[0], list):
				revs = [revs]
			for rev in revs:
				contents(wrkdir, options.fname, rev)
		else:
			revisions = info(system, url, wrkdir, revision)

	sys.exit(0)

# end: VersionControlSystem.py

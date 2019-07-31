#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:
#	VersionControlSystem:
#	    Wrapper class to get some information like revision
#	    number from source code version control system.
#	    Currently available systems are:
#		'GitHub'	GitHub system
#
#  INITIALIZER:
#	obj = VersionControlSystem(url, wrkdir='.', verbose=0)
#	arguments:
#	    url:	Url of git server.
#	    wrkdir:	Directory to perform action (Git directory).
#	    verbose:	Verbose level (int) (0: silent).
#	    
#  METHODS:
#	code, rev, err = revision(platform='Windows')
#	    Get revision number string.
#	    arguments:
#		platform:   Platform name ('unix' or 'Windows') (str).
#	    returns:
#		code:	    0: success, other: failure (int).
#		rev:	    Revision (str).
#		err:	    Error message got from stderr (str).
#
#	info = revision_info(commit_id='HEAD', platform='Windows')
#	    Get commit information.
#	    arguments:
#		commit_id:  Commit-id to retrieve (short form) (str).
#			    If 'all', get all commit info.
#		platform:   Platform name ('unix' or 'Windows') (str).
#	    returns:
#		'all':	    List of triplets.
#		other:	    Triplet [short-id, long-id, date]
#				short_id:   Short form commit id (str).
#				full_id:    Long form commit id (str).
#				date:	    Commit date ('YYYY-MM-DD').
#			    [] if can not get information.
#
#	content = get_file_content(path, commit_id, platform='Windows')
#	    Get file contents of specified revision.
#	    arguments:
#		path:	    File path relative to repository top dir (str).
#		commit_id:  Commit-id to retrieve (short or full) (str).
#		platform:   Platform name ('unix' or 'Windows') (str).
#	    returns:	    File contents (str).
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2017/01/11 F.Kanehori	First version (Subversion only).
#	Ver 1.1  2017/09/13 F.Kanehori	Python library revised.
#	Ver 1.2  2017/11/16 F.Kanehori	Python library path の変更.
#	Ver 1.3  2017/12/20 F.Kanehori	GitHub 版実装.
#	Ver 1.4  2018/01/18 F.Kanehori	Add get_file_content().
#	Ver 1.5  2018/05/08 F.Kanehori	Code reviewd.
#	Ver 1.6  2018/05/14 F.Kanehori	Use sprphys's commit id.
#	Ver 2.0  2018/09/13 F.Kanehori	Subversion system obsoleted.
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
	def __init__(self, url, wrkdir='.', verbose=0):
		self.clsname = self.__class__.__name__
		self.version = 2.0
		#
		self.url = Util.upath(url)
		self.wrkdir = Util.upath(wrkdir)
		self.verbose = verbose
		#
		self.ResultURL = 'DailyBuild/Result'
		#
		if verbose:
			print('VersionControlSystem:')
			print('  connecting to "%s"' % self.url)
			print('  working dir:  "%s"' % self.wrkdir)

	#  Get revision number.
	#
	def revision(self, platform='Windows'):
		self.__pushd()
		#
		url = self.url
		cmnd = 'git log --abbrev-commit --oneline --max-count=1'
		'''
		if self.ResultURL in self.url:
			# どちらの"result.log"か？
			cmnd += ' --grep=unix'
			if platform != 'unix':
				cmnd += ' --invert-grep'
		'''
		status, out, err = self.__exec(cmnd)
		#
		rev = None
		err = "can't get current revision"
		if status == 0:
			pattern = '(^[0-9a-f]+\s)'
			m = re.match(pattern, out)
			if m:
				rev = m.group(1)
				err = None
		#
		self.__popd()
		return status, rev, err

	#  Get revision information.
	#
	def revision_info(self, commit_id='HEAD', platform='Windows'):
		self.__pushd()
		#
		url = self.url
		cmnd = 'git log'
		'''
		if self.ResultURL in self.url:
			# どちらの"result.log"か？
			cmnd += ' --grep=unix'
			if platform != 'unix':
				cmnd += ' --invert-grep'
		'''
		status, out, err = self.__exec(cmnd)
		if status != 0:
			self.__popd()
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
					self.__popd()
					return info
				if commit_id == 'all':
					infos.append(info)
		#
		self.__popd()
		return infos

	#  Get file contents of specified revision.
	#
	def get_file_content(self, path, commit_id, platform='Windows'):
		self.__pushd()
		#
		url = self.url
		cmnd = 'git show %s:%s' % (commit_id, path)
		status, out, err = self.__exec(cmnd)
		#
		self.__popd()
		if status != 0:
			return None
		return out

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
			if self.verbose:
				print('__pushd: %s' % self.wrkdir)

	def __popd(self):
		if self.dirsave:
			if self.verbose:
				print('__popd:  %s' % self.dirsave)
			os.chdir(self.dirsave)
			self.dirsave = None

	def __exec(self, cmnd):
		cmnd1 = cmnd
		cmnd2 = 'nkf -w' if Util.is_unix() else 'nkf -s'
		shell = True if Util.is_unix() else False
		proc1 = Proc(verbose=self.verbose)	# git
		proc2 = Proc(verbose=self.verbose)	# nkf
		proc1.execute(cmnd1, stdout=Proc.PIPE, stderr=Proc.STDOUT,
				     shell=shell)
		proc2.execute(cmnd2, stdin=proc1.proc.stdout,
				     stdout=Proc.PIPE, stderr=Proc.STDOUT,
				     shell=shell)
		status, out, err = proc2.output()
		return status, out, err

# ----------------------------------------------------------------------
#  Test main
# ----------------------------------------------------------------------
from optparse import OptionParser
from Error import *
if __name__ == '__main__':
	prog = sys.argv[0].split(os.sep)[-1].split('.')[0]

	repository_def = {
	    'Springhead': {
		'url': 'http://github.com/sprphys/Springhead/',
		'dir': '../../../../Springhead'
	    },
	    'Result': {
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
	parser.add_option('-S', '--springhead', dest='springhead',
				action='store_true', default=False,
				help='use Springhead repository')
	parser.add_option('-R', '--result', dest='result',
				action='store_true', default=False,
				help='use DailyBuild/Result repository')
	parser.add_option('-f', '--fname', dest='fname',
				action='store', default=None,
				help='get file content')
	parser.add_option('-p', '--platform', dest='platform',
				action='store', default='Windwos',
				help='platform name')
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
	fname = options.fname
	platform = options.platform
	if fname and platform == 'unix':
		fname = 'unix/' + fname
	verbose = options.verbose
	commit_id = args[0]

	# --------------------------------------------------------------
	if options.springhead:
		repository = repository_def['Springhead']
	elif options.result:
		repository = repository_def['Result']
	url = repository['url']
	wrkdir = repository['dir']
	if platform == 'unix':
		url += 'unix/'
		wrkdir += '/unix'

	print('using:')
	print('    url:      %s' % url)
	print('    wrkdir:   %s' % wrkdir)
	if fname:
		print('    fname:    %s' % fname)
	print('    platform: %s' % platform)
	print()

	vcs = VersionControlSystem(url, wrkdir, verbose)

	print('[revision] - HEAD')
	status, rev, err = vcs.revision(platform)
	print('    status %s, rev %s, err %s' % (status, rev, err))
	print()

	print('[revision_info]')
	revs = vcs.revision_info(commit_id, platform)
	if revs != []:
		if commit_id != 'all':
			revs = [revs]
		for rev in revs:
			print('    %s,%s,%s' % (rev[0], rev[1], rev[2]))

	if fname:
		print()
		print('[get_file_content]')
		content = vcs.get_file_content(fname, commit_id, platform)
		print('--------')
		print(content)
		print('--------')

	sys.exit(0)

# end: VersionControlSystem.py

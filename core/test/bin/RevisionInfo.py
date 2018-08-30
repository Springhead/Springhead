#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	RevisionInfo [options] revision
#	arguments:
#	    revision:	One of follows.
#			HEAD:	Extract from remote HEAD.
#			all:	Extract from all commits.
#			commit-id:
#				Extract from specified commit.
#	options:
#	    -G:		Use Github (GitHub.com) repository.
#	    -H:		Use Gitlab (git.haselab.net) repository.
#	    -f fname:	File name to extract from repository.
#
#  DESCRIPTION:
#	Extract some information from git server.
#	With option '-f':
#		Extract named file from specified commit.
#		Argument with this option must be commit-id.
#		NOTE: This works only for git.haselab.net (-H).
#	Without option '-f':
#		Extract commit id and date/time from git server.
#		Use with any argument.
#
#  NOTE:
#	This script applies only to GitHub and NOT to subversion.
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2018/08/20 F.Kanehori	Separated from class test file
#					"VersionControlSystem.py".
# ======================================================================
version = 1.0

import sys
from optparse import OptionParser

from VersionControlSystem import *

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]

# ----------------------------------------------------------------------
#  Import Springhead python library.
#
sys.path.append('../../src/RunSwig')
from FindSprPath import *
spr_path = FindSprPath('VersionControlSystem')
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from Util import *
from Proc import *
from Error import *

# ----------------------------------------------------------------------
#  Helper methods
#

#  Extract revision information (commit-id and date/time).
def info(system, url, wrkdir, commit_id, out=True):
	vcs = VersionControlSystem(system, url, wrkdir, verbose)
	revs = vcs.revision_info(commit_id)
	if revs == []:
		return []
	if out:
		if commit_id != 'all':
			revs = [revs]
		for rev in revs:
			print('%s,%s,%s' % (rev[0], rev[1], rev[2]))
	return revs

#  Extract file contents of specified revision.
def contents(url, wrkdir, fname, rev):
	vcs = VersionControlSystem(system, url, wrkdir, verbose)
	contents = vcs.get_file_content(fname, rev[0])
	spr_id_fname = 'Springhead.commit.id'
	spr_id_info = vcs.get_file_content(spr_id_fname, rev[0])
	if spr_id_info[0:9] == 'Traceback' or \
	   spr_id_info[0:6] == 'python' or \
	   spr_id_info[0:5] == 'fatal':
		# Kludge -- caused by bug!
		return
	if contents is None:
		return
	if isinstance(contents, str):
		# commit-id:
		#	The current commit id of sprphys/Springhead.
		spr_id = spr_id_info.replace('\r\n', '').split(',')
		spr_id = ','.join(spr_id[0:2])
		# date and time:
		#	Date and time of dailybuild test.
		dt = (','.join(rev[1:]))
		print('--[%s,%s]--' % (spr_id, rev[2]))
		print(contents.replace('\r', ''))

#  Show usage.
def print_usage():
	print()
	cmnd = 'python %s.py --help' % prog
	shell = False if Util.is_unix() else True
	Proc().execute(cmnd, shell=shell).wait()
	sys.exit(1)

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options] {HEAD | all | commit-id}'
parser = OptionParser(usage = usage)
#
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
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
if options.github and options.haselab:
	Error(prog).error('both -G and -H specified')
	print_usage()
if not options.github and not options.haselab:
	Error(prog).error('remote repository is not specified')
	print_usage()
#
if options.github:
	system = 'GitHub'
	url = 'http://github.com/sprphys/Springhead/'
	wrkdir = '../../../../Springhead'
else:
	system = 'haselab'
	url = 'http://git.haselab.net/DailyBuild/Result/'
	wrkdir = '../../../../DailyBuildResult/Result'
verbose = options.verbose
revision = args[0]

# ----------------------------------------------------------------------
#  Main process.
#
if options.fname:
	revs = info(system, url, wrkdir, revision, out=False)
	if revs != []:
		if not isinstance(revs[0], list):
			revs = [revs]
		for rev in revs:
			contents(url, wrkdir, options.fname, rev)
else:
	revisions = info(system, url, wrkdir, revision)

sys.exit(0)

# end: RevisionInfo

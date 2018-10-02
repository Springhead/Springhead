#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	python buildhtml.py options
#	options:
#	    -d dname	Work directory name.
#
#  DESCRIPTION:
#	Generate Springhead manual html version using plastex.
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2018/09/25 F.Kanehori	First version.
# ======================================================================
version = 1.0

import sys
import os
import glob
import subprocess
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
web_host = 'haselab.net'
web_user = 'demo'

# ----------------------------------------------------------------------
#  External tools.
#
sed = 'sed'
nkf = 'nkf'
python = 'c:/python35/python'
plastex = 'c:/Python35/Scripts/plastex'

# ----------------------------------------------------------------------
#  Helper methods
#
#  Platform now running?
def is_unix():
	return True if os.name == 'posix' else False

#  Execute command.
def execute(cmnd, stdin=None, stdout=None, stderr=None, shell=None):
	fd = [ pipe_open(stdin, 'r', dry_run),
	       pipe_open(stdout, 'w', dry_run),
	       pipe_open(stderr, 'w', dry_run) ]
	if dry_run:
		print('EXEC: %s' % cmnd)
		return 0
	if shell is None:
		shell = True if is_unix() else False
	if verbose:
		print('exec: %s' % cmnd)
	proc = subprocess.Popen(cmnd,
				stdin=fd[0], stdout=fd[1], stderr=fd[2],
				shell=shell)
	return proc

def pipe_open(file, mode, dry_run):
	if dry_run:
		return None
	if not isinstance(file, str):
		return file
	try:
		f = open(file, mode)
	except IOError as err:
		f = None
	return f

#  Wait for process termination.
def wait(proc):
	rc = proc.wait()
	if not is_unix():
		rc = -(rc & 0b1000000000000000) | (rc & 0b0111111111111111)
	return rc

#  Remove tree.
def remove_tree(top, dry_run=False, verbose=0):
	if is_unix():
		cmnd = '/bin/rm -rf %s' % top
	else:
		cmnd = 'rd /S /Q %s' % top
	if dry_run or verbose:
		print('remove_tree: %s' % top)
		if dry_run:
			return 0
	rc = wait(execute(cmnd, shell=True))
	return rc

#  Copy file(s).
def cp(src, dst, dry_run=False, verbose=0):
	if os.path.isdir(src) and os.path.isfile(dst):
		msg = 'copying directory to plain file (%s to %s)' % (src, dst)
		abort(msg)
	if os.path.isfile(src) and os.path.isdir(dst):
		msg = 'copying plain file to directory (%s to %s)' % (src, dst)
		abort(msg)
	if dry_run:
		print('cp: %s %s' % (src, dst))
		if dry_run:
			return 0
	if os.path.isdir(src):
		if verbose:
			print('cp: %s -> %s/%s' % (f, dst, f))
		if is_unix():
			## NEED IMPLEMENT
			pass
		else:
			cmnd = 'xcopy /I /E /S /Y /Q %s %s' % (src, dst)
			rc = wait(execute(cmnd, shell=True))
	else:
		if verbose:
			print('cp: %s -> %s' % (f, dst))
		cmnd = '%s %s %s' % (cmndname('cp'), src, dst)
		rc = wait(execute(cmnd, shell=True))
	return rc

def copy_all(src, dst, dry_run=False, verbose=0):
	if verbose:
		print('  clearing "%s"' % dst)
	cmnd = '%s %s/*' % (cmndname('rm'), dst)
	wait(execute(cmnd))
	#
	os.chdir(src)
	print('  copying "%s" to "%s"' % (src, dst))
	names = os.listdir()
	for name in names:
		if os.path.isfile(name):
			cmnd = '%s %s %s' % (cmndname('cp'), src, dst)
			rc = wait(execute(cmnd))
			if rc != 0:
				break
		elif os.path.isdir(name):
			dst_dir = '%s/%s' % (dst, name)
			cmnd = '%s %s %s' % (cmndname('cp'), src, dst_dir)
			rc = wait(execute(cmnd))
			if rc != 0:
				break
	return rc

#
def pathconv(path):
	return path.replace('/', os.sep)

#  Command name.
def cmndname(cmnd):
	nametab = { 'cat':	['cat', 'type'],
		    'cp':	['cp', 'copy'],
		    'rm':	['rm', 'del'],
		}
	indx = 0 if is_unix() else 1
	return nametab[cmnd][indx]

#  Error process.
def error(msg):
	sys.stderr.write('%s\n' % msg)
def abort(msg, exitcode=1):
	error(msg)
	sys.exit(exitcode)

#  Show usage.
def print_usage():
	print()
	cmnd = 'python %s.py --help' % prog
	wait(execute(cmnd))
	sys.exit(1)

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options] texmain'
parser = OptionParser(usage = usage)
#
parser.add_option('-c', '--copy', dest='copy',
			action='store_true', default=False,
			help='copy generated html files to web')
parser.add_option('-w', '--workspace', dest='wrkspace',
			action='store', default='tmp', metavar='dir',
			help='work space name [defailt: %default]')
parser.add_option('-v', '--verbose', dest='verbose',
			action='count', default=0,
			help='set verbose mode')
parser.add_option('-D', '--dry-run', dest='dry_run',
			action='store_true', default=False,
			help='set dry-run mode')
parser.add_option('-V', '--version', dest='version',
			action='store_true', default=False,
			help='show version')
#
(options, args) = parser.parse_args()
if options.version:
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
if len(args) != 1:
	error('incorrect number of arguments')
	print_usage()
#
wrkspace = options.wrkspace
verbose = options.verbose
dry_run = options.dry_run
#
texmain = args[0]
if texmain[-4:] != '.tex':
	texmain += '.tex'

# ----------------------------------------------------------------------
#  Main process.
#
# ----------------------------------------------------------------------
#  Prepare work space.
#
if os.path.exists(wrkspace) and not os.path.isdir(wrkspace):
	msg = '%s exists but not a directory' % wrkspace
	abort(msg)
if os.path.exists(wrkspace):
	rc = remove_tree(wrkspace, dry_run, verbose)
	if rc != 0:
		msg = 'clearing workspace failed'
		abort(msg)
if verbose:
	print('making %s' % wrkspace)
os.mkdir(wrkspace)

#  Copy files to work space.
#
texsrcs = glob.glob('*.tex')
texsrcs.extend(glob.glob('*.sty'))
for f in texsrcs:
	cmnd1 = '%s %s' % (cmndname('cat'), f)
	if is_unix():
		cmnd2 = "%s -e 's/\{sourcecode\}/\{verbatim\}/'" % sed
	else:
		cmnd2 = '%s -e "s/{sourcecode}/{verbatim}/"' % sed
	cmnd3 = '%s -w' % nkf
	outf = '%s/%s' % (wrkspace, f)
	if verbose:
		print('converting %s' % f)
	#print('cmnd1: %s' % cmnd1)
	#print('cmnd2: %s' % cmnd2 )
	#print('cmnd3: %s' % cmnd3 )
	proc1 = execute(cmnd1, stdout=subprocess.PIPE, shell=True)
	proc2 = execute(cmnd2, stdin=proc1.stdout,
				stdout=subprocess.PIPE)
	proc3 = execute(cmnd3, stdin=proc2.stdout,
				stdout=outf)
	proc1.wait()
	proc2.wait()
	rc = proc3.wait()
	if rc != 0:
		msg = 'file conversion failed: "%s"' % f
		abort(msg)
#
others = glob.glob('*.cls')
others.extend(['en', 'fig'])
for f in others:
	dst = pathconv('%s/%s' % (wrkspace, f))
	rc = cp(f, dst, dry_run=dry_run, verbose=verbose)
	if rc != 0:
		msg = 'file copy failed: "%s"' % f
		abort(msg)
#
htmls = texmain.replace('.tex', '')
imgsrc = 'images'
imgdst = pathconv('%s/%s/images' % (wrkspace, htmls))
rc = cp(imgsrc, imgdst, dry_run=dry_run, verbose=verbose)
if rc != 0:
	msg = 'file copy failed: "%s"' % imgsrc
	abort(msg)

#  Generating htmls.
#
cwd = os.getcwd()
os.chdir(wrkspace)
opts = '--renderer=HTML5 --dir=main_html'
cmnd = '%s %s %s' % (python, plastex, texmain)
if verbose:
	print('cwd: %s' % os.getcwd())
	print('cmnd: %s' % cmnd)
rc = wait(execute(cmnd))
os.chdir(cwd)
if rc != 0:
	msg = 'generating html failed'
	abort(msg)
#
if options.copy:
	#  Copy generated files to web server.
	#
	fmdir = '.'
	todir = 'WWW/docroots/springhead/doc/SprManual'
	if Util.is_unix():
		remote = '%s@%s:/home/%s' % (web_user, web_host, todir)
		pkey = '%s/.ssh/id_rsa' % os.environ['HOME']
		opts = '-i %s -o "StrictHostKeyChecking=no"' % pkey
		cmnd = 'scp %s %s/* %s' % (opts, fmdir, remote)
		print('## %s' % cmnd)
		rc = wait(execute(cmnd))
		if rc == 0:
			print('cp %s -> %s:%s' % (fmdir, tohost, todir))
		else:
			print('cp %s failed' % fmdir)
	else:
		remote = '//%s/HomeDirs/%s' % (web_host, todir)
		copy_all(fmdir, remote, dry_run=True)
#
sys.exit(0)

# end: buildhtml.py

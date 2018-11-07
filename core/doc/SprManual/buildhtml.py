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
#	Ver 1.0  2018/10/30 F.Kanehori	First version.
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
PIPE = subprocess.PIPE
NULL = subprocess.DEVNULL

# ----------------------------------------------------------------------
#  External tools.
#
sed = 'sed'
nkf = 'nkf'
python = 'c:/python35/python'
pdflatex = 'pdflatex'
lwarpmk = 'lwarpmk'

# ----------------------------------------------------------------------
#  Helper methods
# ----------------------------------------------------------------------
#
#  Platform now running?
def is_unix():
	return True if os.name == 'posix' else False

#  Executes "cat <file> | sed <patterns>/<patterns> | nkf -w"
#
def exchange_quote(s):
	return s.replace("'", '|').replace('"', "'").replace('|', '"')

#  File encoding and text pattern conversion (awk and sed).
#
def fileconv(ifname, patterns, ofname):
	if verbose:
		print('converting %s to %s' % (ifname, ofname))
	#
	inp_cmnd = '%s %s' % (cmndname('cat'), ifname.replace('/', os.sep))
	med_cmnd = []
	for patt in patterns:
		cmnd = "%s -e 's/%s/%s/g'" % (sed, patt[0], patt[1])
		if not is_unix():
			cmnd = exchange_quote(cmnd)
		med_cmnd.append(cmnd)
	out_cmnd = '%s -w -Lu' % nkf
	#
	if verbose > 1:
		print('EXEC: %s' % inp_cmnd)
	inp_proc = execute(inp_cmnd, stdout=PIPE, shell=True)
	out_pipe = inp_proc.stdout
	med_proc = []
	for cmnd in med_cmnd:
		if verbose > 1:
			print('EXEC: %s' % cmnd)
		proc = execute(cmnd, stdin=out_pipe, stdout=PIPE)
		out_pipe = proc.stdout
		med_proc.append(proc)
	if verbose > 1:
		print('EXEC: %s' % out_cmnd)
	outf = ofname.replace('/', os.sep)
	out_proc = execute(out_cmnd, stdin=out_pipe, stdout=outf)
	#
	inp_proc.wait()
	for proc in med_proc:
		proc.wait()
	rc = out_proc.wait()
	if rc != 0:
		msg = 'file conversion failed: "%s"' % ifname
		abort(msg)

#  Execute command (subprocess).
#
def execute(cmnd, stdin=None, stdout=sys.stdout, stderr=sys.stderr, shell=None):
	if stderr is NULL:
		stderr = subprocess.STDOUT
	fd = [ pipe_open(stdin, 'r', dry_run),
	       pipe_open(stdout, 'w', dry_run),
	       pipe_open(stderr, 'w', dry_run) ]
	if dry_run:
		print('EXEC: %s' % cmnd)
		return 0
	if shell is None:
		shell = True if is_unix() else False
	#if verbose > 1:
	#	print('exec: %s' % cmnd)
	proc = subprocess.Popen(cmnd,
				stdin=fd[0], stdout=fd[1], stderr=fd[2],
				shell=shell)
	return proc

#  Open pipe object.
#
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
#
def wait(proc):
	rc = proc.wait()
	if not is_unix():
		rc = -(rc & 0b1000000000000000) | (rc & 0b0111111111111111)
	return rc

#  Remove tree.
#
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
#
def cp(src, dst, dry_run=False, verbose=0):
	src_cnv = pathconv(src)
	dst_cnv = pathconv(dst)
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
			print('cp: %s -> %s (dir)' % (src, dst))
		if is_unix():
			## NEED IMPLEMENT
			pass
		else:
			cmnd = 'xcopy /I /E /S /Y /Q %s %s' % (src_cnv, dst_cnv)
			rc = wait(execute(cmnd, stdout=NULL, stderr=NULL, shell=True))
	else:
		if verbose:
			print('cp: %s -> %s' % (src, dst))
		cmnd = '%s %s %s' % (cmndname('cp'), src_cnv, dst_cnv)
		rc = wait(execute(cmnd, stdout=NULL, stderr=NULL, shell=True))
	return rc

#  Copy all files and directories.
#
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

#  Convert path separators to fit current OS.
#
def pathconv(path):
	return path.replace('/', os.sep)

#  Command names.
#
def cmndname(cmnd):
	nametab = { 'cat':	['cat', 'type'],
		    'cp':	['cp', 'copy'],
		    'rm':	['rm', 'del'],
		}
	indx = 0 if is_unix() else 1
	return nametab[cmnd][indx]

#  Error process.
#
def error(msg):
	sys.stderr.write('%s\n' % msg)
def abort(msg, exitcode=1):
	error(msg)
	sys.exit(exitcode)

#  Show usage.
#
def print_usage():
	print()
	cmnd = 'python %s.py --help' % prog
	print(cmnd)
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
parser.add_option('-t', '--test', dest='test',
			action='store_true', default=False,
			help='copy test script to workspace')
parser.add_option('-v', '--verbose', dest='verbose',
			action='count', default=0,
			help='set verbose mode')
parser.add_option('-C', '--convert-only', dest='convert_only',
			action='store_true', default=False,
			help='convert files only')
parser.add_option('-K', '--insert-kludge', dest='insert_kludge',
			action='store_true', default=False,
			help='insert kludge code')
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
#
wrkspace = options.wrkspace
verbose = options.verbose
dry_run = options.dry_run
test = options.test
#
if len(args) != 1:
	error('incorrect number of arguments')
	print_usage()
#
texmain = args[0].replace('\\', '/')
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
patterns = [ [ '{sourcecode}', '{sourcecode}' ],
	     [ '\([^\\]\)zw', '\\1\\\\zw' ],
	     [ '^%iflwarp(\\(.*\\))', '\\1' ] ]
#
texsrcs = glob.glob('*.tex')
texstys = glob.glob('*.sty')
for inpf in texstys:
	outf = '%s/%s' % (wrkspace, inpf)
	fileconv(inpf, patterns, outf)
#
for inpf in texsrcs:
	outf = '%s/%s' % (wrkspace, inpf)
	fileconv(inpf, patterns, outf)
#
others = glob.glob('*.cls')
others.append('fig')
if options.insert_kludge:
	others.append('insert_kludge.py')
if test:
	others.append('test.bat')
for f in others:
	dst = '%s/%s' % (wrkspace, f)
	rc = cp(f, dst, dry_run=dry_run, verbose=verbose)
	if rc != 0:
		msg = 'file copy failed: "%s"' % f
		abort(msg)

# Convert figure file format (eps to svg).
#
cwd = os.getcwd()
if verbose:
	print('converting image format')
os.chdir('%s/fig' % wrkspace)
for f in glob.glob('*.eps'):
	f_pdf = f.replace('.eps', '.pdf')
	f_svg = f.replace('.eps', '.svg')
	if verbose:
		print('  %s to svg' % f)
	cmnd = 'lwarpmk epstopdf %s' % f
	rc = wait(execute(cmnd, stdout=NULL))
	if rc != 0:
		print('%s -> %s: faild' % (f, f_pdf))
	cmnd = 'lwarpmk pdftosvg %s' % f_pdf
	rc = wait(execute(cmnd, stdout=NULL))
	if rc != 0:
		print('%s -> %s: faild' % (f_pdf, f_svg))
os.chdir(cwd)
#
if options.convert_only:
	sys.exit(0)

# ----------------------------------------------------------------------
#  Generating htmls.
#
#cwd = os.getcwd()
os.chdir(wrkspace)
if verbose:
	print('enter: %s' % os.getcwd().replace(os.sep, '/'))

# (1) Make pdf by pdflatex.
#
cmnd = '%s %s' % (pdflatex, texmain)
if verbose:
	print('#### %s' % cmnd)
rc = wait(execute(cmnd, stdout=sys.stdout, stderr=sys.stderr))
if rc != 0:
	msg = '%s: failed' % cmnd
	abort(msg)

# (2) Change macro to use lwarpmk.
#
patterns.append(['Lwarpfalse', 'Lwarptrue'])
ofname = 'sprmacros.sty'
ifname = '%s/%s' % ('..', ofname)
fileconv(ifname.replace('/', os.sep), patterns, ofname)

# (2.5) Kludge
#	lwarpmk html を実行するとANKから漢字に変化する箇所でエラーを起こす。
#	    pdfTeX error: pdflatex,exe (file cyberb30): Font cyberb30 at 420 not found
#	おまじないとして、ダミーのvruleを挿入しておく。
#
if options.insert_kludge:
	cmnd = 'python insert_kludge.py'
	if verbose:
		print('#### %s' % cmnd)
	rc = wait(execute(cmnd, stdout=sys.stdout, stderr=sys.stderr))
	if rc != 0:
		msg = '%s: failed' % cmnd
		abort(msg)

# (3) Make htmls.
#
cmnds = [ '%s html' % lwarpmk,
	  '%s again' % lwarpmk,
	  '%s html' % lwarpmk,
	  '%s print' % lwarpmk,
	  '%s htmlindex' % lwarpmk,
	  '%s html' % lwarpmk,
	  '%s html1' % lwarpmk,
	  '%s limages' % lwarpmk,
	  '%s html' % lwarpmk ]
for cmnd in cmnds:
	if verbose:
		print('#### %s' % cmnd)
	rc = wait(execute(cmnd, stdout=sys.stdout, stderr=sys.stderr))
	if rc != 0:
		msg = '%s: failed' % cmnd
		abort(msg)
os.chdir(cwd)
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

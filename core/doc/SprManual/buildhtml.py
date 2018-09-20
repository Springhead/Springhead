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
#	Ver 1.0  2018/09/20 F.Kanehori	First version.
# ======================================================================
version = 1.0

import sys
import os
import glob
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
web_host = 'haselab.net'
web_user = 'demo'

# ----------------------------------------------------------------------
#  Import Springhead python library.
#
sys.path.append('../../src/RunSwig')
from FindSprPath import *
spr_path = FindSprPath(prog)
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from FileOp import *
from Proc import *
from Util import *
from Error import *

# ----------------------------------------------------------------------
#  External tools.
#
sed = 'sed'
nkf = 'nkf'
python = 'python'
plastex = 'c:/Python35/Scripts/plastex'

# ----------------------------------------------------------------------
#  Helper methods
#
#  Copy all files.
def copy_all(src, dst, dry_run=False, verbose=0):
	if verbose:
		print('  clearing "%s"' % dst)
	fop = FileOp(info=1, dry_run=dry_run, verbose=verbose)
	fop.rm('%s/*' % dst, recurse=True)
	#
	os.chdir(src)
	print('  copying "%s" to "%s"' % (src, dst))
	names = os.listdir()
	for name in names:
		if os.path.isfile(name):
			fop.cp(name, dst)
		elif os.path.isdir(name):
			dst_dir = '%s/%s' % (dst, name)
			fop.cp(name, dst_dir)

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
	Error(prog).error('incorrect number of arguments')
	print_usage()
#
wrkspace = options.wrkspace
verbose = options.verbose
dry_run = options.dry_run
#
texmain = args[0]
if texmain[-4:] != '.tex':
	texmain += '.tex'
#
shell = True if Util.is_unix() else False

# ----------------------------------------------------------------------
#  Main process.
#
# ----------------------------------------------------------------------
#  Create work space.
#
if os.path.exists(wrkspace) and not os.path.isdir(wrkspace):
	msg = '%s exists but not a directory' % wrkspace
	Error(prog).abort(msg)
os.makedirs(wrkspace, exist_ok=True)

#  Clear work space.
#
fop = FileOp()
fop.rm('%s/*' % wrkspace)

#  Copy files to work space.
#
texsrcs = glob.glob('*.tex')
texsrcs.extend(glob.glob('*.sty'))
for f in texsrcs:
	if Util.is_unix():
		cmnd1 = 'cat %s' % f
		cmnd2 = "%s -e 's/\{sourcecode\}/\{verbatim\}/'" % sed
	else:
		cmnd1 = 'type %s' % f
		cmnd2 = '%s -e "s/{sourcecode}/{verbatim}/"' % sed
	cmnd3 = '%s -w' % nkf
	outf = '%s/%s' % (wrkspace, f)
	if verbose:
		print('converting %s' % f)
	proc1 = Proc()
	proc2 = Proc()
	proc3 = Proc()
	#print('cmnd1: %s' % cmnd1)
	#print('cmnd2: %s' % cmnd2)
	#print('cmnd3: %s' % cmnd3)
	proc1.execute(cmnd1, stdout=Proc.PIPE, shell=True)
	proc2.execute(cmnd2, stdin=proc1.proc.stdout,
					stdout=Proc.PIPE, shell=shell)
	proc3.execute(cmnd3, stdin=proc2.proc.stdout,
					stdout=outf, shell=shell)
	proc1.wait()
	proc2.wait()
	rc = proc3.wait()
	if rc != 0:
		msg = 'file conversion failed: "%s"' % f
		Error(prog).abort(msg)
#
others = glob.glob('*.cls')
others.extend(['en', 'fig'])
for f in others:
	rc = FileOp().cp(f, '%s/%s' % (wrkspace, f))
	if rc != 0:
		msg = 'file copy failed: "%s"' % f
		Error(prog).abort(msg)
#
htmls = texmain.replace('.tex', '')
imgsrc = 'images'
imgdest = '%s/%s/images' % (wrkspace, htmls)
rc = FileOp(verbose=verbose).cp(imgsrc, imgdest)
if rc != 0:
	msg = 'file copy failed: "%s"' % imgsrc
	Error(prog).abort(msg)

#  Generating htmls.
#
cwd = os.getcwd()
os.chdir(wrkspace)
cmnd = '%s %s %s' % (python, plastex, texmain)
rc = Proc(verbose=verbose).execute(cmnd, shell=shell).wait()
os.chdir(cwd)
if rc != 0:
	msg = 'generating html failed'
	Error(prog).abort(msg)
#
if not options.copy:
	sys.exit(0)

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
	rc = proc.execute(cmnd, shell=True).wait()
	if rc == 0:
		print('cp %s -> %s:%s' % (fmdir, tohost, todir))
	else:
		print('cp %s failed' % fmdir)
else:
	remote = '//%s/HomeDirs/%s' % (web_host, todir)
	copy_all(fmdir, remote, dry_run=True)

sys.exit(0)

# end: buildhtml.py

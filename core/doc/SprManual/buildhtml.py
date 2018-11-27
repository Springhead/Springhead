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
#	Ver 1.0  2018/11/27 F.Kanehori	First version.
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

# for test
svg_dir = '../../../../../../SprManual-fig/svg'.replace('/', os.sep)

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
	if dry_run:
		return
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
	if dry_run:
		return 0
	rc = proc.wait()
	if not is_unix():
		rc = -(rc & 0b1000000000000000) | (rc & 0b0111111111111111)
	return rc

#  Remove tree.
#
def remove_tree(top, verbose=0):
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
def cp(src, dst, verbose=0):
	src_cnv = pathconv(src)
	dst_cnv = pathconv(dst)
	if os.path.isdir(src) and os.path.isfile(dst):
		msg = 'copying directory to plain file (%s to %s)' % (src, dst)
		abort(msg)
	if os.path.isfile(src) and os.path.isdir(dst):
		msg = 'copying plain file to directory (%s to %s)' % (src, dst)
		abort(msg)
	if dry_run:
		Print('cp: %s %s' % (src, dst))
		if dry_run:
			return 0
	if os.path.isdir(src):
		if verbose:
			Print('cp: %s -> %s (dir)' % (src, dst))
		if is_unix():
			## NEED IMPLEMENT
			pass
		else:
			cmnd = 'xcopy /I /E /S /Y /Q %s %s' % (src_cnv, dst_cnv)
			rc = wait(execute(cmnd, stdout=NULL, stderr=NULL, shell=True))
	else:
		if verbose:
			Print('cp: %s -> %s' % (src, dst))
		cmnd = '%s %s %s' % (cmndname('cp'), src_cnv, dst_cnv)
		rc = wait(execute(cmnd, stdout=NULL, stderr=NULL, shell=True))
	return rc

#  Copy all files and directories.
#
def copy_all(src, dst, verbose=0):
	os.chdir(src)
	Print('  copying "%s" to "%s"' % (src, dst))
	names = os.listdir()
	for name in names:
		if os.path.isfile(name):
			if verbose:
				Print('    ISFILE %s ...' % name)
			cmnd = '%s %s %s' % (cmndname('cp'), src, dst)
			rc = wait(execute(cmnd), shell=True)
			if rc != 0:
				break
		elif os.path.isdir(name):
			dst_dir = '%s/%s' % (dst, name)
			if verbose:
				Print('    %s -> %s' % (src, dst_dir))
			cmnd = '%s %s %s' % (cmndname('cp'), src, dst_dir)
			rc = wait(execute(cmnd), shell=True)
			if rc != 0:
				break
	return rc

#  Convert path separators to fit current OS.
#
def pathconv(path):
	return path.replace('/', os.sep)

#  Print and flush
#
def Print(msg):
	print(msg)
	sys.stdout.flush()

#  Command names.
#
def cmndname(cmnd):
	nametab = { 'cat':	['cat', 'type'],
		    'cp':	['cp', 'copy'],
		    'rm':	['rm', 'del'],
		    'rmdir':	['rmdir', 'rmdir /s /q'],
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
parser.add_option('-E', '--replace-tex_es', dest='replace_tex_es',
			action='store_true', default=False,
			help='replace and revive tex escape sequence')
parser.add_option('-K', '--insert-kludge', dest='insert_kludge',
			action='store_true', default=False,
			help='insert kludge code')
parser.add_option('-R', '--replace-csarg', dest='replace_csarg',
			action='store_true', default=False,
			help='replace and revive cs-arg')
parser.add_option('-S', '--skip-to-lwarpmk', dest='skip_to_lwarpmk',
			action='store_true', default=False,
			help='skip to lwarpmk')
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
if options.skip_to_lwarpmk:
	dry_run_save = dry_run
	dry_run = True

'''
# ----------------------------------------------------------------------
#  Prepare work space.
#
if os.path.exists(wrkspace) and not os.path.isdir(wrkspace):
	msg = '%s exists but not a directory' % wrkspace
	abort(msg)
if os.path.exists(wrkspace):
	rc = remove_tree(wrkspace, verbose)
	if rc != 0:
		msg = 'clearing workspace failed'
		abort(msg)
if verbose:
	print('making %s' % wrkspace)
os.makedirs(wrkspace, exist_ok=True)

#  Copy files to work space.
#
patterns = [ [ '{sourcecode}', '{sourcecode}' ],
	     [ '\([^\\]\)zw', '\\1\\\\zw' ],
	     [ '^%iflwarp(\\(.*\\))', '\\1' ] ]
#
texstys = glob.glob('*.sty')
texsrcs = glob.glob('*.tex')
texcsss = glob.glob('*.css')
for inpf in texstys:
	outf = '%s/%s' % (wrkspace, inpf)
	fileconv(inpf, patterns, outf)
#
for inpf in texsrcs:
	outf = '%s/%s' % (wrkspace, inpf)
	fileconv(inpf, patterns, outf)
#
for inpf in texcsss:
	outf = '%s/%s' % (wrkspace, inpf)
	cp(inpf, outf)
#
others = glob.glob('*.cls')
others.append('fig')
if options.insert_kludge:
	others.append('insert_kludge.py')
if test:
	others.append('test.bat')
for f in others:
	dst = '%s/%s' % (wrkspace, f)
	rc = cp(f, dst, verbose=verbose)
	if rc != 0:
		msg = 'file copy failed: "%s"' % f
		abort(msg)

#  "fig/*.eps"をsvgに変換する
#
cwd = os.getcwd()
if verbose:
	print('converting image file format')
os.chdir('%s/fig' % wrkspace)
if test and os.path.isdir(svg_dir) and os.listdir(svg_dir):
	#  テスト時にsvf_dir/にsvgファイルがあればそれをコピーする
	#  これは単に時間の節約のため
	for f in glob.glob('*.eps'):
		f_svg = f.replace('.eps', '.svg')
		fname = '%s%s%s' % (svg_dir, os.sep, f_svg)
		print('  copying from %s' % fname.replace(os.sep, '/'))
		cp(fname, f_svg)
else:
	#  時間がかかるし無駄のようだがlwarpmkではこうするしかない
	for f in glob.glob('*.eps'):
		f_pdf = f.replace('.eps', '.pdf')
		f_svg = f.replace('.eps', '.svg')
		if verbose:
			print('  %s -> %s' % (f, f_svg))
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
	# for debug
	sys.exit(0)

# ----------------------------------------------------------------------
#  Generating htmls.
#
os.chdir(wrkspace)
if verbose:
	print('enter: %s' % os.getcwd().replace(os.sep, '/'))

# (1) pdflatexを用いてpdfを作成する
#
cmnd = '%s %s' % (pdflatex, texmain)
if verbose:
	print('#### %s' % cmnd)
rc = wait(execute(cmnd, stdout=sys.stdout, stderr=sys.stderr))
if rc != 0:
	msg = '%s: failed' % cmnd
	abort(msg)

"""
#  ここでできたpdfを退避しておく（この後で書き換えられてしまうから）
#	栞の文字が化けてしまうことへの対処 － うまく機能しない！
#	他の方法を考えたほうが良い
#
pdf_fname = texmain.replace('.tex', '.pdf')
pdf_fsave = '%s_save.pdf' % pdf_fname
rc = cp(pdf_fname, pdf_fsave)
if rc != 0:
	msg = 'save %s failed' % pdf_fname
	abort(msg)
#
if options.skip_to_lwarpmk:
	dry_run = dry_run_save
"""

# (2) Lwarpmkで使うマクロを有効にするために"sprmacros.sty"を書き換える
#
patterns.append(['Lwarpfalse', 'Lwarptrue'])
ofname = 'sprmacros.sty'
ifname = '%s/%s' % ('..', ofname)
fileconv(ifname.replace('/', os.sep), patterns, ofname)

# (2.1)	Replace TeX escape sequence.
#	出力するhtmlをセクション毎分割しようとすると、TeXのエスケープ
#	シーケンスが悪さをする（ファイル名がおかしくなる）のでいったん
#	別の文字列に置き換えておく（htmlができてから表示を調整する）
#	※ 対応表は "escch.replace" で定義する
#
if options.replace_tex_es:
	cmnd = 'python ../escch_replace.py -v -d ../escch.replace enc *.tex'
	if verbose:
		print('#### %s' % cmnd)
	rc = wait(execute(cmnd, stdout=sys.stdout, stderr=sys.stderr))
	if rc != 0:
		msg = '%s: failed' % cmnd
		abort(msg)
	sys.stdout.flush()

# (2.2) Replace control sequence argument.
#	\chapter{}, \section{} 等の引数に漢字があるとエラーとなるので、
#	いったん別の文字列に置き換えておく（htmlができてから元へ戻す）
#
if options.replace_csarg:
	cmnd = 'python ../csarg_replace.py -v enc *.tex'
	if verbose:
		print('#### %s' % cmnd)
	rc = wait(execute(cmnd, stdout=sys.stdout, stderr=sys.stderr))
	if rc != 0:
		msg = '%s: failed' % cmnd
		abort(msg)
	sys.stdout.flush()

# (2.3) Insert kludge.
#	lwarpmk htmlを実行するとANKから漢字に変化する箇所でエラーを起こす
#	    pdfTeX error: pdflatex,exe (file cyberb30): Font cyberb30 at 420 not found
#	おまじないとして、ダミーのvruleを挿入しておく
#	    \def\KLUDGE{\vrule width 0pt height 1pt }	(in "sprmacros.sty")
#
if options.insert_kludge:
	cmnd = 'python insert_kludge.py'	# -U
	if verbose:
		print('#### %s' % cmnd)
	rc = wait(execute(cmnd, stdout=sys.stdout, stderr=sys.stderr))
	if rc != 0:
		msg = '%s: failed' % cmnd
		abort(msg)
	sys.stdout.flush()

# (3) htmlファイルを作成する
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

# (3.1)	(2.2)で変更した引数情報を元に戻す（html本体）
#
if options.replace_csarg:
	cmnd = 'python ../csarg_replace.py -v dec *.html'
	if verbose:
		print('#### %s' % cmnd)
	rc = wait(execute(cmnd, stdout=sys.stdout, stderr=sys.stderr))
	if rc != 0:
		msg = '%s: failed' % cmnd
		abort(msg)
	sys.stdout.flush()

# (3.2)	(2.1)で変更したエスケープ文字情報を元に戻す（html本体）
#
if options.replace_tex_es:
	cmnd = 'python ../escch_replace.py -v -d ../escch.replace dec *.html'
	if verbose:
		print('#### %s' % cmnd)
	rc = wait(execute(cmnd, stdout=sys.stdout, stderr=sys.stderr))
	if rc != 0:
		msg = '%s: failed' % cmnd
		abort(msg)
	sys.stdout.flush()
#
'''
os.chdir('tmp')
if options.copy:
	#  生成されたファイルを"generated/doc/SprManual"にコピーする
	#
	fmdir = wrkspace
	todir = '../../../../generated/doc/SprManual'

	Print('=== %s' % os.getcwd())
	if os.path.exists(todir):
		if verbose:
			Print('  clearing "%s"' % todir)
		cmnd = '%s %s' % (cmndname('rmdir'), pathconv(todir))
		Print('#### %s' % cmnd)
		wait(execute(cmnd, shell=True))
	os.makedirs(todir)

	#
	targets = ['lateximages', 'fig/*.svg', '*.html', '*.css']
	if verbose:
		absdir = pathconv(os.path.abspath(todir))
		Print('copying htmls to "%s"' % absdir)
		sys.stdout.flush()

	for target in targets:
		Print('...[ %s ]...' % target)
		component = target.split('/')
		if len(component) > 1:
			d = '/'.join(component[:-1])
			os.makedirs('%s/%s' % (todir, d), exist_ok=True)

		if os.path.isdir(target):
			Print('COPY_ALL: %s, %s' % (target, todir))
			copy_all(target, todir, verbose=verbose)
		else:
			for f in glob.glob(target):
				tofile = '%s/%s' % (todir, f.replace(os.sep, '/'))
				Print('COPY: %s, %s' % (f, tofile))
				cp(f, tofile)

#
os.chdir(cwd)
sys.exit(0)

# end: buildhtml.py

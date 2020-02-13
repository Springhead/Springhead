#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	python buildhtml.py options
#	options:
#	    -d dname	Work directory name.
#
#  DESCRIPTION:
#	Generate HpwToUseCMake manual html version using plastex.
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2018/11/29 F.Kanehori	First version.
#	Ver 1.1  2019/01/29 F.Kanehori	Add: call html_escape()
#	Ver 1.2  2019/08/05 F.Kanehori	HowToUseCMake version.
# ======================================================================
version = 1.2

import sys
import os
import glob
import subprocess
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].replace(os.sep, '/').split('/')[-1].split('.')[0]
web_host = 'haselab.net'
web_user = 'demo'
PIPE = subprocess.PIPE
NULL = subprocess.DEVNULL

# for test
svg_dir = '../../../../../../HowToUseCMake-fig/svg'.replace('/', os.sep)

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
#  OSの判定
def is_unix():
	return True if os.name == 'posix' else False

#  引用符(')と(")とを相互に入れ替える
#
def exchange_quote(s):
	return s.replace("'", '|').replace('"', "'").replace('|', '"')

#  入力ファイルをutf8に変換したのち、patternsで指定されたパターンすべて
#  についてsedで書き換え処理をする。
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
		proc = execute(cmnd, stdin=out_pipe, stdout=PIPE, shell=True)
		out_pipe = proc.stdout
		med_proc.append(proc)
	if verbose > 1:
		print('EXEC: %s' % out_cmnd)
	outf = ofname.replace('/', os.sep)
	out_proc = execute(out_cmnd, stdin=out_pipe, stdout=outf, shell=True)
	#
	inp_proc.wait()
	for proc in med_proc:
		proc.wait()
	rc = out_proc.wait()
	if rc != 0:
		msg = 'file conversion failed: "%s"' % ifname
		abort(msg)

#  指定されたコマンドを実行する
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

#  Pipeオブジェクトをオープンする
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

#  プロセスの終了を待つ
#
def wait(proc):
	if dry_run:
		return 0
	rc = proc.wait()
	if not is_unix():
		rc = -(rc & 0b1000000000000000) | (rc & 0b0111111111111111)
	return rc

#  指定されたディレクトリ以下をすべて削除する
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

#  ファイルをコピーする
#  ※ dstがディレクトリ名のときはcp()を、ファイル名のときはcp0()を使用する
#
def cp(src, dst):
	#  srcとdstの組み合わせ
	#     file to file	N/A
	#     fiel to dir	src -> dst/leaf(src)
	#     dir to file	Error!
	#     dir to dir	src/* -> dst/*
	#  file to file は cp0(src, dst) で扱う

	if os.path.isdir(src) and os.path.isfile(dst):
		msg = 'copying directory to plain file (%s to %s)' % (src, dst)
		abort('Error: %s' % msg)
	if dry_run:
		Print('cp: %s %s' % (src, dst))
		return 0

	rc = 0
	if os.path.isdir(src):
		rc = __cp(src, dst)
	else:	
		for s in glob.glob(src):
			rc = __cp(pathconv(s, True), dst)
			if rc != 0: return 1	# copying failed
	return rc

def __cp(src, dst):
	if os.path.isfile(src):
		org_src_cnv = pathconv(src)
		plist = src.split('/')
		if len(plist) > 1:
			src = plist[-1]
			dst = '%s/%s' % (dst, '/'.join(plist[:-1]))
		if not os.path.exists(dst):
			Print('  creating directory %s' % dst)
			os.makedirs(dst)
		if verbose > 1:
			Print('  cp: %s -> %s' % (src, dst))
		src_cnv = pathconv(src)
		dst_cnv = pathconv(dst)
		cmnd = '%s %s %s' % (cmndname('cp'), org_src_cnv, dst_cnv)
		rc = wait(execute(cmnd, stdout=NULL, stderr=NULL, shell=True))

	else:	# both src and dst are directory
		dst = '%s/%s' % (dst, src)
		if not os.path.exists(dst):
			abspath = os.path.abspath(dst)
			Print('  creating directory %s' % pathconv(abspath, True))
			os.makedirs(dst)
		if verbose > 1:
			Print('  cp: %s -> %s' % (src, dst))
		src_cnv = pathconv(src)
		dst_cnv = pathconv(dst)
		if is_unix():
			cmnd = 'cp -r %s %s' % (src_cnv, dst_cnv)
		else:
			cmnd = 'xcopy /I /E /S /Y /Q %s %s' % (src_cnv, dst_cnv)
		rc = wait(execute(cmnd, stdout=NULL, stderr=NULL, shell=True))

	return rc

def cp0(src, dst):
	#  srcとdstの組み合わせ
	#     file to file	src -> dst

	if verbose > 1:
		Print('  cp: %s -> %s' % (src, dst))
	src_cnv = pathconv(src)
	dst_cnv = pathconv(dst)
	cmnd = '%s %s %s' % (cmndname('cp'), src_cnv, dst_cnv)
	rc = wait(execute(cmnd, stdout=NULL, stderr=NULL, shell=True))
	return rc

#  パス表記を変換する (第2引数がFalseなら現在のOSでの表記に変換する)
#
def pathconv(path, unix=False):
	if unix:
		return path.replace(os.sep, '/')
	return path.replace('/', os.sep)

#  Flush付きのprint
#
def Print(msg):
	print(msg)
	sys.stdout.flush()

#  現在のOSの元でのコマンド名を返す
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
parser.add_option('-E', '--replace-tex_esc', dest='replace_tex_esc',
			action='store_true', default=False,
			help='replace and revive tex escape sequence')
parser.add_option('-H', '--replace-html_esc', dest='replace_html_esc',
			action='store_true', default=False,
			help='replace html escape sequence')
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
#  メイン処理開始
#
if options.skip_to_lwarpmk:
	dry_run_save = dry_run
	dry_run = True

# ----------------------------------------------------------------------
#  作業場所を作成してそこに移動する
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

#  必要なファイルを作業場所にコピーする
#
patterns = [ [ '{sourcecode}', '{sourcecode}' ],
	     [ '\([^\\]\)zw', '\\1\\\\zw' ],
	     [ '^%iflwarp(\\(.*\\))', '\\1' ] ]
patterns = []
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
	cp(inpf, wrkspace)
#
others = glob.glob('*.cls')
others.append('fig')
if options.insert_kludge:
	others.append('insert_kludge.py')
if test:
	others.append('test.bat')
for f in others:
	rc = cp(f, wrkspace)
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
	#  テスト時にsvg_dir/にsvgファイルがあればそれをコピーする
	#  これは単に時間の節約のため
	for f in glob.glob('*.eps'):
		f_svg = f.replace('.eps', '.svg')
		fname = '%s%s%s' % (svg_dir, os.sep, f_svg)
		print('  copying from %s' % fname.replace(os.sep, '/'))
		cp0(fname, f_svg)
else:
	#  時間がかかるし無駄のようだがlwarpmkではこうするしかない
	for f in glob.glob('*.eps'):
		f_pdf = f.replace('.eps', '.pdf')
		f_svg = f.replace('.eps', '.svg')
		if verbose:
			print('  %s -> %s' % (f, f_svg))
		cmnd = 'lwarpmk epstopdf %s' % f
		rc = wait(execute(cmnd, stdout=NULL, shell=True))
		if rc != 0:
			print('%s -> %s: faild' % (f, f_pdf))
		cmnd = 'lwarpmk pdftosvg %s' % f_pdf
		rc = wait(execute(cmnd, stdout=NULL, shell=True))
		if rc != 0:
			print('%s -> %s: faild' % (f_pdf, f_svg))
os.chdir(cwd)
#
if options.convert_only:
	# for debug
	sys.exit(0)

# ----------------------------------------------------------------------
#  Htmlを作成する
#
os.chdir(wrkspace)
if verbose:
	print('enter: %s' % os.getcwd().replace(os.sep, '/'))

# (1) pdflatexを用いてpdfを作成する
#
cmnd = '%s %s' % (pdflatex, texmain)
if verbose:
	print('#### %s' % cmnd)
rc = wait(execute(cmnd, stdout=sys.stdout, stderr=sys.stderr, shell=True))
if rc != 0:
	msg = '%s: failed' % cmnd
	abort(msg)

"""
#  ここでできたpdfを退避しておく（この後で書き換えられてしまうから）
#	栞の文字が化けてしまうことへの対処
#  －これはうまく機能しない！
#	他の方法を考えないといけない
#
pdf_fname = texmain.replace('.tex', '.pdf')
pdf_fsave = '%s_save.pdf' % pdf_fname
rc = cp0(pdf_fname, pdf_fsave)
if rc != 0:
	msg = 'save %s failed' % pdf_fname
	abort(msg)
#
if options.skip_to_lwarpmk:
	dry_run = dry_run_save
"""

# (2) Lwarpmkで使うマクロを有効にするために"lwarp_macros.sty"を書き換える
#     これ以降 \ifLwarp が真となる
#
patterns.append(['Lwarpfalse', 'Lwarptrue'])
ofname = 'lwarp_macros.sty'
ifname = '%s/%s' % ('..', ofname)
fileconv(ifname.replace('/', os.sep), patterns, ofname)

# (2.1)	Replace TeX escape sequence.
#	TeXのエスケープシーケンス(\#)はpdf/htmlではそのまま"\#"と表示される
#	いったん別の文字列に置き換えておく（htmlができてから表示を調整する）
#	※ 対応表は "escch.replace" で定義する
#
if options.replace_tex_esc:
	cmnd = 'python ../escch_replace.py -v -d ../escch.replace enc *.tex'
	if verbose:
		print('#### %s' % cmnd)
	rc = wait(execute(cmnd, stdout=sys.stdout, stderr=sys.stderr, shell=True))
	if rc != 0:
		msg = '%s: failed' % cmnd
		abort(msg)
	sys.stdout.flush()

# (2.2) Replace control sequence argument.
#	\chapter{}, \section{} 等の引数に漢字があるとエラーとなるので、
#	いったん別の文字列に置き換えておく（htmlができてから元へ戻す）
#	セクション毎のファイルの名称も変化してしまうが、それはそのまま
#	にしておく (漢字のファイル名は翻訳ページで問題を起こすかも…)
#
if options.replace_csarg:
	cmnd = 'python ../csarg_replace.py -v enc *.tex'
	if verbose:
		print('#### %s' % cmnd)
	rc = wait(execute(cmnd, stdout=sys.stdout, stderr=sys.stderr, shell=True))
	if rc != 0:
		msg = '%s: failed' % cmnd
		abort(msg)
	sys.stdout.flush()

# (2.3) html_escape.
#	html特殊文字('&','<','>')はhtmlソース上ではエスケープしないといけないが、
#	Lwarpはこの点に関して不十分である。ここでエスケープ処理を施しておく。
#	将来Lwarpがhtmlエスケープを正しく処理するようになったなら、この部分の
#	処理は不要となる（-H オプションをやめること）
#
if options.replace_html_esc:
	opts = ' -v' * verbose
	cmnd = 'python ../html_escape.py %s *.tex' % opts
	if verbose:
		print('#### %s' % cmnd)
	rc = wait(execute(cmnd, stdout=sys.stdout, stderr=sys.stderr, shell=True))
	if rc != 0:
		msg = '%s: failed' % cmnd
		abort(msg)
	sys.stdout.flush()

# (2.4) Insert kludge.
#	lwarpmk htmlを実行するとANKから漢字に変化する箇所でエラーを起こす
#	    pdfTeX error: pdflatex,exe (file cyberb30): Font cyberb30 at 420 not found
#	おまじないとして、ダミーのvruleを挿入しておく
#	    \def\KLUDGE{\vrule width 0pt height 1pt }	(in "lwarp_macros.sty")
#	これは単純にフォントだけの問題なので、そちらが解決できたらここの
#	部分の処理は不要となる (-K オプションをやめること)
#
if options.insert_kludge:
	cmnd = 'python insert_kludge.py'	# -U
	if verbose:
		cmnd += ' -v'
		print('#### %s' % cmnd)
	rc = wait(execute(cmnd, stdout=sys.stdout, stderr=sys.stderr, shell=True))
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
		sys.stdout.flush()
	rc = wait(execute(cmnd, stdout=sys.stdout, stderr=sys.stderr, shell=True))
	if rc != 0:
		msg = '%s: failed' % cmnd
		abort(msg)

# (3.1)	(2.2)で変更した引数情報を元に戻す（html本体）
#
if options.replace_csarg:
	cmnd = 'python ../csarg_replace.py -v dec *.html'
	if verbose:
		print('#### %s' % cmnd)
	rc = wait(execute(cmnd, stdout=sys.stdout, stderr=sys.stderr, shell=True))
	if rc != 0:
		msg = '%s: failed' % cmnd
		abort(msg)
	sys.stdout.flush()

# (3.2)	(2.1)で変更したエスケープ文字情報を元に戻す（html本体）
#
if options.replace_tex_esc:
	cmnd = 'python ../escch_replace.py -v -d ../escch.replace dec *.html'
	if verbose:
		print('#### %s' % cmnd)
	rc = wait(execute(cmnd, stdout=sys.stdout, stderr=sys.stderr, shell=True))
	if rc != 0:
		msg = '%s: failed' % cmnd
		abort(msg)
	sys.stdout.flush()
#
if options.copy:
	#  生成されたファイルを"generated/doc/HowToUseCMake"にコピーする
	#
	fmdir = wrkspace
	todir = '../../../../generated/doc/HowToUseCMake'
	#
	targets = ['lateximages', 'fig/*.svg', '*.html', '*.css']
	if verbose:
		absdir = pathconv(os.path.abspath(todir))
		Print('copying htmls to "%s"' % absdir.replace(os.sep, '/'))

	for target in targets:
		cp(target, todir)
#
os.chdir(cwd)
sys.exit(0)

# end: buildhtml.py

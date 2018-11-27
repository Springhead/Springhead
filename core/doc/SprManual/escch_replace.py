#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	escseq_replace [options] func files
#	    func:	'enc': encode csname argument to random string.
#			'dec': decode replaced_argument to original one.
#	    files:	Input file names.
#
#	options:
#	    -d fname:	Conversion definition file name.
#	    -s:		Save original file (cf. -t option below).
#	    -t suffix:	Suffix of save file.
#
#  DESCRIPTION:
#	変換定義ファイルで指定された文字列の変換処理を行なう。
#	変換定義ファイルの各行の書式は、
#	    <元の文字列>,<lwarpmkに処理させる文字列>,<最終表示文字列>
#	である。
#	    'enc'では <元の文字列> → <lwarpmkに処理させる文字列>
#	    'dec'では <lwarpmkに処理させる文字列> → <最終表示文字列>
#	の処理を行なう。
#	'ren'ではファイル名に対して'dec'と同様の処理を行なう。
#
#  TODO:
#	エスケープ文字が'\#'の場合：
#	    一見うまく変換できているように見えるがリンクがたどれない。
#	    これは、'#'が<a>タグhrefのアンカーを示す文字だから。
#	    現在は置換え文字(escch.replaceの第3項)を漢字の＃にして回避
#	    ちゃんとやるなら<a>タグのhref="..."の中は変換しなければよい
#	    のだが、それには真面目にパーズしないと駄目みたい。
#	    漢字の＃でも見た目はほとんど変わらないからこれで良い？
#	    
#  VERSION:
#	Ver 1.0  2018/11/27 F.Kanehori	First release version.
# ======================================================================
version = '1.0'

import sys
import os
import re
import glob
import subprocess
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
#

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options] func file ...'
parser = OptionParser(usage = usage)
parser.add_option('-d', '--definition-file', dest='def_file',
			action='store', default='escseq.replace',
			help='conversion definition file name (default: %default)')
parser.add_option('-s', '--save-orginal', dest='save_original',
			action='store_true', default=False,
			help='save original file')
parser.add_option('-t', '--tmp-suffix', dest='suffix',
			action='store', default='escseq.org',
			help='temporary file\'s suffix (default: .%default)')
parser.add_option('-v', '--verbose', dest='verbose',
			action='count', default=0,
			help='set verbose mode')
parser.add_option('-V', '--version', dest='version',
			action='store_true', default=False,
			help='show version')

# ----------------------------------------------------------------------
#  Process for command line
#
(options, args) = parser.parse_args()
if options.version:
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
if len(args) < 2:
	parser.error("incorrect number of arguments")

# get options and input file names
def_file = options.def_file
suffix = options.suffix
verbose = options.verbose
#
func = args[0]
fn_args = args[1:]
fnames = []
for fn in fn_args:
	fnames.extend(glob.glob(fn))

if verbose:
	print('func:    %s' % func)
	print('deffile: %s' % def_file)
	print('suffix:  %s' % suffix)
	print('files:   %s' % fnames)

# ----------------------------------------------------------------------
#  メソッドの定義
# ----------------------------------------------------------------------

#  ファイル中の文字列の置換処理
#
def process(func, ix_fm, ix_to):

	#  定義ファイルを読む
	defs = read_defs(def_file)

	#  各ファイルを走査して定義された文字列を置き換える
	#
	replaces = []
	for f in fnames:
		if verbose:
			print('  processing %s' % f)
		lines = []
		replaced = False
		for line in open(f, 'r', encoding='utf-8'):
			for d in defs:
				fm = d[ix_fm]
				to = d[ix_to]
				if line.find(fm) >= 0:
					if func == 'enc':
						fm = fm.replace('\\', '\\\\')
					if verbose > 1:
						print('    %s -> %s' % (fm, to))
					line = re.sub(fm, to, line)
					replaced = True
			lines.append(re.sub(fm, to, line))

		if replaced:
			#  必要ならばファイルを書き換える
			print('  -- replacing file: %s' % f)
			if options.save_original:
				tmpfname = mktmpfname(f, suffix)
				if rename(f, tmpfname) != 0:
					msg = 'rename failed: %s -> %s' % (f, tmpfname)
					sys.exit(1)
			outf = open(f, 'w', encoding='utf-8')
			for line in lines:
				outf.write(line)
			outf.close()
	sys.stdout.flush()

#  変換定義ファイルを読む
#
def read_defs(fname):
	defs = []
	for line in open(fname, 'r', encoding='utf-8'):
		if line[-1] == '\n':
			line = line[:-1]
		defs.append(line.split(','))
	return defs

#  作業ファイル名を作成する
#
def mktmpfname(basename, suffix):
	tmpfname = '%s.%s' % (basename, suffix)
	if os.path.exists(tmpfname):
		return mktmpfname(tmpfname, suffix)
	return tmpfname

#  ファイル名を変更する
#
def rename(fm , to):
	if is_unix():
		cmnd = 'mv "%s" "%s"' % (fm, to)
	else:
		cmnd = 'rename "%s" "%s"' % (fm, to)
	proc = subprocess.Popen(cmnd, shell=True)
	rc = proc.wait()
	if not is_unix():
		rc = -(rc & 0b1000000000000000) | (rc & 0b0111111111111111)
	return rc

#  現在のOSを判定する
#
def is_unix():
	return True if os.name == 'posix' else False


# ----------------------------------------------------------------------
#  Main process.
#
if func == 'enc':
	# field #1 to #2 (for file body)
	process(func, 0, 1)
elif func == 'dec':
	# field #2 to #3 (for file body)
	process(func, 1, 2)

# ----------------------------------------------------------------------
#  End of process.
#
if verbose:
	print('%s: %s: done' % (prog, func))
sys.exit(0)

# end: csname_replace.py

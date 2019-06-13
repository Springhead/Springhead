#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	python cmake_ln.py [options] link=target ...
#	arguments:
#	    link	作成するリンクのパス
#	    target	リンクの実体を置くディレクトリのパス
#
#  DESCRIPTION:
#	指定された名称(link)のリンクをターゲットディレクトリ(target)
#	上に作成する。
#	unix では symbolic link を、Windows では junction を使用する。
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2019/05/23 F.Kanehori	First version.
# ======================================================================
version = 1.0

import sys
import os
import shutil
import glob
import subprocess
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
target_mark = '_target_body_'

# ----------------------------------------------------------------------
#  Helper methods
#

#  ファイルをムーブする
#
def move_files(src, dst):
	src = upath(src)
	dst = upath(dst)
	cwd = os.getcwd()
	os.chdir(src)
	for f in glob.glob('*'):
		if os.path.isfile(f):
			if verbose > 1:
				print('  copying %s -> %s' % (f, dst))
			shutil.move(f, dst)
		elif os.path.isdir(f):
			os.makedirs('%s/%s' % (dst, f), exist_ok=True)
			move_files('%s/%s' % (src, f), '%s/%s' % (dst, f))
	os.chdir(cwd)
	if verbose > 1:
		print('  removing directory %s' % src)
	os.rmdir(src)
			
#  動作環境の判定
#
def is_unix():
	return os.name == 'posix'

#  パスセパレータの変換（表示用）
#
def upath(path):
	return path.replace('\\', '/')

#  エラーメッセージを表示して処理を中止する
#
def fatal(msg, exitcode=1):
	sys.stderr.write('Error: %s\n' % msg)
	sys.exit(exitcode)

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options] texmain'
parser = OptionParser(usage = usage)
#
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
#
verbose = options.verbose

# ----------------------------------------------------------------------
#  メイン処理開始
#

for arg in args:
	link, target = arg.replace(os.sep, '/').split('=')
	if verbose:
		print('making link: %s -> %s' % (link, target))

	# target directory 及び target mark file の存在を保証する
	#	Windows では特権がないと symbolic link が作成できない。
	#	仕方がないので junciton を作成するのだが、junction は
	#	symbolic link とは異なり、通常の directory との区別が
	#	簡単にはできない。
	#	代替案として、実体 directory に target mark を置く
	#	ことで既に junction が張られているか否かを判定する。
	#	unix では必要ないがコードが煩雑になるので踏襲する。
	#
	if os.path.exists(target) and not os.path.isdir(target):
		fatal('"%s" is not a directory' % target)
	os.makedirs(target, exist_ok=True)
	if not os.path.exists('%s/%s' % (target, target_mark)):
		f = open('%s/%s' % (target, target_mark), 'w')
		f.close()

	# link が存在する場合
	#
	if os.path.exists(link):

		# それはディレクトリでなければいけない。
		#
		if not os.path.isdir(link):
			fatal('"%s" is not a directory' % link)

		# それが target (実体) を指していれば何もしなくてよい。
		#
		if os.path.exists('%s/%s' % (link, target_mark)):
			if verbose > 1:
				print('"%s" already exists' % link)
			continue

		# さもなければ、内容を target にコピーした後で削除する。
		#
		move_files(os.path.abspath(link), os.path.abspath(target))

	# link の名称で target にリンクを張る。
	#	Window では junction, unix では symbolic link を使う。
	#
	if is_unix():
		cmnd = 'ln -s %s %s' % (target, link)
	else:
		dos_link = link.replace('/', os.sep)
		dos_target = target.replace('/', os.sep)
		cmnd = 'mklink /j %s %s' % (dos_link, dos_target)
	if verbose > 1:
		print('%s %s' % ('%%' if is_unix() else '>', cmnd))
	proc = subprocess.Popen(cmnd, shell=True, stdout=subprocess.DEVNULL)
	stat = proc.wait()
	if stat != 0:
		fatal('%s failed (%d)' % 'ln' if is_unix() else 'mklink')

sys.exit(0)

# end: cmake_ln.py

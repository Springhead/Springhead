#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  FILE:
#	CheckClosedSrc.py
#
#  SYNOPSIS:
#	python CheckClosedSrc.py [options]
#
#  OPTIONS:
#	-s file		ヘッダファイルのサンプル ("UseColsedSrcOrNot.h.sample")
#
#  DESCRIPTION:
#	Closed source の使用/非使用を定義するファイル "UseClosedSrcOrNot.h"
#	を適切に用意するためのスクリプト。
#
#	次の手順で処理を行なう．
#	    (1)	Springhead/core/include に移動する。
#	    (2)	UseClosedSrcOrNot.h がなければ、UseClosedSrcOrNot.h.sample を
#		コピーして UseClosedSrcOrNot.h とする。
#	    (3) UseClosedSrcOrNot.h で USE_CLOSED_SRC を define しているのに
#		ディレクトリ ../../closed がなければ、UseClosedSrcOrNot.h の
#		内容を #undef USE_CLOSED_SRC と書き換える。
#	    (4) 元のディレクトリに戻る。
#
# ==============================================================================
#  Version:
#     Ver 1.00	 2019/01/08 F.Kanehori	初版
#     Ver 1.01	 2019/04/01 F.Kanehori	Python library path 検索方法変更.
#     Ver 1.011	 2019/04/11 F.Kanehori	Discard Ver.1.02 and after.
# ==============================================================================
version = 1.011

import sys
import os
import re
import shutil
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
patt = r'#define[ \t]+USE_CLOSED_SRC'

# ----------------------------------------------------------------------
#  Import Springhead python library.
#
from FindSprPath import *
spr_path = FindSprPath(prog)
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from Proc import *
from Error import *
from Util import *

# ----------------------------------------------------------------------
#  Directories
#
sprtop = spr_path.abspath()
incdir = spr_path.relpath('inc')
closed = '../../closed'

# ----------------------------------------------------------------------
#  Files
#
header_file = 'UseClosedSrcOrNot.h'
default_file = header_file + '.sample'
saved_file = header_file + '.save'

# ----------------------------------------------------------------------
#  オプションの定義
#
usage = 'Usage: %prog [options]'
parser = OptionParser(usage = usage)
parser.add_option('-s', '--sample-file', dest='sample_file',
                        action='store', default=default_file,
                        help='sample header file', metavar='FILE')
parser.add_option('-v', '--verbose', dest='verbose',
			action='count', default=0,
                        help='set verbose count')
parser.add_option('-V', '--version', dest='version',
			action='store_true', default=False,
                        help='show version')

# ----------------------------------------------------------------------
#  コマンドラインの解析
#
(options, args) = parser.parse_args()
if options.version:
        print('%s: Version %s' % (prog, version))
        sys.exit(0)
if len(args) != 0:
	Error(prog).error('incorrect number of arguments')
	Proc().execute('python %s.py -h' % prog).wait()
	sys.exit(0)

sample_file = options.sample_file
verbose = options.verbose

# ----------------------------------------------------------------------
#  Main process
# ----------------------------------------------------------------------
#  Step 1.
#	Springhead/core/include に移動する。
#	
cwd = os.getcwd()
os.chdir(incdir)
if verbose:
	print('step 1: moved to %s' % os.getcwd().replace(os.sep, '/'))

# ----------------------------------------------------------------------
#  Step 2.
#	UseClosedSrcOrNot.h がなければ、UseClosedSrcOrNot.h.sample を
#	コピーして UseClosedSrcOrNot.h とする。
#
if not os.path.exists(header_file):
	if verbose:
		print('step 2: copying from %s' % sample_file)
	path = shutil.copy(sample_file, header_file)

# ----------------------------------------------------------------------
#  Step 3.
#	UseClosedSrcOrNot.h で USE_CLOSED_SRC を define しているのに
#	ディレクトリ ../../closed がなければ、UseClosedSrcOrNot.h の
#	内容を #undef USE_CLOSED_SRC と書き換える。
#
if not os.path.exists(closed):
	need_rewrite = False
	with open(header_file) as f:
		lines = f.readlines()
		for line in lines:
			m = re.search(patt, line)
			if m:
				need_rewrite = True
				break
	f.close()
	#
	if need_rewrite:
		shutil.move(header_file, saved_file)
		out = open(header_file, 'w')
		for line in lines:
			if verbose:
				print('        ==> %s' % line.strip())
			m = re.search(patt, line)
			if m:
				line = '#undef USE_CLOSED_SRC\n'
				if verbose:
					print('step 3. -> %s' % line.strip())
			out.write(line)
		out.close()
		f.close()
		if os.path.exists(saved_file):
			os.remove(saved_file)

# ----------------------------------------------------------------------
#  Step 4.
#	元のディレクトリに戻る。
#
os.chdir(cwd)
if verbose:
	print('step 4: returned to %s' % cwd.replace(os.sep, '/'))

sys.exit(0)

# end: CheckClosedSrc.py

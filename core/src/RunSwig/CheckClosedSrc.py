#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  SYNOPSIS:
#	python CheckClosedSrc.py
#
#  DESCRIPTION:
#	配布されたファイル"SprUseClosedSrcOrNot.h"では、closed sourceが使用
#	できるようにマクロ"USE_CLOSED_SRC"が#defineされている。
#	(これは誤コミットによるファイル変更のリスクが少ないと思われるから)
#
#	もし"Springhead"と同じレベルに"closed"というディレクトリが存在しない
#	ならば、マクロ"USE_CLOSED_SRC"の#defineを#undefと書き直すことにより
#	closed sourceを使用しないようにする。
#	ただし、既に#undefに書き換えられているならば何もしない。
#
# ==============================================================================
#  Version:
#     Ver 1.00	 2019/01/08 F.Kanehori	初版
#     Ver 1.01	 2019/04/01 F.Kanehori	Python library path 検索方法変更.
#     Ver 2.00	 2020/04/09 F.Kanehori	方針の全面変更 (DESCRIPTION参照)
#     Ver 2.01	 2020/04/12 F.Kanehori	Bug fix.
# ==============================================================================
version = '2.01'

import sys
import os
import re
import shutil
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]

# ----------------------------------------------------------------------
#  Import Springhead python library.
#
from FindSprPath import *
spr_path = FindSprPath(prog)
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from Proc import *
from Error import *
#from Util import *

# ----------------------------------------------------------------------
#  Directories
#
sprtop = spr_path.abspath()
incdir = spr_path.relpath('inc')
closed = '../../../closed'

# ----------------------------------------------------------------------
#  Files
#
header_file = 'SprUseClosedSrcOrNot.h'
tmp_file = header_file + '.tmp'

# ----------------------------------------------------------------------
#  オプションの定義
#
usage = 'Usage: %prog [options]'
parser = OptionParser(usage = usage)
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

verbose = options.verbose

# ----------------------------------------------------------------------
#  Springhead/core/include に移動する。
#	
cwd = os.getcwd()
os.chdir(incdir)
if verbose:
	print('cwd: %s' % os.getcwd().replace(os.sep, '/'))

# ----------------------------------------------------------------------
#  ディレクトリ "../../closed" が存在しない場合に限り、
#  ファイル "SprUseClosedSRcOrNot.h" 中に "#define USE_CLOSED_SRC"
#  という行があれば、それを"#undef USE_CLOSED_SRC"と書き換える。
#
if not os.path.exists(closed):
	patt = r'#define[ \t]+USE_CLOSED_SRC'
	need_rewrite = False
	lines_out = []
	with open(header_file) as f:
		lines = f.readlines()
		for line in lines:
			line = line.strip()
			m = re.search(patt, line)
			if m:
				line = '#undef USE_CLOSED_SRC'
				need_rewrite = True
				if verbose:
					print('  line changed to: %s' % line)
			lines_out.append(line)
	f.close()

	if need_rewrite:
		out = open(tmp_file, 'w', newline="\n")
		for line in lines_out:
			out.write(line+'\n')
		out.close()
		shutil.move(tmp_file, header_file)
		if verbose:
			print('  rewrite file: "%s"' % header_file)

# ----------------------------------------------------------------------
#  元のディレクトリに戻る。
#
os.chdir(cwd)

sys.exit(0)

# end: CheckClosedSrc.py

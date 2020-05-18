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
#
#	処理の手順：
#	    (1)	Springheadのトップディレクトリに移動する。
#	    (2) ディレクトリ"../closed"がなければ、
#	      (2-1) "core/include"に移動する。
#	      (2-2) "SprUseClosedSrcOrNot.h"の"#define"を"#undef"と編集する。
#	    (3) 元のディレクトリに戻る。
#
# ==============================================================================
#  Version:
#     Ver 1.00	 2019/01/08 F.Kanehori	初版
#     Ver 1.01	 2019/04/01 F.Kanehori	Python library path 検索方法変更.
#     Ver 2.00	 2020/04/09 F.Kanehori	方針の全面変更 (DESCRIPTION参照)
# ==============================================================================
version = '2.00'

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
#	ディレクトリ"../../closed"があれば、"SprUseClosedSRcOrNot.h"の
#	内容を"#undef USE_CLOSED_SRC"と書き換える。
#
if not os.path.exists(closed):
	out = open(tmp_file, 'w')
	with open(header_file) as f:
		lines = f.readlines()
		for line in lines:
			m = re.search(patt, line)
			if m:
				line = '#undef USE_CLOSED_SRC\n'
				if verbose:
					print('step 2. -> %s' % line.strip())
			out.write(line)
	out.close()
	f.close()
	shutil.move(tmp_file, header_file)
	print('    do not use closed source')
else:
	print('    use closed source')

# ----------------------------------------------------------------------
#  Step 3.
#	元のディレクトリに戻る。
#
os.chdir(cwd)
if verbose:
	print('step 3: returned to %s' % cwd.replace(os.sep, '/'))

sys.exit(0)

# end: CheckClosedSrc.py

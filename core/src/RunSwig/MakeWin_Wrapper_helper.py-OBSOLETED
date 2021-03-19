#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	python MakeWin_Wrapper_helper.py setup-file tmp-file
#
#  DESCRIPTION:
#	引数 setup-file で指定されたファイルに記述された内容を見て、
#	バッチファイルで処理しやすい形式の作業ファイルを作成する。
#
#	(注) 移行期間の措置として、環境変数 MIGRATION_TEST も設定する。
#
# ----------------------------------------------------------------------
#  Version:
#     Ver 1.00  2020/12/10 F.Kanehori	初版
# ======================================================================
version = "1.00"

import sys
import os
from optparse import OptionParser

print('[[[[ set_environ ]]]]')
# ----------------------------------------------------------------------
#  このスクリプトは "<SprTop>/core/src/RunSwig" に置く
#
ScriptFileDir = os.sep.join(os.path.abspath(__file__).split(os.sep)[:-1])
SrcDir = '/'.join(ScriptFileDir.split(os.sep)[:-1])

# ----------------------------------------------------------------------
#  引数とオプションの処理
#
usage = 'Usage: %prog setup-file out-file'
parser = OptionParser(usage = usage)
parser.add_option('-v', '--verbose',
			dest='verbose', action='count', default=0,
			help='set verbose count')
parser.add_option('-V', '--version',
			dest='version', action='store_true', default=False,
			help='show version')

(options, args) = parser.parse_args()
if options.version:
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
if len(args) != 2:
	parser.error("incorrect number of arguments")

setup_file = args[0]
out_file = args[1]

verbose = options.verbose
if verbose:
	print('  setup file: %s' % setup_file)
	print('  out file:   %s' % out_file)

# ----------------------------------------------------------------------
#  処理開始
#
if not os.path.exists('%s/setup.conf' % SrcDir):
	#  セットアップファイルがないときは何もしない
	#
	sys.exit(0)

#  ライブラリを取り込む
#
libdir = '%s/RunSwig/pythonlib' % SrcDir
sys.path.append(libdir)
from SetupFile import *
from TextFio import *

#  設定値を取得する
#
sf = SetupFile(setup_file)

all_values = {}
all_values['PROG'] = {}
for key in sf.get_keys(sf.PROG):
	all_values['PROG'][key] = sf.get_prog(key)
all_values['PATH'] = {}
for key in sf.get_keys(sf.PATH):
	all_values['PATH'][key] = sf.get_path(key)
all_values['DATA'] = {}
for key in sf.get_keys(sf.DATA):
	all_values['DATA'][key] = sf.get_data(key)

#  ファイルに出力する
#
outf = TextFio(out_file, 'w')
outf.open()

#  プログラム名
values = all_values['PROG']
for key in values:
	if key == 'python' or values[key] == 'NOT FOUND':
		continue
	line = 'PROG __%s__ %s' % (key, values[key])
	#print(line)
	outf.writeline(line)
#  パス
#
values = all_values['PATH']
for key in values:
	if key == 'python':
		continue
	line = 'PATH __%s__ %s' % (key, values[key])
	#print(line)
	outf.writeline(line)
#  データ
values = all_values['DATA']
for key in values:
	if key != 'MIGRATION_TEST':
		continue
	line = 'DATA __%s__ %s' % (key, values[key])
	#print(line)
	outf.writeline(line)

outf.close()

# ----------------------------------------------------------------------
#  処理終了
#
sys.exit(0)

# end: set_environ.py

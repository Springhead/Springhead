#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  SYNOPSIS:
#	python make_projs_file out-file [env-name [--cmake]]
#	    out-file	出力する(バッチ)ファイル名
#	    env-name	設定する(環境)変数名
#
#  DESCRIPTION:
#	引数が2個以上のとき
#	    プロジェクト名一覧を空白区切りの文字列として環境変数 env-name に
#	    設定するバッチファイルを生成する.
#	    プロジェクト名一覧を取得するには call out-file とする.
#	    引数 --cmake が指定されたときは, バッチファイルの代わりに cmake
#	    で include 可能な cmake text ファイルを生成する.
#	引数が1個のとき
#	    プロジェクトとその依存関係を表したファイル(旧 do_swigall.projs と
#	    同じ形式)のテキストファイルを生成する.
#
#	プロジェクト名は, "../../RunSwig/project_dependencies.py" に定義された
#	プロジェクト依存関係定義から取得したものに Base を加えたものである.
#
# ==============================================================================
#  VERSION:
#     Ver 1.0	 2021/07/19 F.Kanehori	do_swigall.projs を python script 化.
# ==============================================================================
version = '1.0'
debug = False

import sys
import os

# ----------------------------------------------------------------------
#  このスクリプトは ".../core/src/SprCSharp/RunSwig_CSharp" に置く.
#
ScriptFileDir = os.sep.join(os.path.abspath(__file__).split(os.sep)[:-1])
prog = sys.argv[0].replace('/', os.sep).split(os.sep)[-1].split('.')[0]
SrcDir = '/'.join(ScriptFileDir.split(os.sep)[:-2])

# ----------------------------------------------------------------------
#  引数の処理
#
projs_dept = False
make_batch = False
if len(sys.argv) == 2:
	projs_dept = True
if len(sys.argv) >= 3:
	make_batch = True
	cmake = False
	if (len(sys.argv) == 4 and sys.argv[3] == '--cmake'):
		cmake = True
if not projs_dept and not make_batch:
	print('Error: invalid number of arguments')
	print('Usage: %s out-file             # for projs-dept' % prog)
	print('       %s out-file env-name    # for make-batch' % prog)
	sys.exit(1)
if projs_dept:
	out_file = sys.argv[1]
	if debug:
		print('  make project depndency list file.')
		print('    out-file: %s' % out_file)
if make_batch:
	out_file = sys.argv[1]
	env_name = sys.argv[2]
	if debug:
		print('  make setting all projects batch file.')
		print('    out-file: %s' % out_file)
		print('    env-name: %s' % env_name)
		print('    cmake:    %s' % cmake)

# ----------------------------------------------------------------------
#  プロジェクト依存関係定義ファイルを取り込む.
#
sys.path.append('%s/RunSwig' % SrcDir)
from project_dependencies import *

# ----------------------------------------------------------------------
#  ファイルを生成する.
#
if projs_dept:
	with open(out_file, 'w') as f:
		for key in sorted(projs):
			depts = proj_depts[key]
			if depts == 'None':
				depts = ''
			f.write('%s %s\n' % (key, depts))
			if debug:
				print('  %s %s' % (key, depts))

if make_batch:
	#  環境変数 env-name を設定するバッチファイル
	#	set <env-name>=<proj proj ... proj>
	#  または
	#  cmake で include できるテキストファイル
	#	set(<env-name> <proj proj ... proj>)
	#
	all_projects = ['Base']
	all_projects.extend(sorted(projs))
	project_list = ' '.join(all_projects)
	if debug:
		print('  projects: %s' % project_list)
	#  ファイルに書き出す.
	#
	with open(out_file, 'w') as f:
		if cmake:
			f.write('set(%s %s)\n' % (env_name, project_list))
		else:
			f.write('set %s=%s\n' % (env_name, project_list))

sys.exit(0)

#end: make_projs_file.py

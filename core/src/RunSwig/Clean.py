#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  SYNOPSIS:
#	python Clean src-dir lib-path
#
#	Arguments:
#	    src-dir:	Top directory of source tree (e.g. .../core/src).
#	    lib-path:	Library file path
#			(e.g. .../generated/lib/win64/Springhead15.0x64.lib).
#
#  DESCRIPTION:
#	RunSwig を実行して生成したファイルを削除する。
#	従来の Visual Studio 版では、RunSwig project は makefile projectとして
#	実装されており、そこでは build, rebuild, clean のそれぞれで実行するコマ
#	ンドを個別に記述することができた。
#	しかし CMake の add_custom_target で生成される RunSwig project は C++
#	default projectであり、RunSwig の実体であるコマンドは CustomBuild の
#	Command として実行される。この場合、clean ではこれらのコマンドは実行
#	されない (claen の実行はマニフェストファイル(?)で規定されている)。
#	したがって、RunSwig の clean を実行するための仮プロジェクトを追加する
#	ものとする。RunSwig の生成物まで claen したい場合は、他のプロジェクト
#	を rebuild するより先にこの仮プロジェクトを「ビルド」することで、この
#	スクリプトを実行する必要がある。
#	このスクリプトでは、さらに、generated/lib 以下に生成されたライブラリ
#	ファイルも削除する。
#
# ==============================================================================
#  Version:
#	Ver 1.0	 2019/03/07 F.Kanehori	First release version.
# ==============================================================================
version = 1.0

import sys
import os
import shutil
import subprocess
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = os.path.basename(sys.argv[0]).split('.')[0]

# ---------------------------------------------------------------------
#  Options
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
#  Process for command line
#
(options, args) = parser.parse_args()
if options.version:
        print('%s: Version %s' % (prog, version))
        sys.exit(0)
if len(args) != 2:
	print('%s: incorrect number of arguments\n' % prog)
	subprocess.Popen('python %s.py -h' % prog, shell=True).wait()
	sys.exit(1)

srcdir = args[0].replace('\\', '/')
libpath = args[1].replace('\\', '/')
verbose = options.verbose

if verbose:
	print('src-dir: %s' % srcdir)

# ----------------------------------------------------------------------
#  Projects
#
projects = ('Collision Creature FileIO Foundation Framework Graphics ' \
	    'HumanInterface Physics').split()

# ----------------------------------------------------------------------
#  Files to delete
#
for_all = ('#.i #Decl.hpp #Stub.cpp #Stub.mak.txt do_swigone.projs ' \
	   'makefile.swig Spr#Decl.hpp swig_spr.log').split()
for_FW  = ('FWOldSpringheadDecl.hpp FWOldSpringheadStub.cpp').split()

# ----------------------------------------------------------------------
#  Main process
#
cwd = os.getcwd()

#  RunSwig が生成したファイルを削除する
#
for project in projects:
	files = list(map(lambda x: x.replace('#', project), for_all))
	if project == 'Framework':
		files.extend(for_FW)
	if verbose:
		print('%s: ' % project)
	#
	os.chdir('%s/%s' % (srcdir, project))
	for f in files:
		if os.path.exists(f):
			if verbose:
				print('  delete %s' % f)
			os.remove(f)

#  ライブラリファイルを削除する
#
if os.path.exists(libpath):
	if verbose:
		print('  delete %s' % libpath)
	os.remove(libpath)

#  以上
#
os.chdir(cwd)
sys.exit(0)

# end: CleanRunSwig.py

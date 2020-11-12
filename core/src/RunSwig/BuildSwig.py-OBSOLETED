#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  SYNOPSIS:
#	python BuildSwig.py [options] build-dir
#	    build-dir	    ソリューションファイルのあるディレクトリ
#
#  OPTIONS:
#	    -c config	    ビルド構成 (Debug of <Release>)
#	    -l path	    ログファイルパス
#	    -p platform	    プラットフォーム (x86 or <x64>)
#	    -t toolset	    ツールセットバージョン (<15.0>)
#	    -D		    dry run 指定
#
#  DESCRIPTION:
#	swig をビルドするための wrapper スクリプト (Windows用)
#
# ==============================================================================
#  Version:
#	Ver 1.00 2020/05/18 F.Kanehori	初版
#	Ver 1.10 2020/06/11 F.Kanehori	devenv 検索方式変更
# ==============================================================================
version = 1.10
debug = False
trace = False

import sys
import os
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
if trace:
	print('ENTER: %s: %s' % (prog, sys.argv[1:]))
	sys.stdout.flush()

# ----------------------------------------------------------------------
#  Import Springhead python library.
#
from FindSprPath import *
spr_path = FindSprPath(prog)
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)
from Util import *
from Error import *
from VisualStudio import *

# ----------------------------------------------------------------------
#  Globals
#
E = Error(prog)
U = Util()

# ---------------------------------------------------------------------
#  オプションの定義
#
usage = 'Usage: %prog [options]'
parser = OptionParser(usage = usage)
parser.add_option('-D', '--dry_run',
			dest='dry_run', action='store_true', default=False,
			help='do not execute command')
parser.add_option('-c', '--config',
			dest='config', action='store', default='Release',
			metavar='CONFIG',
			help='build configuration (default: Release)')
parser.add_option('-d', '--debug',
			dest='debug', action='store_true', default=False,
			help='debug mode')
parser.add_option('-l', '--logfile',
			dest='logfile', action='store', default='./swig_build.log',
			metavar='PATH (default: <build_dir>/swig_build.log)',
			help='log file path')
parser.add_option('-p', '--platform',
			dest='platform', action='store', default='x64',
			metavar='PLATFORM',
			help='build configuration (default: x64)')
parser.add_option('-t', '--toolset',
			dest='toolset', action='store', default='15.0',
			help='toolset version')
parser.add_option('-v', '--verbose',
			dest='verbose', action='count', default=0,
			help='set verbose count')
parser.add_option('-V', '--version',
			dest='version', action='store_true', default=False,
			help='show version')

# ----------------------------------------------------------------------
#  コマンドラインの解析
#
(options, args) = parser.parse_args()
if options.version:
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
if len(args) != 1:
	parser.error("incorrect number of arguments")

build_dir = args[0]
sln_file  = 'swig%s.sln' % options.toolset

config	 = options.config
logfile	 = options.logfile
platform = options.platform
toolset	 = options.toolset
verbose  = options.verbose
dry_run	 = options.dry_run
debug    = options.debug
if verbose:
	print('  cwd:         %s' % U.upath(os.getcwd()))
	print('  build-dir:   %s' % U.upath(build_dir))
	print('  sln-file:    %s' % sln_file)
	print('  config:      %s' % config)
	print('  platform:    %s' % platform)
	print('  toolset:     %s' % toolset)
	print('  logfile:     %s' % logfile)

# ----------------------------------------------------------------------
#  引数のチェック
#
if not os.path.exists(build_dir) and not os.path.isdir(build_dir):
	E.abort('no build diretory found: "%s"' % build_dir)
if not os.path.exists('%s/%s' % (build_dir, sln_file)):
	E.abort('no solution file found: "%s"' % sln_file)

# ----------------------------------------------------------------------
#  ビルド
#
cwd = os.getcwd()
os.chdir(build_dir)
if trace:
	print('ENTER: %s at "%s"' % (prog, U.upath(cwd)))

vs = VisualStudio(toolset)
if vs.error() is not None:
	if 'devenv not found' in vs.error():
		print('  ***************************************************************')
		print('  ** Swig is not built because devenv.exe not found.           **')
		print('  ** Please add appropreate path to environment variable PATH. **')
		print('  ** To find the path,                                         **')
		print('  **   invoke "Developer Command Prompt for VS 20xx" and       **')
		print('  **   type "where devenv.exe".                                **')
		print('  ***************************************************************')
vs.solution(sln_file)
vs.set(VisualStudio.LOGFILE, logfile)
vs.set(VisualStudio.DRYRUN, dry_run)
msg = vs.error()
if not msg is None:
	E.error(msg)
if verbose:
	vs.info()

status = vs.build(platform, config)
print('%s: status %d' % (prog, status))
os.chdir(cwd)

# ----------------------------------------------------------------------
#  処理終了.
#
if trace:
	print('LEAVE: %s' % prog)
print()
sys.stdout.flush()
sys.exit(0)

# end create_mkf.py

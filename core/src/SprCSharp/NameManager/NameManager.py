#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  SYNOPSIS:
#	NameManager [-d srctop] [-i inifile] [-o outfile] [--cmake]
#	    srctop:	Top directory name of system src tree (str).
#	    inifile:	Environment variable definition file name (str).
#	    outfile:	Output file name (str).
#	    cmake:	Generate cmake source text intead of DOS command.
#
#  DESCRIPTION:
#	SprCSharp で共通に使用する名前 (環境変数) を定義する.
#	設定する名前は定義ファイル NameManager.ini から読み込まれ, Windows
#	バッチファイル NameManager.bat に出力される.
#	これらの名前を使用するには, ここで作成したバッチファイルを呼び出せば
#	よい.
#	    call NameManager.bat
#
# -----------------------------------------------------------------------------
#  VERSION:
#     Ver 1.0	 2017/01/16 F.Kanehori	First release version.
#     Ver 1.1	 2017/01/18 F.Kanehori	Change directory position (-d).
#     Ver 1.2	 2017/01/27 F.Kanehori	Bug fix.
#     Ver 1.3	 2017/09/07 F.Kanehori	Change to new python library.
#     Ver 1.4	 2017/11/29 F.Kanehori	Change python library path.
#     Ver 1.5	 2021/07/15 F.Kanehori	追加: オプション --cmake.
#     Ver 1.5.1	 2021/09/13 F.Kanehori	Bug fix.
# ==============================================================================
version = "1.5.1"
debug = False
trace = False

import sys
import os
import re
from optparse import OptionParser

# -----------------------------------------------------------------------------
#  このスクリプトは ".../core/src/SprCSharp/NameManager" に置く	
#
ScriptFileDir = os.sep.join(os.path.abspath(__file__).split(os.sep)[:-1])
prog = sys.argv[0].replace('/', os.sep).split(os.sep)[-1].split('.')[0]
TopDir = '/'.join(ScriptFileDir.split(os.sep)[:-4])
SrcDir = '/'.join(ScriptFileDir.split(os.sep)[:-2])
SetupExists = os.path.exists('%s/setup.conf' % SrcDir)
BaseDir = '/'.join(ScriptFileDir.split(os.sep)[:-5])
if trace:
	print('ENTER: %s %s' % (prog, ' '.join(sys.argv[1:])))
	sys.stdout.flush()

# -----------------------------------------------------------------------------
#  Springhead python library の導入
#
libdir = '%s/RunSwig/pythonlib' % SrcDir
sys.path.append(libdir)
from KvFile import *
from Util import *
from Proc import *
from Error import *
from SetupFile import *

# -----------------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options]'
parser = OptionParser(usage = usage)
parser.add_option('-c', '--cmake',
			dest='cmake', action='store_true', default=False,
			help='generate cmake source text')
parser.add_option('-d', '--srctop',
			dest='srctop', default=SrcDir,
			metavar='DIR',
			help='top directory of src tree [default: %default]')
parser.add_option('-i', '--inifile',
			dest='inifile', default='NameManager.ini',
			metavar='FILE',
			help='name definiton file name [default: %default]')
parser.add_option('-o', '--outfile',
			dest='outfile', default='NameManager.bat',
			metavar='FILE',
			help='output batch file name [default: %default]')
parser.add_option('-t', '--test',
			dest='test_only', action='store_true', default=False,
			help='do not make batch file (for debug)')
parser.add_option('-v', '--verbose',
			dest='verbose', action='count', default=0,
			help='set verbose mode')
parser.add_option('-V', '--version',
			dest='version', action='store_true', default=False,
			help='show version')

# -----------------------------------------------------------------------------
#  コマンドラインオプションの解析
#
(options, args) = parser.parse_args()
if options.version:
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
if len(args) != 0:
	proc = Proc(verbose=options.verbose)
	proc.execute('%s %s.py -h' % (python, prog)).wait()
	parser.error("incorrect number of arguments")
	sys.exit(0)

# Options.
filetype  = 'cmake' if options.cmake else 'dos_batch'
srctop	  = options.srctop	# top directory of source tree
inifile	  = options.inifile	# initial file name
outfile	  = options.outfile	# outout file name
test_only = options.test_only	# do not make output file (for debug)
verbose	  = options.verbose	# verbose mode

if filetype == 'cmake' and outfile == 'NameManager.bat':
	outfile = 'NameManager.cmake'

if verbose:
	print('argument used:')
	print('  srctop:    %s' % srctop)
	print('  filetype:  %s' % filetype)
	print('  inifile:   %s' % inifile)
	print('  outfile:   %s' % outfile)
	print('  test_only: %s' % test_only)
	print()

# -----------------------------------------------------------------------------
#  定義ファイルを読み込む
#	定義ファイルは "name=value" 形式で KvFile で扱えるものとする.
#	このファイルはマクロ TOPDIR を与えたときすべての参照が解決するように
#	定義されていなければならない.
#
kvf = KvFile(inifile, sep='=')
count = kvf.read(dic={'TOPDIR': os.path.abspath(TopDir)})
if count < 0:
	Error(prog).put(kvf.error())

# -----------------------------------------------------------------------------
#  次のプログラムについては, ここで適切なパスを探索し設定する.
#	python, nmake
#
buildtool_python = '%s/buildtool/win32/python.exe' % TopDir
hidden_python = '%s/DailyBuildTestTools/Python/python.exe' % BaseDir
hidden_nmake  = '%s/DailyBuildTestTools/bin/nmake.exe' % BaseDir
if SetupExists:
	sf = SetupFile('%s/setup.conf' % SrcDir, verbose=1)
	sf.setenv()
	python = sf.getenv('python')
	nmake = sf.getenv('nmake')
else:
	if os.path.exists(buildtool_python):
		python = buildtool_python
	elif os.path.exists(hidden_python):
		python = hidden_python		# for debug
	else:
		python = 'python'		# default python
	#
	if os.path.exists(hidden_nmake):
		nmake = hidden_nmake		# for debug
	else:
		nmake = 'nmake'			# default nmake
#
python = re.sub(r'^"(.*)"$', r'\1', python)	# strip enclosing quotation
nmake = re.sub(r'^"(.*)"$', r'\1', nmake)
nmake.replace('""', '"')
kvf.set('PYTHON', python)
kvf.set('NMAKE', nmake)

# -----------------------------------------------------------------------------
#  ここまでで登録されたキーは次のとおり
#
keys = sorted(kvf.keys())

if verbose:
	##  Make fixed length string.
	#   @param name		Any string.
	#   @param width	Field width (int).
	#   @returns		Fixed width string.
	def fixed(name, width):
		name += ':'
		if len(name) < width-1:
			name += ' ' * (width-len(name))
		return name

	print('names defined (%d):' % count)
	for key in keys:
		value = Util().pathconv(kvf.get(key), 'unix')
		if os.path.isfile(value):	kind = 'file'
		elif os.path.isdir(value):	kind = 'dir'
		else:				kind = 'name'
		print('  %s%s (%s)' % (fixed(key, 24), value, kind))

if test_only:
	sys.exit(0)

# -----------------------------------------------------------------------------
#  Windows のバッチファイルを生成する.
#	This is unnecessary if names can be exported directly!!
#
if filetype == 'dos_batch':
	##  Header strings of output file.
	headers	 = [ '@echo off', 'rem FILE: %s' % outfile, '' ]
	##  Trailer strings of output file.
	trailers = [ '', 'rem end: %s' % outfile ]

	f = TextFio(outfile, 'w')
	if f.open() < 0:
		Error(prog).put(f.error())
	#
	f.writelines(headers)
	for key in keys:
		line = 'set %s=%s' % (key, Util().pathconv(kvf.get(key)))
		if verbose > 1:
			print(line)
		f.writeline(line)
	f.writelines(trailers)
	f.close()

# -----------------------------------------------------------------------------
#  Cmake で include 可能なテキストファイルを生成する.
#
if filetype == 'cmake':
	f = TextFio(outfile, 'w')
	if f.open() < 0:
		Error(prog).put(f.error())
	#
	for key in keys:
		line = 'set(%s "%s")' % (key, Util().upath(kvf.get(key)))
		if verbose > 1:
			print(line)
		f.writeline(line)
	f.close()

# -----------------------------------------------------------------------------
#  処理終了
# -----------------------------------------------------------------------------
sys.exit(0)

# end: NameManager.py

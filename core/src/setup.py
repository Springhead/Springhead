#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	python setup.py [-f (--force) | -F (--Force)] python-path
#	options:
#	    -f:		無条件に再セットアップを実行する。
#	    -F:		-f と同様 (swig を clean build する)。
#	    -c:		セットアップファイルの検査のみを実行する。
#	    -s:		セットアップファイル名称
#	    -d:		devenv の選択番号 (ユーザ入力なし)
#
#	python-path	Python binary のパス
#			buildtool の python を使うときは python が
#			パスに入っていない場合があるから
#
#  DESCRIPTION:
#	Springhead のビルド環境をチェックする。
#	step 1:		Python のバージョンをチェックする。
#	step 2:		セットアップファイルが存在するならば、
#			その内容の整合性を検査する。
#			-c オプションが指定されていたらここで終わり。
#	step 3:		必要なバイナリのパスの現状を調べる。
#	step 4:		次の何れかの場合は次の step へ進む。
#			さもなければこれで終了。
#	　		　・セットアップファイルがない
#	　		　・ファイルに必要な情報が揃っていない
#	　		　・ファイルの内容と現在の状況に相違がある
#	　		　・'-f'オプションが指定されている
#	step 5:		処理継続の確認をする (-f/-F オプション無指定時)
#	step 6:		ビルドパラメータを設定する。
#	step 7:		パス情報を設定する。
#	step 8:		swigをmakeする。
#	step 9:		セットアップファイルを(再)作成する。
#
#  -c オプションが指定されたときの終了コード:
#	     0:		セットアップファイルに記録されたプログラムが
#			すべて起動可能である。
#	    -1:		起動できないプログラムがある。
#	    -2:		セットアップファイルが存在しない。
# ----------------------------------------------------------------------
#  VERSION:
#     Ver 1.00   2020/12/14 F.Kanehori	初版
#     Ver 1.00.1 2021/01/06 F.Kanehori	微修正
# ======================================================================
version = "1.00.1"

import sys
import os
import platform
import shutil
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Identify myself.
#
ScriptFileDir = os.sep.join(os.path.abspath(__file__).split(os.sep)[:-1])
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
progpath = sys.argv[0]

# ----------------------------------------------------------------------
#  Import local libaries
#
os.chdir(ScriptFileDir)
if not os.path.exists('./RunSwig/pythonlib'):
	print('%s: can\'t find "pythonlib" in current path' % prog)
	print('%s: please invoke this at ".../core/src"' % prog)
	sys.exit(1)
sys.path.append('./RunSwig/pythonlib')
from TextFio import *
from SetupFile import *
from Error import *
from Util import *

# ----------------------------------------------------------------------
#  Constants
#
required_windows = [ 'devenv', 'nmake', 'swig', 'cmake', 'nkf' ]
required_unix = [ 'gcc', 'swig', 'cmake', 'gmake', 'nkf' ]
required = required_unix if Util.is_unix() else required_windows
optional_tools = [ 'cmake' ]

vs_path_interface = '__vs_path_interface__'
CONF = 'Release'

# ----------------------------------------------------------------------
#  Globals
#
##setup_file = './setup.conf'
progs = {}
paths = {}
versions = {}
parms = {}
is_windows = Util.is_windows()
is_unix = Util.is_unix()
which = 'which' if is_unix else 'where'
U = Util()
E = Error(prog)

# ----------------------------------------------------------------------
#  Helper methods
# ----------------------------------------------------------------------
def has_nkf():
	return get_path('nkf')
def get_path(name):
	return progs[name] if name in progs else None

version_save = version
from setup_helpers import *
version = version_save

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options] python-path'
parser = OptionParser(usage = usage)
#
parser.add_option('-c', '--check', dest='check',
			action='store_true', default=False,
			help='check setup-file contents')
parser.add_option('-d', '--devenv-number', dest='devenv_number', metavar='number',
			action='store', type='int', default=None,
			help='devenv selection number (Windows only)')
parser.add_option('-f', '--force', dest='force',
			action='store_true', default=False,
			help='force rewrite setup-file')
parser.add_option('-F', '--Force', dest='Force',
			action='store_true', default=False,
			help='force rewrite setup-file')
parser.add_option('-o', '--old-version', dest='old_version',
			action='store_true', default=False,
			help='invoke python version 2.7')
parser.add_option('-s', '--setup-file', dest='setup_file',
			action='store', default='setup.conf',
			metavar='FILE',
			help='setup file name (defailt: %default)')
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
if len(args) != 1:
	E.error('incorrect number of arguments')
	print_usage()
python_path = args[0]
#
check = options.check
devenv_number = options.devenv_number
force = options.force
Force = options.Force
setup_file = options.setup_file
verbose = options.verbose
#
if verbose:
	print('python: "%s"' % U.upath(python_path))
	if check: print('%s: option -c' % prog)
	if devenv_number != 0:
		print('%s: devenv number: %s' % (prog, devenv_number))
	if force: print('%s: option -f' % prog)
	if Force: print('%s: option -F' % prog)
	print('%s: setup file: %s' % (prog, U.upath(setup_file)))
if Force:
	force = True

# ----------------------------------------------------------------------
#  step 1
#	Python のバージョンをチェックする。
#	python のメジャーバージョンは 3 以上でなければならない。
#
sys.stdout.write('-- checking python ... ')
(major, minor, micro, release, serial) = sys.version_info
if major < 3:
	abort('python version 3 or greater is required.')
out, ver = try_find(which, 'python', os.path.abspath(python_path))
if out == SetupFile.NOTFOUND:
	abort("pan: can't find python's path.")
print('found (version %s)' % ver)
progs['python'] = U.pathconv(out.strip())
versions['python'] = ver

# ----------------------------------------------------------------------
#  step 2
#	セットアップファイルが存在するならばその内容の整合性を検査する。
#
prog_registered = {}
vers_registered = {}
prog_scanned = {}
vers_scanned = {}
progs_lacking = []

if os.path.exists(setup_file):
	print()
	print('setup file ("%s") exists.' % U.upath(setup_file))

	# ファイル内容の整合性の検査
	sf = SetupFile(setup_file, verbose=verbose, register=True)
	keys_prog = sf.get_keys(sf.PROG)
	keys_path = sf.get_keys(sf.PATH)

	for prog in required:
		sys.stdout.write('-- checking path for %s ... ' % prog)
		if not prog in keys_prog:
			prog_registered[prog] = None
			vers_registered[prog] = SetupFile.NOTFOUND
			print('%s' % SetupFile.NOTFOUND)
			progs_lacking.append(prog)
			continue
		path = sf.get_prog(prog)
		prog_registered[prog] = path
		if path == SetupFile.NOTFOUND:
			print('%s' % path)
			vers_registered[prog] = 'n/a'
			progs_lacking.append(prog)
			continue
		#
		if prog == 'devenv':
			vsinfo = vswhere()
			if vsinfo is None:
				ver = None
			else:
				info = identify_vsinfo(vsinfo, path)
				out = info['productPath']
				ver = info['installVers']
				os.environ[vs_path_interface] = out
		else:
			out, ver = try_find(which, prog, path, devenv_number)
		if ver is None:
			out = prog
			ver = SetupFile.NOTFOUND
		vers_registered[prog] = ver
		if ver != SetupFile.NOTFOUND:
			print('OK (version %s)' % ver)
		else:
			print('%s (%s)' % (SetupFile.NOTFOUND, path))
			progs_lacking.append(prog)
else:
	print()
	print('setup file ("%s") not exists.' % U.upath(setup_file))

if check:
	# ファイル内容の表示
	print('progs recorded in the file are ...')
	for prog in keys_prog:
		print('    %s\t%s' % (prog, Util.upath(sf.get_prog(prog))))
	print('paths recorded in the file are ...')
	for key in keys_path:
		print('    %s\t%s' % (key, Util.upath(sf.get_path(key))))

	# -c オプション指定時はここまで
	print('check done')
	print()

	#
	if len(progs_lacking) > 0:
		sys.exit(-1)	# 起動できないプログラムがある
	if not os.path.exists(setup_file):
		sys.exit(-2)	# セットアップファイルがない
	sys.exit(0)

# ----------------------------------------------------------------------
#  step 3
#	必要なバイナリのパスの現状を調べる。
#
print()
print('currently available binaries are ...')
for prog in required:
	sys.stdout.write('-- checking %s ... ' % prog)
	out, ver = try_find(which, prog, None, devenv_number)
	if out == SetupFile.NOTFOUND:
		print('NOT FOUND')
		prog_scanned[prog] = SetupFile.NOTFOUND
		vers_scanned[prog] = ''
		continue
	print('found (version: %s)' % ver)
	path = os.path.abspath(U.upath(out.strip()))
	prog_scanned[prog] = path
	vers_scanned[prog] = ver

# ----------------------------------------------------------------------
#  step 4
#	次の何れかの場合は次の step へ進む。さもなければこれで終了。
#
setup_needed = False
setup_recommended = False
setup_cant_go_on = False
setup_reason_need = []
setup_reason_recm = []
setup_reason_fail = []

#  セットアップファイルがない
if not os.path.exists(setup_file):
	setup_needed = True
	setup_reason_need.append('setup file "%s" not found' % setup_file)

#  ファイルに必要な情報が揃っていない
if not setup_needed:
	print()
	print('information')
	for prog in required:
		# ファイルに情報があり、かつ実行できる
		if prog in prog_registered.keys() and \
			   prog_registered[prog] != SetupFile.NOTFOUND:
			continue

		# ファイル情報の有無に関係なく、実行ができない
		if prog_scanned[prog] == SetupFile.NOTFOUND:
			if prog in optional_tools:
				continue
			if prog == 'nmake':
				if prog_scanned['swig'] != SetupFile.NOTFOUND:
					# swigがあればnamkeは不要
					continue
				bin = 'swig'
			else:
				bin = 'Springhead'
			msg = "'%s' is required to build %s but not found" % (prog, bin)
			setup_cant_go_on = True
			setup_reason_fail.append(msg)
			continue

		# ファイルに情報はないが、実行できる
		msg = 'setup info (%s) is not sufficient' % prog
		if prog in optional_tools:
			setup_recommended = True
			setup_reason_recm.append(msg)
			if prog == 'cmake':
				msg = '-- cmake: not required but strongly recommended'
			if prog == 'nkf':
				msg = '-- nkf: may help in case "devenv" is not found'
			print(msg)
		else:
			setup_needed = True
			setup_reason_need.append(msg)
			print('-- %s: required to build Springhead' % prog)
			continue
	
#  ファイルの内容と現在の状況に相違がある
if not setup_needed:
	print()
	print('differences between setup file and currently availables are ...')
	for prog in required:
		path_regd = prog_registered[prog]
		path_scan = prog_scanned[prog]
		vers_regd = vers_registered[prog]
		vers_scan = vers_scanned[prog]
		if path_regd is None and path_scan is not None:
			msg = '"%s" is now available' % prog
			if prog in optional_tools:
				setup_recommended = True
				setup_reason_recm.append(msg)
			else:
				setup_needed = True
				setup_reason_need.append(msg)

		elif path_regd is not None and path_scan is None:
			msg = '"%s" not found on this system' % prog
			if prog in optional_tools:
				setup_needed = True
				setup_reason_need.append(msg)
			else:
				setup_cant_go_on = True
				setup_reason_fail.append(msg)

		elif path_regd != path_scan:
			msg = '"%s" path differs' % prog
			if prog in optional_tools:
				setup_recommended = True
				setup_reason_recm.append(msg)
			else:
				setup_needed = True
				setup_reason_need.append(msg)
			if path_regd != SetupFile.NOTFOUND:
				path_regd += ' (%s)' % vers_regd
			if path_scan != SetupFile.NOTFOUND:
				path_scan += ' (%s)' % vers_scan
			path_regd = path_regd.replace('()', '(n/a)')
			path_scan = path_scan.replace('()', '(n/a)')
			print('-- %s:' % prog)
			print('     in setup file ... %s' % path_regd)
			print('     now available ... %s' % path_scan)
			continue
		if vers_regd != vers_scan:
			print('-- %s:' % prog)
			path_regd = '%s (%s)' % (path_regd, vers_regd)
			path_scan = '%s (%s)' % (path_scan, vers_scan)
			path_regd = path_regd.replace('()', '(n/a)')
			path_scan = path_scan.replace('()', '(n/a)')
			print('     in setup file ... %s' % path_regd)
			print('     now available ... %s' % path_scan)

#  '-f/-F'オプションが指定されている
if force:	# -F sets -f implicitly
	setup_needed = True
	if Force:
		msg = 'force option \'-F\' specified'
	else:
		msg = 'force option \'-f\' specified'
	setup_reason_need = [msg]

#  判定結果の提示
print()
print('check result is ...')
if setup_cant_go_on:
	reason = set(setup_reason_fail)
	print('-- setup failed (reason: %s).' % ', '.join(reason))
elif setup_needed:
	reason = set(setup_reason_need)
	print('-- setup is required (reason: %s).' % ', '.join(reason))
elif setup_recommended:
	reason = set(setup_reason_recm)
	print('-- setup is recommended (reason: %s).' % ', '.join(reason))
else:
	print('-- no need to execute \'setup\'.')
	
# ----------------------------------------------------------------------
#  step 5
#	処理を継続するか確認する (-f オプション無指定時)
#
if setup_cant_go_on:
	E.abort('aborted')
if not force and not setup_needed and not setup_recommended:
	print('done')
	sys.exit(0)
if not force:
	print()
	yn = input('continue? [y/n]: ')
	if yn != 'y' and yn != 'Y':
		print('done')
		sys.exit(0)

# ----------------------------------------------------------------------
#  step 6
#	ビルドパラメータを設定する
#
parms['plat'] = 'x%s' % platform.architecture()[0].replace('bit', '')
parms['conf'] = 'Release'
if is_windows:
	parms['vers'] = vers_scanned['devenv'].split('.')[0] + '.0'

# ----------------------------------------------------------------------
#  step 7
#	パス情報を設定する。
print()
print('-- setting path information --')

#  Visual Studio installation path
if is_windows:
	vsinfo = vswhere()
	info = identify_vsinfo(vsinfo, prog_scanned['devenv'])
	vs_install_path = info['installPath']
	paths['VSinstall'] = vs_install_path
	print('-- Visual Studio install path: %s' % vs_install_path)

cwd = os.getcwd().split(os.sep)[::-1]
top = None
for n in range(len(cwd)):
	if not cwd[n].startswith('core'): continue
	top = '/'.join(cwd[::-1][0:len(cwd)-n-1])
	break
if top is None:
	#  冒頭でチェックしているからここには来ないはず
	Error(prog).abort('can\'t find "core" in current path')
sprtop = Util.pathconv(top)
paths['sprtop'] = sprtop
paths['sprcore'] = Util.pathconv('%s/core' % sprtop)
paths['sprsrc'] = Util.pathconv('%s/core/src' % sprtop)
paths['sprlib'] = Util.pathconv('%s/core/src/RunSwig/pythonlib' % sprtop)
print('-- Springhead top directory: %s' % sprtop)

# ----------------------------------------------------------------------
#  step 8
#	swigをmakeする。
#
print()
cmake_path = Util.upath(prog_scanned['cmake'])
if cmake_path != 'NOT FOUND':
	print('-- making swig ... using "%s"' % cmake_path)
else:
	print('-- making swig')

#  make 環境を設定する
env_set = False

#  make する
cwd = os.getcwd()
if is_unix:
	os.chdir('../bin/src/swig')
	stat = make_swig_unix(Force)
	if stat == 0:
		shutil.copy('./swig', '../../swig')
else:
	env = os.environ['PATH']
	os.environ['PATH'] = '%s;%s' % (prog_scanned['devenv'], env)
	os.chdir('../bin/src/swig/msvc')
	cmake = prog_scanned['cmake']
	devenv = prog_scanned['devenv']
	plat = parms['plat']
	conf = parms['conf']
	vers = parms['vers']
	stat = make_swig_windows(cmake, devenv, plat, conf, vers, Force)
os.chdir(cwd)
if stat == 0:
	print('OK')
else:
	sys.exit(stat)

#  生成した swig のパスを登録する
out, ver = try_find(which, 'swig')
if out == SetupFile.NOTFOUND:
	prog_scanned['swig'] = SetupFile.NOTFOUND
	vers_scanned['swig'] = ''
else:
	path = os.path.abspath(out)
	prog_scanned['swig'] = path
	vers_scanned['swig'] = ver

# ----------------------------------------------------------------------
#  step 9
#	セットアップファイルを(再)作成する。
#
print()
print('-- (re)generating setup file ...')
if verbose:
	print('\nprogram paths')
	for prog in prog_scanned:
		print('  %s: %s%s%s' % (prog, prog_scanned[prog], os.sep, prog))
	print('\nother path infos')
	for key in paths:
		print('  %s: %s' % (key, paths[key]))
	print('\nbuild parameters')
	for key in parms.keys():
		print('  %s: %s' % (key, parms[key]))
	print()

#  ファイルへの書き出し
lines = []
lines.append('# %s' % setup_file)
lines.append('')
lines.append('[prog]')
lines.append('python\t%s' % progs['python'])
for prog in required:
	if prog_scanned[prog] == '':
		continue
	lines.append('%s\t%s' % (prog, prog_scanned[prog]))
lines.append('')
lines.append('[path]')
for key in paths:
	lines.append('%s\t%s' % (key, paths[key]))
lines.append('')
lines.append('[data]')
for key in parms.keys():
	lines.append('%s\t%s' % (key, parms[key]))

#  移行期間中は次の変数も設定しておく
#	移行期間が終了したら削除すること
lines.append('')
lines.append('MIGRATION_TEST\ttrue')

fio = TextFio(setup_file, 'w')
if fio.open() < 0:
	E.abort('can not open "%s" for writing' % setup_file)
if fio.writelines(lines) < 0:
	E.abort('write error on "%s"' % setup_file)
fio.close()

#  ファイル内容の表示
#
print()
sf = SetupFile(setup_file)
sf.show()
print()
print('written to "%s"' % Util.upath(os.path.abspath(setup_file)))

#  終了
#
print('done')
sys.exit(0)

# end: setup.py

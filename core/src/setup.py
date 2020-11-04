#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	python setup.py [-f (--force)]
#	options:
#	    -f:		無条件に再セットアップを実行する。
#	    -c:		セットアップファイルの検査のみを実行する。
#
#  DESCRIPTION:
#	Springhead のビルド環境をチェックする。
#	step 1:		Python のバージョンをチェックする。
#	step 2:		セットアップファイルが存在するならば、
#			その内容の整合性を検査する。
#	step 3:		必要なバイナリのパスの現状を調べる。
#	step 4:		次の何れかの場合は次の step へ進む。
#			さもなければこれで終了。
#	　		　・セットアップファイルがない
#	　		　・ファイルに必要な情報が揃っていない
#	　		　・ファイルの内容と現在の状況に相違がある
#	　		　・'-f'オプションが指定されている
#	step 5:		処理継続の確認をする (-f オプション無指定時)
#	step 6:		swigをmakeする。
#	step 7:		セットアップファイルを(再)作成する。
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2020/11/04 F.Kanehori	First version.
# ======================================================================
version = 1.0

import sys
import os
import platform
import datetime
import shutil
from optparse import OptionParser

sys.path.append('./RunSwig/pythonlib')
from TextFio import *
from SetupFile import *
from Error import *
from Util import *

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
progpath = sys.argv[0]

required_windows = [ 'nmake', 'devenv', 'swig', 'cmake', 'nkf' ]
required_unix = [ 'gcc', 'gmake', 'swig', 'cmake' ]
required = required_unix if Util.is_unix() else required_windows
optional_tools = [ 'cmake', 'nkf' ]

PLAT = 'x64'
CONF = 'Release'
VERS = '15.0'		# replaced later

# ----------------------------------------------------------------------
#  Globals
#
setup_file = './setup.conf'
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
	return paths[name] if name in paths else None

from setup_helpers import *

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options] texmain'
parser = OptionParser(usage = usage)
#
parser.add_option('-c', '--check', dest='check',
			action='store_true', default=False,
			help='check setup-file contents')
parser.add_option('-f', '--force', dest='force',
			action='store_true', default=False,
			help='force rewrite setup-file')
parser.add_option('-o', '--old-version', dest='old_version',
			action='store_true', default=False,
			help='invoke python version 2.7')
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
check = options.check
force = options.force
verbose = options.verbose
#
if len(args) != 0:
	E.error('incorrect number of arguments')
	print_usage()
#
if verbose:
	if check: print('option -c')
	if force: print('option -f')

# ----------------------------------------------------------------------
#  step 1
#	Python のバージョンをチェックする。
#	python のメジャーバージョンは 3 以上でなければならない。
#
sys.stdout.write('-- checking python ... ')
(major, minor, micro, release, serial) = sys.version_info
if major < 3:
	abort('python version 3 or greater is required.')
out, ver = try_find(which, 'python')
if out == not_found():
	abort("pan: can't find python's path.")
print('found (version %s)' % ver)
out = '/'.join(U.upath(out.strip()).split('/')[:-1])
paths['python'] = out
versions['python'] = ver

# ----------------------------------------------------------------------
#  step 2
#	セットアップファイルが存在するならばその内容の整合性を検査する。
#
path_registered = {}
vers_registered = {}
path_scanned = {}
vers_scanned = {}
progs_lacking = []

if os.path.exists(setup_file):
	print()
	print('setup file ("%s") exists.' % U.upath(setup_file))

	# ファイル内容の整合性の検査
	print('paths recoarded in the file are ...')
	sf = SetupFile(setup_file, verbose=verbose, register=True)
	keys_path = sf.get_keys(sf.PATH)
	keys_data = sf.get_keys(sf.DATA)
	for prog in required:
		sys.stdout.write('-- checking path for %s ... ' % prog)
		if not prog in keys_path:
			path_registered[prog] = None
			vers_registered[prog] = not_found()
			print('%s' % not_found())
			progs_lacking.append(prog)
			continue
		path = sf.get_path(prog)
		path_registered[prog] = path
		if path == not_found():
			print('%s' % path)
			vers_registered[prog] = 'n/a'
			progs_lacking.append(prog)
			continue
		#
		if prog == 'devenv':
			out = path
			ver = get_vs_version(path)
		else:
			out, ver = try_find(which, prog, path)
		if ver is None:
			out = prog
			ver = not_found()
		vers_registered[prog] = ver
		if ver != not_found():
			print('OK (version %s)' % ver)
		else:
			print('%s (%s/%s)' % (not_found(), path, prog))
			progs_lacking.append(prog)
else:
	print()
	print('setup file ("%s") not exists.' % U.upath(setup_file))

# ----------------------------------------------------------------------
#  step 3
#	必要なバイナリのパスの現状を調べる。
#
print()
print('currently available binaries are ...')
for prog in required:
	sys.stdout.write('-- checking %s ... ' % prog)
	out, ver = try_find(which, prog)
	if out == not_found():
		print('NOT FOUND')
		path_scanned[prog] = not_found()
		vers_scanned[prog] = ''
		continue
	print('found (version: %s)' % ver)
	# directory部分だけにする
	path = '/'.join(U.upath(out.strip()).split('/')[:-1])
	path = os.path.abspath(path)
	path_scanned[prog] = path
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
		if prog in path_registered.keys() and \
			   path_registered[prog] != not_found():
			continue
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
		path_regd = path_registered[prog]
		path_scan = path_scanned[prog]
		vers_regd = vers_registered[prog]
		vers_scan = vers_scanned[prog]
		if path_regd is None and path_scan is not None:
			msg = '%s is now available' % prog
			if prog in optional_tools:
				setup_recommended = True
				setup_reason_recm.append(msg)
			else:
				setup_needed = True
				setup_reason_need.append(msg)

		elif path_regd is not None and path_scan is None:
			msg = '%s not found on this system' % prog
			if prog in optional_tools:
				setup_needed = True
				setup_reason_need.append(msg)
			else:
				setup_cant_go_on = True
				setup_reason_fail.append(msg)

		elif path_regd != path_scan:
			msg = '%s: path differs' % prog
			if prog in optional_tools:
				setup_recommended = True
				setup_reason_recm.append(msg)
			else:
				setup_needed = True
				setup_reason_need.append(msg)
			if path_regd != not_found():
				path_regd += ' (%s)' % vers_regd
			if path_scan != not_found():
				path_scan += ' (%s)' % vers_scan
			print('-- %s:' % prog)
			print('     in setup file ... %s' % path_regd)
			print('     now available ... %s' % path_scan)
			continue
		if vers_regd != vers_scan:
			print('-- %s:' % prog)
			path_regd = '%s (%s)' % (path_regd, vers_regd)
			path_scan = '%s (%s)' % (path_scan, vers_scan)
			print('     in setup file ... %s' % path_regd)
			print('     now available ... %s' % path_scan)

#  '-f'オプションが指定されている
if force:
	setup_needed = True
	msg = 'force option \'-f\' specified'
	setup_reason_need = [msg]

#  判定結果の提示
print()
print('check result is ...')
if setup_cant_go_on:
	reason = set(setup_reason_fail)
	print('-- setup failed (reason: %s)' % ', '.join(reason))
elif setup_needed:
	reason = set(setup_reason_need)
	print('-- setup is required (reason: %s)' % ', '.join(reason))
elif setup_recommended:
	reason = set(setup_reason_recm)
	print('-- setup is recommended (reason: %s)' % ', '.join(reason))
elif force:
	print('-- no need to setup, but force option \'-f\' specified')
else:
	print('-- no need to execute \'setup\'')
	
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
#	swigをmakeする。
#
print()
print('-- making swig ... ')

#  make 環境を設定する
sf = SetupFile(setup_file, verbose=verbose)
sf.add_environment()

#  make する
cwd = os.getcwd()
if is_unix:
	os.chdir('../bin/src/swig')
	stat = make_swig_unix()
	if stat == 0:
		shutil.copy('./swig', '../../swig')
else:
	os.chdir('../bin/src/swig/msvc')
	path = sf.get_path('cmake')
	plat = sf.get_data('plat')
	conf = sf.get_data('conf')
	vers = sf.get_data('vers')
	stat = make_swig_windows(path, plat, conf, vers)
os.chdir(cwd)
print('OK' if stat == 0 else 'failed')

#  生成した swig のパスを登録する
out, ver = try_find(which, 'swig')
if out == not_found():
	#print('NOT FOUND')
	path_scanned[prog] = not_found()
	vers_scanned[prog] = ''
#print('found (version: %s)' % ver)
# directory部分だけにする
path = '/'.join(U.upath(out.strip()).split('/')[:-1])
path = os.path.abspath(path)
path_scanned['swig'] = path
vers_scanned['swig'] = ver


# ----------------------------------------------------------------------
#  step 7
#	セットアップファイルを(再)作成する。
#
print()
print('-- (re)generating setup file ...')

#  ビルドパラメータの設定
parms['plat'] = 'x%s' % platform.architecture()[0].replace('bit', '')
parms['conf'] = 'Release'
if is_windows:
	parms['vers'] = vers_scanned['devenv'].split('.')[0] + '.0'
if verbose:
	print('\nprogram paths')
	for prog in path_scanned:
		print('  %s: %s' % (prog, path_scanned[prog]))
	print('\nbuild parameters')
	for key in parms.keys():
		print('  %s: %s' % (key, parms[key]))
	print()

#  ファイルへの書き出し
lines = []
lines.append('# %s' % setup_file)
lines.append('')
lines.append('[path]')
lines.append('python\t%s' % paths['python'])
for prog in required:
	if path_scanned[prog] == '':
		continue
	lines.append('%s\t%s' % (prog, path_scanned[prog]))
lines.append('')
lines.append('[data]')
for key in parms.keys():
	lines.append('%s\t%s' % (key, parms[key]))

fio = TextFio(setup_file, 'w')
if fio.open() < 0:
	E.abort('can not open "%s" for writing' % setup_file)
if fio.writelines(lines) < 0:
	E.abort('write error on "%s"' % setup_file)
fio.close()

#  終了
#
print('done')
sys.exit(0)

# end: setup.py

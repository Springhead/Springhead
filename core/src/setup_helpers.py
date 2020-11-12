#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  Helper functions for setup.
#
# ----------------------------------------------------------------------
#  VERSION:
#     Ver 1.00  2020/11/11 F.Kanehori	First version.
# ======================================================================
version = '1.00'

import sys
import os
import re
import subprocess
import shutil

sys.path.append('./RunSwig/pythonlib')
from Proc import *
from Util import *
from Error import *

# ----------------------------------------------------------------------
#  擬似定数
#
def not_found():
	return 'NOT FOUND'

# ----------------------------------------------------------------------
#  Globals
#
caller = 'setup'
is_unix = Util.is_unix()

# ----------------------------------------------------------------------
#  デバッグ用の定義
#
exec_devenv = False		# バージョン情報が取得できない!

# ----------------------------------------------------------------------
#  コマンドを実行してその出力を得る
#
def execute(cmnd, timeout=None, stdout=Proc.PIPE):
	# execute command
	proc = Proc().execute(cmnd, stdout=stdout,	#stdout=Proc.PIPE,
				    stderr=Proc.NULL, shell=True)
	# get output
	if stdout == Proc.PIPE:
		status, out, err = proc.output(timeout)
	else:
		status = proc.wait(timeout=timeout)
		out = ''
	return status, out

# ----------------------------------------------------------------------
#  簡易grep
#
def match(lines, patt, first=False):
	matches = []
	for line in lines:
		m = re.search(patt, line)
		if m:
			matches.append(m.group(1))
	if matches == []:
		matches = None
	elif first:
		matches = matches[0]
	return matches

# ----------------------------------------------------------------------
#  個別のプログラムの処理
#
def try_find(which, prog, check_path=None):
	out, ver = not_found(), None
	if prog == 'python':
		out, ver = try_find_python(which)
	if prog == 'cmake':
		patt = r'cmake .+ ([\d\.]+$)'
		out, ver = try_find_common(which, prog, patt, True, check_path)
	if prog == 'devenv':
		out, ver = try_find_devenv(which)
	if prog == 'gcc':
		patt = r'gcc .+ ([\d\.]+$)'
		out, ver = try_find_common(which, prog, patt, True, check_path)
	if prog == 'gmake':
		patt = r'[mM]ake.* ([\d\.]+$)'
		out, ver = try_find_common(which, prog, patt, True, check_path)
	if prog == 'nkf':
		patt = r'.+Version ([\d\.]+ \(.+\))'
		out, ver = try_find_common(which, prog, patt, True, check_path)
	if prog == 'nmake':
		patt = r'.+Version ([\d\.]+)'
		out, ver = try_find_common(which, prog, patt, True, check_path)
	if prog == 'swig':
		patt = r'SWIG Version ([\d\.]+)'
		out, ver = try_find_swig(which, prog, patt)
	return out, ver

#  python
#
def try_find_python(which):
	cmnd = '%s python' % which
	stat, out = execute(cmnd)
	if stat != 0:
		return not_found(), None
	out = out.split('\n')[0]
	(major, minor, micro, release, serial) = sys.version_info
	return out, '%s.%s.%s' % (major, minor, micro)

#  common check
#
def try_find_common(which, prog, ver_patt, first=False, check_path=None):
	if check_path is None:
		cmnd = '%s %s' % (which, prog)
		stat, out = execute(cmnd)
		if stat != 0 or out is None:
			return not_found(), None
		out = out.split('\n')[0].strip()
	else:
		prog = '"%s/%s"' % (check_path, prog)
		out = check_path
	prog = '%s --version' % prog
	stat, ver_out = execute(prog)
	ver = match(ver_out.split('\n'), ver_patt, first)
	return out, ver

#  devenv
#
def try_find_devenv(which):
	cmnd = 'C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe'
	stat, out = execute(cmnd)
	if stat != 0:
		Error(caller).error('sorry, can\'t find "Installer/vswhere.exe"');
		return not_found(), None
	path_patt = r'productPath: (.+)'
	path = match(out.split('\n'), path_patt)
	if len(path) <= 0:
		return not_found(), None
	if len(path) == 1:
		path = path[0]
	else:
		print('found multiple "devenv"')
		print('     Please select which one to use')
		for n in range(len(path)):
			path[n] = upath(path[n].strip())
			tmp_path = '/'.join(path[n].split('/')[:-1])
			vers = get_vs_version(tmp_path)
			print('\t(%d) %s (%s)' % (n+1, path[n], vers))
		while True:
			try:
				n = int(input('     enter number: '))
			except:
				n = 0
			if 1 <= n and n <= len(path):
				break
			print('\tWrong number (should be 1..%d)' % len(path))
		path = path[n-1]
	devenv_path = path

	# バージョン情報の取得
	#   pathを設定
	ver = 'no info'
	if exec_devenv:
		path = path.replace('\\devenv.exe', '').strip()
		org_path = os.environ['PATH']
		os.environ['PATH'] = '%s;%s' % (path, org_path)
		#   directoryを移動
		cwd = os.getcwd()
		drive, path = path.split(':')
		os.chdir('%s:' % drive)
		os.chdir(upath(path))
		#   コマンドを実行
		cmnd = 'devenv.exe --help'
		stat = 1
		stat, out = execute(cmnd)
		#   もとに戻す
		os.chdir(cwd)
		os.environ['PATH'] = org_path
		#   結果を評価
		if stat == 0 and out is not None:
			path_patt = r'Microsoft Visual Studio .+ ([0-9\.]+)'
			ver = match(out.split('\n'), path_patt)
	else:
		path = upath(devenv_path.strip())
		path = '/'.join(path.split('/')[:-1])
		ver = get_vs_version(path)

	sys.stdout.write('\t')
	return devenv_path, ver

#  gcc
#
def try_find_gcc(which):
	out, ver = not_found(), None
	return out, ver

#  swig
#
def try_find_swig(which, prog, ver_patt):
	swig = 'swig' if is_unix else 'swig.exe'
	path = '../bin/swig/%s' % swig
	cmnd = Util.pathconv('%s -version' % path)
	stat, out = execute(cmnd)
	if stat == 0 and out is not None:
		ver = match(out.split('\n'), ver_patt, first=True)
	else:
		ver = None
	out = path if ver is not None else not_found()
	return out, ver

# ----------------------------------------------------------------------
#  Try to get Visual Studio version.
#	devenv.exe と同じディレクトリにある devenv.exe.config のうち、
#	newVersion="N.x.y.z" の N を読み出し、そのうちの最大のものが
#	Visual Studio のバージョンではないのかと...
#	これで良いのかは全く不明
#	とりあえず
#	    Visual Studio Community 2017
#	    Visual Studio Community 2019
#	ではうまく動くみたい
#
def get_vs_version(path):
	path = '%s/devenv.exe.config' % upath(path)
	patt = r'newVersion="([\d\.]+)"'
	matches = []
	with open(path) as f:
		for line in f:
			m = re.search(patt, line)
			if m:
				#print('-- match: %s' % m.group(1))
				matches.append(m.group(1))
	version = '0'
	for v in matches:
		if int(v.split('.')[0]) < 10:
			continue
		if v > version:
			version = v
	return version

# ----------------------------------------------------------------------
#  Make swig.
#
def make_swig_unix():
	cmnd = '/bin/sh buildswig.sh'
	stat, out = execute(cmnd, stdout=None)
	if stat != 0:
		Error(caller).error('make swig failed')
	return stat

def make_swig_windows(path, plat, conf, vers):
	VS = { '15.0':	'Visual Studio 15',
	       '16.0':	'Visual Studio 16' }
	if path is not None and path != not_found():
		print('-- generating solution file by cmake ...')
		print()
		cmake = '%s/cmake' % path
		build = 'build'
		if os.path.exists(build):
			shutil.rmtree(build)
		cmnd = '%s -B %s -A %s -G "%s"' % (cmake, build, plat, VS[vers])
		stat, out = execute(cmnd, stdout=None)
		if stat != 0:
			Error(caller).error('cmake configure failed')
		#
		print()
		print('-- building swig')
		print()
		os.chdir(build)
		solutionfile = 'swig.sln'
		logfile = '.\\log\\swig.build.log'
		errfile = '.\\log\\swig.error.log'
		cmnd = 'devenv %s /rebuild %s /log %s /out %s' % \
				(solutionfile, conf, logfile, errfile)
		stat, out = execute(cmnd, stdout=None)
		if stat != 0:
			Error(caller).error('swig build failed')
		else:
			os.chdir('../../../../swig')
			show_swig_entry(out)
	else:
		print('-- no cmake found')
		print()
		print('-- building swig')
		solutionfile = 'swig15.0.sln'
		logfile = '.\\log\\swig.build.log'
		errfile = '.\\log\\swig.error.log'
		cmnd = 'devenv %s /rebuild %s /log %s /out %s' % \
				(solutionfile, conf, logfile, errfile)
		stat, out = execute(cmnd, stdout=None)
		if stat != 0:
			Error(caller).error('swig build failed')
		else:
			os.chdir('../../../swig')
			show_swig_entry(out)
	return stat

# ----------------------------------------------------------------------
#  Show swig.exe's directory entry.
#
def show_swig_entry(lines):
	stat, out = execute('dir swig.exe')
	if stat == 0:
		print()
		lines = out.split('\n')
		for line in lines:
			if 'swig.exe' in line:
				print(line)
		print()

# ----------------------------------------------------------------------
#  Change path separators.
#
def upath(path):
	return path.replace(os.sep, '/')

# ----------------------------------------------------------------------
#  Show usage.
#
def print_usage():
	print()
	cmnd = 'python %s --help' % progpath
	subprocess.Popen(cmnd, shell=True).wait()
	sys.exit(1)

# end: setup_helpers.py

#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  Helper functions for setup.
#
# ----------------------------------------------------------------------
#  Version:
#     Ver 1.00   2020/12/03 F.Kanehori	First version.
#     Ver 1.00.1 2021/01/07 F.Kanehori	Bug fixed.
#     Ver 1.00.2 2021/01/15 F.Kanehori	Bug fixed.
#     Ver 1.01   2021/02/15 F.Kanehori	try_find_python() 修正
# ======================================================================
version = '1.01'

import sys
import os
import re
import subprocess
import shutil

sys.path.append('./RunSwig/pythonlib')
from Proc import *
from Util import *
from Error import *
from SetupFile import *

# ----------------------------------------------------------------------
#  ヘルパー関数間インターフェイス用定数
#
vs_path_interface = '__vs_path_interface__'

# ----------------------------------------------------------------------
#  Globals
#
caller = 'setup'
is_unix = Util.is_unix()

# ----------------------------------------------------------------------
#  コマンドを実行してその出力を得る
#
def execute(cmnd, timeout=None, stdout=Proc.PIPE, stderr=Proc.NULL):
	# execute command
	proc = Proc().execute(cmnd, stdout=stdout,
				    stderr=stderr, shell=True)
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
def match(lines, patt, first=False, flags=0):
	matches = []
	for line in lines:
		m = re.search(patt, line, flags)
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
def try_find(which, prog, check_path=None, devenv_number=None):
	out, ver = SetupFile.NOTFOUND, None
	if prog == 'python':
		out, ver = try_find_python(which, check_path)
	if prog == 'cmake':
		patt = r'cmake .+ ([\d\.]+$)'
		out, ver = try_find_common(which, prog, patt, True, check_path)
	if prog == 'devenv':
		out, ver = try_find_devenv(which, devenv_number)
	if prog == 'nmake':
		out, ver = try_find_nmake(which)
	if prog == 'gcc':
		patt = r'gcc .+ ([\d\.]+$)'
		out, ver = try_find_common(which, prog, patt, True, check_path)
	if prog == 'gmake':
		patt = r'[mM]ake.* ([\d\.]+$)'
		out, ver = try_find_common(which, prog, patt, True, check_path)
		out, ver = check_if_gmake(out, ver, patt, True)
	if prog == 'nkf':
		patt = r'.+Version ([\d\.]+ \(.+\))'
		out, ver = try_find_common(which, prog, patt, True, check_path)
	if prog == 'swig':
		patt = r'SWIG Version ([\d\.]+)'
		out, ver = try_find_swig(which, prog, patt)
	return out, ver

#  python
#
def try_find_python(which, python_path):
	if python_path is None:
		(major, minor, micro, release, serial) = sys.version_info
		stat, out = execute('which python')
		if out != 0:
			Error(caller).abort('python not found', prompt='Pan')
		python_path = os.path.abspath(out)
		return python_path, '%s.%s.%s' % (major, minor, micro)
	#
	cmnd = '%s --version' % python_path
	stat, out = execute(cmnd, stderr=Proc.STDOUT)
	if stat != 0 or out is None:
		return SetupFile.NOTFOUND, None
	ver_out = out.split('\n')[0].strip()
	ver_patt = r'Python[\s\t]+([\d\.]+$)'
	ver = match(ver_out.split('\n'), ver_patt, True)
	return out, ver

#  common check
#
def try_find_common(which, prog, ver_patt, first=False, check_path=None):
	if check_path is None:
		cmnd = '%s %s' % (which, prog)
		stat, out = execute(cmnd)
		if stat != 0 or out is None:
			return SetupFile.NOTFOUND, None
		out = out.split('\n')[0].strip()
	else:
		out = check_path
	prog = '%s --version' % prog
	stat, ver_out = execute(prog)
	if ver_out is None:
		stat, ver_out = execute(prog, stderr=Proc.STDOUT)
	ver = match(ver_out.split('\n'), ver_patt, first)
	return out, ver

#  devenv
#
def try_find_devenv(which, selection_number):
	print('selection_number: %s' % selection_number)
	vsinfo = vswhere()
	if vsinfo is None:
		return SetupFile.NOTFOUND, None
	#
	if len(vsinfo) == 1:
		# only one VS found
		path = vsinfo[0]['productPath']
		vers = vsinfo[0]['installVers']
		index = 0
	else:
		# multiple VS's found
		print('found multiple "devenv"')
		print('     Please select which one to use')
		for n in range(len(vsinfo)):
			path = vsinfo[n]['productPath']
			vers = vsinfo[n]['installVers']
			print('\t(%d) %s (%s)' % (n+1, path, vers))
		if selection_number != None:
			n = int(selection_number)
			if n < 1 or len(vsinfo) < n:
				msg = 'option \'-d\' gives wrong number %d ' % n \
				    + '(should be between 1 and %d).' % len(vsinfo)
				Error(caller).abort(msg)
			print('     selection forced to (%d) ... ' % n)
		else:
			while True:
				try:
					n = int(input('     enter number: '))
				except:
					n = 0
				if 1 <= n and n <= len(vsinfo):
					break
				print('\tWrong number (should be 1..%d)' % len(vsinfo))
		path = vsinfo[n-1]['productPath']
		vers = vsinfo[n-1]['installVers']
		index = n - 1
	#
	os.environ[vs_path_interface] = path
	sys.stdout.write('\t')
	return path, vers

def vswhere():
	cmnd = 'C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe'
	stat, out = execute(cmnd)
	if stat != 0:
		Error(caller).error('sorry, can\'t find "Installer/vswhere.exe"');
		return None
	#
	vsinfo = []
	vsinfo_ix = -1
	patt_instance_id = r'instanceId: (.+)'
	patt_install_path = r'installationPath: (.+)'
	patt_install_vers = r'installationVersion: (.+)'
	patt_product_path = r'productPath: (.+)'
	linesep_in = os.linesep in out
	newline_in = '\n' in out
	split_ch = os.linesep if linesep_in else '\n'
	for line in out.split(split_ch):
		result = re.match(patt_instance_id, line)
		if result:
			# instance id: new VS found
			vsinfo.append({})
			vsinfo_ix += 1
			continue
		result = re.match(patt_install_path, line)
		if result:
			# installation path (origin to fine nmake.exe)
			vsinfo[vsinfo_ix]['installPath'] = result.group(1)
			continue
		result = re.match(patt_install_vers, line)
		if result:
			# installation version (VS version)
			vsinfo[vsinfo_ix]['installVers'] = result.group(1)
			continue
		result = re.match(patt_product_path, line)
		if result:
			# product path (= devenv.exe path)
			vsinfo[vsinfo_ix]['productPath'] = result.group(1)
	return vsinfo

def identify_vsinfo(vsinfo, vs_path):
	for info in vsinfo:
		path = info['productPath'].replace('\\\\', '\\')
		if path == vs_path:
			return info
	# not in vsinfo
	return None

#  nmake
#
def try_find_nmake(which):
	vsinfo = vswhere()
	if vsinfo is None:
		return SetupFile.NOTFOUND, None
	#
	vs_path = os.getenv(vs_path_interface)
	info = identify_vsinfo(vsinfo, vs_path)
	if info is None:
		Error(caller).warn('can\'t get VS installation info')
		return SetupFile.NOTFOUND, None
	# 
	cmnd = 'where /R "%s" nmake.exe' % info['installPath']
	stat, out = execute(cmnd)
	if stat != 0:
		# not found
		return SetupFile.NOTFOUND, None
	is_x64 = sys.maxsize > 2**32
	if is_x64:
		# run on 64bit machine
		for line in out.split(os.linesep):
			if 'Hostx64\\x64' in line:
				break
	else:
		for line in out.split(os.linesep):
			if 'Hostx86\\x86' in line:
				break
	path = line
	#
	ver_patt = r'Version ([\d\.]+)'
	stat, ver_out = execute(path, stderr=Proc.STDOUT)
	if stat != 0:
		Error(caller).warn('can\'t get nmake version')
		ver = 'unknown'
	else:
		ver = match(ver_out.split(os.sep), ver_patt)
	return path, ver[0]

#  make
#
def check_if_gmake(path, ver, ver_patt, first):
	testfile = 'setup.test.Makefile'
	cmnd = '%s -f %s' % (path, testfile)
	status, out = execute(cmnd)
	if status == 0:
		return path, ver

	# try to find 'make' in the PATH.
	#
	env_path = os.getenv('PATH').split(os.pathsep)
	for p in env_path:
		cmnd = '/bin/ls %s/make' % p
		status, out = execute(cmnd, stdout=Proc.NULL)
		#print('TRY: %s -> %d' % (p, status))
		if status != 0:
			continue

		# test if GNU function works
		#	make -f setup.test.Makefile
		status, out = execute(cmnd, stdout=Proc.NULL)
		if status != 0:
			# this make does not work!
			continue
		# get version
		prog = '%s/make' % p
		cmnd = '%s --version' % prog
		stat, ver_out = execute(cmnd)
		if ver_out is None:
			stat, ver_out = execute(cmnd, stderr=Proc.STDOUT)
		ver = match(ver_out.split('\n'), ver_patt, first)
		return prog, ver

	# can not find suitable one
	#
	return SetupFile.NOTFOUND, None

#  gcc
#
def try_find_gcc(which):
	out, ver = SetupFile.NOTFOUND, None
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
	out = path if ver is not None else SetupFile.NOTFOUND
	return out, ver

# ----------------------------------------------------------------------
#  Make swig.
#
def make_swig_unix(clean):
	flag = ' -c' if clean else ''
	cmnd = '/bin/sh buildswig.sh %s' % flag
	stat, out = execute(cmnd, stdout=None)
	if stat != 0:
		Error(caller).error('make swig failed')
	return stat

def make_swig_windows(cmake, devenv, plat, conf, vers, clean):
	VS = { '15.0':	'Visual Studio 15 2017',
	       '16.0':	'Visual Studio 16 2019' }
	if cmake is not None and cmake != SetupFile.NOTFOUND:
		print('-- generating solution file by cmake ...')
		print()
		build = 'build'
		if clean and os.path.exists(build):
			print('-- removing ./%s' % build)
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
	else:
		print('-- no cmake found')
		print()
		print('-- building swig')
		solutionfile = 'swig15.0.sln'

	if os.path.exists(devenv.replace('.exe', '.com')):
		# devenv.com があればそちらを起動する
		#	build 過程が stdout に出力される
		devenv = devenv.replace('.exe', '.com')
		cmnd = '%s "%s" /build %s' % (devenv, solutionfile, conf)
	else:
		# devenv.com がこのディレクトリになければ devenv.exe を起動する
		#	build 過程はログファイルに出力する
		print('** devenv.com not found')
		print('** compile log is not shown here (see "./log/swig.build.log").')
		print()
		os.makedirs('./log', exist_ok=True)
		logfile = '.\\log\\swig.build.log'
		if os.path.exists(logfile):
			os.remove(logfile)
		config = '/rebuild' if clean else 'build'
		cmnd = '%s "%s" /rebuild %s /out %s' % \
			(devenv, solutionfile, conf, logfile)
	print(cmnd)
	stat = subprocess.Popen(cmnd).wait()
	if stat != 0:
		Error(caller).error('swig build failed')
	else:
		os.chdir('../../../swig')
		show_swig_dir_entry()
	return stat

# ----------------------------------------------------------------------
#  Show swig.exe's directory entry.
#
def show_swig_dir_entry():
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
	cmnd = 'python setup.py --help'
	subprocess.Popen(cmnd, shell=True).wait()
	sys.exit(1)

# end: setup_helpers.py

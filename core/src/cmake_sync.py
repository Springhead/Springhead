#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	python cmake_sync.py [options] spr_blddir app_blddir app_name projs
#	arguments:
#	    spr_blddir	"${SPR_SRC_DIR}/${OOS_BLD_DIR}"
#	    app_blddir	"${CMAKE_BINARY_DIR}"
#	    app_name	Application name (used in solution file name)
#	    projs	"${SPR_PROJS}"	(dependent projects cmake-list)
#
#  DESCRIPTION:
#	指定されたプロジェクトについて、Springhead 側と application 側
#	との間で同期をとる。
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.1  2019/06/24 F.Kanehori	New function implemented.
#	Ver 1.0  2019/06/10 F.Kanehori	First version.
# ======================================================================
version = 1.0

import sys
import os
import stat
import subprocess
import shutil
import re
from collections import deque
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
spr_prev_suffix = 'prev.spr'
app_prev_suffix = 'prev.app'

# ----------------------------------------------------------------------
#  Helper methods
#

#  ファイルのタイムスタンプを比較する
#	file1 の方が新しければ True を、さもなければ False を返す
#	タイムスタンプは stat.st_mtime を使用する
#
def is_newer_file(file1, file2):
	stat1 = os.stat(file1)
	stat2 = os.stat(file2)
	result = stat1.st_mtime > stat2.st_mtime
	if verbose > 1:
		msg = 'NEWER' if result else 'OLDER'
		print('  %s is %s than %s' % (file1, msg, file2))
	return result

#  makefile/projectfile を比較する
#	次の何れかに変更があれば True を、さもなければ False を返す
#	・platform 又は configuration の何れか
#	・依存するソースファイル
#
def is_config_changed(file1, file2):
	if is_unix():
		return is_config_changed_unix(file1, file2)
	else:
		return is_config_changed_win(file1, file2)

def is_config_changed_unix(file1, file2):
	#  未実装
	#
	return False

def is_config_changed_win(file1, file2):
	patterns = ['<ProjectConfiguration', '<ClCompile Include=']
	grep1 = grep(patterns, file1)
	grep2 = grep(patterns, file2)
	if grep1 is None or grep2 is None:
		return True
	if verbose == 0:
		return grep1 != grep2

	#  以下はテスト用
	#
	result = False
	try:
		str1 = grep1.popleft()
		str2 = grep2.popleft()
		while (str1 and str2):
			if str1 != str2:
				if verbose > 2:
					print('  < %s' % str1)
					print('  > %s' % str2)
				result = True
				break
			str1 = grep1.popleft()
			str2 = grep2.popleft()
	except:
		pass
	return result

#  ソリューションファイルに記載されたプロジェクトの GUID を調べる
#
def collect_sln_guid(blddir, projs, fname):
	cwd = os.getcwd()
	os.chdir(blddir)
	#
	guids = {}
	for proj in projs:
		pattern = ['Project\("{.+}"\) = "%s",.*, "{(.+)}"' % proj]
		guid = grep(pattern, fname, extract='match')
		guids[proj] = guid.popleft()
	if verbose > 1:
		print('collect_sln_guid: %s' % guids)
	#
	os.chdir(cwd)
	return guids

#  プロジェクトファイルの GUID を調べる
#
def collect_vcx_guid(blddir, projs, in_subdir=False):
	cwd = os.getcwd()
	os.chdir(blddir)
	#
	pattern = ['<ProjectGuid>{(.+)}</ProjectGuid>']
	guids = {}
	for proj in projs:
		if in_subdir:
			fname = '%s.vcxproj' % proj
		else:
			fname = '%s/%s.vcxproj' % (proj, proj)
		guid = grep(pattern, fname, extract='match')
		guids[proj] = guid.popleft()
	if verbose > 1:
		print('collect_vcx_guid: %s' % guids)
	#
	os.chdir(cwd)
	return guids

def print_guids(title, guids):
	print(title)
	projs = guids.keys()
	for proj in projs:
		print('%16s: %s' % (proj, guids[proj]))

#  簡易 grep
#	与えられた文字列配列の中から指定されたパターンを含む行を抜き出して返す
#	返すのは list ではなくて collections.deque (効率が良い)
#
def grep(patterns, fname, extract='line'):
	lines = read_file(fname)
	if lines is None:
		return None
	#
	greps = deque()
	for line in lines:
		match = False
		for patt in patterns:
			m = re.search(patt, line)
			if m:
				match = True
				break
		if match:
			if extract == 'line':
				greps.appendleft(line.strip())
			elif extract == 'match':
				greps.appendleft(m.group(1))
	return greps

#  ファイルを読み込む
#
def read_file(fname, encoding='utf-8'):
	try:
		f = open(fname, 'r', encoding=encoding)
	except Exception:
		print('Error: file open error: "%s"' % fname)
		return None
	try:
		lines = f.readlines()
	except Exception as e:
		print('Error: %s: read error: (%s)' % (fname, e))
		return None
	f.close()
	return lines

#  ファイルに書き込む
#
def write_file(fname, lines):
	try:
		f = open(fname, 'w', encoding='utf-8')
	except Exception:
		print('Error: file open error: "%s"' % fname)
		return None
	try:
		f.writelines(lines)
	except Exception as e:
		print('Error: %s: read error: (%s)' % (fname, e))
		return None
	f.close()
	return 0

#  ファイルを属性込みでコピーする
#
def copy_file(src, dst):
	if verbose > 1:
		print('  copy: %s -> %s' % (src, dst))
	try:
		shutil.copyfile(src, dst)
		shutil.copystat(src, dst)
	except shutil.SameFileError:
		pass
	except Exception as e:
		print(e)

#  ファイルを削除する
#
def remove(fname):
	if verbose > 1:
		print('  remove: %s' % fname)
	try:
		os.remove(fname)
	except:
		pass

#  ファイルのリンクを張る
#	unix の場合は symbolic link
#	Windows の場合は hard link (symbolic link は権限不足で利用できない)
#
def make_link(linkname, target):
	if verbose > 1:
		print('  link: %s -> %s' % (linkname, target))
	if is_unix():
		#  use symbolic link for unix
		cmnd = 'ln -s %s %s' % (upath(target), linkname)
	else:
		#  use hard link for Windows
		cmnd = 'mklink /h %s %s' % (linkname, dpath(target))
	if verbose > 2:
		print('  %% %s' % cmnd)
	proc = subprocess.Popen(cmnd, shell=True)
	stat = proc.wait()
	if stat != 0:
		fatal('link: failed (%d)' % stat)


#  利用環境(OS)の判定
#
def is_unix():
	return os.name == 'posix'
def is_windows():
	return os.name == 'nt'

#  パス表記の変換
#
def upath(path):
	return path.replace('\\', '/')
def dpath(path):
	return path.replace('/', '\\')

#  エラー時の扱い
#
def fatal(msg, exitcode=1):
	sys.stderr.write('Error: %s\n' % msg)
	sys.exit(exitcode)

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options] spr_blddir app_blddir projs'
parser = OptionParser(usage = usage)
#
parser.add_option('-a', '--plat', dest='plat',
			action='store', default=None,
			help='platform')
parser.add_option('-c', '--conf', dest='conf',
			action='store', default=None,
			help='configuration')
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
plat = options.plat
conf = options.conf
verbose = options.verbose
#
spr_blddir = args[0]
app_blddir = args[1]
app_name = args[2]
projs = args[3:]
if verbose:
	print('arguments:')
	print('  build plat: %s' % plat)
	print('        conf: %s' % conf)
	print('  spr_blddir: %s' % spr_blddir)
	print('  app_blddir: %s' % app_blddir)
	print('  projs: %s' % projs)
	print()
projs.append('RunSwig')

# ----------------------------------------------------------------------
#  メイン処理開始
#
cwd = os.getcwd()
need_spr_sln_modify = False

if is_windows():
	#  この時点におけるプロジェクトファイルの GUID を記録しておく
	#
	spr_org_guids = collect_vcx_guid(spr_blddir, projs)
	app_org_guids = collect_vcx_guid(app_blddir, projs)
	verbose = 1
	if verbose > 1:
		print_guids('Spr.ORG', spr_org_guids)
		print_guids('App.ORG', app_org_guids)

for proj in projs:
	os.chdir(proj)
	print('sync: %s' % proj)
	if verbose > 1:
		print('[%s]' % os.getcwd().replace(os.sep, '/'))

	stampfile = 'generate.stamp'
	if is_unix():
		projfile = 'Makefile'
	else:
		projfile = '%s.vcxproj' % proj
	spr_projdir = '%s/%s' % (spr_blddir, proj)
	app_projdir = '.'
	spr_stampfile = '%s/CMakeFiles/%s' % (spr_projdir, stampfile)
	app_stampfile = '%s/CMakeFiles/%s' % (app_projdir, stampfile)
	spr_dependfile = '%s.depend' % spr_stampfile
	app_dependfile = '%s.depend' % app_stampfile
	spr_projfile = '%s/%s' % (spr_projdir, projfile)
	app_projfile = '%s/%s' % (app_projdir, projfile)
	#  以下のファイルは app_prohdir に置く
	spr_stamp_prev = '%s.%s' % (app_stampfile, spr_prev_suffix)
	app_stamp_prev = '%s.%s' % (app_stampfile, app_prev_suffix)
	spr_proj_prev = '%s.%s' % (projfile, spr_prev_suffix)
	app_proj_prev = '%s.%s' % (projfile, app_prev_suffix)
	if verbose > 1:
		print('  spr_projfile:   %s (%s)' % (spr_projfile, spr_proj_prev))
		print('  spr_stampfile:  %s (%s)' % (spr_stampfile, spr_stamp_prev))
		print('  spr_dependfile: %s' % spr_dependfile)
		print('  app_projfile:   %s (%s)' % (app_projfile, app_proj_prev))
		print('  app_stampfile:  %s (%s)' % (app_stampfile, app_stamp_prev))
		print('  app_dependfile: %s' % app_dependfile)
	if verbose > 2:
		print('cwd: %s' % os.getcwd().replace(os.sep, '/'))

	#  もし spr_stampfile が存在しなければ…
	#	Spr 側が configure されていない
	#
	if not os.path.exists(spr_stampfile):
		#  Spr 側に App 側の状態をコピーする
		#
		print('  synchronize SPR to APP (library not configured)')
		os.makedirs('%s/CMakeFiles' % spr_projdir, exist_ok=True)
		copy_file(app_projfile, spr_projfile)
		copy_file(spr_projfile, spr_proj_prev)		####
		copy_file(app_stampfile, spr_stampfile)
		copy_file(app_dependfile, spr_dependfile)
		copy_file(app_stampfile, spr_stamp_prev)

	#  もし spr_stamp_prev が存在しなければ...
	#	App 側で初めての configure である
	#  Spr 側のスタンプの方が spr_stamp_prev より新しければ...
	#	Spr 側で cmake が行なわれた
	#	or 他のアプリが Spr 側のスタンプを更新した
	#
	elif not os.path.exists(spr_stamp_prev) \
	    or is_newer_file(spr_stampfile, spr_stamp_prev):
		#  App 側を Spr 側に同期させる
		#
		if not os.path.exists(spr_stamp_prev):
			print('  synchronize APP to SPR (no stamp file saved)')
		else:
			print('  synchronize APP to SPR (lib\'s stamp file newer)')
		copy_file(spr_projfile, app_projfile)		####
		copy_file(spr_projfile, spr_proj_prev)		####
		copy_file(spr_stampfile, spr_stamp_prev)
		copy_file(spr_stampfile, app_stampfile)
		copy_file(spr_dependfile, app_dependfile)

	#  もし今作ったプロジェクトファイルが Spr 側のものと異なる内容ならば...
	#	このアプリで変更を施した
	#
	elif is_config_changed(app_projfile, spr_projfile):
		#  Spr 側を App 側に同期させる
		#
		print('  synchronize SPR to APP (makefile/projfile changed)')
		copy_file(app_projfile, spr_projfile)
		copy_file(spr_projfile, spr_proj_prev)		####
		copy_file(app_stampfile, spr_stampfile)
		copy_file(app_dependfile, spr_dependfile)
		copy_file(app_stampfile, spr_stamp_prev)

		#  Spr 側の .sln の整合性を図る必要がある (GUID が変化している)
		#	ここではフラグを立てるだけ (あとでまとめて実行する)
		#
		need_spr_sln_modify = True

	#  さもなければ同期の必要はない
	#
	#else:
		#  App 側のスタンプを Spr 側に合わせておく
		#
		#print('  synchronize APP to SPR (project GUID changed)')
		#copy_file(spr_stampfile, spr_stamp_prev)
		#copy_file(spr_stampfile, app_stampfile)
		#copy_file(spr_dependfile, app_dependfile)

	#  このアプリの最新のスタンプファイルをセーブしておく
	#  makefile/projectfile の実体を Spr 側へのリンクに変更する
	#	毎回行なうのは無駄だが、cmake がファイルを作り変えてしまうので
	#	仕方がない
	#
	'''
	remove(app_projfile)
	make_link(projfile, spr_projfile)
	copy_file(app_stampfile, app_stamp_prev)
	'''
	if is_config_changed_win(spr_projfile, spr_proj_prev):
		copy_file(spr_projfile, projfile)
	else:
		shutil.copystat(spr_projfile, projfile)
	
	os.chdir(cwd)
# endfor

#  Windows (Visual Studio) の場合はソリューションファイルとプロジェクトファイルとの
#  整合性を保証する必要がある (プロジェクトの GUID が変更されていることがあるから)
#
if is_windows():
	#  この時点におけるプロジェクトファイルの GUID を記録しておく
	#
	spr_new_guids = collect_vcx_guid(spr_blddir, projs)
	app_new_guids = collect_vcx_guid(app_blddir, projs)
	if verbose > 1:
		print_guids('Spr.NEW', spr_new_guids)
		print_guids('App.NEW', app_new_guids)

	# (1) Spr 側のソリューションファイルの整合性を図る
	#	App 側で構成の変更を行なった場合
	#	(プロジェクトの GUID が変更されているため)
	#
	if need_spr_sln_modify:
		sln_fname = '%s/Springhead.sln' % spr_blddir
		spr_old_guids = collect_sln_guid(spr_blddir, projs, sln_fname)
		new_lines = []
		modified = False
		modified_info = {}
		for line in read_file(sln_fname):
			for proj in projs:
				str1 = spr_old_guids[proj]
				str2 = spr_new_guids[proj]
				changed = line.find(str1) and (str1 != str2)
				if changed:
					line = line.replace(str1, str2)
					modified = True
				if verbose and changed \
					   and not proj in modified_info.keys():
					fmt = '%s: %s -> %s (GUID changed)'
					modified_info[proj] = fmt % (proj, str1, str2)
			new_lines.append(line)
		#
		if modified:
			rc = write_file(sln_fname, new_lines)
			if rc == 0:
				print('solution file is modified (%s)' % upath(sln_fname))
				if verbose:
					for proj in modified_info.keys():
						print('  %s' % modified_info[proj])

	# (2) App 側のソリューションファイルの整合性を図る
	#	すべて Spr 側に合わせる
	#	
	sln_fname = '%s/%s.sln' % (app_blddir, app_name)
	new_lines = []
	modified = False
	for line in read_file(sln_fname):
		for proj in projs:
			str1 = app_org_guids[proj]
			str2 = app_new_guids[proj]
			if line.find(str1) >= 0:
				tmp_line = line.replace(str1, str2)
				if tmp_line != line:
					if verbose > 1:
						print('  ** %s' % tmp_line.strip())
					line = tmp_line
					modified = True
		new_lines.append(line)
	#
	if modified:
		rc = write_file(sln_fname, new_lines)
		if rc == 0:
			print('solution file is modified (%s)' % upath(sln_fname))
			print('******************************************************')
			print('** Push (R)-button to make solution file up-to-date **')
			print('******************************************************')
			print()

# endif is_windows()

sys.exit(0)

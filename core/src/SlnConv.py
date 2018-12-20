#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	python SlnConv.py [options] slnfile
#	options:
#	    -d version	変換して作成する Visual Studio のバージョン
#	    -f:		ファイルを上書きするとき確認をとらない
#	    -s version	変換する元となる Visual Studio のバージョン
#	arguments:
#	    slnfile	ソリューション名 or ソリューションファイル名
#
#  DESCRIPTION:
#	指定されたソリューションファイルを変換して新しいバージョンの
#	ソリューションファイル (Format Version 12.00) を生成する。
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2018/12/17 F.Kanehori	First version.
# ======================================================================
version = 1.0

import sys
import os
import re
import glob
import codecs
import subprocess
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
progpath = sys.argv[0]
PIPE = subprocess.PIPE
default_vs_version = '15.0.27703.0'
encoding_list = "'utf-8', 'utf-8-sig', 'cp932', etc."

# ----------------------------------------------------------------------
#  External tools.
#
devenv = 'devenv'

# ----------------------------------------------------------------------
#  Helper methods
# ----------------------------------------------------------------------

#  ソリューションファイル名からバージョンと拡張子を取り除く
#
def get_slnname(fname, version, suffix):
	s_len = len(suffix)
	if fname[-s_len:] == suffix:
		fname = fname[:-s_len]
	v_len = len(version)
	if fname[-v_len:] == version:
		fname = fname[:-v_len]
	return fname

#  ファイルの存在検査
#
def check_files(srcfile, dstfile, force):
	if not os.path.exists(srcfile):
		abort('No such file: "%s"' % srcfile)
	if not force and os.path.exists(dstfile):
		fmt = '%s: file "%s" exists. overwrite [y/n] ? '
		ans = input(fmt % (prog, dstfile))
		if ans.lower() != 'y':
			print('%s: aborted' % prog)
			sys.exit(1)
	return 0

#  ファイルの読み書き
#
def read_file(name, encoding):
	lines = []
	fobj = open(name, 'r', encoding=encoding)
	try:
		for line in fobj:
			lines.append(line)
	except:
		msg = "can't decode character. "
		msg += 'try another one (%s)' % encoding_list
		abort(msg)
	fobj.close()
	return lines

def write_file(name, lines, encoding):
	ofobj = open(name, 'w', encoding=encoding)
	for line in lines:
		try:
			ofobj.write(line)
		except:
			msg = "can't encode character. "
			msg += 'try another one (%s)' % encoding_list
			abort(msg)
	ofobj.close()

#  Visual Studio のベースを調べる
#	ベースとは、"Common7", "DIA SDK" があるディレクトリのこと
#
def get_vs_base_directory():
	cwd = os.getcwd()
	base = 'C:/Program Files (x86)/Microsoft Visual Studio/'
	os.chdir(base)
	year = 0
	for d in glob.glob('[0-9]*'):
		if int(d) > year:
			year = int(d)
	if year == 0:
		error('can not find "devenv.exe"')
		print('assume default VS version: %s' % default_vs_version)
		return default_vs_version
	os.chdir(str(year))
	vs_editions = glob.glob('*')
	if len(vs_editions) == 1:
		vs_edition = vs_editions[0]
	else:
		print('found followings')
		while True:
			n = 0
			for e in vs_editions:
				n += 1
				print('  %d: %s' % (n, e))
			rc = int(input('select => ')) - 1
			if 0 <= rc and rc < len(vs_editions):
				vs_edition = vs_editions[rc]
				break
			print('error: index out of range')
	os.chdir(cwd)
	return '%s/%d/%s' % (base, year, vs_edition)

#  Visual Studio のバージョンを調べる
#
def get_vs_version():
	# 'devenv /?' で調べる
	#
	devenv_dir = '%s/Common7/IDE' % get_vs_base_directory()
	#
	cmnd = '"%s/%s" /?' % (devenv_dir.replace('/', '\\'), devenv)
	status, out, err = command(cmnd)
	if status != 0:
		print('NG')
		abort('"%s" failed')
	outlist = out.replace('\r', '').split('\n')
	patt = r'([0-9]+.[0-9]+.[0-9]+.[0-9]+)'
	version = None
	for s in outlist:
		if s.find('Visual Studio') >= 0:
			m = re.search(patt, s)
			if m:
				version = m.group(1)
			break
	print('OK')
	print('using %s/devenv.exe' % devenv_dir)
	return version

#  toolset のバージョンを調べる
#
def get_toolset_version():
	# "DIA2Dump.vcxproj" を読んで PlatformToolsetVersion を調べる
	#
	DIA2Dump_dir = '%s/DIA SDK/Samples/DIA2Dump' % get_vs_base_directory()
	fname = '%s/DIA2Dump.vcxproj' % DIA2Dump_dir
	#
	patt = r'<PlatformToolset>(v[0-9]+)</PlatformToolset>'
	lines = read_file(fname, 'utf_8')	
	version = None
	for line in lines:
		m = re.search(patt, line)
		if m:
			version = m.group(1)
			break
	return version

#  コマンドを実行してその出力を得る
#
def command(cmnd, timeout=None):
	proc = subprocess.Popen(cmnd, stdout=PIPE, shell=True)
	try:
		out, err = proc.communicate(timeout=timeout)
		status = proc.returncode
	except subprocess.TimeoutExpired:
		proc.kill()
		out, err = proc.communicate()
		status = 1
	encoding = os.device_encoding(1)
	if encoding is None:
		encoding = 'cp932'
	out = out.decode(encoding) if out else None
	err = err.decode(encoding) if err else None
	return status, out, err

#  Change path separators.
#
def upath(path):
	return path.replace(os.sep, '/')

#  Error process.
#
def error(msg):
	sys.stderr.write('%s: Error: %s\n' % (prog, msg))
def abort(msg, exitcode=1):
	error(msg)
	sys.exit(exitcode)

#  Show usage.
#
def print_usage():
	print()
	cmnd = 'python %s --help' % progpath
	subprocess.Popen(cmnd, shell=True).wait()
	sys.exit(1)

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options] texmain'
parser = OptionParser(usage = usage)
#
parser.add_option('-d', '--dst-version', dest='dst_version',
			action='store', default='15.0', metavar='VER',
			help='destination VS version [default: %default]')
parser.add_option('-e', '--encoding', dest='encoding',
			action='store', default='utf-8-sig',
			help='solution/project file encoding [default: %default]')
parser.add_option('-f', '--force', dest='force',
			action='store_true', default=False,
			help='force overwrite even if file exists')
parser.add_option('-s', '--src-version', dest='src_version',
			action='store', default='14.0', metavar='VER',
			help='source VS version [default: %default]')
parser.add_option('-u', '--user-data', dest='user_data',
			action='store_true', default=False,
			help='convert user data file (.vcxproj.user)')
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
src_version = options.src_version
dst_version = options.dst_version
encoding = options.encoding
force = options.force
user_data = options.user_data
verbose = options.verbose
#
if len(args) != 1:
	error('incorrect number of arguments')
	print_usage()
#
slnfile = upath(args[0])
slnname = get_slnname(slnfile, src_version, '.sln')
src_slnfile = '%s%s.sln' % (slnname, src_version)
dst_slnfile = '%s%s.sln' % (slnname, dst_version)

sys.stdout.write('getting Visual Studio Version (take long, be patience) ... ')
sys.stdout.flush()
vs_version = get_vs_version()
vs_version = '.'.join(vs_version.split('.')[:-1]) + '.0'
pts_version = get_toolset_version()

print()
print('Version info:')
print("  Visual Studio: '%s'" % vs_version)
print("  Platform Toolset: '%s'" % pts_version)
print('Target File: %s -> %s (%s)' % (src_slnfile, dst_slnfile, encoding))
print()

# ----------------------------------------------------------------------
#  メイン処理開始
#

#  ソリューションファイルの存在チェック
#
check_files(src_slnfile, dst_slnfile, force)

#  指定されたソリューションファイルを読み、
#   (1) バージョン情報、プロジェクト情報を書き換える
#   (2) 関連するプロジェクトファイルをリストアップする
#
patt_vsv = r'VisualStudioVersion = ([0-9.]+)'
patt_prj = r'Project\(.*\) = ".*", "(.+)(%s).vcxproj", ".*"' % src_version
patt_nam = r'Project\(.*\) = "(.+)(%s)", ".*", ".*"' % src_version
#
projects = []		# 関連プロジェクトのリスト
out_lines = []		# 書き出すファイルの内容
lines = read_file(src_slnfile, encoding)
for line in lines:
	m = re.match(patt_vsv, line)
	if m:
		line = line.replace(m.group(1), vs_version)
		if verbose:
			print(line.strip())
	m = re.match(patt_prj, line)
	if m:
		new_prjfile = '%s%s.vcxproj' % (m.group(1), dst_version)
		line = line.replace(m.group(2), dst_version)
		projects.append(new_prjfile)
		if verbose:
			print('  => %s' % line.strip())
	m = re.match(patt_nam, line)
	if m:
		line = line.replace(m.group(1), dst_version)
		if verbose:
			print('  => %s' % line.strip())
	out_lines.append(line)
#
write_file(dst_slnfile, out_lines, encoding)
print('created: %s' % dst_slnfile)

#  関連するプロジェクトのうち、Springhead ライブラリに関するものは除く
#	ただし 'core/src', 'core/src/EmbPython' から呼び出されたときは例外
#
excludes = [
	'Base', 'Collision', 'Creature', 'EmbPython', 'FileIO', 'Foundation',
	'Framework', 'Graphics', 'HumanInterface', 'Physics', 'RunSwig']
invoked_by_librarian = False

#  ただし次の場合は特別に扱う
if slnname == 'Springhead' or slnname == 'EmbPython':
	excludes = []
elif slnname == 'SprPythonDLL':
	excludes.remove('EmbPython')
if verbose:
	print()
	print('exclude projects: %s' % excludes)
#
if not invoked_by_librarian:
	spr_projs = []
	for proj in projects:
		p = proj.split('\\')[-1]
		prjname = get_slnname(p, dst_version, '.vcxproj')
		if prjname in excludes:
			spr_projs.append(proj)
	for proj in spr_projs:
		projects.remove(proj)
if verbose:
	print()
	print('related project files: %s' % ', '.join(projects))
	print()

#  関連するプロジェクトファイル(.vcxproj)を作成する
#　(1) ToolsVersion を書き換える
#　(2) <ProjectReference Include="...RunSwig...vcxproj">
#	を見つけたら直後に <Provate>false</Private> という行を挿入する
#
patt_tv = r'ToolsVersion="(%s)"' % src_version
patt_ref = r'ProjectReference\s+Include="(.+)(%s).vcxproj"' % src_version
patt_DST = [patt_tv, patt_ref]
copylocal_false = '      <Private>false</Private>\n'
#
patt_pts = r'<PlatformToolset>(v[0-9]+)</PlatformToolset>'
patt_ns1 = r'<RootNamespace>%s([0-9]+)</RootNamespace>' % slnname
patt_ns2 = r'<RootNamespace>[a-zA-Z0-9_]+?([0-9]+)</RootNamespace>'
patt_NSP = [patt_ns1, patt_ns2]
#
patt_cfg = r'<ConfigurationType>(.+)</ConfigurationType>'
#
out_lines = []
for proj in projects:
	prjname = get_slnname(proj, dst_version, '.vcxproj')
	src = '%s%s.vcxproj' % (prjname, src_version)
	dst = '%s%s.vcxproj' % (prjname, dst_version)
	check_files(src, dst, force)
	apply_pts = False
	#
	lines = read_file(src, encoding)	
	out_lines = []
	for line in lines:
		# trap
		m = re.search(patt_cfg, line)
		if m:
			if m.group(1) == 'Makefile':
				apply_pts = True
		line_add_after = []
		
		# ToolsVersion
		m = re.search(patt_tv, line)
		if m:
			line = line.replace(m.group(1), dst_version)
			if verbose:
				print('  => %s' % line.strip())

		# ProjectReference
		m = re.search(patt_ref, line)
		if m:
			line = line.replace(m.group(2), dst_version)
			if verbose:
				print('  => %s' % line.strip())
			if m.group(1).split('\\')[-1] == 'RunSwig':
				line_add_after.append(copylocal_false)
				if verbose:
					print('  => %s' % copylocal_false.strip())

		# PlatformToolset
		if apply_pts:
			m = re.search(patt_pts, line)
			if m:
				line = line.replace(m.group(1), pts_version)
				if verbose:
					print('  => %s' % line.strip())

		# RootNamespace
		for patt in patt_NSP:
			m = re.search(patt, line)
			if m:
				line = line.replace(m.group(1), pts_version[1:])
				if verbose:
					print('  => %s' % line.strip())
				break
		#
		out_lines.append(line)
		if line_add_after != []:
			for d in line_add_after:
				out_lines.append(d)

	write_file(dst, out_lines, encoding)
	print('created: %s' % dst)

#  関連するユーザーファイル(.vcxproj.user)を作成する
#　(1) ToolsVersion を書き換える
#
if user_data:
	out_lines = []
	for proj in projects:
		prjname = get_slnname(proj, dst_version, '.vcxproj')
		src = '%s%s.vcxproj.user' % (prjname, src_version)
		dst = '%s%s.vcxproj.user' % (prjname, dst_version)
		if not os.path.exists(src):
			continue
		check_files(src, dst, force)
		#
		lines = read_file(src, encoding)	
		out_lines = []
		for line in lines:
			# ToolsVersion
			m = re.search(patt_tv, line)
			if m:
				line = line.replace(m.group(1), dst_version)
				if verbose:
					print('  => %s' % line.strip())
			#
			out_lines.append(line)
	
		write_file(dst, out_lines, encoding)
		print('created: %s' % dst)

#  終了
#
sys.exit(0)

# end: SlnConv.py

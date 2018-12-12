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
#	Ver 1.0  2018/12/12 F.Kanehori	First version.
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

#  Visual Studio のバージョンを調べる
#
def get_vs_version():
	# 最新の devenv.exe のパスを見つける
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
		print('found folowings')
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
	devenv_dir = '%s/%d/%s/Common7/IDE' % (base, year, vs_edition)
	print('OK')
	print('using %s/devenv.exe' % devenv_dir)
	os.chdir(cwd)
	#
	cmnd = '"%s/%s" /?' % (devenv_dir.replace('/', '\\'), devenv)
	status, out, err = command(cmnd)
	if status != 0:
		return -1
	outlist = out.replace('\r', '').split('\n')
	patt = r'([0-9]+.[0-9]+.[0-9]+.[0-9]+)'
	version = None
	for s in outlist:
		if s.find('Visual Studio') >= 0:
			m = re.search(patt, s)
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
print('Visual Studio Version: %s' % vs_version)

if verbose:
	print('converting')
	print('  from: %s' % src_slnfile)
	print('  to:   %s' % dst_slnfile)
	print('encoding: %s' % encoding)
	print('version info')
	print('    VS:   %s' % vs_version)
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
patt_prj = r'Project\(.*\) = ".*", "(.*)", ".*"'
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
		prjname = get_slnname(m.group(1), src_version, '.vcxproj')
		new_prjname = '%s%s.vcxproj' % (prjname, dst_version)
		line = line.replace(m.group(1), new_prjname)
		projects.append(new_prjname)
		if verbose:
			print(line.strip())
	out_lines.append(line)
#
write_file(dst_slnfile, out_lines, encoding)
print('created: %s' % dst_slnfile)

#  関連するプロジェクトのうち、Springhead ライブラリに関するものは除く
#  ただし、SpringheadXX.sln が指定された場合には除外はしない
#
excludes = [
	'Base', 'Collision', 'Creature', 'EmbPython', 'FileIO', 'Foundation',
	'Framework', 'Graphics', 'HumanINterface', 'PHysics', 'RunSwig']
#
if slnname != 'Springhead':
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

#  関連するプロジェクトファイルを作成する（ToolsVersion のみ書き換える）
#
patt_tv = r'ToolsVersion="([0-9.]+)"'
out_lines = []
for proj in projects:
	prjname = get_slnname(proj, dst_version, '.vcxproj')
	src = '%s%s.vcxproj' % (prjname, src_version)
	dst = '%s%s.vcxproj' % (prjname, dst_version)
	check_files(src, dst, force)
	#
	lines = read_file(src, encoding)	
	out_lines = []
	for line in lines:
		m = re.search(patt_tv, line)
		if m:
			line = line.replace(m.group(1), dst_version)
			if verbose:
				print(line.strip())
		out_lines.append(line)
	write_file(dst, out_lines, encoding)
	print('created: %s' % dst)

#  終了
#
sys.exit(0)

# end: SlnConv.py

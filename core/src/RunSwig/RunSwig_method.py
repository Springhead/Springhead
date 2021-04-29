#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  METHODS:
#	RunSwig(module dept_list='')
#	    module:	処理対象モジュール名
#	    dept_list:	依存するモジュール名リスト (カンマ区切り)
#
#  DESCRIPTION:
#	"do_swigall.py" の中で import され, 呼び出される.
#	呼び出された時点で, 当該プロジェクトのディレクトリにいること.
#
#	指定されたモジュールについて interface file 及び makefile を作成し, 
#	make (swig) を実行することで Stub ファイルを生成する.
#
#	作成するファイル名:
#	      interface file:	"<project>.i"
#	      makefile:		"<project>Stub.makefile"
#
# ==============================================================================
#  Version:
#     Ver 2.0    2021/04/08 F.Kanehori	全面見直し. RunSwig.py をメソッド化.
# ==============================================================================
import sys
import os
import glob

# ----------------------------------------------------------------------
#  このスクリプトは ".../core/src/RunSwig" に置く	
#
ScriptFileDir = os.sep.join(os.path.abspath(__file__).split(os.sep)[:-1])
SrcDir = '/'.join(ScriptFileDir.split(os.sep)[:-1])
SetupExists = os.path.exists('%s/setup.conf' % SrcDir)

libdir = '%s/RunSwig/pythonlib' % SrcDir
sys.path.append(libdir)
from SetupFile import *
from TextFio import *
from Proc import *
from Util import *
from Error import *
from Trace import *

# ----------------------------------------------------------------------
#  ディレクトリパスには絶対パスを使用する (cmake 使用時の混乱を避けるため)
#
sprtop = os.path.abspath('%s/../..' % SrcDir)
bindir = '%s/core/bin' % sprtop
srcdir = '%s/core/src' % sprtop
incdir = '%s/core/include' % sprtop
swigbindir = Util.pathconv('%s/swig' % bindir)

# ----------------------------------------------------------------------
#  ファイル出力用メソッド
# ----------------------------------------------------------------------
def output(fname, lines, verbose=0):
	if verbose:
		print('%s:' % fname)
		for line in lines:
			print('  %s' % line)
	fobj = TextFio(fname, 'w', encoding='utf8')
	if fobj.open() < 0:
		Error(prog).put(fobj.error(), exitcode=0, alive=True)
	if fobj.writelines(lines, '\n') < 0:
		Error(prog).put(fobj.error(), exitcode=0, alive=True)
	fobj.close()

# ----------------------------------------------------------------------
#  RunSwig 本体
# ----------------------------------------------------------------------
def RunSwig(module, dept='', verbose=0, dry_run=False):
	prog = sys._getframe().f_code.co_name
	trace =  Trace().flag('RunSwig')
	if trace:
		print('ENTER: %s(%s, %s)' % (prog, module, dept))
		sys.stdout.flush()

	incdir_rel = Util.upath(os.path.relpath(incdir))
	srcdir_rel = Util.upath(os.path.relpath(srcdir))

	args = [module]
	args.extend(dept.split(','))
	target_list = args[::-1]		# 逆順にした

	# ----------------------------------------------------------------------
	#  Springhead 拡張版の swig を起動するためのパス.
	#
	addpath = os.pathsep.join([swigbindir, bindir])

	# ----------------------------------------------------------------------
	#  生成するファイル名
	#
	interfacefile = '%s.i' % module
	makefile = '%sStub.makefile' % module
	stubfile = '%sStub.cpp' % module

	# ----------------------------------------------------------------------
	#  ヘッダファイル情報を収集する.
	#
	incf_names = ['Springhead.h', 'Base/Env.h', 'Base/BaseDebug.h']
	srcf_names = []		# ['Foundation/UTTypeDesc.h']
	auxdep_inc = list(map(lambda x: '%s/%s' % (incdir_rel, x), incf_names))
	auxdep_src = list(map(lambda x: '%s/%s' % (srcdir_rel, x), srcf_names))
	auxdep = copy.deepcopy(auxdep_inc)
	auxdep.extend(auxdep_src)

	srcinf = []
	srcimp = []
	srcinfdep = []
	srcimpdep = []
	for target in target_list:
		srcinf.extend(glob.glob('%s/%s/*.h' % (incdir_rel, target)))
		srcimp.extend(glob.glob('%s/%s/*.h' % (srcdir_rel, target)))
		srcinfdep.extend(glob.glob('%s/%s/*.h' % (incdir_rel, target)))
		srcimpdep.extend(glob.glob('%s/%s/*.h' % (srcdir_rel, target)))
	srcinf = Util.upath(srcinf)
	srcimp = Util.upath(srcimp)
	srcinfdep = Util.upath(srcinfdep)
	srcimpdep = Util.upath(srcimpdep)
	if verbose:
		print('srcinf: %s' % srcinf)
		print('srcimp: %s' % srcimp)
		print('srcinfdep: %s' % srcinfdep)
		print('srcimpdep: %s' % srcimpdep)

	# ----------------------------------------------------------------------
	#  インターフェイスファイルを生成する.
	#
	lines = []
	lines.append('#\tDo not edit. RunSwig.py will update this file.')
	lines.append('%%module %s' % module)
	lines.append('#define DOUBLECOLON ::')
	for fname in auxdep_inc:
		lines.append('%%include "%s"' % fname)
	for fname in srcinf:
		lines.append('%%include "%s"' % fname)
	for fname in auxdep_src:
		lines.append('%%include "%s"' % fname)
	for fname in srcimp:
		lines.append('%%include "%s"' % fname)
	#
	#
	path = '%s/%s' % (os.getcwd(), interfacefile)
	print('\t%s: generating "%s"' % (prog, Util.upath(path)))
	sys.stdout.flush()
	output(interfacefile, lines, verbose)

	# ----------------------------------------------------------------------
	#  make で使用する外部スクリプト
	#
	if SetupExists:
		sf = SetupFile('%s/setup.conf' % SrcDir, verbose=1)
		sf.setenv()
		python = sf.getenv('python')
		if Util.is_unix():
			make = sf.getenv('gmake')
		else:
			make = '%s /NOLOGO' % sf.getenv('nmake')
		swig = sf.getenv('swig')
	else:
		python = 'python'
		make = 'gmake' if Util.is_unix() else 'nmake /NOLOGO'
		swig = 'swig'

	# ----------------------------------------------------------------------
	#  makefile を生成する.
	#
	deptlist = []
	deptlist.extend(auxdep)
	deptlist.extend(srcinfdep)
	deptlist.extend(srcimpdep)
	lines = []
	lines.append('#\tDo not edit. RunSwig.py will update this file.')
	lines.append('all:\t%s' % stubfile)
	lines.append('')
	lines.append('%s:\t\\' % stubfile)
	lines.append('\t%s' % '\\\n\t'.join(Util.pathconv(deptlist)))
	line = Util.pathconv('\t%s -I%s/Lib' % (swig, swigbindir))
	line += ' -spr -w305,312,319,325,401,402'
	line += ' -DSWIG_%s -c++ %s' % (module, interfacefile)
	if Util.is_unix():
		lines.append('\t@echo "%s"' % line)
	lines.append(line)
	#
	path = '%s/%s' % (os.getcwd(), makefile)
	print('\t%s: generating "%s"' % (prog, Util.upath(path)))
	sys.stdout.flush()
	output(makefile, lines, verbose)

	# ----------------------------------------------------------------------
	#  make を実行する（ここで必要ならば swig が実行される）.
	#
	cmd = '%s -f %s' % (make, Util.pathconv(makefile))
	proc = Proc(dry_run=dry_run)
	proc.execute(cmd, addpath=addpath, shell=True)
	status = proc.wait()
	if status != 0:
		msg = '%s failed (%d)' % (make, status)
		Error(prog).put(msg, exitcode=0, alive=True)
	
	if trace:
		print('LEAVE: %s' % prog)
		sys.stdout.flush()
	return status

# end: RunSwig.py

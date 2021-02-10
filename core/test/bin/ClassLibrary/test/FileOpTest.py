#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  FILE:
#	FileOpTest.py
#
#  DESCRIPTION:
#	Test program for class FileOp (for Ver 1.4 and after).
# ======================================================================
from __future__ import print_function
import sys
import os
import glob
import datetime
import time
import shutil

sys.path.append('..')
from FileOp import *
from Util import *

# ----------------------------------------------------------------------
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]

F = FileOp(verbose=1)
print('Test program for class: %s, Ver %s\n' % (F.clsname, F.version))

# ----------------------------------------------------------------------
def remove_tree(top):
	if os.path.exists(top):
		remove_all(top)
		os.rmdir(top)

def remove_all(top):
	if not os.path.exists(top):
		return
	another_drive = False
	if Util.is_windows():
		t_drive = os.path.abspath(top)[0]
		c_drive = os.getcwd()[0]
		another_drive = t_drive != c_drive
	#print('remove_all: %s (another drive: %s)' % (top, another_drive))
	if another_drive:
		# os.walk() seemes to work on current drive only.
		dlist = Util.upath(top).split('/')
		wrkdir = '/'.join(dlist[:-1])
		top = dlist[-1]
		cwd = os.getcwd()
		os.chdir(wrkdir)
	#print('remove_all: top: %s' % top)
	for root, dirs, files in os.walk(top, topdown=False):
		for name in files:
			os.remove(os.path.join(root, name))
		for name in dirs:
			os.rmdir(os.path.join(root, name))
	if another_drive:
		os.chdir(cwd)

def make_tree(top):
	remove_all(top)
	Util.makedirs(top, exist_ok=True)
	Util.makedirs(testpath(top, 'test1'), exist_ok=True)
	Util.makedirs(testpath(top, 'test1/test1'), exist_ok=True)
	Util.makedirs(testpath(top, 'test2'), exist_ok=True)
	Util.makedirs(testpath(top, 'test3'), exist_ok=True)
	FileOp().touch(testpath(top, 'test1/file_op_test_x'))
	FileOp().touch(testpath(top, 'test1/file_op_test_y'))
	FileOp().touch(testpath(top, 'wild_x.txt'))
	FileOp().touch(testpath(top, 'wild_y.txt'))
	FileOp().touch(testpath(top, 'test1/wild_1.txt'))
	FileOp().touch(testpath(top, 'test1/wild_2.txt'))
	FileOp().touch(testpath(top, 'test1/test1/foo.txt'))

def testpath(top, path):
	return '%s/%s' % (top, path)

def Ls(fop, path, show='mtime'):
	Print(path)
	lslist = fop.ls(path)
	if not lslist:
		Print('no such file: "%s"' % path)
		return
	if isinstance(lslist, str):
		Print(lslist)
		return
	for item in lslist:
		Print(item)

def Print(msg, indent=2):
	print('%s%s' % (' '*indent, msg))

# ----------------------------------------------------------------------
test_suit = ['ls', 'touch', 'cp', 'mv', 'rm']
#test_suit = ['rm']
if Util.is_windows():
	another_fs = 'C:/tmp/FileOpTest'

verbose = 0
dry_run = True
top = 'test/file_op_test'
remove_tree(top)
'''
os.makedirs(top)
'''
Util.makedirs(top)

# ls
#
if 'ls' in test_suit:
	print('-- ls --')
	files = F.ls('.')
	print('sorted by name')
	for f in files:
		print(f)
	print()

	#print('sorted by ctime')
	#files = F.ls('.', sort='ctime', show='ctime')
	print('sorted by mtime')
	files = F.ls('.', sort='mtime', show='mtime')
	for f in files:
		print(f)
	print()

# touch
#
if 'touch' in test_suit:
	make_tree(top)
	print('-- touch no_create -- ')
	fname = '%s/test1/file_op_test' % top
	if os.path.exists(fname):
		os.remove(fname)
	F.touch(fname, no_create=True)
	Ls(F, fname)
	print()

	print('-- touch --')
	F.touch(fname)
	Ls(F, fname)
	print()

	print('wait until next minute')
	now = datetime.datetime.today()
	start = str(now.hour) + str(now.minute)
	for n in range(60):
		now = datetime.datetime.today()
		check = str(now.hour) + str(now.minute)
		if check != start:
			break
		disp = '%d' % (60 - now.second)
		sys.stdout.write('\r%4s sec' % disp)
		sys.stdout.flush()
		time.sleep(1)
	print()
	F.touch(fname)
	Ls(F, fname)
	print()

# cp
#
if 'cp' in test_suit:
	make_tree(top)
	if Util.is_windows():
		remove_tree(another_fs)
		if os.path.exists(another_fs):
			os.rmdir(another_fs)
	Ls(F, top)
	print()
	for dry_run in [True, False]:
		opt = 'dry_run' if dry_run else 'test'
		print('-- cp (%s) --' % opt)
		F = FileOp(info=1, dry_run=dry_run, verbose=0)

		src = testpath(top, 'test1/file_op_test_x')
		wc1 = testpath(top, '*.txt')
		wc2 = testpath(top, 'test1/*.txt')
		F.cp(src, testpath(top, 'test1/file_op_test_1'))
		F.cp(wc1, testpath(top, 'test2'))
		F.cp(wc2, testpath(top, 'test2'))
		F.cp(src, testpath(top, 'test1/test1/file_op_test_11'))
		F.cp(src, testpath(top, 'test2/file_op_test_2'))
		F.cp(src, testpath(top, 'test3'))
		F.cp(testpath(top, 'test1'), testpath(top, 'test4'))
		#Ls(F, top)
		#print()

		if Util.is_windows():
			F.cp(testpath(top, 'test1'), another_fs)
			Ls(F, another_fs)
			remove_tree(another_fs)
			print()

		src = testpath(top, 'test1')
		F.cp(src, testpath(top, 'test5'))
		Ls(F, top)
		print()

# mv
#
if 'mv' in test_suit:
	make_tree(top)
	#Ls(F, top)
	#print()
	for dry_run in [True, False]:
		opt = 'dry_run' if dry_run else 'test'
		print('-- mv (%s) --' % opt)
		F = FileOp(info=1, dry_run=dry_run, verbose=0)

		src = testpath(top, 'test1/file_op_test_x')
		dst = testpath(top, 'test1/file_op_test_1')
		F.mv(src, dst)
		Ls(F, top)
		print()

		src = dst
		dst = testpath(top, 'test1/test1/file_op_test_11')
		F.mv(src, dst)
		Ls(F, top)
		print()

		src = dst
		dst = testpath(top, 'test2/file_op_test_2')
		F.mv(src, dst)
		Ls(F, top)
		print()

		src = testpath(top, 'test2')
		dst = testpath(top, 'test3')
		F.mv(src, dst)
		Ls(F, top)
		print()

# rm
#
if 'rm' in test_suit:
	make_tree(top)
	#Ls(F, top)
	#print()
	for dry_run in [True, False]:
		opt = 'dry_run' if dry_run else 'test'
		print('-- rm (%s) --' % opt)
		F = FileOp(info=1, dry_run=dry_run, verbose=0)

		F.rm(testpath(top, 'test1/file_op_test_x'))
		Ls(F, top)
		print()

		make_tree(top)
		F.rm(testpath(top, 'test1/file_op_test_*'))
		Ls(F, top)
		print()

		make_tree(top)
		F.rm(testpath(top, 'test1/*'))
		Ls(F, top)
		print()

		make_tree(top)
		F.rm(testpath(top, 'test1/test1'))
		Ls(F, top)
		print()
		F.rm(testpath(top, 'test1'))
		Ls(F, top)
		print()

remove_tree(top)
sys.exit(0)

# end: FileOpTest.py

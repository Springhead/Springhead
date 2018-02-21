#!/usr/local/bin/python3.4
# -*- coding: utf-8 -*-
# ======================================================================
#  FILE:
#	FileOpTest.py
#
#  DESCRIPTION:
#	Test program for class FileOp (for Ver 1.3 and after).
# ======================================================================
import sys
import os
import glob
import datetime
import time
import shutil
sys.path.append('..')
from FileOp import *

# ----------------------------------------------------------------------
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
testdir1 = 'test/test1'
testdir2 = 'test/test1/test2'
testdir3 = 'test/test1/test3'
testdir4 = 'test/test1/test4'
testfile = 'file_op_test'

# ----------------------------------------------------------------------
def Print(msg, indent=2):
	print('%s%s' % (' '*indent, msg))

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

# ----------------------------------------------------------------------
F = FileOp(verbose=1)
print('Test program for class: %s, Ver %s\n' % (F.clsname, F.version))

# ls
#
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
os.makedirs('test/test1', exist_ok=True)
print('-- touch no_create -- ')
fname = '%s/%s' % (testdir1, testfile)
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
	mod = n % 10
	ch = str(int(n/10)+1) if mod == 9 else '+' if mod == 4 else '.'
	sys.stdout.write(ch)
	sys.stdout.flush()
	time.sleep(1)
print()
F.touch(fname)
Ls(F, fname)
print()

# cp, mv, rm
#
print('-- cp --')
testpath1 = '%s/%s' % (testdir1, testfile)
testpath2 = '%s/%s_2' % (testdir1, testfile)
testpath3 = '%s/%s_3' % (testdir1, testfile)
testpath4 = '%s/%s_4' % (testdir2, testfile)
checkpath1 = '%s/*' % testdir1
checkpath2 = '%s/*' % testdir2

F = FileOp(info=1)
F.cp(testpath1, testpath2)
Ls(F, checkpath1)
print()

print('-- mv --')
os.makedirs('test/test1/test2', exist_ok=True)
F.mv(testpath2, testpath3)
Ls(F, checkpath1)
print()

F.mv(testpath3, testpath4)
Ls(F, checkpath1)
#Ls(F, checkpath2)
print()

os.makedirs(testdir3, exist_ok=True)
F.mv(testdir3, testdir4)
Ls(F, 'test/test1')
os.rmdir(testdir4)
print()

os.makedirs(testdir3, exist_ok=True)
F.mv(testdir3, testdir2)
Ls(F, 'test/test1')
os.rmdir(testdir3)
print()

print('-- rm original --')
def make_tree():
	if os.path.exists(testdir1):
		F.rm('%s/*' % testdir1)
	#os.mkdir(testdir1)
	#os.mkdir(testdir2)
	F.touch('%s/%s' % (testdir1, testfile))
	F.touch('%s/%s' % (testdir2, testfile))
make_tree()
Ls(F, testdir1)
print()
print('-- rm %s --' % testdir1)
make_tree()
F.rm('%s' % testdir1)
Ls(F, testdir1)
print()

print('-- rm %s/* --' % testdir1)
make_tree()
Ls(F, testdir1)
F.rm('%s/*' % testdir1)
Ls(F, testdir1)
print()

print('-- rm -r %s --' % testdir1)
make_tree()
F.rm('%s' % testdir1, recurse=True)
Ls(F, testdir1)
print()

#  dry_run
#
print('-- dry_run --')
F.touch('%s/%s' % (testdir1, testfile))
F = FileOp(info=1, dry_run=True)
F.cp(testpath1, testpath2)
Ls(F, checkpath1)
print()
F.mv(testpath1, testpath3)
Ls(F, checkpath1)
print()
F.rm('%s/*' % testdir1)
F.rm('%s/*' % testdir2)
Ls(F, checkpath1)

sys.exit(0)

# end: FileOpTest.py

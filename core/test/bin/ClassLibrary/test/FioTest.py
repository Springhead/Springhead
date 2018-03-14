#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  FILE:
#	FioTest.py
#
#  DESCRIPTION:
#	Test program for class Fio (Ver 2.01).
# ======================================================================
import sys
import os
import stat
from datetime import datetime
sys.path.append('..')
from Fio import *
from FileOp import *

# ----------------------------------------------------------------------
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
verbose = 1

# ----------------------------------------------------------------------
def Print(msg, indent=2):
	print('%s%s' % (' '*indent, msg))

def self_test(fname, mode, verbose):
	f = Fio(fname, mode, verbose)
	status = f.open()
	Print("open status: %d (mode '%s')" % (status, mode))
	if status != 0:
		print(f.error())
		sys.exit(-1)
	f.close()

# ----------------------------------------------------------------------
F = Fio('dummy')
Fop = FileOp()
print('Test program for class: %s, Ver %s\n' % (F.clsname, F.version))

print('-- Read Open --')
fname = 'test/empty.txt'
for mode in ['r', 'rb']:
	self_test(fname, mode, verbose)
Print(Fop.ls(fname))
print()

print('-- Write Open --')
fname = 'test/foo.fio'
for mode in ['w', 'a', 'wb', 'ab']:
	self_test(fname, mode, verbose)
Print(Fop.ls(fname))

sys.exit(0)

# end: FioTest.py

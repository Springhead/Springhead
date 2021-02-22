#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  FILE:
#	SetupFileTest.py
#
#  DESCRIPTION:
#	Test program for class SetupFile.
# ======================================================================
from __future__ import print_function
import sys
import os
sys.path.append('..')
from SetupFile import *

# ----------------------------------------------------------------------
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
verbose = 1
setupfile = 'test/setup.conf'

# ----------------------------------------------------------------------
def Print(msg, indent=2):
	print('%s%s' % (' '*indent, msg))

# ----------------------------------------------------------------------
S = SetupFile(setupfile, verbose=1, register=True)
print('Test program for class: %s, Ver %s\n' % (S.clsname, S.version))

# remove setup file if exists
if os.path.exists(setupfile):
	print('-- remove existent file --')
	os.remove(setupfile)

# make new setup file
#
print('-- setup new data --')
S.add_prog('prog1', '/home/usr/foo/bin/prog1')
S.add_prog('prog2', 'C:\\Users\\usr\\bar\\bin\\prog2')

S.add_path('path1', '/home/usr/foo/bin')
S.add_path('path2', 'C:\\Users\\usr\\bar\\bin')

S.add_data('plat', 'x64')
S.add_data('conf', 'Release')
S.add_data('vers', '15.0')

print('-- write to file --')
st = S.write()
if st != 0:
	print('open error: st %d' % st)
	sys.exit(1)
S.show()

# set paths to environment variable
#
S.setenv()

sys.exit(0)

# end: SetupFileTest.py

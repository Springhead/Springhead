#!/usr/local/bin/python3.4
# -*- coding: utf-8 -*-
# ======================================================================
#  FILE:
#	UtilTest.py
#
#  DESCRIPTION:
#	Test program for class Util (Ver 2.0).
# ======================================================================
import sys
import os
sys.path.append('..')
from Util import *

# ----------------------------------------------------------------------
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
verbose = 1

# ----------------------------------------------------------------------
def Print(msg, indent=2):
	print('%s%s' % (' '*indent, msg))

# ----------------------------------------------------------------------
U = Util()
print('Test program for class: %s, Ver %s\n' % (U.clsname, U.version))

# is_unix, is_windows
#
print('-- is_unix, is_windows --')
Print('is_unix -> %s' % Util.is_unix())
Print('is_windows -> %s' % Util.is_windows())
print()

# pathconv
#
print('-- pathconv --')
u = os.getcwd().replace('\\', '/')
w = os.getcwd().replace('/', '\\')
Print('u: %s' % u)
Print('w: %s' % w)
Print('auto u: %s' % Util.pathconv(u))
Print('auto w: %s' % Util.pathconv(w))
Print('u to u: %s' % Util.pathconv(u, to='unix'))
Print('w to u: %s' % Util.pathconv(w, to='linux'))
Print('u to w: %s' % Util.pathconv(u, to='windows'))
Print('w to w: %s' % Util.pathconv(w, to='win'))
print()

# date/time/now
#
print('-- date/time/now --')
Print('date: %s' % Util.date())
Print('time: %s' % Util.time())
Print('now:  %s' % Util.now())
print('')

sys.exit(0)

# end: UtilTest.py

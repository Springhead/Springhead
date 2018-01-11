#!/usr/local/bin/python3.4
# -*- coding: utf-8 -*-
# ======================================================================
#  FILE:
#	ErrorTest.py
#
#  DESCRIPTION:
#	Test program for class Error (Ver 2.3).
# ======================================================================
import sys
import os
sys.path.append('..')
from Error import *

# ----------------------------------------------------------------------
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
verbose = 1

# ----------------------------------------------------------------------
E = Error(prog, verbose=verbose)
E.set_testcase(True)
print('Test program for class: %s, Ver %s\n' % (E.clsname, E.version))

E.print('This is information.', prompt=None)
E.print('This is warning', prompt='Warning')
E.print('This is fatal', prompt='Fatal')
E.print('This is error')
E.print('This is abort', prompt='Abort')
E.print('This is panic', prompt='Pan')
print()

E.set_testcase(False)
E.print('Must return to this program.', alive=True)
E.print('Must abort program.')
print('Should not come here')

sys.exit(0)

# end: ErrorTest.py

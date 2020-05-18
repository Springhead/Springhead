#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  FILE:
#	ErrorTest.py
#
#  DESCRIPTION:
#	Test program for class Error (Ver 3.0).
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
print('Test program for class: %s, Ver %s\n' % (E.clsname, E.version))

E.put('This is information.', prompt=None, alive=True)
E.put('This is warning', prompt='Warning', alive=True)
E.put('This is fatal', prompt='Fatal', alive=True)
E.put('This is error', alive=True)
E.put('This is abort', prompt='Abort', alive=True)
E.put('This is panic', prompt='Pan', alive=True)
print()
E.info('Error.info() called')
E.warn('Error.warn() called')
E.error('Error.error() called')
print()

E.put('Must return to this program.', alive=True)
print()
print('Exit code should be 99. Try "echo %ERRORLEVEL%"')
E.abort('Must abort program.', prompt='Pan', exitcode=99)
print('Should not come here')

sys.exit(0)

# end: ErrorTest.py

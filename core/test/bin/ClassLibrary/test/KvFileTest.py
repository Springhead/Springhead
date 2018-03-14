#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  FILE:
#	KvFileTest.py
#
#  DESCRIPTION:
#	Test program for class KvFile (Ver 3.0).
# ======================================================================
import sys
import os
sys.path.append('..')
from KvFile import *

# ----------------------------------------------------------------------
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
verbose = 0

# ----------------------------------------------------------------------
def Print(msg, indent=2):
	print('%s%s' % (' '*indent, msg))

def defined_sections(count, kvf):
	if count == 0:
		msg = 'no section defined'
	else:
		sections = kvf.sections()
		msg = '%d section(s) defined: %s' % (count, sections)
	return msg

# ----------------------------------------------------------------------
K = KvFile('')
print('Test program for class: %s, Ver %s\n' % (K.clsname, K.version))

##
fname = 'test/KvFileTest.ini'
print('-- %s --' % fname)
kvf = KvFile(fname, verbose=verbose)
count = kvf.read()
if count < 0:
	Print(kvf.error())
	sys.exit(-1)
Print('file: "%s"' % fname)
Print(defined_sections(count, kvf))
Print('----')
kvf.show(2)
print()
#
keys = ['Springhead2', 'TestRoot', 'OutFile', 'SceneList']
Print('-- all keys are defined --')
Print('keys to check: %s' % keys)
if kvf.check(keys) == 0:
	Print('All keys are defined')
else:
	Print('Not all keys are defined !!')
print()
#
keys.append('GhostKey')
Print('-- GhostKey is not defined --')
Print('keys to check: %s' % keys)
if kvf.check(keys) == 0:
	Print('All keys are defined')
else:
	Print('Not all keys are defined !!')
print()

##
fname = 'test/KvFileTest_eq.ini'
print('-- %s --' % fname)
kvf = KvFile(fname, sep='=', verbose=verbose)
count = kvf.read()
if count < 0:
	Print(kvf.error())
	sys.exit(-1)
Print('file: "%s"' % fname)
Print(defined_sections(count, kvf))
Print('----')
kvf.show(2)
print()

##
fname = 'test/KvFileTest.ini2'
print('-- %s (overwrite: True)--' % fname)
kvf = KvFile(fname, overwrite=True, verbose=verbose)
count = kvf.read()
if count < 0:
	Print(kvf.error())
	sys.exit(-1)
Print('file: "%s"' % fname)
Print(defined_sections(count, kvf))
Print('----')
kvf.show(2)
print()

##
fname = 'test/KvFileTest.ini2'
print('-- %s (overwrite: False)--' % fname)
kvf = KvFile(fname, overwrite=False, verbose=verbose)
count = kvf.read()
if count < 0:
	Print(kvf.error())
	sys.exit(-1)
Print('file: "%s"' % fname)
Print(defined_sections(count, kvf))
Print('----')
kvf.show(2)
print()

##
fname = 'test/KvFileTest.ini3'
print('-- %s --' % fname)
kvf = KvFile(fname, overwrite=False, verbose=verbose)
count = kvf.read()
if count < 0:
	Print(kvf.error())
	sys.exit(-1)
Print('file: "%s"' % fname)
Print(defined_sections(count, kvf))
Print('----')
kvf.show(2)
print()

print('-- set(aux_def, aux_val_def) --')
kvf.set('aux_def', 'aux_val_def')
print('-- set(aux_sec1, aux_val_sec1, section1) --')
kvf.set('aux_sec1', 'aux_val_sec1', 'section1')
print('-- set(aux_sec2, aux_val_sec2, section2) --')
kvf.set('aux_sec2', 'aux_val_sec2', 'section2')
print()

print('-- get(KvFile.DEFAULT) --')
kvf.show(2, KvFile.DEFAULT)
print()
sections = kvf.sections()
for s in sections:
	print('-- get(%s) --' % s)
	kvf.show(2, s)
	print()

sys.exit(0)

# end: KvFileTest.py

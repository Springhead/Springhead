#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  FILE:
#	TextFioTest.py
#
#  DESCRIPTION:
#	Test program for class TextFio (Ver 2.2).
#
#  NOTE:
#	This test program works on python 2.7.17 UNLESS output is
#	redirected.
# ======================================================================
#from __future__ import print_function
import sys
import os
sys.path.append('..')
from TextFio import *
from Util import *

# ----------------------------------------------------------------------
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
verbose = 1

# ----------------------------------------------------------------------
def Print(msg, indent=2):
	print('%s%s' % (' '*indent, msg))

def fileread(f, which='lines', filters=None):
	if f.open() < 0:
		Print(f.error())
		sys.exit(-1)
	if filters is not None:
		for filt in filters:
			if isinstance(filt, list):
				f.add_filter(filt[0], filt[1])
			else:
				f.add_filter(filt)
	if which == 'lines':
		lines = f.read()
	else:
		lines = f.read()
		infos = f.lineinfo()
		print(infos)
	f.close()
	if which == 'lines':
		return lines
	return lines, infos

def print_lines(lines, changed=None):
	count = 0
	for line in lines:
		count += 1
		Print('%3d: %s<EOL>' % (count, line))
	if changed:
		Print('- %d lines changed' % changed)

def print_info(infos, changed=None):
	for info in infos:
		Print('%3d: %s<EOL>' % (info[f.NUMB], info[f.DATA]))
	if changed:
		Print('- %d lines changed' % changed)

# ----------------------------------------------------------------------
T = TextFio('')
print('Test program for class: %s, Ver %s\n' % (T.clsname, T.version))

# encoding
#
print('---- encoding ----')
print()
encs = ['utf8', 'sjis', 'unicode', 'jis', 'euc', 'ascii', 'utf8-bom', 'utf16']
for enc in encs:
	fname = 'test/TextFioTest.' + enc
	f = TextFio(fname, encoding='utf_8', verbose=verbose)
	lines = fileread(f)
	print()

print('---------------------------------------')
if Util.is_unix():
	files = ['test/TextFioTest.utf8', 'test/empty.txt']
else:
	files = ['test/TextFioTest.sjis', 'test/empty.txt']
encoding = 'auto'
for file in files:
	f = TextFio(file, verbose=verbose)

	Print('-- original lines --')
	lines, infos = fileread(f, 'lineinfo')
	if len(infos) == 0:
		Print('TestMain: lines empty!')
		print()
		continue
	print_info(infos)
	print()

	Print('-- apply filter --')
	filters = []
	filters.append(TextFio.WRAP)
	#filters.append([TextFio.WRAP, ['\\'])
	filters.append(TextFio.ELIM)
	filters.append([TextFio.ELIM, ['/*', True]])
	lines2, lines3 = fileread(f, 'lineinfo', filters)
	if len(lines2) == 0:
		print()
		continue

	Print('-- line data --')
	print_lines(lines2)
	print()

	Print('-- line data replace --')
	patterns = ['0', '-', '0']
	lines2, c = f.replace(patterns, lines2)
	print_lines(lines2, c)
	print()
	patterns = [['--1', '1--', '--1'], ['com', 'Com', 'com']]
	lines2, c = f.replace(patterns, lines2)
	print_lines(lines2, c)
	print()

	Print('-- line info --')
	print_info(lines3)
	print()

	Print('-- line info replace --')
	patterns = ['0', '-']
	lines3, c = f.replace(patterns, lines3)
	print_info(lines3, c)
	print()
	patterns = [['--1', '1--'], ['com', 'Com']]
	lines3, c = f.replace(patterns, lines3)
	print_info(lines3, c)
	print()

print('---------------------------------------')
print('-- write file --')
for enc in encs:
	fname = 'test/foo.' + enc
	f = TextFio(fname, 'w', encoding=enc, verbose=verbose)
	if f.open() < 0:
		Print(f.error())
		sys.exit(-1)
	lines = ['# %s' % fname, '', '', '', 'end']
	if enc == 'ascii':
		 lines[2] = ' This is the line to verify encoding.'
	else:
		 lines[2] = ' これはエンコディングを確認するための行.'
	f.writelines(lines)
	f.close()

	f = TextFio(fname, verbose=verbose)
	if f.open() < 0:
		Print(f.error())
		sys.exit(-1)
	leaf = fname.split('/')[-1]
	Print('"%s": encoding: %s (verify)' % (leaf, f.get_encoding()))
	f.close()
	print()

sys.exit(0)

# end: TextFioTest.py

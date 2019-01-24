#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	html_escape file ...
#	    file:	Input file name.
#
#  DESCRIPTION:
#	TeXソース中に出現するhtml特殊文字(<,>,&)について、htmlソース上
#	に正しく出力されるように適切なescape処理を施す。
#
#  NOTE:
#	Lwarp の html特殊文字(<,>,&)の扱いは（現時点では）奇妙である。
#	Math mode:
#	    数式はimageとして出力される。文字'<'と'>'は正しく表示され
#	    るが、文字'&'（TeXでは直接'&'と記述することはできないので
#	    実際には'\&'であるが）は'&amp;'と表示される。
#	    マクロ\textless, \textgreaterを使用しても同様である。
#	    \symbol{...}を使用しても構わないが、数式中の'<','>'とは
#	    判断されない。
#	    alt="..."出力の中では、マクロはマクロ名が出力される。
#	Text mode:
#	    Verbatim環境:
#		"#include <...>" の'<','>'は&lt;,&gt;に変換される。
#		それ以外の特殊文字は変換されずにそのまま出力される。
#	    それ以外:
#		特殊文字は変換されずにそのまま出力される。
#
#  VERSION:
#	Ver 1.0  2019/01/24 F.Kanehori	First release version.
# ======================================================================
version = '1.0'

import sys
import os
import re
import glob
import subprocess
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].replace(os.sep, '/').split('/')[-1].split('.')[0]
#
MD_TEXT		= 1
MD_MATH		= 2
MD_DMATH	= 3
MD_TABLE	= 4
MD_VERBATIM	= 5
#
BGN_MATH	= '$'
BGN_DMATH	= r'\['
BGN_TABLE	= r'\begin{tabular}'
BGN_VERBATIM	= r'\begin{verbatim}'
BGN_SRCCODE	= r'\begin{sourcecode}'
#
END_MATH	= '$'
END_DMATH	= r'\]'
END_TABLE	= r'\end{tabular}'
END_VERBATIM	= r'\end{verbatim}'
END_SRCCODE	= r'\end{sourcecode}'

# ----------------------------------------------------------------------
#  Globals
#
global end_environ		# VERBATIM or SRCCODE

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options] file...'
parser = OptionParser(usage = usage)
parser.add_option('-v', '--verbose', dest='verbose',
			action='count', default=0,
			help='set verbose mode')
parser.add_option('-V', '--version', dest='version',
			action='store_true', default=False,
			help='show version')

# ----------------------------------------------------------------------
#  Process for command line
#
(options, args) = parser.parse_args()
if options.version:
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
if len(args) == 0:
	print('%s: Error: incorrect number of arguments' % prog)
	print()
	subprocess.Popen('python %s.py --help' % prog).wait()
	sys.exit(0)

# get options and input file name
fnames = []
for arg in args:
	fnames.extend(glob.glob(arg))
if options.verbose:
	print('infile: %s' % fnames)

# ----------------------------------------------------------------------
# for verbose
def __verbose(msg, level=1):
	if options.verbose >= level:
		print('%s%s' % ((' '*indent.get()), msg))
		sys.stdout.flush()

class INDENT:
	def __init__(self):
		self.indent = 0
	def inc(self, val): self.indent += val
	def dec(self, val): self.indent -= val
	def get(self): return self.indent
indent = INDENT()

# ----------------------------------------------------------------------
#  メソッドの定義
# ----------------------------------------------------------------------

#  a = str.split() と同じだが、2番目以降の要素は（あれば）まとめる。
#
def split(line, ch):
	segment = line.split(ch)
	if len(segment) == 1:
		# has no 'ch'
		__verbose("split: has no '%s'" % ch, 2)
		__verbose('  ---> [%s]' % line.strip(), 2)
		return [line]
	return [segment[0], ch.join(segment[1:])]

#  1行分の処理を行なう。
#
def process_oneline(mode, line):
	global end_environ
	indent.inc(2)
	__verbose('IN:  [%s]' % line.strip())
	__verbose('%s: [%s]' % (mode_str(mode), line.strip()), 2)
	__verbose('--- end env [%s]' % end_environ, 2)
	#
	if mode is MD_MATH or mode is MD_DMATH:
		__verbose('process: MATH', 2)
		separator = BGN_MATH if mode is MD_MATH else BGN_DMATH
		segments = split(line, separator)
		if len(segments) == 1:
			__verbose('process: MATH: still in math/dmath mode', 2)
			__verbose('nothing to do', 2)
			pass
		else:
			__verbose('process: MATH: end_math FOUND', 2)
			__verbose('split: [%s, TEXT]' % mode_str(mode), 2)
			mode, text = process_oneline(MD_TEXT, segments[1])
			line = separator.join([segments[0], text])

	elif mode is MD_TABLE:
		__verbose('process: TABLE', 2)
		if line.find(END_TABLE) >= 0:
			__verbose('process: TABLE: end_table FOUND', 2)
			__verbose('split: [TABLE, TEXT]', 2)
			segments = split(line, END_TABLE)
			mode1, text1 = process_oneline(MD_TABLE, segments[0])
			mode2, text2 = process_oneline(MD_TEXT, segments[1])
			line = END_TABLE.join([text1, text2])
			__verbose('returned: mode1 %s, mode2 %s' \
					% (mode_str(mode1), mode_str(mode2)), 2)
			mode = mode2
		else:
			__verbose('process: TABLE: still in table mode', 2)
			# convert '<'( and '>'.  ** do not convert '&'!
			if has_one_of('<>', line):
				__verbose('convert to tex macro', 2)
			line = line.replace('<', r'\textless ')
			line = line.replace('>', r'\textgreater ')
		# end if

	elif mode is MD_VERBATIM:
		__verbose('process: VERBATIM')
		if end_environ is None:
			print('%s: Panic: no environment was detected so far' % prog)
			sys.exit(1)
		if line.find(end_environ) >= 0:
			# segments = [ MD_VARBATIM, MD_TEXT ]
			__verbose('process: VERBATIM: end_environ FOUND', 2)
			__verbose('split: [VERBATIM, TEXT]', 2)
			segments = split(line, end_environ)
			mode1, text1 = process_oneline(MD_VERBATIM, segments[0])
			mode2, text2 = process_oneline(MD_TEXT, segments[1])
			line = end_environ.join([text1, text2])
			__verbose('returned: mode1 %s, mode2 %s' \
					% (mode_str(mode1), mode_str(mode2)), 2)
			mode = mode2
			end_environ = None
		else:
			__verbose('process: VERBATIM: still in verbatim mode', 2)
			if re.search(r'#include\s*<.+.', line):
				# do not convert '#include <...>'.
				segments = split(line, '>')
				if len(segments) == 1:
					__verbose('has #incliude <...>', 2)
					# do not have any comment.
					pass
				else:
					__verbose('has #incliude <...> // comment', 2)
					mode, text = process_oneline(MD_TEXT, segments[1])
					line = '>'.join([segments[0], text])
			else:
				# convert '&', '<'( and '>' ('&' first).
				if has_one_of('&<>', line):
					__verbose('convert to html escape', 2)
				line = line.replace('&lt;', '==ESCAPEsLT==')
				line = line.replace('&gt;', '==ESCAPEsGT==')
				line = line.replace('&', '&amp;')
				line = line.replace('==ESCAPEsGT==', '&gt;')
				line = line.replace('==ESCAPEsLT==', '&lt;')
				line = line.replace('<', '&lt;')
				line = line.replace('>', '&gt;')
		# end if
	else:
		# mode is MD_TEXT
		__verbose('process: TEXT', 2)
		if line.find(BGN_MATH) >= 0:
			__verbose('process: TEXT: bgn_math FOUND', 2)
			__verbose('split: [TEXT, MATH]', 2)
			segments = split(line, BGN_MATH)
			mode1, text1 = process_oneline(MD_TEXT, segments[0])
			mode2, text2 = process_oneline(MD_MATH, segments[1])
			line = BGN_MATH.join([text1, text2])
			__verbose('returned: mode1 %s, mode2 %s' \
					% (mode_str(mode1), mode_str(mode2)), 2)
			mode = mode2
		elif line.find(BGN_DMATH) >= 0:
			__verbose('process: TEXT: bgn_dmath FOUND', 2)
			__verbose('split: [TEXT, DMATH]', 2)
			segments = split(line, BGN_DMATH)
			mode1, text1 = process_oneline(MD_TEXT, segments[0])
			mode2, text2 = process_oneline(MD_DMATH, segments[1])
			line = BGN_DMATH.join([text1, text2])
			__verbose('returned: mode1 %s, mode2 %s' \
					% (mode_str(mode1), mode_str(mode2)), 2)
			mode = mode2
		elif line.find(BGN_TABLE) >= 0:
			__verbose('process: TEXT: bgn_table FOUND', 2)
			__verbose('split: [TEXT, TABLE]', 2)
			segments = split(line, BGN_TABLE)
			mode1, text1 = process_oneline(MD_TEXT, segments[0])
			mode2, text2 = process_oneline(MD_TABLE, segments[1])
			line = BGN_TABLE.join([text1, text2])
			__verbose('returned: mode1 %s, mode2 %s' \
					% (mode_str(mode1), mode_str(mode2)), 2)
			mode = mode2
		elif line.find(BGN_VERBATIM) >= 0:
			__verbose('process: TEXT: bgn_verbatim FOUND', 2)
			__verbose('split: [TEXT, VERBATIM]', 2)
			segments = split(line, BGN_VERBATIM)
			end_environ = END_VERBATIM
			mode1, text1 = process_oneline(MD_TEXT, segments[0])
			mode2, text2 = process_oneline(MD_VERBATIM, segments[1])
			line = BGN_VERBATIM.join([text1, text2])
			__verbose('returned: mode1 %s, mode2 %s' \
					% (mode_str(mode1), mode_str(mode2)), 2)
			mode = mode2
		elif line.find(BGN_SRCCODE) >= 0:
			__verbose('process: TEXT: bgn_srccode FOUND', 2)
			__verbose('split: [TEXT, SRCCODE]', 2)
			segments = split(line, BGN_SRCCODE)
			end_environ = END_SRCCODE
			mode1, text1 = process_oneline(MD_TEXT, segments[0])
			mode2, text2 = process_oneline(MD_VERBATIM, segments[1])
			line = BGN_SRCCODE.join([text1, text2])
			__verbose('returned: mode1 %s, mode2 %s' \
					% (mode_str(mode1), mode_str(mode2)), 2)
			mode = mode2
		else:
			__verbose('process: TEXT: still in text mode', 2)
			# convert '&', '<'( and '>' ('&' first).
			if has_one_of('&<>', line):
				__verbose('convert to tex macro', 2)
			line = line.replace(r'\&', '=ESCAPEx26=')
			line = line.replace('&', r'\&')
			line = line.replace('=ESCAPEx26=', r'\&')
			line = line.replace('<', r'\textless ')
			line = line.replace('>', r'\textgreater ')
		# end if
	# end if

	__verbose('OUT: [%s]' % line.strip())
	indent.dec(2)
	return mode, line

def has_one_of(chars, string):
	for s in chars:
		if string.find(s) >= 0:
			return True
	return False

def mode_str(mode):
	if mode is MD_TEXT:	return 'TEXT'
	if mode is MD_MATH:	return 'MATH'
	if mode is MD_DMATH:	return 'DMATH'
	if mode is MD_VERBATIM:	return 'VERBATIM'

#  1ファイル分の処理を行なう。
#
def process_onefile(ifname):
	global end_environ
	end_environ = None
	print(ifname)

	ofname = mktmpfname(ifname, 'tmp')
	outf = open(ofname, 'w', encoding='utf-8')
	mode = MD_TEXT
	for line in open(ifname, 'r', encoding='utf-8'):
		mode, line = process_oneline(mode, line)
		outf.write(line)
	outf.close()
	os.remove(ifname)
	os.rename(ofname, ifname)

#  作業ファイル名を作成する
#
def mktmpfname(basename, suffix):
	tmpfname = '%s.%s' % (basename, suffix)
	if os.path.exists(tmpfname):
		return mktmpfname(tmpfname, suffix)
	return tmpfname

# ----------------------------------------------------------------------
#  Main process.
#
print('%s: start' % prog)
for fn in fnames:
	process_onefile(fn)

# ----------------------------------------------------------------------
#  End of process.
#
print('%s: done' % prog)
sys.exit(0)

# end: html_escape.py

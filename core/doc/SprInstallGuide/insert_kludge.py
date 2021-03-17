#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  SYNOPSIS:
#	insert_kludge
#
#  DESCRIPTION:
#	Insert 'kludge' before every location where character code
#	changes from 'ascii' to another multi-byte one.
#	File names to be processed is read from '\input{...}' lines
#	in the 'chapters.tex' file.
#
#  VERSION:
#	Ver 1.0  2018/10/30 F.Kanehori	First release version.
#	Ver 1.1  2018/11/20 F.Kanehori	Add exclusion code for \url{...}.
#	Ver 1.11 2019/01/22 F.Kanehori	Correct comment line.
#	Ver 1.2  2019/01/29 F.Kanehori	Rewrite process_oneline().
# ======================================================================
version = '1.2'

import sys
import os
import re
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].replace(os.sep, '/').split('/')[-1].split('.')[0]
#
target_defs = {
	'file': 'chapters.tex',
	'patt': r'\\input\{([a-zA-Z0-9_\.]+)\}'
}
kludge_code = b'\\KLUDGE '
code_bs	= 92	# '\\'
code_ob	= 123	# '{'
code_cb	= 125	# '}'
code_u	= 117	# 'u'
code_r	= 114	# 'r'
code_l	= 108	# 'l'
code_as	= 42	# '*'
#
MD_TEXT	    = 1
MD_VERBATIM = 2
MD_SRCCODE  = 3
#
BGN_VERBATIM	= r'\begin{verbatim}'
END_VERBATIM	= r'\end{verbatim}'
BGN_SRCCODE	= r'\begin{sourcecode}'
END_SRCCODE	= r'\end{sourcecode}'

# ----------------------------------------------------------------------
#  Globals
#
global end_environ		# VERBATIM or SRCCODE

# ----------------------------------------------------------------------
#  Options
#
usage = 'Usage: %prog [options]'
parser = OptionParser(usage = usage)
parser.add_option('-s', '--save-orginal', dest='save_original',
			action='store_true', default=False,
			help='save original file')
parser.add_option('-U', '--exclude-url', dest='exclude_url',
			action='store_true', default=False,
			help='exclude inserting in \\url{...}')
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
if len(args) != 0:
	parser.error("incorrect number of arguments")

# get options and input file name
verbose = options.verbose

# ----------------------------------------------------------------------
# for verbose
#
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
#  処理の対象となるファイルを"chapters.tex"から読む
#
def read_targets():
	if verbose:
		print('file: %s' % target_defs['file'])
		print('patt: %s' % target_defs['patt'])
	fname = target_defs['file']
	fnames = [fname]
	for line in open(fname, 'r', encoding='utf-8'):
		print('LINE: [%s]' % line.strip())
		m = re.match(target_defs['patt'], line)
		if m:
			fname = m.group(1)
			if fname[-4:] != '.tex':
				fname += '.tex'
			fnames.append(fname)
			print('   m: [%s]' % fname)
	if verbose:
		print('targets: %s' % fnames)
	return fnames

#  指定されたファイルを処理する
#
def process_onefile(ifname, ofname):
	global end_environ
	end_environ = None

	outf = open(ofname, 'w', encoding='utf-8')
	mode = MD_TEXT
	for line in open(ifname, 'r', encoding='utf-8'):
		mode, line = process_oneline(mode, line)
		outf.write(line)
	outf.close()

#  指定された文字列(1行分)にkludgeコードを挿入する
#
def process_oneline(mode, line):
	global end_environ
	indent.inc(2)
	__verbose(' in: [%s]' % line.strip(), 2)
	if options.verbose:
		line_org = line
	#
	if mode is MD_VERBATIM:
		__verbose('process: VERBATIM', 2)
		if end_environ is None:
			print('%s: Panic: no environment was detected so far' % prog)
			sys.exit(1)
		if line.find(end_environ) >= 0:
			__verbose('process: VERBATIM: end_environ FOUND', 3)
			__verbose('split: [VERBATIM, TEXT]', 3)
			segments = split(line, end_environ)
			mode1, text1 = process_oneline(MD_VERBATIM, segments[0])
			mode2, text2 = process_oneline(MD_TEXT, segments[1])
			line = end_environ.join([text1, text2])
			__verbose('returned: mode1 %s, mode2 %s' \
					% (mode_str(mode1), mode_str(mode2)), 3)
			mode = mode2
		else:
			__verbose('process: VERBATIM: still in verbatim mode', 3)
			__verbose('nothing to do', 3)
			pass

	else:
		# mode is TEXT
		__verbose('process: TEXT', 2)
		if line.find(BGN_VERBATIM) >= 0:
			__verbose('process: TEXT: bgn_verbatim FOUND', 3)
			__verbose('split: [TEXT, VERBATIM]', 3)
			segments = split(line, BGN_VERBATIM)
			end_environ = END_VERBATIM
			mode1, text1 = process_oneline(MD_TEXT, segments[0])
			mode2, text2 = process_oneline(MD_VERBATIM, segments[1])
			line = BGN_VERBATIM.join([text1, text2])
			__verbose('returned: mode1 %s, mode2 %s' \
					% (mode_str(mode1), mode_str(mode2)), 3)
			mode = mode2
		elif line.find(BGN_SRCCODE) >= 0:
			__verbose('process: TEXT: bgn_srccode FOUND', 3)
			__verbose('split: [TEXT, SRCCODE]', 3)
			segments = split(line, BGN_SRCCODE)
			end_environ = END_SRCCODE
			mode1, text1 = process_oneline(MD_TEXT, segments[0])
			mode2, text2 = process_oneline(MD_VERBATIM, segments[1])
			line = BGN_SRCCODE.join([text1, text2])
			__verbose('returned: mode1 %s, mode2 %s' \
					% (mode_str(mode1), mode_str(mode2)), 3)
			mode = mode2
		else:
			__verbose('process: TEXT: still in text mode', 3)

			# bytes型に変換して処理を行なう
			byte = line.encode('utf-8')
			size = len(list(byte))	# byte数
			__verbose('utf8: %s' % byte, 3)
		
			# 各文字を表すutf8のバイト数を求める
			mark = []
			ix = 0
			while ix < size:
				leng = codelen(byte[ix])
				mark.append(leng)
				ix += 1
				for n in range(leng-1):
					mark.append(leng)
					ix += 1
			__verbose('mark: %s' % mark, 3)
			__verbose('  %s' % line.replace('\n', ''), 3)
			__verbose('mark before: %s' % mark, 3)
		
			if options.exclude_url and line.find('\\url') >= 0:
				# \url{...} が見つかった
				# オプション -U が指定されたらこの中にはKLUDGEを入れない
				ix = 0
				while ix < size:
					ps, pe = locate_url(byte, size, ix)
					if ps > 0:
						__verbose('\\url found at (%d, %d)'\
							% (ps, pe), 3)
						while ps <= pe:
							mark[ps] = 1
							ps += 1
					ix += 1
			__verbose('mark after:  %s' % mark, 3)
			__verbose('', 3)
		
			# マルチバイトに変わる場所にKLUDGEを入れる
			bout = bytearray()
			prev = 0
			ix = 0
			while ix < size:
				curr = mark[ix]
				if curr != prev and curr > 1:
					bout.extend(kludge_code)
					__verbose('kludge inserted at %d' % ix, 2)
				bout.append(byte[ix])
				prev = curr
				ix += 1
			line = bout.decode('utf-8')
		# end if line.find()
	# end if mode is TEXT

	__verbose('out: [%s]' % line.strip(), 2)
	if options.verbose and line.strip() != line_org.strip():
		__verbose(' IN: [%s]' % line_org.strip())
		__verbose('OUT: [%s]' % line.strip())
	indent.dec(2)
	return mode, line

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

#  指定されたコードの中にTeXマクロ \ur{...} があるか調べてその範囲を返す
#
def locate_url(code, size, ix):
	pos = (-1, -1)
	if code[ix+0] == code_bs and code[ix+1] == code_u  and \
	   code[ix+2] == code_r  and code[ix+3] == code_l  and \
	   code[ix+4] != code_as:
		ix += 5
		ix_s = ix
		while ix < size:
			if code[ix] == code_cb:
				break
			ix += 1
		pos = (ix_s, ix)
	return pos

#  先頭コードを見てこの文字が何バイトで構成されているかを判断する(utf8)
#
def codelen(code):
	nb = 1
	if   code >= 240: nb = 4
	elif code >= 224: nb = 3
	elif code >= 192: nb = 2
	return nb

#  モード表示文字列
#
def mode_str(mode):
	if mode is MD_TEXT:	return 'TEXT'
	if mode is MD_VERBATIM:	return 'VERBATIM'
	if mode is MD_SRCCODE:	return 'SRCCODE'
	return ''

# ----------------------------------------------------------------------
#  Main process.
#
fnames = read_targets()
if verbose:
	print('inserting...')
for fn in fnames:
	ifn = fn + '.org'
	ofn = fn
	if not os.path.exists(ifn):
		os.rename(ofn, ifn)	# fn -> fn.org
	if verbose:
		print('  %s' % fn)
	process_onefile(ifn, ofn)
	if options.save_original:
		os.remove(ifn)		# remove

# ----------------------------------------------------------------------
#  End of process.
#
if verbose:
	print('%s: done' % prog)
sys.exit(0)

# end: insert_kludge.py

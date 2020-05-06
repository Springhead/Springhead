#!/usr/local/bin/python3.4
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:
#	TextFio(Fio)
#	    Text file I/O wrapper class.
#
#  INITIALIZER:
#	obj = TextFio(path, mode='r', encoding='utf8', size=8192, 
#				nl=None, verbose=0)
#	  arguments:
#	    path:	File path to open (str) or system stream object
#			(sys.stdin|sys.stdout|sys.stderr).
#	    mode:	File open mode ('r'|'w'|'a')
#	    encoding:	Character encoding (write open only).
#			    'ascii'	   (or 'us-ascii')
#			    'utf-8'	   (or 'utf_8', 'utf8')
#			    'cp932'	   (or 'shift_jis', 'sjis')
#			    'utf-16'	   (or 'unicode', 'utf16')
#			    'iso-2022-jp'  (or 'jis')
#			    'euc-jp'	   (or 'euc')
#	    size:	Read buffer size for encoding check (int).
#	    nl:		Newline code (str).
#	    verbose:	Verbose mode (int).
#
#  CLASS CONSTANTS:
#	ELIM = 0	Filter name: Eliminate comments.
#	WRAP = 1	Filter name: Wrap continuation lines.
#	NUMB = 0	Index of line data list: Line number
#	DATA = 1	Index of line data list: Line data
#
#  METHODS:
#	status = open()
#	    Open the file.
#	  returns:	0: suaccess, -1: failure
#
#	truncate(size=0)
#	    Truncate the file to specified amount.
#	  arguments:
#	    size:	Size to truncate (int).
#
#	obj = add_filter(name, [opts]=[])
#	    Add filter which will be applied in reading the file.
#	  arguments:
#	    name:	Filter name.  One of TextFio.ELIM or TextFio.WRAP.
#	    opts[]:	Options of the filter specified by 'name'.
#			ELIM: opts[0]: comment designator ('#')
#			      opts[1]: eliminate empty lines or not (True)
#			WRAP: opts[0]: line continuation symbol ('\\')
#	  returns:	Self object.
#
#	lines[] = read(maxlines=sys.maxsize)
#	    Read all lines from the file (filters are applied here).
#	  arguments:
#	    maxlines:	Number of maxmum lines to be read in.
#	  returns:	List of line data (list of str).
#
#	replaced, count = replace(patterns, lines=self.lines)
#	    Replace string in the lines.
#	  arguments:
#	    patterns:	List of pattern trio ([[from, to, search], ...]).
#	      from:	Pattern to replace from (str)
#			Used as search pattern if 'search' is omitted.
#	      to:	Pattern to replace to (str).
#	      search:	Pattern to search (re) (optional).
#	    lines:	Original line data (str or line info structure).
#	  returns:
#	    replaced:	Replaced line data (str or line info structure).
#	    count:	Number of replaced lines (int).
#
#	info[] = lineinfo()
#	    Get list of line info.  Must call read() first.
#	  returns:	List of line info structure as follows;
#			info structure: [line#(int), line_data(str)].
#
#	set_lines(lines)
#	    Set list of string to self.lines to write out to the file.
#	  arguments:
#	    lines[]:	List of string (NOT line info structure).
#
#	encoding = get_encoding()
#	    Get currently opened file encoding.
#	  returns:	Encoding string ('utf-8'|'cp932'|...).
#
#	status = writelines(lines=None, nl=None)
#	    Write lines data to the file.
#	  arguments:
#	    lines[]:	List of string or line info structure.
#			e.g. [str, ...] or [[int, str], ...]
#	    nl:		Newline code ('\n' or '\r\n').
#	  returns:	0 if successfully written, otherwise -1.
#
#	status = writeline(line, nl=None)
#	    Write one line data to the file.
#	  arguments:
#	    lines:	(str)
#	    nl:		Newline code ('\n' or '\r\n').
#	  returns:	0 if successfully written, otherwise -1.
#
#	flush()
#	    Flushes the file.
#
#	close()
#	    Close the file.
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2016/11/06 F.Kanehori	Release version.
#	Ver 1.1  2017/01/11 F.Kanehori	Interface changed: read()
#					Added: writeline()
#	Ver 1.2  2017/01/12 F.Kanehori	Now sys.std{in|out|err} is OK.
#	Ver 1.3  2017/04/06 F.Kanehori	Newline code depends os.
#	Ver 2.0  2017/04/10 F.Kanehori	Ported to unix.
#	Ver 2.1  2017/09/13 F.Kanehori	Add flush().
# ======================================================================
import sys
import io
import re
import codecs
import inspect
from Fio import *
from Util import *

class TextFio(Fio):
	#  Class constants
	#
	ELIM = 0	# filter name: eliminate comments
	WRAP = 1	# filter name: wrap continuation lines
	#
	NUMB = 0	# index of lines: line number
	DATA = 1	# index of lines: line data

	#  Encoding name dictionary
	#
	ENCODINGS = {
		'ascii':	'ascii',
		'us-ascii':	'ascii',
		'utf-8':	'utf-8',
		'utf_8':	'utf-8',
		'utf8':		'utf-8',
		'cp932':	'cp932',
		'shift_jis':	'cp932',
		'sjis':		'cp932',
		'utf-16':	'utf-16',
		'unicode':	'utf-16',
		'utf16':	'utf-16',
		'iso-2022-jp':	'iso-2022-jp',
		'jis':		'iso-2022-jp',
		'euc-jp':	'euc-jp',
		'euc':		'euc-jp'
	}

	#  Class initializer
	#
	def __init__(self, path, mode='r', encoding=None, size=8192,
		     nl=None, verbose=0):
		#
		self.clsname = self.__class__.__name__
		self.version = 2.1
		#
		disp_mode = 'r' if mode == 'r' else mode	# for error msg
		super().__init__(path, mode, disp_mode=disp_mode, verbose=verbose)
		self.mode = mode	# override
		self.encoding = encoding
		self.size = size
		if mode in ['r', 'rb', 'wb'] and encoding is not None:
			self.encoding = None
		elif encoding is not None:
			self.encoding = self.ENCODINGS[encoding]
		#
		self.filters = []
		self.lines = []
		#  Wao!
		#  Do not use os.linesep as a line terminator when writing files
		#  opened in text mode (the default); use a single '\n' instead,
		#  on all platforms.
		#self.nl = nl if nl is not None else os.linesep
		self.nl = nl if nl is not None else '\n'

	#  Open
	#
	def open(self):
		org_mode = self.mode
		if self.mode == 'r':
			# encoding check (decode) requires 'bytes' type
			self.mode = 'rb'
		status = super().open(encoding=self.encoding)
		self.mode = org_mode
		if status < 0:
			return status
		#
		if self.sys_stream:
			self.encoding = sys.getdefaultencoding()
		elif self.mode == 'r':
			self.encoding = self.__check_encoding()
		elif self.mode == 'rb':
			self.encoding = None
		if self.verbose:
			print('"%s": encoding: %s' % (self.name, self.encoding))
		return status

	#  Trauncate
	#
	def truncate(self, size=0):
		self.obj.truncate(size)

	#  Set filters.
	#
	def add_filter(self, name, opts=[]):
		filter_names = [self.ELIM, self.WRAP]
		if name in filter_names:
			self.filters.append([name, opts])
		else:
			msg = 'invalid filter name "%s"' % name
			self.errmsg = msg
		if self.verbose:
			repl = name
			if name == self.ELIM: repl = 'ELIM'
			if name == self.WRAP: repl = 'WRAP'
			print('add_filter: %s, opts=%s' % (repl, opts))
		return self

	#  Read entire lines (newline stripped).
	#
	def read(self, maxlines=sys.maxsize):
		if not self.opened:
			msg = 'file "%s" is not opend' % self.path
			self.errmsg = msg
			return []
		#
		self.lines = []
		count = 0
		#
		try:
			data = self.obj.read()
			if isinstance(data, str):
				# case: system stream
				data = data.encode(self.encoding)
		except IOError as err:
			msg = 'file read error: "%s" (line %d)\n%s' \
					% (self.path, count, err)
			self.errmsg = msg
			return self.lines
		#
		lines = data.decode(self.encoding).split('\n')[0:-1]
		for line in lines:
			count += 1
			line = line.rstrip()
			if self.verbose > 1:
				print('%3d: %s' % (count, line))
			self.lines.append([count, line])
			if count >= maxlines:
				print('break')
				break
		if self.verbose:
			print('"%s": %d lines read' % (self.name, len(lines)))
		#
		for name, opts in self.filters:
			if name == self.ELIM:
				self.__elim_comments(opts)
			if name == self.WRAP:
				self.__wrap_lines(opts)
		#
		return list(map(lambda x: x[self.DATA], self.lines))

	#  Replace string in the lines.
	#
	def replace(self, patterns, lines=None):
		if lines is None:
			lines = self.lines
		replaced = []
		count = 0
		for line in lines:
			if isinstance(line, list):
				numb, data = line
				repl, c = self.replace_sub(patterns, data)
				data = [numb, repl]
			else:
				data, c = self.replace_sub(patterns, line)
			replaced.append(data)
			count += c
		return replaced, count

	def replace_sub(self, patts, data):
		if not isinstance(patts, list):
			return data
		patts = patts if isinstance(patts[0], list) else [patts]
		replaced = data
		for patt in patts:
			patt_s = patt[2] if len(patt) ==3 else patt[0]
			patt_f = patt[0]	# form
			patt_t = patt[1]	# to
			m = re.search(patt_s, replaced)
			if m:
				replaced = replaced.replace(patt_f, patt_t)
		changed = 1 if replaced != data else 0
		return replaced, changed

	#  Get list of line info.
	#
	def lineinfo(self):
		return self.lines

	#  Set lines (convert list of str into list of line info).
	#
	def set_lines(self, lines):
		count = 0
		for line in lines:
			count += 1
			self.lines.append([count, line])
		if self.verbose > 1:
			for x in self.lines:
				print('set_lines: %s' % x)

	#  Return detecte character encoding of this file.
	#
	def get_encoding(self):
		return self.encoding

	#  Write lines
	#
	def writelines(self, lines=None, nl=None):
		if lines is None:
			lines = self.lines
		count = 0
		if isinstance(lines, str):
			if self.writeline(lines, nl) < 0:
				return -1
			count += 1
		else:
			for line in lines:
				if isinstance(line, list):
					data = line[self.DATA]
				else:
					data = line
				if self.writeline(data, nl) < 0:
					return -1
				count += 1
		#
		if self.verbose:
			print('"%s": %d line(s) written' % (self.name, count))
		return 0

	#  Write one line
	#
	def writeline(self, line, nl=None):
		if nl is None:
			nl = self.nl
		try:
			self.obj.write(line + nl)
		except IOError:
			msg = 'file write error: "%s"' % self.path
			self.errmsg = msg
			return -1
		return 0

	#  Flush
	#
	def flush(self):
		if self.opened:
			self.obj.flush()

	#  Close
	#
	def close(self):
		if self.opened:
			self.obj.close()
		self.obj = None
		self.opened = False
		self.encoding = None

	# --------------------------------------------------------------
	#  For class private use
	# --------------------------------------------------------------

	#  Determine the character encoding of this file.
	#
	def __check_encoding(self):
		# arguments:
		#   path:	File path to be checked.
		#   size:	Buffer size for checking.
		# returns:	Determined character encoding.

		lookup = ['iso-2022-jp', 'ascii', 'euc-jp',
			  'utf-8', 'utf-16', 'cp932']
			  #'utf-8-sig', 'utf-16', 'cp932']
		try:
			f = open(self.path, 'rb')
			data = f.read(self.size)
			f.close()
		except:
			return None
		encoding = None
		for enc in lookup:
			try:
				data.decode(enc)
				encoding = enc
				break
			except:
				pass
		return encoding

	#  Wrap continued lines.
	#
	def __wrap_lines(self, opts=['\\']):
		# arguments:
		#   symbol:	Line continuation symbol.

		pat = '(.*)(.)\s*$'	# last symbol of the line
		symbol = opts[0] if opts and len(opts) > 0 else '\\'
		lines = []
		count = 0
		prev_line = ''
		for n, data in self.lines:
			if prev_line == '':
				numb = n
			m = re.match(pat, data)
			if m and m.group(len(m.groups())) == symbol:
				prev_line += m.group(1).strip() + ' '
				continue
			else:
				data = prev_line + data.strip()
				prev_line = ''
			lines.append([numb, data])
			count += 1
		#
		if self.verbose:
			func = inspect.currentframe().f_code.co_name
			info = '%s: %d lines left' % (func, count)
			print('apply filter: %s' % info)
		self.lines = lines
	
	#  Eliminate both comment lines and inline commnet.
	#
	def __elim_comments(self, opts=['#', True]):
		# arguments:
		#   opts:
		#     [0] designator:	Comment designator.
		#     [1] emptyline:	Eliminate empty lines or not.
		# returns:	Self object.

		designator = opts[0] if opts and len(opts) > 0 else '#'
		emptyline  = opts[1] if opts and len(opts) > 1 else True
		designator = designator.replace('*', '\*')
		designator = designator.replace('.', '\.')
		#
		pat_e = '^\s*$'			  # empty line
		pat_c = '^\s*%s' % designator	  # comment line
		pat_i = '(.*)\s*%s' % designator  # inline comment
		lines = []
		for numb, data in self.lines:
			if re.match(pat_e, data) and emptyline:	continue
			if re.match(pat_c, data):		continue
			m = re.match(pat_i, data)
			if m:
				lines.append([numb, m.group(1).rstrip()])
			else:
				lines.append([numb, data])
		#
		if self.verbose:
			func = inspect.currentframe().f_code.co_name
			info = '%s: %d lines left' % (func, len(lines))
			print('apply filter: %s' % info)
		self.lines = lines

# end: TextFio.py

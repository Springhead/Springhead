#!/usr/local/bin/python3.4
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:
#	KvFile
#	    Class for making dictionary from key-value pair file.
#
#  INITIALIZER:
#	obj = KvFile(path, sep=None, overwrite=True, desig='#', verbose=0)
#	  arguments:
#	    path:	Key-value pair file path (str).
#	    sep:	Separator of key and value (str, len=1).
#	    overwrite:	Overwrite previous value if key duplicates (bool).
#			If False, make list of values for the same key.
#	    desig:	Comment designator (str, len=1).
#	    verbose:	Verbose level (int).
#
#  METHODS:
#	nsections = read(dic=None)
#	  Read the file named 'path' and construct a dictionary.
#	  arguments:
#	    dic:	Dictionary having predefined key-value pair.
#			If 'dic' has no sections, all k-v pairs are
#			assumed to belong default section.
#	  returns:	Number of sections in the dictionary (int).
#			Note: Default section is not counted.
#
#	set(key, value, section=None)
#	  Add arbitraly key-value pair to the dictionary.
#	  arguments:
#	    key:	Key to add the dictionary (str).
#	    value:	Value associated with the key (obj).
#	    section:	Section name to add key-value pair (str).
#			Default section is assumed if no section is given.
#
#	sections_list = sections()
#	  Get section name list defined in the dictionary excluding
#	  default section name 'default'.
#	  returns:	List of section names (str[]).
#
#	change_section(name)
#	  Set current section name.
#	  arguments:
#	    name:	section name (str).
#
#	value = get(key, section=None)
#	  Get value associated with the key.  Key is searched by
#	  following order:
#		(1) in the section specified by argument.
#		(2) in the section specified by section() method.
#		(3) in the default section.
#	  arguments:
#	    key:	Key to get value from the dictionary (str).
#	    section:	Section name in which key is searched (str).
#	  returns:	Value associated with the key (obj).
#
#	keys = keys(section=None)
#	  Get all keys defined in the current section and defaule
#	  section.  Current section is determined by following order:
#		(1) if present, specified section by the argument.
#		(2) otherwise, specified section by section() method.
#		(3) if both none, empty.
#	  arguments:
#	    section:	Section name (str).
#	  returns:	List of keys (str[]).
#
#	result = check(keys, section=None)
#	  Check if all of specified keys are defiend in the dictionary.
#	  If argument 'section' is specifed, check is limited to the
#	  specified section and default section.  Method section() has
#	  no effect in this method.
#	  arguments:
#	    section:	Section name (str).
#	    keys:	List of keys to check (str[]).
#	  returns:	0: all are in the dictionary, -1: else
#
#	show(line_by_line=0, section=None)
#	  Show contents of the dictionary.  Argument 'section' has the
#	  same meaning as check() method.
#	  arguments:
#	    line_by_line:	>= 0: Number of preceeding spaces.
#				      (line-by-line mode)
#				else: Use system format.
#	    section:	Section name (str).
#
#	errmsg = error()
#	  returns:	Error message (most recent one only) (str).
#	
#  VERSION:
#	Ver 1.0  2016/06/13 F.Kanehori	Release version.
#	Ver 2.0  2017/04/10 F.Kanehori	Ported to unix.
#	Ver 3.0  2017/12/06 F.Kanehori	Section construction introduced.
# ======================================================================
import sys
import re
from TextFio import *
from Util import *

class KvFile:
	#  Class constant
	#
	DEFAULT = '__default__'		# default section name

	#  Initializer
	#
	def __init__(self, path, sep=None, overwrite=True, desig='#', verbose=0):
		self.clsname = self.__class__.__name__
		self.version = 3.0
		#
		self.sep = sep
		self.overwrite = overwrite
		self.desig = desig
		self.verbose = verbose
		#
		self.defined_sections = [self.DEFAULT]
		self.curr_section = self.DEFAULT
		self.dict = {}
		self.fobj = TextFio(path, verbose=verbose)
		self.errmsg = None

	#  Read file and make dictionary.
	#
	def read(self, dic=None):
		f = self.fobj
		if f.open() < 0:
			self.errmsg = f.error()
			return -1
		f.add_filter(f.WRAP)
		f.add_filter(f.ELIM)
		f.read()
		lines = f.lineinfo()
		f.close()
		if lines is None:
			return 0
		dic = self.__initial_dict(dic)
		self.dict = self.__make_dict(lines, dic)
		return len(self.defined_sections) - 1
		
	#  Add new key-value-pair to the dictionary.
	#
	def set(self, key, value, section=None):
		if section is None:
			section = self.DEFAULT
		if not section in self.defined_sections:
			self.dict[section] = {}
			self.defined_sections.append(section)
		self.dict[section][key] = value

	#  Get section name list defined in the dictionary.
	#
	def sections(self):
		return self.defined_sections[1:]

	#  Set current section name.
	#
	def change_section(self, name):
		self.curr_section = name

	#  Get the value associated with the key.
	#
	def get(self, key, section=None):
		if section is None:
			section = self.curr_section
		if key in self.dict[section]:
			return self.dict[section][key]
		if key in self.dict[self.DEFAULT]:
			return self.dict[self.DEFAULT][key]
		return None

	#  Get list of the keys in the dictionary.
	#
	def keys(self, section=None):
		if section is None:
			section = self.curr_section
		if not section in list(self.dict.keys()):
			section = self.DEFAULT
		keylist = list(self.dict[section].keys())
		if section != self.DEFAULT:
			# merge keys in default section
			tmp_keys = list(self.dict[self.DEFAULT].keys())
			for k in tmp_keys:
				if not k in keylist:
					keylist.append(k)
		return keylist

	#  Check if all of specified keys are defiend.
	#
	def check(self, keys, section=None):
		if section is None:
			sections = self.defined_sections
		else:
			sections = [section, self.DEFAULT]
		missing = None
		for s in sections:
			for key in keys:
				val = self.get(key, section=s)
				if val is None:
					missing = key
					break
			if missing:
				break
		if missing:
			self.errmsg = "required key '%s' missing" % missing
			return -1
		return 0

	#  Show the contents of dictionary.
	#
	def show(self, line_by_line=0, section=None):
		if section is None:
			sections = self.sections()
			if sections == []:
				sections = [self.DEFAULT]
		else:
			sections = [section]
		wid = self.__max_wid(sections, line_by_line) + 1
		indent = ' ' * line_by_line
		for s in sections:
			disp_dict = self.__make_disp_dict(s)
			if s != self.DEFAULT:
				print('[%s]' % s)
			if line_by_line:
				for key in sorted(disp_dict):
					key_part = indent + key
					kp = key_part.ljust(wid, ' ')
					vp = str(disp_dict[key])
					print('%s\t%s' % (kp, vp))
			else:
				print(disp_dict)

	#  Error message adapter.
	#
	def error(self):
		return self.errmsg


	# --------------------------------------------------------------
	#  For class private use
	# --------------------------------------------------------------

	#  Make unsectioned kv-pair belong to default section.
	#
	def __initial_dict(self, dic):
		# arguments:
		#   dic:	Given dictionary (may be empty) (dict).
		# returns:	Initial dictionary (dict).

		if dic is None or len(dic) <= 0:
			return None
		k = list(dic.keys())[0]		# first key
		if isinstance(dic[k], dict):
			# assume 'dic' is right form dictionary
			return dic
		newdic = {self.DEFAULT: {}}
		for k,v in dic.items():
			newdic[self.DEFAULT][k] = v
		if self.verbose:
			print('initial dictionary: %s' % newdic)
		return newdic

	#  Make dictionary.
	#
	def __make_dict(self, lines, dic):
		# arguments:
		#   lines:	Line data read from the file (list).
		# returns:	Dictionary (dict).

		dict = dic if dic is not None else {self.DEFAULT: {}}
		section = self.curr_section
		for numb, line in lines:
			# %include
			m = re.match('^%include\s+([\w/\\.]+)', line)
			if m:
				if self.verbose:
					print('%%include %s' % m.group(1))
				kvf = KvFile(Util.pathconv(m.group(1)),
					     sep=self.sep, desig=self.desig,
					     verbose=self.verbose)
				if kvf is None:
					msg = 'can\'t include "%s"' % m.group(1)
					self.errmsg = msg
					result = -1
				if kvf.read(dict) <= 0:
					if self.verbose:
						print('-> include file empty')
					continue
				for sec in kvf.sections():
					for key in kvf.keys(sec):
						dict[sec][key] = kvf.get(key, sec)
				continue

			# section designator
			m = re.match('^\[([^\s]+)\]', line)
			if m:
				section = m.group(1)
				if self.verbose:
					print('[%s]' % section)
				if not section in self.defined_sections:
					self.defined_sections.append(section)
					dict[section] = {}
				self.curr_section = section
				continue

			# key-value pair
			if self.sep is None:
				pair = line.rstrip().split()
			else:
				pair = line.rstrip().split(self.sep)
			if len(pair) < 2:
				self.__register(pair[0], True, dict)
			else:
				key = pair[0].strip()
				val = ' '.join(pair[1:]).strip()
				val = self.__expand(val, dict)
				self.__register(key, self.__convert(val), dict)
		return dict

	#  Expand macros.
	#  Only already registered keys are valid for macro expansion.
	#
	def __expand(self, str, dict):
		# arguments:
		#   str:	Original line data (str).
		#   dict:	Dictionary to lookup prior to self.dict.
		# returns:	Expanded line data (str).
		#		Same that input data if no macros met.

		section = self.curr_section
		m = re.match('([^\$]*)\$\(([^\)]+)\)(.*$)', str)
		if m:
			if self.verbose:
				group = [m.group(1), m.group(2), m.group(3)]
				msg = 'MATCH: \'' + m.group(0) + '\' => '
				msg += '[ ' + ', '.join(group) + ' ]'
				print(msg)
			if m.group(2) in dict[section]:
				val = dict[section][m.group(2)]
			elif m.group(2) in dict[self.DEFAULT]:
				val = dict[self.DEFAULT][m.group(2)]
			else:
				val = self.get(m.group(2), section)
			if val is not None:
				str = m.group(1) + val + m.group(3)
				str = self.__expand(str, dict)
		return str

	#  Register to the dictionary.
	#
	def __register(self, key, value, dict):
		# arguments:
		#   key:	Register key (str).
		#   value:	Value to be registered (str).
		#   dict:	Dictionary to register the value (dict).

		section = self.curr_section
		if self.overwrite:
			# override previous value (if exists).
			dict[section][key] = value
		else:
			# append to the list.
			if key in dict[section]:
				if isinstance(dict[section][key], list):
					dict[section][key].append(value)
				else:
					prev = dict[section][key]
					dict[section][key] = [prev, value]
			else:
				dict[section][key] = value

	#  Convert 'True'/'False' to corresponding boolean value.
	#
	def __convert(self, value):
		# arguments:
		#   value:	Any object (obj).
		# returns:	True/False if value is 'True'/'False'.
		#		Otherwise value itself.

		if value in ['True', 'False']:
			return True if value == 'True' else False
		return value

	#  Make dictionary for display.
	#
	def __make_disp_dict(self, section):
		# arguments:
		#   sections:	Name of targeted sections (str[]).
		# returns:	Dictionary for display (dict).

		disp_dict = {}
		keys = list(self.dict[section].keys())
		if section != self.DEFAULT:
			# merge keys in default section
			tmp_keys = list(self.dict[self.DEFAULT].keys())
			for k in tmp_keys:
				if not k in keys:
					keys.append(k)
		for key in keys:
			value = self.get(key, section)
			if isinstance(value, list):
				showlist = []
				for elem in value:
					elem = self.__disp_elem(elem)
					showlist.append(elem)
				disp_dict[key] = showlist
			else:
				disp_dict[key] = self.__disp_elem(value)
		return disp_dict

	#  Make display string for specified object.
	#
	def __disp_elem(self, value):
		# arguments:
		#   vale:	Any object (obj).
		# returns:	Display string (str).

		disp_elem = str(value)
		if isinstance(value, bool):
			disp_elem += ' <bool>'
		return disp_elem

	#  Find maximun key length.
	#
	def __max_wid(self, sections, offset=0):
		# arguments:
		#   sections:	Name of targeted sections (str[]).
		# returns:	Maximum key length (int).

		wid = 0
		for s in sections:
			keys = list(self.dict[s].keys())
			for k in keys:
				keylen = len(k) + offset
				if keylen > wid:
					wid = keylen
		return wid

# end: KvFile.py

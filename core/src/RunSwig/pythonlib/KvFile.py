#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:	KvFile(path, sep=None, overwrite=True, desig='#', verbose=0)
#
#  METHODS:
#	nsections = read(dic=None)
#	set(key, value, section=None)
#	sections_list = sections()
#	change_section(name)
#	value = get(key, section=None)
#	keys = keys(section=None)
#	result = check(keys, section=None)
#	show(line_by_line=0, section=None)
#	errmsg = error()
#	
# ----------------------------------------------------------------------
#  VERSION:
#     Ver 1.00   2016/06/13 F.Kanehori	Release version.
#     Ver 2.00   2017/04/10 F.Kanehori	Ported to unix.
#     Ver 3.00   2017/12/06 F.Kanehori	Section construction introduced.
#     Ver 3.01   2018/02/05 F.Kanehori	Bug fixed.
#     Ver 3.01.1 2018/03/12 F.Kanehori	Now OK for doxygen.
#     Ver 3.01.2 2018/05/07 F.Kanehori	Bug fixed.
#     Ver 3.02   2021/02/10 F.Kanehori	Can run on python 2.7.
# ======================================================================
from __future__ import print_function
import sys
import os
import re
from TextFio import *
from Util import *

##  Making dictionary from key-value pair file class.
#
class KvFile:
	##  Default section name.
	#
	DEFAULT = '__default__'

	##  The initializer.
	#   @param path		Path of key-value pair file (str).
	#   @param sep		Separator between key and value (str, len=1).
	#   @param overwrite	Overwrite previous value if key duplicates
	#			(bool).
	#   @n			If False, make list of values for the same key.
	#   @param desig	Comment designator (str, len=1).
	#   @param verbose	Verbose level (0: silent) (int).
	#
	def __init__(self, path, sep=None, overwrite=True, desig='#', verbose=0):
		self.clsname = self.__class__.__name__
		self.version = 3.1
		#
		self.path = path
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

	##  Read the file specified by 'path' and construct a dictionary.
	#   @param dic		Dictionary having predefined key-value pair.
	#   @n			If 'dic' has no sections, all key-value
	#			pairs are assumed to belong to the default
	#			section.
	#   @returns		Number of sections in the dictionary (int).
	#   @n			Note: Default section is not counted.
	#
	def read(self, dic=None):
		if self.verbose:
			abspath = Util.upath(os.path.abspath(self.path))
			print('%s: "%s"' % (self.clsname, abspath))
		f = self.fobj
		if f.open() < 0:
			self.errmsg = f.error()
			return -1
		f.add_filter(f.WRAP)
		f.add_filter(f.ELIM, opts=[self.desig, True])
		f.read()
		lines = f.lineinfo()
		f.close()
		if lines is None:
			if self.verbose:
				print('%s: empty' % self.clsname)
			return 0
		dic = self.__initial_dict(dic)
		self.dict = self.__make_dict(lines, dic)
		return len(self.defined_sections) - 1
		
	##  Add arbitraly key-value pair to the dictionary.
	#   @param key		Key to add the dictionary (str).
	#   @param value	Value associated with the key (obj).
	#   @param section	Section name to add key-value pair (str).
	#			Default section is assumed if no section
	#			is given.
	#
	def set(self, key, value, section=None):
		if section is None:
			section = self.DEFAULT
		if not section in self.defined_sections:
			self.dict[section] = {}
			self.defined_sections.append(section)
		self.dict[section][key] = value

	##  Get section name list defined in the dictionary (default section is excluded).
	#   @returns		List of section names ([str]).
	#
	def sections(self):
		return self.defined_sections[1:]

	##  Set current section name.
	#   @param name		New section name.
	#
	def change_section(self, name):
		self.curr_section = name

	##  Get the value associated with the key.
	#	 Key is searched by following order:
	#   @n		(1) in the section specified by argument.
	#   @n		(2) in the section specified by section() method.
	#   @n		(3) in the default section.
	#   @param key		Key to get value from the dictionary (str).
	#   @param section	Section name in which key is searched (str).
	#   @returns		Value associated with the key (obj).
	#
	def get(self, key, section=None):
		if section is None:
			section = self.curr_section
		if section in self.dict and key in self.dict[section]:
			return self.dict[section][key]
		if key in self.dict[self.DEFAULT]:
			return self.dict[self.DEFAULT][key]
		return None

	##  Get all keys defined in the current section and default section.
	#	Current section is determined by following order:
	#   @n		(1) if present, specified section by the argument.
	#   @n		(2) otherwise, specified section by section() method.
	#   @n		(3) if both none, empty.
	#   @param section	Section name (str).
	#   @returns		List of keys (str[]).
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

	##  Check if all of specified keys are defiend in the dictionary.
	#	If argument 'section' is specifed, check is limited to the
	#	specified section and default section.  Method section()
	#	has no effect in this method.
	#   @param section	Section name (str).
	#   @param keys:	List of keys to check (str[]).
	#   @retval 0		all are in the dictionary
	#   @retval -1		otherwise
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

	##  Show contents of the dictionary.
	#	Argument 'section' has the same meaning as check() method.
	#   @param line_by_line
	#			non-negative: Number of preceeding spaces.
	#			(line-by-line mode)
	#   @n			else: Use system format.
	#   @param section	Section name (str).
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
			if s not in self.dict:
				continue
			disp_dict = self.__make_disp_dict(s)
			print('[%s]' % s)
			if line_by_line:
				for key in sorted(disp_dict):
					key_part = indent + key
					kp = key_part.ljust(wid, ' ')
					vp = str(disp_dict[key])
					print(' %s\t%s' % (kp, vp))
			else:
				print(disp_dict)

	##  Error message adapter.
	#   @returns		Error message (most recent one only) (str).
	#
	def error(self):
		return self.errmsg


	# --------------------------------------------------------------
	#  For class private use
	# --------------------------------------------------------------

	##  Make unsectioned key-value pair (belong to default section).
	#   @param dic		Given dictionary (may be empty) (dict).
	#   @returns		Initial dictionary (dict).
	#
	def __initial_dict(self, dic):
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

	##  Make dictionary.
	#   @param lines	Line data read from the file (list).
	#   @param dic		Initial members to be registered (dict).
	#   @returns		Dictionary (dict).
	#
	def __make_dict(self, lines, dic):
		dict = dic if dic else {self.DEFAULT: {}}
		section = self.curr_section
		if self.verbose:
			print('[%s]' % section)
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
				dict = self.__register(section, pair[0], True, dict)
			else:
				key = pair[0].strip()
				val = ' '.join(pair[1:]).strip()
				val = self.__expand(val, dict)
				val = self.__convert(val)
				dict = self.__register(section, key, val, dict)
		return dict

	##  Expand macros.
	# NOTE
	# @n	Only already registered keys are valid for macro expansion.
	#   @param string	Original line data (str).
	#   @param dict		Dictionary to lookup prior to self.dict.
	#   @returns		Expanded line data (str).
	#   @n			Same that input data if no macros met.
	#   @returns		Expanded line data (str).
	#
	def __expand(self, string, dict):
		section = self.curr_section
		m = re.match('([^\$]*)\$\(([^\)]+)\)(.*$)', string)
		if m:
			if self.verbose > 1:
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
				string = m.group(1) + str(val) + m.group(3)
				string = self.__expand(string, dict)
		return string

	##  Register to the dictionary.
	#   @param section	Section name (str).
	#   @param key		Register key (str).
	#   @param value	Value to be registered (str).
	#   @param dict		Dictionary to register the value (dict).
	#   @returns		Processed dictionary.
	#
	def __register(self, section, key, value, dict):
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
		if self.verbose:
			print('    %s[%s] = %s' % (section, key, value))
		return dict

	##  Convert 'True'/'False' to corresponding boolean value.
	#   @param value	Boolean value.
	#   @retval True	If value is 'True'.
	#   @retval False	If value is 'False'.
	#   @retval 'value'	Otherwise.
	#
	def __convert(self, value):
		# arguments:
		#   value:	Any object (obj).
		# returns:	True/False if value is 'True'/'False'.
		#		Otherwise value itself.

		if value in ['True', 'False']:
			return True if value == 'True' else False
		return value

	##  Make dictionary for display.
	#   @param section	Name of targeted sections ([str]).
	#   @returns		Dictionary for display (dict).
	#
	def __make_disp_dict(self, section):
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

	##  Make display string for specified object.
	#   @param value:	Any object (obj).
	#   @returns		Display string (str).
	#
	def __disp_elem(self, value):
		disp_elem = str(value)
		if isinstance(value, bool):
			disp_elem += ' <bool>'
		return disp_elem

	##  Find maximun key length.
	#   @param sections	Name of targeted sections ([str]).
	#   @param offset	Offset value added to key length (int).
	#   @returns		Maximum key length (int).
	#
	def __max_wid(self, sections, offset=0):
		wid = 0
		for s in sections:
			if s not in self.dict:
				continue
			keys = list(self.dict[s].keys())
			for k in keys:
				keylen = len(k) + offset
				if keylen > wid:
					wid = keylen
		return wid

# end: KvFile.py

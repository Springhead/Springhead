#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:	SetupFile(path, verbose=0, register=False)
#
#  METHODS:
#	add_environment()	必要なパスを設定する
#	keys = get_key(section)	指定したセクションにある全キーを返す
#				section は PATH または DATA の何れか
#	value = get_path(key)	指定したキーの値(パス)を返す
#	value = get_data(key)	指定したキーの値(パス以外)を返す
#	show()			設定ファイルの内容を表示する
#
#	これ以下は register が True のときのみ使用可
#	add(key, value)		(キー, 値) の組を登録する
#	status = write()	登録されたデータを設定ファイルに書く
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.00  2020/11/01 F.Kanehori	Release version.
# ======================================================================
import sys
import os
from KvFile import *
from TextFio import *
from Util import *

##  Setup file manipulation class.
#
class SetupFile:
	##  Class constants
	#
	PATH	= 'path'
	DATA	= 'data'

	##  The initializer.
	#   @param path		Setup file path (str).
	#   @param verbose	Verbose level (0: silent) (int).
	#   @param register	Register mode flag (bool).
	#
	def __init__(self, path, verbose=0, register=False):
		self.clsname = self.__class__.__name__
		self.version = 1.00
		#
		self.path = path
		self.filename = Util.upath(path).split('/')[-1]
		self.verbose = verbose
		self.register = register
		#
		self.kvf = None
		self.path_sep = ':' if Util.is_unix() else ';'
		self.path_list = {}
		self.data_list = {}
		self.file_read = False

	##  Add registered paths to current path.
	#
	def add_environment(self):
		if not self.file_read:
			self.__read_file()
		#
		if len(self.path_list) <= 0:
			if self.verbose:
				print('no paths added')
			return
		#
		paths = []
		for prog in self.path_list:
			if self.path_list[prog] is None \
				or type(self.path_list[prog]) == bool:
				continue
			paths.append(Util.pathconv(self.path_list[prog]))
		add_path = self.path_sep.join(paths)
		if self.verbose:
			print('paths.ADD: [%s]' % add_path)
		#
		env = os.environ['PATH']
		os.environ['PATH'] = '%s:%s' % (add_path, env)
		if self.verbose:
			print('paths.ENV: [%s]' % os.environ['PATH'])
		return

	##  Get registered path associated with the prog.
	#   @section		SetpFile.PATH or SetupFile.DATA
	#   @returns		Keys in the section (array)
	#
	def get_keys(self, section):
		if not self.file_read:
			self.__read_file()
		return self.kvf.keys(section)

	##  Get registered path associated with the prog.
	#   @prog		Program name to get path (stirng)
	#   @returns		Registered path (string)
	#
	def get_path(self, prog):
		return self.__get_value(prog, self.PATH, 'get_path')

	##  Get registered data associated with the key.
	#   @key		Key to get daa (stirng)
	#   @returns		Registered data (string)
	#
	def get_data(self, key):
		return self.__get_value(key, self.DATA, 'get_data')

	##  Show registered paths and data.
	#
	def show(self):
		if not self.file_read:
			self.__read_file()
		#
		print('path:')
		for key in self.path_list:
			print('  %s = %s' % (key, self.path_list[key]))
		print('data:')
		for key in self.data_list:
			print('  %s = %s' % (key, self.data_list[key]))
		return

	##  Add path to path_list.
	#   @prog		Program name (string)		
	#   @path		Path to add 'PATH' (string)		
	#
	def add_path(self, prog, path):
		if not self.register:
			if self.verbose: print('add_path: has no permission')
			return

		self.path_list[prog] = path
		if self.verbose:
			print('add_path: %s = %s' % (prog, path))
		return

	##  Add data to data_list.
	#   @key		Key to add the dictionary (string)		
	#   @value		Value associated with the key (string)		
	#
	def add_data(self, key, value):
		if not self.register:
			if self.verbose: print('add_data: has no permission')
			return

		self.data_list[key] = value
		if self.verbose:
			print('add_data: %s = %s' % (key, value))
		return

	##  Write registered data to the config file.
	#   @returns		0 if successfully written.
	#   @n			-1 if open failed.
	#   @n			-2 if write failed.
	#
	def write(self):
		if not self.register:
			if self.verbose: print('write: has no permission')
			return
		#
		lines = []
		lines.append('# %s' % self.filename)
		#
		lines.append('')
		lines.append('[%s]' % self.PATH)
		for key in self.path_list.keys():
			line = '%s\t%s' % (key, self.path_list[key])
			lines.append(line)
		#
		lines.append('')
		lines.append('[%s]' % self.DATA)
		for key in self.data_list.keys():
			line = '%s\t%s' % (key, self.data_list[key])
			lines.append(line)
		#
		if self.verbose > 1:
			for line in lines:
				print('[%s]' % line)
		fio = TextFio(self.path, 'w', verbose=self.verbose)
		if fio.open() < 0:
			return -1
		if fio.writelines(lines) < 0:
			fio.close()
			return -2

		fio.close()
		return 0

	# --------------------------------------------------------------
	#  For class private use
	# --------------------------------------------------------------

	##  Add registered paths to current path.
	#   @param		none
	#   @returns		none
	#
	def __read_file(self):
		# initialize
		self.path_list = {}
		self.data_list = {}

		# read file
		kvf = KvFile(self.path)
		if kvf.read() <= 0:
			return
		self.kvf = kvf

		# path list
		keys = kvf.change_section(self.PATH)
		keys = kvf.keys()
		for key in keys:
			self.path_list[key] = kvf.get(key)

		# data list
		keys = kvf.change_section(self.DATA)
		keys = kvf.keys()
		for key in keys:
			self.data_list[key] = kvf.get(key)

		if self.verbose:
			for key in self.path_list.keys():
				print('path: %s = %s' % (key, self.path_list[key]))
			for key in self.data_list.keys():
				print('data: %s = %s' % (key, self.data_list[key]))

		self.file_read = True
		return

	##  Get registered data associated with the key.
	#   @key		Key name to get data (stirng)
	#   @section		Section name where key belong (string)
	#   @caller		Callers name (string)
	#   @returns		Registered data (string)
	#
	def __get_value(self, key, section, caller):
		if not self.file_read:
			self.__read_file()
		value = self.kvf.get(key, section=section)
		#
		return value

# end: SetupFile.py

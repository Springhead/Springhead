#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:	SetupFile(path, verbose=0, register=False)
#
#  METHODS:
#	setenv(force)		環境変数 _setup_done_ が未設定ならば
#				セットアップファイルの path セクション
#				に設定されているプログラムを環境変数に
#				設定する。
#				  os.environ[__prog__] = path/prog
#				引数 force が指定されたら環境変数の
#				設定状況に拘わらず処理を行なう
#	getenv(prog)		環境変数 __prog__ の値を取得する
#	keys = get_key(section)	指定したセクションにある全キーを返す
#				section は PROG, PATH, DATA の何れか
#	value = get_prog(key)	指定したキーの値(プログラムパス)を返す
#	value = get_path(key)	指定したキーの値(パス)を返す
#	value = get_data(key)	指定したキーの値(パス以外)を返す
#	show()			設定ファイルの内容を表示する
#
#	これ以下は register が True のときのみ使用可
#	add(key, value)		(キー, 値) の組を登録する
#	status = write()	登録されたデータを設定ファイルに書く
#
# ----------------------------------------------------------------------
#  Version:
#     Ver 1.00   2020/11/25 F.Kanehori	Release version.
#     Ver 1.01   2020/12/03 F.Kanehori	Add: getenv()
#     Ver 1.02   2021/02/10 F.Kanehori	Can run on pytho9n 2.7.
# ======================================================================
#from __future__ import print_function
import sys
import os
from KvFile import *
from TextFio import *
from Util import *
## for debug
#from Proc import *

##  Setup file manipulation class.
#
class SetupFile:
	##  Class constants
	#
	PROG	= 'prog'
	PATH	= 'path'
	DATA	= 'data'
	DONE	= '_setup_done_'
	NOTFOUND = 'NOT FOUND'

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
		self.prog_list = {}
		self.path_list = {}
		self.data_list = {}
		self.file_read = False

	##  Set registered paths to current environment variable.
	#   @param force	Force set environment (bool).
	#
	def setenv(self, force=False):
		if not force and os.getenv(self.DONE) == 'true':
			# setenv is already done
			return
		if not self.file_read:
			self.__read_file()
		#
		for prog in self.prog_list:
			if self.prog_list[prog] is None \
				or self.prog_list[prog] == self.NOTFOUND \
				or type(self.prog_list[prog]) == bool:
				continue
			name = '__%s__' % prog
			path = self.prog_list[prog]
			os.environ[name] = Util.pathconv(path)
			print('using\t%s' % Util.upath(path))
		#
		for key in self.path_list:
			if self.path_list[key] is None \
				or type(self.path_list[key]) == bool:
				continue
			name = '__%s__' % key
			path = self.path_list[key]
			os.environ[name] = Util.pathconv(path)
			print('%s: %s' % (key, path))
		#
		os.environ[self.DONE] = 'true'

		# 移行期間中の特別処理
		os.environ['MIGRATION_TEST'] = 'true'
		#print('MIGRATION_TEST: %s' % os.getenv('MIGRATION_TEST'))

		## for debug
		#cmnd1 = 'printenv' if Util.is_unix() else 'env'
		#cmnd2 = 'grep __'
		#proc1 = Proc().execute(cmnd1, stdout=Proc.PIPE)
		#proc2 = Proc().execute(cmnd2, stdin=proc1.proc.stdout)
		#proc1.wait()
		#proc2.wait()
			
	##  Get specified program path from environment variable.
	#   @prog		Program name to get path (str)
	#   @returns		Program path if defined (str)
	#   @n			Otherwise None
	#
	def getenv(self, prog):
		env = os.getenv('__%s__' % prog)
		if Util.is_windows():
			env = '"' + env + '"'
		return env

	##  Get registered path associated with the prog.
	#   @section		SetpFile.{PROG | PATH | DATA}
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
	def get_prog(self, prog):
		return self.__get_value(prog, self.PROG)

	##  Get registered path associated with the key.
	#   @key		Key name to get path (stirng)
	#   @returns		Registered path (string)
	#
	def get_path(self, key):
		return self.__get_value(key, self.PATH)

	##  Get registered data associated with the key.
	#   @key		Key to get daa (stirng)
	#   @returns		Registered data (string)
	#
	def get_data(self, key):
		return self.__get_value(key, self.DATA)

	##  Show registered paths and data.
	#
	def show(self):
		if not self.file_read:
			self.__read_file()
		#
		print('prog:')
		for key in self.prog_list:
			print('   %s: %s' % (key, self.prog_list[key]))
		print('path:')
		for key in self.path_list:
			print('   %s: %s' % (key, self.path_list[key]))
		print('data:')
		for key in self.data_list:
			print('   %s: %s' % (key, self.data_list[key]))
		return

	##  Add prog to prog_list.
	#   @prog		Program name (string)		
	#   @path		Path to add 'PROG' (string)		
	#
	def add_prog(self, prog, path):
		if not self.register:
			if self.verbose: print('add_prog: has no permission')
			return

		self.prog_list[prog] = path
		if self.verbose:
			print('add_prog: %s = %s' % (prog, path))
		return

	##  Add path to path_list.
	#   @key		Key name (string)		
	#   @path		Path to add 'PATH' (string)		
	#
	def add_path(self, key, path):
		if not self.register:
			if self.verbose: print('add_path: has no permission')
			return

		self.path_list[key] = path
		if self.verbose:
			print('add_path: %s = %s' % (key, path))
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
		lines.append('[%s]' % self.PROG)
		for key in self.prog_list.keys():
			line = '%s\t%s' % (key, self.prog_list[key])
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
		self.prog_list = {}
		self.data_list = {}

		# read file
		kvf = KvFile(self.path)
		if kvf.read() <= 0:
			print('%s: can\'t open file "%s"' % (self.clsname, self.path))
			return -1
		self.kvf = kvf

		# prog list
		keys = kvf.change_section(self.PROG)
		keys = kvf.keys()
		for key in keys:
			self.prog_list[key] = kvf.get(key)

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

		if self.verbose > 1:
			for key in self.prog_list.keys():
				print('prog: %s = %s' % (key, self.prog_list[key]))
			for key in self.path_list.keys():
				print('path: %s = %s' % (key, self.path_list[key]))
			for key in self.data_list.keys():
				print('data: %s = %s' % (key, self.data_list[key]))

		self.file_read = True
		return 0

	##  Get registered data associated with the key.
	#   @key		Key name to get data (stirng)
	#   @section		Section name where key belong (string)
	#   @returns		Registered data (string)
	#
	def __get_value(self, key, section):
		if not self.file_read:
			self.__read_file()
		value = self.kvf.get(key, section=section)
		#
		return value

# end: SetupFile.py

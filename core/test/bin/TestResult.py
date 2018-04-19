#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
#  CLASS:
#	TestResult
#	    Test result manager class.
#
#  INITIALIZER:
#	obj = TestResult(fname, scratch=True, verbose=0)
#	arguments:
#	    fname:	Test result file base name (str).
#			Use two files to save serialized data.
#			    "<fname>.r" for test results.
#			    "<fname>.v" for visited list.
#	    scracth:	Erase named result file if exists (bool).
#	    verbose:	Verbose level (0: silent) (int).
#
#  METHODS:
#	set_info(name, category, info)
#	    Set error/skip/expcted information.
#	    arguments:
#		name:	    Test name (e.g. directory name) (str).
#		category:   Information category (enum RST).
#				RST.ERR, RST.SKP or RST.EXP
#		info:	    Information to be set (str or int).
#
#	set_result(name, category, platform, config, result)
#	    Set test result.
#	    arguments:
#		name:	    Test name (e.g. directory name) (str).
#		category:   Information category (enum RST).
#				RST.BLD or RST.RUN
#		platform:   Test platform (PLATS).
#		config:	    Test configuration (CONFS).
#		result:	    Test result (int).
#
#	results, visited = get()
#	    Get whole result data.
#	    arguments:	    None
#	    returns:
#		results:    Result information (see DATA STRUCTURE).
#		visited:    Tested name list (see DATA STRUCTURE).
#
#	status = finish()
#	    Sirialize result data and save to the file.
#	    arguments:	    None
#	    returns:	    Write status (int).
#				0: succ, -1: fail
#
#	erase()
#	    Erase result file.
#
#	edit_result_log()
#	    Edit result data in  "result.log" format.
#	    returns:	    Editted data lines (list[str]).
#
#	dump()
#	    Dump test result.
#
#  DATA STRUCTURE:
#	results[name][RST.ERR]:		  info (not build/run error)
#	results[name][RST.SKP]:		  info (this test is skipped)
#	results[name][RST.BLD][p][c]:	  result code of build step
#	results[name][RST.RUN][p][c]:	  result code of run step
#	results[name][RST.RUN][RST.EXP]:  expected result status
#	visited[]:			  executed test name list
#	    where p and c stands for platform and config for each.
#
# ----------------------------------------------------------------------
#  VERSION:
#	Ver 1.0  2018/02/26 F.Kanehori	First version.
#	Ver 1.01 2018/03/14 F.Kanehori	Dealt with new Error class.
#	Ver 1.02 2018/04/19 F.Kanehori	Refine status check.
# ======================================================================
import sys
import os
import pickle

# local python library
#
sys.path.append('../../src/RunSwig')
from FindSprPath import *
spr_path = FindSprPath('ControlParams')
libdir = spr_path.abspath('pythonlib')
sys.path.append(libdir)

from Fio import *
from Error import *
from Proc import *
from FileOp import *
from ConstDefs import *

class TestResult:
	#  Class initializer
	#
	def __init__(self, fname, scratch=True, verbose=0):
		self.clsname = self.__class__.__name__
		self.version = 1.01
		#
		self.verbose = verbose
		#
		self.rfname = fname + '.r'
		self.vfname = fname + '.v'
		if scratch:
			self.erase()
		if os.path.exists(self.vfname) and os.path.exists(self.rfname):
			self.results = self.__deserialize(self.rfname)
			self.visited = self.__deserialize(self.vfname)
		else:
			dirname = os.path.dirname(os.path.abspath(self.rfname))
			os.makedirs(dirname, exist_ok=True)
			self.results = {}
			self.visited = []
		if verbose:
			print('results file: "%s"' % self.rfname)
			print('visited file: "%s"' % self.vfname)

	#  Set error/skip/expcted information.
	#
	def set_info(self, name, category, info):
		if category not in [RST.ERR, RST.SKP, RST.EXP]:
			msg = 'bad category: %s' % category
			Error(self.clsname).error(msg)
			return
		#
		if name not in self.visited:
			self.results[name] = self.__init_result()
			self.visited.append(name)
		#
		if category == RST.ERR or category == RST.SKP:
			self.results[name][category] = info
		if category == RST.EXP:
			self.results[name][RST.RUN][RST.EXP] = info

	#  Set test result.
	#
	def set_result(self, name, category, platform, config, result):
		if category not in [RST.BLD, RST.RUN]:
			msg = 'bad category: %s' % category
			Error(self.clsname).error(msg)
			return
		if platform not in PLATS:
			msg = 'bad platform: %s' % platform
			Error(self.clsname).error(msg)
			return
		if config not in CONFS:
			msg = 'bad config: %s' % config
			Error(self.clsname).error(msg)
			return
		#
		self.results[name][category][platform][config] = result

	#  Get whole result data.
	#
	def get(self):
		return self.results, self.visited

	#  Sirialize result data and save to the file.
	#
	def finish(self):
		stat1 = self.__serialize(self.results, self.rfname)
		stat2 = self.__serialize(self.visited, self.vfname)
		return 0 if stat1+stat2 == 0 else -1

	#  Erase result file.
	#
	def erase(self):
		fop = FileOp()
		if os.path.exists(self.rfname):
			fop.rm(self.rfname)
		if os.path.exists(self.vfname):
			fop.rm(self.vfname)

	#  Edit result data in  "result.log" format.
	#
	def edit_result_log(self, platform, config):
		succs = {RST.BLD: [], RST.RUN: []}
		fails = {RST.BLD: [], RST.RUN: []}
		lines = []
		name = None
		dirty = False
		#
		r = self.results
		p = platform
		c = config
		for v in self.visited:
			data = v.split('/')
			if len(data) != 3:
				# must be 'tests/x/y' or 'Samples/x/y'
				continue
			if data[0] != name:
				if dirty:
					self.__edit_log_data(lines, name, succs, fails)
				succs = {RST.BLD: [], RST.RUN: []}
				fails = {RST.BLD: [], RST.RUN: []}
				name = data[0]
				dirty = False
			#
			if r[v][RST.ERR]:
				msg = 'error: %s (%s)' % (v, r[v][RST.ERR])
				print(msg, file=sys.stderr)
				continue
			if r[v][RST.SKP]:
				msg = 'skip:  %s (%s)' % (v, r[v][RST.SKP])
				print(msg, file=sys.stderr)
				continue
			#
			module = '%s:%s' % (data[1], data[2])
			stat = r[v][RST.BLD][p][c]
			if stat == 0:
				succs[RST.BLD].append(module)
			else:
				fails[RST.BLD].append(module)
			dirty = True
			if stat != 0:
				continue
			stat = r[v][RST.RUN][p][c]
			if str(stat) == str(r[v][RST.RUN][RST.EXP]):
				succs[RST.RUN].append(module)
			elif stat == Proc.ETIME:	# assume success
				succs[RST.RUN].append(module)
			elif stat:
				fails[RST.RUN].append(module)
		# end visited	

		self.__edit_log_data(lines, name, succs, fails)
		return lines

	#  Dump test result.
	#
	def dump(self):
		r, v = self.get()
		for name in v:
			print('%s:' % name)
			err = r[name][RST.ERR] if RST.ERR in r[name] else None
			skp = r[name][RST.SKP] if RST.SKP in r[name] else None
			bld = r[name][RST.BLD] if RST.BLD in r[name] else None
			run = r[name][RST.RUN] if RST.RUN in r[name] else None
			exp = r[name][RST.EXP] if RST.EXP in r[name] else None
			if err: print('  ERR: %s' % err)
			if skp: print('  SKP: %s' % skp)
			if exp: print('  EXP: %s' % exp)
			if skp or err: continue
			fmt = '  %s: %s-%-7s: %s'
			for s in [RST.BLD, RST.RUN]:
				if r[name][s] is None: continue
				for p in PLATS:
					for c in CONFS:
						v = r[name][s][p][c]
						print(fmt % (s, p, c, v))
			v = r[name][RST.RUN][RST.EXP]
			print('  %s: %s' % (RST.EXP, v))


	# --------------------------------------------------------------
	#  For class private use
	# --------------------------------------------------------------

	#  Initialize result structure.
	#
	def __init_result(self):
		# arguments:	None
		# returns:	Initialized result list (obj).

		result = {}
		result[RST.ERR] = None
		result[RST.SKP] = None
		for s in [RST.BLD, RST.RUN]:
			result[s] = {}
			for p in PLATS:
				result[s][p] = {}
				for c in CONFS:
					result[s][p][c] = None
		result[RST.RUN][RST.EXP] = None
		return result

	#  Serialize/desierialize object.
	#
	def __serialize(self, obj, fname):
		# arguments:
		#   obj:	Instance of TestResult class (obj).
		#   fname:	File name to store serialized data (str).
		# returns:	0: succ, -1: fail.

		if self.verbose:
			print('serializing data to "%s"' % fname)
		f = Fio(fname, 'wb')
		if f.open() < 0:
			Error(self.clsname).error(f.error())
			return -1
		try:
			pickle.dump(obj, f.obj)
		except pickle.PickleError as err:
			Error(self.clsname).error(err)
			f.close()
			return -1
		f.close()
		return 0
	
	def __deserialize(self, fname):
		# arguments:
		#   fname:	File name to read serialized data (str).
		# returns:	Deserialized object (TestResult class).

		if self.verbose:
			print('deserializing data from "%s"' % fname)
		f = Fio(fname, 'rb')
		if f.open() < 0:
			Error(self.clsname).error(err)
			return None
		try:
			obj = pickle.load(f.obj)
		except pickle.PickleError as err:
			Error(self.clsname).error(err)
			f.close()
			return None
		f.close()
		return obj

	#  Put editted lines ("result.log" format).
	#
	def __edit_log_data(self, lines, name, succs, fails):
		# arguments:
		#   lines:	Edit result log data lines ([str]).
		#   name:	Test module name (str).
		#   succs:	List of success module names ([str]).
		#   fails:	List of failure module names ([str]).

		lines.append(name)
		lines.append('ビルド成功 (%s)' % ','.join(succs[RST.BLD]))
		lines.append('ビルド失敗 (%s)' % ','.join(fails[RST.BLD]))
		lines.append('実行成功 (%s)' % ','.join(succs[RST.RUN]))
		lines.append('実行失敗 (%s)' % ','.join(fails[RST.RUN]))
		lines.append(' ')


# ----------------------------------------------------------------------
#  Test main
# ----------------------------------------------------------------------
if __name__ == '__main__':

	from optparse import OptionParser
	verbose = 0

	usage = 'Usage: TestResult [res-file]'
	parser = OptionParser(usage = usage)
	(options, args) = parser.parse_args()

	# Print content of specified result file.
	#
	if len(args) > 0:
		fname = args[0]
		res = TestResult(fname, scratch=False, verbose=verbose)
		res.dump()
		sys.exit(0)

	# Self test program
	#
	def init_for_test_only(res, name):
		res.set_info(name, RST.ERR, 'dummy')
		res.set_info(name, RST.ERR, None)

	fname = 'TestResult_test'
	res = TestResult(fname, scratch=True, verbose=verbose)
	name = 'test1'
	init_for_test_only(res, name)
	for p in PLATS:
		for c in CONFS:
			res.set_result(name, RST.BLD, p, c, 1)
			res.set_result(name, RST.RUN, p, c, 2)
	res.set_info(name, RST.EXP, 3)
	name = 'test2'
	res.set_info(name, RST.ERR, 'error in test2')
	res.set_info(name, RST.SKP, 'skiped by test condition')
	r = res.finish()
	print('finish returns %d - test1 and test2' % r)
	res.dump()
	print()

	res = TestResult(fname, scratch=False, verbose=verbose)
	name = 'test3'
	init_for_test_only(res, name)
	for p in PLATS:
		for c in CONFS:
			res.set_result(name, RST.BLD, p, c, 11)
			res.set_result(name, RST.RUN, p, c, 22)
	res.set_info(name, RST.EXP, 33)
	r = res.finish()
	print('finish returns %d - test1, test2 and test3' % r)
	res.dump()
	print()

	res = TestResult(fname, scratch=True, verbose=verbose)
	name = 'test4'
	init_for_test_only(res, name)
	for p in PLATS:
		for c in CONFS:
			res.set_result(name, RST.BLD, p, c, 111)
			res.set_result(name, RST.RUN, p, c, 222)
	res.set_info(name, RST.EXP, 333)
	r = res.finish()
	print('finish returns %d - test4' % r)
	res.dump()
	print()

	sys.exit(0)

# end: TestResult.py

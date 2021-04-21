#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  CLASS:	Trace()
#
#  METHODS:
#	flag = flag(module)
#
# ==============================================================================
#  VERSION:
#     Ver 1.0    2021/04/08 F.Kanehori	初版.
# ==============================================================================

class Trace:
	##  The initializer.
	#
	def __init__(self):
		self.flags = {
			'do_swigall':		False,
			'RunSwig':		False,
			'RunSwigFramework':	False,
			'RunSwig_clean':	False,
			'CheckClosedSrc':	False,
			'ScilabSwig':		False,
			'EmbPythonSwig':	False,
			'RunSwig_EmbPython':	False,
		}
		self.clsname = self.__class__.__name__
		self.version = 1.0

	##  Get module's trace flag.
	#
	def flag(self, module):
		return self.flags[module]

#end: trace.py


#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# =============================================================================
import sys
import os
import glob

sys.path.append('../RunSwig/pythonlib')
sys.path.append(libdir)
from Proc import *

outdir = './html'
logfile = 'doxygen.log'
os.makedirs(outdir, exist_ok=True)
Proc().execute('doxygen SprCSharp.doxy', stdout=logfile).wait()

sys.exit(0)

# end MakeDoc.py

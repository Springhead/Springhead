#!/usr/local/bin/python3.4
# -*- coding: utf-8 -*-
# ======================================================================
#  FILE:
#	ProcTest.py
#
#  DESCRIPTION:
#	Test program for class Proc (Ver 1.0).
# ======================================================================
import sys
import os
import time
sys.path.append('..')
from Proc import *
from Util import *
from FileOp import *

# ----------------------------------------------------------------------
prog = sys.argv[0].split(os.sep)[-1].split('.')[0]
verbose = 1
Fop = FileOp()

# ----------------------------------------------------------------------
def Print(msg, indent=2):
	print('%s%s' % (' '*indent, msg))

def PrintResult(status, out, err):
	Print('-> returned status: %d' % status)
	Print('---- stdout ----')
	print(out if out else '  (nil)')
	Print('---- stderr ----')
	print(err if err else '  (nil)')

def Exec(proc, cmnd, **keywords):
	proc.exec(cmnd, **keywords)
	stat = proc.wait()
	Print('-> returned status: %d' % stat)

def Ls(path):
	lslist = Fop.ls(path)
	if not lslist:
		Print('no such file: "%s"' % path)
		return
	if isinstance(lslist, str):
		Print(lslist)
		return
	for item in lslist:
		Print(item)

# ----------------------------------------------------------------------
P = Proc(verbose=1, dry_run=False)
Ps = Proc(verbose=0, dry_run=False)
Pd = Proc(verbose=1, dry_run=True)

print('Test program for class: %s, Ver %s\n' % (P.clsname, P.version))

commands = {}
if Util.is_unix():
	commands = {'cat': 'cat', 'path': 'echo $PATH'}
else:
	commands = {'cat': 'type', 'path': 'path'}

# exec
#
print('-- exec --')
fname = 'test/ProcTest'
fname += '.utf8' if Util.is_unix() else '.sjis'

cmd = '%s %s' % (commands['cat'], Util.pathconv(fname))
Exec(P, cmd, shell=True)
print()

# (redirect)
print('-- exec (redirect) --')
Exec(P, cmd, stdout='test/proc.1', shell=True)
cmd = cmd.split()
Exec(P, cmd, stdout='test/proc.2', shell=True)
print()
Ls(fname)
Ls('test/proc.[12]')
cmd = 'diff %s %s' % (fname, 'test/proc.1')
Print(cmd)
Ps.exec(cmd, shell=True)
cmd = 'diff %s %s' % (fname, 'test/proc.2')
Print(cmd)
Ps.exec(cmd, shell=True)
print()

# (environ)
print('-- exec (environ) --')
addpath = os.getcwd()
print('---- before ----')
print(os.environ['PATH'])
print('---- in proc ----')
cmd = commands['path']
P.exec(cmd, stdout='test/proc.3', addpath=addpath, shell=True)
st = P.wait()
Print('-> returned status: %d' % st)
cmd = '%s %s' % (commands['cat'], Util.pathconv('test/proc.3'))
P.exec(cmd, shell=True)
stat = P.wait()
print('---- after ----')
print(os.environ['PATH'])
print()

# (dry_run)
print('-- dry_run (True) --')
outfile = 'test/proc.X'
Exec(Pd, 'dummy', stdout=outfile, shell=True)
print()
Ls(outfile)
if os.path.exists(outfile):
	Print('  dry_run ** FAILED **')
print()

# (timeout)
timeout_test_go = False
if timeout_test_go:
	print('-- exec (sleep 10, timeout 15) --')
	cmd = '%s 10' % ('sleep' if Util.is_unix() else 'timeout')
	P.exec(cmd, shell=True, stdout=Proc.NULL)
	st = P.wait(timeout=15)
	Print('  status = %d %s' %(st, '(timeout)' if st == Proc.ETIME else ''))
	print()
	Print('-- exec (sleep 10, timeout 5) --')
	P.exec(cmd, shell=True, stdout=Proc.NULL)
	st = P.wait(timeout=5)
	Print('  status = %d %s' %(st, '(timeout)' if st == Proc.ETIME else ''))
	print()

# (pipe)
print('-- exec (pipe) --')
import time
if Util.is_unix():
	cmnd1 = 'cat tool/head.py'
	cmnd2 = 'cat -n'
	#cmnd3 = 'python tool/head.py -10'
	cmnd3 = '/usr/local/bin/python3.4 tool/head.py -10'
else:
	cmnd1 = 'type tool\\head.py'
	cmnd2 = 'python tool\\cat.py -n'
	cmnd3 = 'python tool\\head.py -10'

proc1 = Proc(verbose=1)
proc2 = Proc(verbose=1)
proc3 = Proc(verbose=1)

proc1.exec(cmnd1, stdout=Proc.PIPE, shell=True)
proc2.exec(cmnd2, stdin=proc1.proc.stdout, stdout=Proc.PIPE, shell=True)
proc3.exec(cmnd3, stdin=proc2.proc.stdout, shell=True)
stat1 = proc1.wait()
stat2 = proc2.wait()
stat3 = proc3.wait()
Print('status1 = %d' % stat1)
Print('status2 = %d' % stat2)
Print('status3 = %d' % stat3)
print()

# (pipe and redirect)
print('-- exec (pipe and redirect) --')
infile = Util.pathconv('tool/head.py')
proc1.exec(cmnd1, stdin=infile, stdout=Proc.PIPE, shell=True)
proc2.exec(cmnd2, stdin=proc1.proc.stdout, stdout=Proc.PIPE, shell=True)
proc3.exec(cmnd3, stdin=proc2.proc.stdout, stdout=Proc.PIPE, shell=True)
stat1 = proc1.wait()
stat2 = proc2.wait()
stat3 = proc3.wait()
out, err = proc3.output()
PrintResult(stat3, out, err)
print()

# kill
#
kill_test_go = True
if kill_test_go:
	print('-- kill --')
	waittime = 3
	cmnd = '%s' % ('sleep' if Util.is_unix() else 'timeout')
	args = '%s 10' % cmnd
	Print('running command "%s"' % args)
	P.exec(args, stdout=Proc.NULL, shell=True)
	Print('  sleep %d seconds' % waittime)
	time.sleep(waittime)
	P.kill()
	P.wait()
	print()
	#
	'''
	imagename = 'sleep' if Util.is_unix() else 'cmd.exe'
	P.exec(args, stdout=Proc.NULL, shell=True)
	Print('  sleep %d seconds' % waittime)
	time.sleep(waittime)
	P.kill(image='%s' % imagename)
	P.wait()
	print()
	'''

sys.exit(0)

# end: ProcTest.py

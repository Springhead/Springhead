# ======================================================================
#  FILE:
#	CatArgs.py
#
#  DESCRIPTION:
#	Put character sequence specified by command line arguments
#	to stdout or generate it as keyboard event.
#
#  VERSION:
#	Ver 1.0  2016/11/02 F.Kanehori	First release version.
#	Ver 1.1  2017/09/28 F.Kanehori	Change TEST path.
# ======================================================================
import sys
import subprocess

# debug control
TEST = False

# ----------------------------------------------------------------------
#  Main program
# ----------------------------------------------------------------------
#  Usage:	python CatArgs.py [{-stdout | -kbevent}] seq ...
#	options:	
#	    -stdout:	Put following seq(s) to stdout.
#	    -kbevent:	Generate following seq(s) as keyboard event.
#	seq:	Character sequence.
#		Allowed escape characters are: '\n', '\t', '\\'.
# ----------------------------------------------------------------------
argc = len(sys.argv)
type = 'stdout'		# default

for n in range(1, argc):
	arg = sys.argv[n]
	if arg == 'TEST':
		# for debug
		TEST = True
		continue

	if arg[0] == '-':
		type = arg[1:]
		if type not in ['stdout', 'kbevent']:
			sys.stderr.write("%s: bad arg '%s'" % (sys.argv[0], arg))
			sys.exit(-1)
		continue

	if type == 'stdout':
		arg = arg.replace('\\n', '\n')
		arg = arg.replace('\\t', '\t')
		arg = arg.replace('\\\\', '\\')
		sys.stdout.write(arg)
	else:
		cmnd = 'GenKbEvent %s' % arg
		if TEST:
			cmnd = 'GenKbEvent/x64/Release/GenKbEvent TEST %s' % arg
		status = subprocess.call(cmnd)

sys.exit(0)

# end: CatArgs.py

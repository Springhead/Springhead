#!/Python/python
# ==============================================================================
#  SYNOPSIS:
#	find_path [options] fname
#	options:
#	    -m		Machine architecture ('x64' or 'x86').
#	    -s		Replace 'sp' to '/' (for Windows silly batch).
#	    -v		Set verbose mode.
#
#  DESCRIPTION:
#	fname で指定したファイルが存在するディレクトリを検索する。
#	検索は次のディレクトリで行なう。
#	  (1) "C:\Program Files (86)" の下（再帰的に検索）
#	  (2) "C:\Program Files" の下（再帰的に検索）
#	複数のパスが見つかったならば最初に見つかったたものを採用する。
#
#  VERSION:
#       Ver 1.0  2019/07/29 F.Kanehori	初版
# ==============================================================================
version = '1.0'

import os
import sys
import subprocess
import re
from optparse import OptionParser

# ----------------------------------------------------------------------
#  Constants
#
prog = sys.argv[0].replace(os.sep, '/').split('/')[-1].split('.')[0]
PIPE = subprocess.PIPE
NULL = subprocess.DEVNULL
STDOUT = subprocess.STDOUT

# ----------------------------------------------------------------------
#  Methods
#
def execute(cmnd, shell=True, stdin=None, stdout=None, stderr=None):
	if verbose:
		print('  execute: %s' % cmnd)
	pipe = open_pipe(stdin, stdout, stderr)
	proc = subprocess.Popen(cmnd, shell=shell,
				stdin=pipe[0], stdout=pipe[1], stderr=pipe[2],
				creationflags=0, start_new_session=True)
	return [proc, pipe]

def wait(proc_info):
	proc, pipe = proc_info
	status = proc.wait()
	close_pipe(pipe)
	return s16(status)

def output(proc_info):
	proc, pipe = proc_info
	out, err = proc.communicate()
	status = s16(proc.returncode)
	close_pipe(pipe)
	#
	encoding = os.device_encoding(1)
	if encoding is None:
		encoding = 'UTF-8' if is_unix() else 'cp932'
	out = out.decode(encoding) if out else None
	err = err.decode(encoding) if err else None
	#
	if status == 0:
		out = out.split('\n')
		out = list(map(lambda x: x.strip('\r'), out))
	if verbose > 1:
		print('  output: status %d' % status)
		for line in out:
			print('    [%s]' % line)
	return status, out, err

def open_pipe(stdin, stdout, stderr):
	p1 = open_pipe_sub(stdin, 'r')
	p2 = open_pipe_sub(stdout, 'w')
	p3 = open_pipe_sub(stderr, 'w')
	return [p1, p2, p3]

def open_pipe_sub(stream, mode):
	if stream is None:
		return None;
	if not isinstance(stream, str):
		return stream
	try:
		f = open(stream, mode)
	except IOError as err:
		f = None
	return f

def close_pipe(pipe):
	close_pipe_sub(pipe[0])
	close_pipe_sub(pipe[1])
	close_pipe_sub(pipe[2])

def close_pipe_sub(stream):
	if not stream in [None, PIPE, STDOUT, NULL]:
		stream.close()

def s16(value):
	return -(value & 0b1000000000000000) | (value & 0b0111111111111111)

def grep(lines, pattern, iflag=False, vflag=False):
	flags = re.I if iflag else 0
	matches = []
	for line in lines:
		m = re.search(pattern, line, flags)
		if m and not vflag:
			matches.append(line)
		else:
			matches.append(line)
	if verbose:
		for line in matches:
			print('    %s' % line)
	return matches

def is_unix():
	return os.name == 'posix'

def fatal(msg):
	print('%s: Error: %s' % (prog, msg))
	sys.exit(1)

# ------------------------------------------------------------------------------
#  Options
#
usage = "Usage: %prog [options] fname"
parser = OptionParser(usage = usage)
parser.add_option('-m', '--machine', dest='machine',
		    action="store", default=None,
                    metavar="{x64 | x86}", help='machine hardware name')
parser.add_option('-s', '--substitute', dest='substitute',
		    action="store_true", default=False,
                    help='substitute "sp" to "/"')
parser.add_option('-v', '--verbose', dest='verbose',
                    action="count", default=0,
		    help='set verbose mode')
parser.add_option('-V', '--version', dest='version',
		    action="store_true", default=False,
                    help='show version')

# ------------------------------------------------------------------------------
#  Process for command line
#
(options, args) = parser.parse_args()
if options.version:
	print('%s: Version %s' % (prog, version))
	sys.exit(0)
if len(args) < 1:
	subprocess.Popen('python %s.py -h' % prog, shell=True).wait()
	parser.error("incorrect number of arguments")
	sys.exit(-1)

fname = args[0]
machine = options.machine
substitute = options.substitute
verbose = options.verbose

if not machine in [None, 'x64', 'x86']:
	print('%s: bad machine hardwear name [%s]' % (prog, machine))
	print('\tmust be one of "x64" and "x86"')
	sys.exit(1)

# ------------------------------------------------------------------------------
#  Main process
#

#  Machine architecture
#
if machine is None:
	cmnd = 'uname -m'
	proc_info = execute(cmnd, stdout=PIPE, stderr=STDOUT)
	status, out, err = output(proc_info)
	if status != 0:
		fatal('machine architecture unknown')
	if out[0] == 'x86_64':
		machine = 'x64'
	else:
		machine = 'x86'
if verbose:
	print('  machine architecture: %s' % machine)

#  Search start directory
#
dir64 = 'C:\Program Files'
dir86 = 'C:\Program Files (x86)'

#  Try 'where <fname>' first.
#
found = False
cmnd = 'where %s' % fname
proc = execute(cmnd, stdout=PIPE, stderr=NULL)
status, out, err = output(proc)
if status == 0 and len(out) > 0:
	fpath = out[0]
	found = True

#  Try "C:\Program Files (x86)" next.
#
if not found and os.path.exists(dir86):
	cmnd = 'where /r "%s" %s' % (dir86, fname)
	proc = execute(cmnd, stdout=PIPE, stderr=NULL)
	status, out, err = output(proc)
	if status == 0:
		if machine == 'x64':
			lines = grep(out, 'x64', iflag=True)
			if len(lines) > 0:
				# 'x64' を含むパスがあった
				fpath = lines[0]
				found = True
			else:
				# 'x64' を含まないパスがあった
				fpath = out[0]
				found = True
		else:
			# is this the case?
			lines = grep(out, 'x86', iflag=True)
			lines = grep(lines, 'x64', iflag=True, vflag=True)
			if len(lines) > 0:
				fpath = lines[0]
				found = True

#  Then try "C:\Program Files" last.
#
if not found:
	cmnd = 'where /r "%s" %s' % (dir64, fname)
	proc = execute(cmnd, stdout=PIPE, stderr=NULL)
	status, out, err = output(proc)
	if status == 0 and len(out) > 0:
		fpath = out[0]
		found = True

#  Show result.
#
if found:
	path = os.sep.join(fpath.split(os.sep)[:-1])
	if substitute:
		path = path.replace(' ', '/')
	print(path)
	exitcode = 0
else:
	exitcode = 1

#  End
sys.exit(exitcode)

#end: find_path.py

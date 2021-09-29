#!/usr/local/bin/python
# ==============================================================================
#  SYNOPSIS:
#       python ShowResultpy [options]
#       options:
#           -l dir,  --log directory	ログディレクトリ名 [log]
#           -o file, --outfile file	showresult 出力ファイル名 [stdout]
#           -r file, --resultfile file	テスト結果ファイル名 [result.log]
#           -t dir,  --topdir dir	ソースツリーのトップディレクトリ名
#           
#  DESCRIPTION:
#       dailybuild のテスト結果ファイル "result.log" の内容は少し見にくいので,
#       少し見やすく整形して出力する.
#
#  VERSION:
#     Ver 1.0	 2014/11/13 F.Kanehori  MS-batch/perl からの移植版
#     Ver 1.1	 2021/09/29 F.Kanehori  オプションの変更
# ==============================================================================
version = '1.1'
import os
import sys
import subprocess
import optparse

ScriptFileDir = os.sep.join(os.path.abspath(__file__).split(os.sep)[:-1])
prog = sys.argv[0].replace('/', os.sep).split(os.sep)[-1].split('.')[0]

#-------------------------------------------------------------------------------
# Guess test directory path from cwd
#
cwd = os.getcwd().split(os.path.sep)
guessed_topdir = '.'
for n in reversed(range(1, len(cwd)+1)):
	leaf = cwd[0:n][-1]
	if leaf == 'core':
		guessed_topdir = '/'.join(cwd[0:n]) + '/test'
		break
if guessed_topdir == '.':
	tmp = '/'.join(cwd) + '/core/test'
	if os.path.exists(tmp):
		guessed_topdir = tmp

#-------------------------------------------------------------------------------
# Options
#
parser = optparse.OptionParser()
parser.add_option('-l', '--logdir', action='store', dest='logdir',
                    default='log', metavar='dir',
		    help='test log directory [default: "%default"]')
parser.add_option('-o', '--outfile', action='store', dest='outfile_name',
                    default='stdout', metavar='file',
		    help='output file path [default: "%default"]')
parser.add_option('-r', '--resultfile', action='store', dest='resultfile',
                    default='result.log', metavar='file',
		    help='test result file name [default: "%default"]')
parser.add_option('-t', '--topdir', action='store', dest='topdir',
                    default=guessed_topdir, metavar='dir',
		    help='test result directory [default: "%default"]')
parser.add_option('-v', '--verbose', action="store_true", dest='verbose',
                    default=False, help='set verbose mode')
parser.add_option('-V', '--version', action="store_true", dest='version',
                    help='show version')
options, remainder = parser.parse_args(sys.argv[1:])
#
if options.version:
	print(sys.argv[0].split('.')[0] + ': Version', version)
	exit(0)
#
topdir = options.topdir
logdir = options.logdir
outfile_name = options.outfile_name
resultfile = options.resultfile
#
if guessed_topdir == '.' and topdir == '.':
	# -t 指定なし かつ cwd から推測できない
	topdir = '.'
elif topdir == '.':
	# -t 指定なし
	topdir = guessed_topdir
else:
	# -t 指定あり
	topdir = '%s/core/test' % topdir
resultfile = '%s/%s/%s' % (topdir, logdir, resultfile)
if outfile_name == 'stdout':
	outfile = sys.stdout
else:
	outfile = open(outfile_name, 'w')
#
if options.verbose:
	print('result dir:  ', '%s/%s' % (topdir, logdir))
	print('result file: ', resultfile)
	print('output file: ', outfile_name)

if not os.path.exists(resultfile):
	print('%s: no such file: %s' % (prog, resultfile))
	sys.stdout.flush()
	cmnd = 'python %s/ShowResult.py --help' % ScriptFileDir
	subprocess.Popen(cmnd, shell=True).wait()
	sys.exit(1)
print('using "%s"' % resultfile)
print()

#-------------------------------------------------------------------------------
#
def read(fname):
	f = open(fname, 'r')
	line = f.read().strip()
	f.close()
	return line

def confirm(prompt, default):
	yn = input(prompt + ' [y/n]? ')
	result = default
	if yn.lower() == 'y': result = True
	if yn.lower() == 'n': result = False
	return result

#-------------------------------------------------------------------------------
# Process body
#
tmpfile = '%s.tmp' % resultfile
if os.name == 'posix':
	cmnd = 'nkf -S -w %s > %s' % (resultfile, tmpfile)
else:
	cmnd = 'nkf -s %s > %s' % (resultfile, tmpfile)
proc = subprocess.Popen(cmnd, stdout=subprocess.PIPE,
			      stderr=subprocess.DEVNULL, shell=True)
out, err = proc.communicate()
rc = proc.returncode
if options.verbose:
	print('%s' % cmnd)
	print('nkf returns %d' % rc)
if rc != 0:
	#if rc != 0xffffffffffffffff:
	#	print('ShowResult: encoding conversion failed: rc %08x' % rc)
	print('ShowResult: encoding conversion failed: rc %08x' % rc)
	if os.path.exists(tmpfile):
		os.remove(tmpfile)
	tmpfile = resultfile

lines = read(tmpfile).split('\n')
if options.verbose:
	print(lines)
for s in (lines):
	c = s.split(' ')
	if c[0] == ' ':
		print(file=outfile)
	elif len(c) == 1:
		if c[0] != '':
			print(c[0], file=outfile)
	else:
		length = '(' + str(len(c[1].split(','))) + ')'
		if len(c[1]) == 0:
			length = ''
		if c[1] == '()':
			length = '(0)'
		print(c[0], length, file=outfile)
		if c[1] != '()' and c[1] != '':
			c[1] = c[1].replace('(', '')
			c[1] = c[1].replace(')', '')
			for m in (c[1].split(',')):
				print('  ', m, file=outfile)

if outfile != sys.stdout:
	outfile.close()
	print(read(outfile_name))
	if not confirm('keep report file', False):
		os.remove(outfile_name)

if tmpfile != resultfile and os.path.exists(tmpfile):
	os.remove(tmpfile)

exit(0)
# end: showresult.py

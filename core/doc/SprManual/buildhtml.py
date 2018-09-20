
import sys
import os
import glob
sys.path.append('../../src/RunSwig/pythonlib')
from FileOp import *
from Proc import *
from Util import *
from Error import *

prog = sys.argv[0].split(os.sep)[-1].split('.')[0]

verbose = 1

wrkdir = 'tmp'
main = 'main_html.tex'
sed = 'sed'
nkf = 'nkf'
python = 'python'
plastex = 'c:/Python35/Scripts/plastex'

shell = True if Util.is_unix() else False

# Create work space.
if os.path.exists(wrkdir) and not os.path.isdir(wrkdir):
	msg = '%s exists but not a directory' % wrkdir
	Error(prog).abort(msg)
os.makedirs(wrkdir, exist_ok=True)

# Clear work space.
fop = FileOp()
fop.rm('%s/*' % wrkdir)

# Copy files to work space.
texsrcs = glob.glob('*.tex')
texsrcs.extend(glob.glob('*.sty'))
for f in texsrcs:
	if Util.is_unix():
		cmnd1 = 'cat %s' % f
		cmnd2 = "%s -e 's/\{sourcecode\}/\{verbatim\}/'" % sed
	else:
		cmnd1 = 'type %s' % f
		cmnd2 = '%s -e "s/{sourcecode}/{verbatim}/"' % sed
	cmnd3 = '%s -w' % nkf
	outf = '%s/%s' % (wrkdir, f)
	if verbose:
		print('converting %s' % f)
	proc1 = Proc()
	proc2 = Proc()
	proc3 = Proc()
	#print('cmnd1: %s' % cmnd1)
	#print('cmnd2: %s' % cmnd2)
	#print('cmnd3: %s' % cmnd3)
	proc1.execute(cmnd1, stdout=Proc.PIPE, shell=True)
	proc2.execute(cmnd2, stdin=proc1.proc.stdout, stdout=Proc.PIPE, shell=shell)
	proc3.execute(cmnd3, stdin=proc2.proc.stdout, stdout=outf, shell=shell)
	proc1.wait()
	proc2.wait()
	rc = proc3.wait()
	if rc != 0:
		msg = 'file conversion failed: "%s"' % f
		Error(prog).abort(msg)
#
others = glob.glob('*.cls')
others.extend(['en', 'fig'])
for f in others:
	if verbose:
		print('copying %s' % f)
	rc = FileOp().cp(f, '%s/%s' % (wrkdir, f))
	if rc != 0:
		msg = 'file copy failed: "%s"' % f
		Error(prog).abort(msg)
#
htmls = main.replace('.tex', '')
imgsrc = 'images'
imgdest = '%s/%s/images' % (wrkdir, htmls)
if verbose:
	print('copying %s' % imgsrc)
rc = FileOp().cp(imgsrc, imgdest)
if rc != 0:
	msg = 'file copy failed: "%s"' % imgsrc
	Error(prog).abort(msg)

# Generating htmls.
cwd = os.getcwd()
os.chdir(wrkdir)
cmnd = '%s %s %s' % (python, plastex, main)
rc = Proc().execute(cmnd, shell=shell).wait()
os.chdir(cwd)
if rc != 0:
	msg = 'generating html failed'
	Error(prog).abort(msg)

sys.exit(0)

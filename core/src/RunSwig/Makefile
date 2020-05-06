# ==============================================================================
#  FILE:
#	Makefile
#
#  DESCRIPTION:
#	各プロジェクトに対して, 必要に応じて RunSwig を実行させる makefile.
#
#	RunSwig 実行のための手順：
#
#	(1) 各プロジェクトに対して, 予め次の処理を施す.
#		compile	    ⇒  makefile.swig がなければ生成する.
#		recompile   ⇒  makefile.swig を削除した後, 新たに生成する.
#		clean	    ⇒  makefile.swig を削除する.
#
#	    実際の処理は, make_manager.py に次の引数を与えることで実現する.
#		compile	    ⇒  python make_manager.py -A -c
#		recompile   ⇒  python make_manager.py -A -d -c
#		clean	    ⇒  python make_manager.py -A -d
#
#	(2) 各プロジェクトに対して,
#	    (a) make -f Makefile.swig を実行して makefile.swig を作成する.
#	    (b) make -f makefile.swig を実行することで, 必要に応じて swig
#		を実行して Stub ファイルを生成する.
#
# ==============================================================================
#  Version:
#	Ver 1.0  2017/05/10 F.Kanehori  unix 用に新規に作成.
# ==============================================================================

#  Python の定義
#
PYTHON_VERSION	= 34
PYTHON_DIR	= ../../bin/Python$(PYTHON_VERSION)
PYTHON		= $(PYTHON_DIR)/python$(PYTHON_VERSION)
ADDPATH		= ../../bin/win64

#  Directory の定義
#
SWIGTEMP	= ../../swigtemp

#  使用するスクリプト
#
MAKEMANAGER	= $(PYTHON) make_manager.py

all:	compile

compile:	
	PATH=$(ADDPATH):${PATH}; $(MAKEMANAGER) -A -c
	make -f Makefile.unix

recompile:
	PATH=$(ADDPATH):${PATH}; $(MAKEMANAGER) -A -d -c
	make -f Makefile.unix clean
	make -f Makefile.unix

clean:	
	PATH=$(ADDPATH):${PATH}; $(MAKEMANAGER) -A -d -v
	/bin/rm -r -f $(SWIGTEMP)
	make -f Makefile.unix clean

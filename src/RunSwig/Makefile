#################################################################################
#  FILE:
#	Makefile
#
#  DESCRIPTION:
#	各プロジェクトに対して, 必要に応じて RunSwig を実行させる makefile.
#
#	RunSwig 実行のための手順：
#
#	処理概要：
#	    このファイルを make するにあたり, 各プロジェクトに対して予め次の
#	    処理を施しておく.
#		compile	    ⇒  makefile.swig がなければ生成する.
#		recompile   ⇒  makefile.swig を削除した後, 新たに生成する.
#		clean	    ⇒  makefile.swig を削除する.
#	    実際の処理は, make_manager.py に次の引数を与えることで実現する.
#		compile	    ⇒  python make_manager.py -A -c
#		recompile   ⇒  python make_manager.py -A -d -c
#		clean	    ⇒  python make_manager.py -A -d
#	    この処理は, この makefile で実現する.
#
#	    各プロジェクトに対して, make -f makefile.swig を実行する.
#
#################################################################################
#  Version:
#	Ver 1.0  2017/04/24 F.Kanehori  unix 用に新規に作成.
#################################################################################

#  Python の定義
#
PYTHON_VERSION	= 34
PYTHON_DIR	= ../../bin/Python$(PYTHON_VERSION)
PYTHON		= $(PYTHON_DIR)/python$(PYTHON_VERSION)

FOUNDATION_DIR	= ../Foundation
FRAMEWORK_DIR	= ../Framework
SCILABSWIG		= ScilabSwig.py
RUNSWIGFRAMEWORK	= RunSwigFramework.py

#  Directory の定義
#
SWIGTEMP	= ../../swigtemp

#  使用するスクリプト
#
MAKEMANAGER	= $(PYTHON) make_manager.py

all:	compile

compile:	
	$(MAKEMANAGER) -A -c
	make -f Makefile.unix

recompile:
	$(MAKEMANAGER) -A -d -c
	/bin/rm -r -f $(SWIGTEMP)
	make -f Makefile.unix

clean:	
	$(MAKEMANAGER) -A -d
	-cd $(FOUNDATION_DIR) && $(PYTHON) $(SCILABSWIG) -c
	-cd $(FRAMEWORK_DIR) && $(PYTHON) $(RUNSWIGFRAMEWORK) -c
	-/bin/rm -r -f $(SWIGTEMP)

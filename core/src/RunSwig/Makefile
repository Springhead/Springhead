# ==============================================================================
#  FILE:
#	Makefile
#
#  DESCRIPTION:
#	各プロジェクトに対して, 必要に応じて RunSwig を実行させる makefile.
#	Python が実行できる環境であること.
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
#	Ver 2.0  2017/08/21 F.Kanehori  Springhead tree 再構成に対応.
#	Ver 2.1  2017/08/23 F.Kanehori  Unix 用に調整.
#	Ver 2.2  2017/10/05 F.Kanehori  使用する python を引数で渡す.
#	Ver 2.21 2017/10/26 F.Kanehori  使用する python の取得方法を変更.
# ==============================================================================

eq = $(and $(findstring $(1)X,$(2)X), $findstring $(2)X,$(1)X)
opt = $(if $(call eq,$(1),),$(if $(call eq,$(2),),$(3),$(2)),$(1))

#  使用する Python (makeのマクロまたは環境変数で指定可能)
#
Python = $(call opt,$(PYTHON),$(SPR_PYTHON),python)

#  Directory の定義
#
SWIGTEMP	= ../../swigtemp

#  使用するスクリプト
#
MAKEMANAGER	= make_manager.py -P $(Python)

#  Make のオプション
#
OPT	= --silent PYTHON=$(Python)

all:	compile

compile:	
	$(Python) $(MAKEMANAGER) -A -c
	make $(OPT) -f Makefile.unix

clean:	
	$(Python) $(MAKEMANAGER) -A -d -c
	/bin/rm -r -f $(SWIGTEMP)
	make $(OPT) -f Makefile.unix clean

realclean:	clean
	

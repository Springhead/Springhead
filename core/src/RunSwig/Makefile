# ==============================================================================
#  NOTE:
#	Unix上でcmakeを使用せずにmakeを行なう場合のためのMakefile
#
#  DESCRIPTION:
#	各プロジェクトに対して, 必要に応じて RunSwig を実行させる makefile.
#	処理の詳細は Makefile.unix を参照のこと.
#
#	Python が実行できる環境であること.
#
# ==============================================================================
#  Version:
#     Ver 1.0	 2017/05/10 F.Kanehori	unix 用に新規に作成.
#     Ver 2.0	 2017/08/21 F.Kanehori  Springhead tree 再構成に対応.
#     Ver 2.1	 2021/04/08 F.Kanehori  コードの整理. SPR_PYTON の廃止.
# ==============================================================================

eq = $(and $(findstring $(1)X,$(2)X), $findstring $(2)X,$(1)X)
opt = $(if $(call eq,$(1),),$(if $(call eq,$(2),),$(3),$(2)),$(1))

#  使用する Python (makeのマクロまたは環境変数で指定可能)
#
#Python = $(call opt,$(PYTHON),$(SPR_PYTHON),python)
Python = $(if $(call eq,$(PYTHON),),python,$(PYTHON))

#  Directory の定義
#
SWIGTEMP	= ../../swigtemp

#  使用するスクリプト
#
MAKE		= gmake
CLEAN		= RunSwig_clean.py

#  Make のオプション
#
OPT		= --silent

#  ターゲット
#
all:	compile

compile:	
	$(MAKE) -f Makefile.unix $(OPT)

clean:	
	$(Python) RunSwig_clean.py --RunSwig
	/bin/rm -r -f $(SWIGTEMP)


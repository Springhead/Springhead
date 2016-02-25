#################################################################################
#  File: Makefile
#
#  Description:
#	他のプロジェクトのビルドより先に RunSwig を実行するためのプロジェクト.
#	Makefile および当プロジェクトプロパティのコマンドラインとを組み合わせる
#	ことで実現する.
#
#	[プロパティ]-[構成プロパティ]-[NMake] のコマンドラインに設定するコマンド
#	とビルドメニューとの関係は, 次のようになっている.
#	　　ビルド　　⇒　ビルドコマンドライン
#	　　リビルド　⇒　クリーンコマンドライン → すべてリビルドコマンドライン
#	　　クリーン　⇒　クリーンコマンドライン
#
#	よって, 次のような設定により機能を実現する.
#	(各ディレクトリについて)
#	　　(コマンドラインによって）
#		ビルド時　　⇒　Makefile がなければ作成 → make の実行
#		リビルド時　⇒　Makefile を強制再作成 → make の実行
#		クリーン時　⇒　Makefile を削除
#	　　(Makefile.swig によって)
#		RunSwig が実行されると判定された時　⇒　Makefile を強制再作成
#		(ただし一時ファイルに作成しておき make 終了後に rename する)
#
#	この Makefile からは $(SCRIPT) を呼び出すだけ.
#	Makefile 作成/削除の制御は, make_manager.bat スクリプトで集中管理する.
#
#	プロパティのコマンドラインに実際に設定するコマンドは次のとおり.
#
#	ビルドコマンドライン：	       make_manager.bat -A -c && nmake -f Makefile
#	すべてリビルドコマンドライン： make_manager.bat -A -d -c && nmake -f Makefile
#	クリーンコマンドライン：       make_manager.bat -A -d
#
#################################################################################
#  Version:
#	Ver 1.0	    2012/10/15 F.Kanehori
#	Ver 1.0a    2012/11/12 F.Kanehori  スクリプトの場所を変更
#	Ver 1.0b    2012/12/19 F.Kanehori  スクリプトの場所を変更
#	Ver 2.0     2013/01/10 F.Kanehori  前処理を追加
#################################################################################

#  ディレクトリの定義
#
BINDIR		= .
INCDIR		= ..\..\include
RUNSWIG		= ..\RunSwig
FOUNDATION	= ..\Foundation
FRAMEWORK	= ..\Framework
EMBPYTHON	= ..\EmbPython

#  使用するスクリプト
#
SCILABSWIG	= ScilabSwig.bat
RUNSWIGFW	= RunSwigFramework.bat
SCRIPT		= do_swigall.bat
EP_SCRIPT	= RunSwig.bat

#  前処理で作成するファイル
#
SCILABSTUBHDR	= $(INCDIR)\ScilabStub.hpp
FWOLDSPRSTUB	= $(FRAMEWORK)\FWOldSpringheadStub.cpp

all:	PreWork RunSwig RunEmbPythonSwig

PreWork:	$(SCILABSTUBHDR) $(FWOLDSPRSTUB)

$(SCILABSTUBHDR):	
		cd $(FOUNDATION) && $(SCILABSWIG)

$(FWOLDSPRSTUB):	
		cd $(FRAMEWORK) && $(RUNSWIGFW)

RunSwig:	
		$(BINDIR)\$(SCRIPT)

RunEmbPythonSwig:	
		@echo *** EmbPython ***
		@cd $(EMBPYTHON)
		$(EP_SCRIPT)
		@cd $(RUNSWIG)

/** \page pageFWAppGLUI Frameworkを使ったGLUIアプリケーション

<b>このページは内容が古くなっています（ただ今改訂の準備中です）</b>
<br>

<div>
	<p>
		FWAppGLUIはFWAppGLUTの機能を継承しつつ，GLUIのGUI機能も取り入れたクラスです．
		面倒な処理はFWAppGLUTと同様SpringheadのFrameworkが全て処理してくれるため，
		自分で行う初期設定を少なくすることが可能です．
		なお，このクラスはオブジェクトを一つしか作る事が許されておりません．
		そのため，内部の設計はシングルトンパターンとなっています．
		FWAppGLUIを用いてアプリケーションを作成するには，
		自分の作りたいアプリケーションクラスにFWAppGLUIを継承させる必要があります．
		以下の例では自分のアプリケーションクラスをclass MYGUIで表すこととします．
	</p>
	<h2>使い方</h2>
	<p>
in MYGUI.h
\verbatim

#include <Framework/SprFWAppGLUI.h>

class MYGUI : public FWAppGLUI{

private:
	GLUI* glui;
	// GUIに関する関数を必ずオーバーライドする
	void DesignGUI();
public:

	// インスタンス生成関数
	static MYGUI* GetInstance(){
		// instance is a static variable declared in class FWAppGLUT.
		if(!instance) instance = new MYGUI();
		return (MYGUI*)instance;
	}
	
	// 必要に応じてFWAppGLUTの仮想関数をオーバーライドする．
	void Init(int argc, char* argv[]); //< Init関数は必ずオーバーライドする
	void ....

public:
	// 自分で作ったメンバ関数を追加
private:
	// 自分で作ったメンバ関数やメンバ変数を追加
	static void __cdecl CallHoge(int control);
	void RealHoge(int control);
};
\endverbatim
	</p>
	<p>
<br>
in MYGUI.cpp
\verbatim

#include "MYGUI.h"

void MYGUI::DesignGUI(){
	glui = CreateGUI();
	new GLUI_Hoge(glui, ....., ..., CallHoge);
}

static void MYGUI::CallHoge(int control){
	((*MYGUI)instance)->RealHoge(control);
}
void MYGUI::RealHoge(int control){
	...;
}

void MYGUI::Init(int argc, char* argv[]){
	FWAppGLUT::Init(argc, argv);
	DesignGUI();
}
//:::::::::::::::::::::::::::::::::::::::::
	// FWAppGLUTと同じ
//:::::::::::::::::::::::::::::::::::::::::
\endverbatim
	</p>

	<p>
<br>
in main.cpp
\verbatim

#include <Springhead.h>
#include "MYGUI.h"

// Global空間で変数を定義しないと，デストラクタが呼ばれない．
// なぜなら，glutから抜ける時にexit関数を使用するほか無いため．
// （main関数が正常に最後まで走らない．）
// またコンストラクタをpublicにして実体を生成すると，
// 複数個のMYGUI型の変数が定義出来てしまう可能性があり危険．
MYGUI* gui = MYGUI::GetInstance();

int main(int argc, char* argv[]){
	gui->Init(argc, argv);
	gui->Start();

	return 0;
}
\endverbatim
</div>
<br>

*/

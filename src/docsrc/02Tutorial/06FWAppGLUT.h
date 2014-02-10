/** \page pageFWAppGLUT Frameworkを使ったGLUTアプリケーション

<b>このページは内容が古くなっています（ただ今改訂の準備中です）</b>
<br>

<div>
	<p>
		FWAppGLUTは簡単にSpringhead2を用いたアプリケーションを作成する事を可能にしたクラスです．
		面倒な処理はSpringheadのFrameworkが全て処理してくれるため，
		自分で行う初期設定を少なくすることができます．
		なお，このクラスはオブジェクトを一つしか作る事が許されておりません．
		そのため，内部の設計はシングルトンパターンとなっています．
		FWAppGLUTを用いてアプリケーションを作成するには，
		自分の作りたいアプリケーションクラスにFWAppGLUTを継承させる必要があります．
		以下の例では自分のアプリケーションクラスをclass MYAppで表すこととします．
	</p>
	<h2>使い方</h2>
	<p>
in MYApp.h
\verbatim

#include <Framework/SprFWAppGLUT.h>

class MYApp : public FWAppGLUT{

private:
	MYApp();
	~MYApp();
	
	// 必要に応じて以下の仮想関数をオーバーライドする．
	// これらは，SprFWApp.hで仮想関数として定義されている．
	void Display();
	void Reshape(int w, int h);
	void Keyboard(int key, int x, int y);
	void MouseButton(int button, int state, int x, int y);
	void MouseMove(int x, int y);
	void Step();
	void Idle();
	void Joystick(unsinged int buttonMask, int x, int y, int z);

public:
	// インスタンス生成関数
	static MYApp* GetInstance(){
		// instance is a static variable declared in class FWAppGLUT.
		if(!instance) instance = new MYApp();
		return (MYApp*)instance;
	}

	void Init(int argc, char* argv[]);
	void Start();
public:
		// 自分で作ったメンバ関数を追加
private:
		// 自分で作ったメンバ関数やメンバ変数を追加
};
\endverbatim
	</p>
	<p>
<br>
in MYApp.cpp
\verbatim

#include "MYApp.h"

void MYApp::Display()
{
	//描画処理，オーバーライドしなければシーングラフが描画される
}
void MYApp::Reshape(int w, int h){
	//画面サイズが変更された時の処理，オーバーライドしなければ規定のReshape関数が呼ばれる
}
void MYApp::Keyboard(int key, int x, int y){
	//キーボードのキーが押された時の処理，オーバーライドしなければ何も処理はされない．
}
void MYApp::MouseButton(int button, int state, int x, int y){
	//マウスが押されたときの処理，オーバーライドしなければ規定のMouseButton関数が呼ばれる．
}
void MYApp::MouseMove(int x, int y){
	//マウスを動かしたときの処理，オーバーライドしなければ規定のMouseMove関数が呼ばれる
}
void MYApp::Step(){
	//シミュレーションのステップ処理，オーバーライドしなければ一定周期ごとに規定のStep関数が呼ばれ，シミュレーション時間が進む
	//GLUTでいうところのglutTimerFuncへ登録する関数と同じ役目
}
void MYApp::Idle(){
	//シミュレーションのアイドル処理，オーバーライドしなければ何も呼ばれない
}
void MYApp::Joystick(unsinged int buttonMask, int x, int y, int z){
	//ゲームコントローラの入力を処理，オーバーライドしなければ何も呼ばれない
}
\endverbatim
	</p>

	<p>
<br>
in main.cpp
\verbatim

#include <Springhead.h>
#include "MYApp.h"

 
// Global空間で変数を定義しないと，デストラクタが呼ばれない．
// なぜなら，glutから抜ける時にexit関数を使用するほか無いため．
// （main関数が正常に最後まで走らない．）
// またコンストラクタをpublicにして実体を生成すると，
// 複数個のMYApp型の変数が定義出来てしまう可能性があり危険．
MYApp* app = MYApp::GetInstance();

int main(int argc, char* argv[]){
	
	app->Init(argc, argv);
	app->Start();

	return 0; // ここまで来ないから，ローカル宣言だとデストラクタが呼ばれない．
}
\endverbatim
</div>
<br>

*/

/** 
 @file  Springhead2/src/tests/Graphics/GRSimple/main.cpp

@brief 描画のテスト

 <PRE>
 <B>概要：</B>
  ・剛体の運動を確認する。
  ・位置を出力し、レンダリングは行わない。
  
 <B>終了基準：</B>
  ・プログラムが正常終了したら0を返す。
 
 <B>処理の流れ：</B>
  ・シミュレーションに必要な情報(質量・重心・慣性テンソルなど)を設定する。
  ・2つの剛体を用意し、各剛体に対して重心の2m上を右に10Nの力を加える。
  ・与えられた条件により⊿t秒後の位置の変化を10ステップ積分し、その都度位置を出力する。
  ・10ステップ分の計算を終えたら、プログラムを終了させる。
 </PRE>
    
 */
#include <Springhead.h>		//	Springheadのインタフェース
#pragma hdrstop
using namespace Spr;


/**
 @brief		メイン関数
 @param		<in/--> argc　　コマンドライン入力の個数
 @param		<in/--> argv　　コマンドライン入力
 @return	0 (正常終了)
 */
int main(int argc, char* argv[]){
	GRSdkIf* graphics = CreateGRSdk();
	GRDebugRenderIf* debugRend = graphics->CreateDebugRender();
	GRDeviceGLIf* device = graphics->CreateDeviceGL();
	debugRend->SetDevice(device);
	return 0;
}

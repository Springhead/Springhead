/** 
 @file  Springhead2/src/tests/Physics/PHSimple/main.cpp

@brief 2つの剛体の位置の変化を確認するテストプログラム（位置を出力）

 <PRE>
 <B>テスト内容</B>
  ・プログラムが正常終了したら0を返す。
 
 <B>プログラム概要</B>
  ・シミュレーションに必要な情報(質量・重心・慣性テンソルなど)を設定する。
  ・2つの剛体を用意し、各剛体に対して重心の2m上を右に10Nの力を加える。
  ・与えられた条件により⊿t秒後の位置の変化を10ステップ積分し、その都度位置を出力する。
  ・10ステップ分の計算を終えたら、プログラムを終了させる。
 </PRE>
    
 */
#include <Springhead.h>		//	Springheadのインタフェース
#pragma hdrstop
using namespace Spr;


PHSdkIf* sdk;
PHSceneIf* scene;
PHSolidIf* solid1, *solid2;

/**
 @brief  メイン関数
 @param	 <in/--> argc    コマンドライン入力の個数
 @param  <in/--> argv    コマンドライン入力
 @return  0 (正常終了)
 */
int main(int argc, char* argv[]){
	sdk = CreatePHSdk();				//	SDKの作成
	scene = sdk->CreateScene();			//	シーンの作成
	PHSolidDesc desc;
	desc.mass = 2.0;
	desc.inertia *= 2.0;
	solid1 = scene->CreateSolid(desc);	//	剛体をdescに基づいて作成
	desc.center = Vec3f(1,0,0);
	solid2 = scene->CreateSolid(desc);	//	剛体をdescに基づいて作成
	for(int i=0; i<10; ++i){
		solid1->AddForce(Vec3f(10,0,0), Vec3f(0,2,0)+solid1->GetCenterPosition());
		solid2->AddForce(Vec3f(10,0,0), Vec3f(0,2,0)+solid2->GetCenterPosition());
		scene->Step();
		std::cout << solid1->GetFramePosition();
		std::cout << solid2->GetFramePosition() << std::endl;
		// std::cout << solid1->GetOrientation() << std::endl;
	}
	//	SDKは開放しなくても良い．しなくてもmainを抜けてから開放される．
	delete sdk;

	//	メモリリークのテスト．
	//	デバッグありで実行するとメモリリークがVCのデバッグ出力に表示される．
	char* memoryLeak = new char[123];
	
	return 0;
}

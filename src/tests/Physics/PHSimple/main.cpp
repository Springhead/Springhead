/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/** 
 Springhead2/src/tests/Physics/PHSimple/main.cpp

【概要】
  2つの剛体の位置の変化を確認するテストプログラム（位置を出力）
  ・剛体の運動を確認する。
  ・位置を出力し、レンダリングは行わない。
  
【終了基準】
  ・プログラムが正常終了したら0を返す。
 
【処理の流れ】
  ・シミュレーションに必要な情報(質量・重心・慣性テンソルなど)を設定する。
  ・2つの剛体を用意し、各剛体に対して重心の2m上を右に10Nの力を加える。
  ・与えられた条件により⊿t秒後の位置の変化を10ステップ積分し、その都度位置を出力する。
  ・10ステップ分の計算を終えたら、プログラムを終了させる。
    
 */
#include <Springhead.h>		//	Springheadのインタフェース
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;


UTRef<PHSdkIf> sdk;
PHSceneIf* scene;
PHSolidIf* solid1, *solid2;

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int SPR_CDECL main(int argc, char* argv[]){
	sdk = PHSdkIf::CreateSdk();				//	SDKの作成
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

	//	メモリリークのテスト．
	//	デバッグありで実行するとメモリリークがVCのデバッグ出力に表示される．
	char* memoryLeak = DBG_NEW char[123];
	
	return 0;
}

// first.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//
#include "stdafx.h"
#include <springhead.h>
using namespace Spr;

int _tmain(int argc, _TCHAR* argv[])
{
	UTRef<PHSdkIf> sdk = PHSdkIf::CreateSdk();			//	物理エンジンSDKの作成
	PHSceneIf* scene = sdk->CreateScene();				//	シーンの作成
	PHSolidIf* solid = scene->CreateSolid();			//	剛体の作成
	for(int i=0; i<10; ++i){
		//	シミュレーションを進める
		scene->Step();
		//	剛体の位置を出力：自由落下の様子が数値で出力される
		std::cout << solid->GetPose().Pos() << std::endl;
	}
	return 0;
}

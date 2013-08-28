/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/** 
 Springhead2/src/tests/Collision/CDShape/main.cpp

【概要】
 Shapeの機能のテスト
  
【終了基準】
  ・プログラムが正常終了したら0を返す。
      
 */
#include <iostream>
#include <Springhead.h>		//	Springheadのインタフェース
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;


/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return	0 (正常終了)
 */
int __cdecl main(int argc, char* argv[]){
	UTRef<PHSdkIf> sdk = PHSdkIf::CreateSdk();				//	SDKの作成
	PHSceneIf* scene = sdk->CreateScene();			//	シーンの作成
	PHSolidDesc desc;
	PHSolidIf* solid = scene->CreateSolid(desc);	//	剛体をdescに基づいて作成
	
	//	形状の作成
	CDConvexMeshDesc md;
	md.vertices.push_back(Vec3f(-1,-1,-1));
	md.vertices.push_back(Vec3f(-1,-1, 1));
	md.vertices.push_back(Vec3f(-1, 1,-1));
	md.vertices.push_back(Vec3f(-1, 1, 1));
	md.vertices.push_back(Vec3f( 1,-1,-1));
	md.vertices.push_back(Vec3f( 1,-1, 1));
	md.vertices.push_back(Vec3f( 1, 1,-1));
	md.vertices.push_back(Vec3f( 1, 1, 1));
	CDConvexMeshIf* mesh = DCAST(CDConvexMeshIf, sdk->CreateShape(md));

	CDBoxDesc bd;
	bd.boxsize = Vec3d(2,2,2);
	CDBoxIf* box = DCAST(CDBoxIf, sdk->CreateShape(bd));
	Vec3d comMesh = mesh->CalcCenterOfMass();
	Vec3d comBox = box->CalcCenterOfMass();
	Matrix3d iMesh = mesh->CalcMomentOfInertia();
	Matrix3d iBox = box->CalcMomentOfInertia();
	std::cout << "CoG:" << comMesh << comBox << std::endl;
	std::cout << "Inertia:" << std::endl;
	std::cout << iMesh << iBox << std::endl;
	if (comMesh != comBox) return -1;
	if (iMesh != iBox) return -1;
	return 0;
}

/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/** 
 Springhead2/src/tests/Physics/PHShape/main.cpp

【概要】
  剛体Solidに形状を持たせたテストプログラム（位置を出力、面の頂点座標を出力）
  ・ペナルティ法による凸多面体同士の接触判定と接触力を確認する。
  ・剛体を自然落下させ、床の上に2個のブロックを積み上げることを想定する。 
  ・頂点座標をデバッグ出力させ、レンダリングは行わない。
  
【終了基準】
  ・プログラムが正常終了したら0を返す。
 
【処理の流れ】
  ・シミュレーションに必要な情報(剛体の形状・質量・慣性テンソルなど)を設定する。
  　剛体の形状はOpenGLで指定するのではなく、Solid自体で持たせる。  
  ・与えられた条件により⊿t秒後の位置の変化を2ステップ積分し、位置を出力する。
　・レンダリングは行わず、デバッグ出力として多面体の面(三角形)の頂点座標を出力する。
     
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
 return	0 (正常終了)
 */
int SPR_CDECL main(int argc, char* argv[]){
	sdk = PHSdkIf::CreateSdk();				//	SDKの作成
	scene = sdk->CreateScene();			//	シーンの作成
	PHSolidDesc desc;
	desc.mass = 2.0;
	desc.inertia *= 2.0;
	solid2 = scene->CreateSolid(desc);	//	剛体をdescに基づいて作成

	desc.mass = 1e20f;
	desc.inertia *= 1e20f;
	solid1 = scene->CreateSolid(desc);	//	剛体をdescに基づいて作成
	solid1->SetGravity(false);
	
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
	CDConvexMeshIf* mesh2 = DCAST(CDConvexMeshIf, sdk->CreateShape(md));

	for(unsigned i=0; i<md.vertices.size(); ++i){
		md.vertices[i].x *= 10;
	}
	CDConvexMeshIf* mesh1 = DCAST(CDConvexMeshIf, sdk->CreateShape(md));

	solid1->AddShape(mesh1);
	solid2->AddShape(mesh2);
	solid1->SetFramePosition(Vec3f(0,-1,0));
	solid2->SetFramePosition(Vec3f(0,2,0));

	scene->SetGravity(Vec3f(0,-9.8f, 0));	// 重力を設定

	for(int i=0; i<2; ++i){
		scene->Step();
		std::cout << solid1->GetFramePosition();
		std::cout << solid2->GetFramePosition() << std::endl;
		// std::cout << solid1->GetOrientation() << std::endl;
	}
	
	// デバッグ出力
	DSTR << "***  solid1  ***\n";
	for(int i=0; i<solid1->NShape();++i){
		CDShapeIf* shape = solid1->GetShape(i);
		CDConvexMeshIf* mesh = DCAST(CDConvexMeshIf, shape);
		Vec3f* base = mesh->GetVertices();
		for(int f=0; f<mesh->NFace();++f){
			CDFaceIf* face = mesh->GetFace(f);
			for(int v=0; v<face->NIndex(); ++v){
				DSTR << base[face->GetIndices()[v]];
			}
			DSTR << std::endl;
		}
	}
	DSTR << "***  solid2  ***\n";
	for(int i=0; i<solid2->NShape();++i){
		CDShapeIf* shape = solid2->GetShape(i);
		CDConvexMeshIf* mesh = DCAST(CDConvexMeshIf, shape);
		Vec3f* base = mesh->GetVertices();
		for(int f=0; f<mesh->NFace();++f){
			CDFaceIf* face = mesh->GetFace(f);
			for(int v=0; v<face->NIndex(); ++v){
				DSTR << base[face->GetIndices()[v]];
			}
			DSTR << std::endl;
		}
	}
}

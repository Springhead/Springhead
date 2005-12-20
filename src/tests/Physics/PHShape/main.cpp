/** 
 @file  Springhead2/src/tests/Physics/PHShape/main.cpp

@brief 剛体Solidに形状を持たせたテストプログラム（位置を出力、面の頂点座標を出力）

 <PRE>
 <B>概要：</B>
  ・ペナルティ法による凸多面体同士の接触判定と接触力を確認する。
  ・剛体を自然落下させ、床の上に2個のブロックを積み上げることを想定する。 
  ・頂点座標をデバッグ出力させ、レンダリングは行わない。
  
 <B>終了基準：</B>
  ・プログラムが正常終了したら0を返す。
 
 <B>処理の流れ：</B>
  ・シミュレーションに必要な情報(剛体の形状・質量・慣性テンソルなど)を設定する。
  　剛体の形状はOpenGLで指定するのではなく、Solid自体で持たせる。  
  ・与えられた条件により⊿t秒後の位置の変化を2ステップ積分し、位置を出力する。
　・レンダリングは行わず、デバッグ出力として多面体の面(三角形)の頂点座標を出力する。
 </PRE>
     
 */
#include <Springhead.h>		//	Springheadのインタフェース
#pragma hdrstop
using namespace Spr;

PHSdkIf* sdk;
PHSceneIf* scene;
PHSolidIf* solid1, *solid2;

/**
 @brief		メイン関数
 @param		<in/--> argc　　コマンドライン入力の個数
 @param		<in/--> argv　　コマンドライン入力
 @return	0 (正常終了)
 */
int main(int argc, char* argv[]){
	sdk = CreatePHSdk();				//	SDKの作成
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
	CDConvexMeshIf* mesh2 = ICAST(CDConvexMeshIf, scene->CreateShape(md));

	for(unsigned i=0; i<md.vertices.size(); ++i){
		md.vertices[i].x *= 10;
	}
	CDConvexMeshIf* mesh1 = ICAST(CDConvexMeshIf, scene->CreateShape(md));

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
	for(int i=0; i<solid1->GetNShapes();++i){
		CDShapeIf** shapes = solid1->GetShapes();
		CDConvexMeshIf* mesh = ICAST(CDConvexMeshIf, shapes[i]);
		Vec3f* base = mesh->GetVertices();
		for(size_t f=0; f<mesh->GetNFaces();++f){
			CDFaceIf* face = mesh->GetFace(f);
			for(int v=0; v<face->GetNIndices(); ++v){
				DSTR << base[face->GetIndices()[v]];
			}
			DSTR << std::endl;
		}
	}
	DSTR << "***  solid2  ***\n";
	for(int i=0; i<solid2->GetNShapes();++i){
		CDShapeIf** shapes = solid2->GetShapes();
		CDConvexMeshIf* mesh = ICAST(CDConvexMeshIf, shapes[i]);
		Vec3f* base = mesh->GetVertices();
		for(size_t f=0; f<mesh->GetNFaces();++f){
			CDFaceIf* face = mesh->GetFace(f);
			for(int v=0; v<face->GetNIndices(); ++v){
				DSTR << base[face->GetIndices()[v]];
			}
			DSTR << std::endl;
		}
	}


	//	SDKは開放しなくても良い．しなくてもmainを抜けてから開放される．
	delete sdk;

}

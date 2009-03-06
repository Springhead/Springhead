/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Framework/FWHapticAppBase.h>
#include <Framework/FWExpandedPHSolid.h>
#include <Physics/PHConstraintEngine.h>
#include <GL/glut.h>
#include <sstream>

#ifdef USE_HDRSTOP
#pragma  hdrstop
#endif

namespace Spr{;

FWHapticAppBase::FWHapticAppBase(){
	localRange = 0.5;
	bStep = true;
	bOneStep = false;
	bDebug = false;
}

void FWHapticAppBase::Init(int argc, char* argv[]){
	FWAppGLUT::Init(argc, argv);
	GetSdk()->Clear();

	FWWinDesc windowDesc;
	windowDesc.title = "(c)Springhead2    FWHapticApp";
	FWWin* window = CreateWin(windowDesc);
	window->scene = GetSdk()->GetScene();
}

void FWHapticAppBase::InitCameraView(){
	std::istringstream issView(
		"((0.9996 0.0107463 -0.0261432 -0.389004)"
		"(-6.55577e-010 0.924909 0.380188 5.65711)"
		"(0.0282657 -0.380037 0.92454 13.7569)"
		"(     0      0      0      1))"
	);
	issView >> cameraInfo.view;
}

void FWHapticAppBase::Start(){
	instance = this;
	if (!NWin()){
		CreateWin();
		wins.back()->SetScene(GetSdk()->GetScene());
	}
//	glutTimerFunc(1, FWAppGLUT::GlutTimerFunc, 0);
	glutIdleFunc(FWAppGLUT::GlutIdleFunc);
	glutMainLoop();
}

void FWHapticAppBase::Display(){
	FWWin* window = GetCurrentWin();
	window->SetScene(GetSdk()->GetScene());
	GRCameraIf* cam = window->scene->GetGRScene()->GetCamera();
	GRDebugRenderIf* render = window->render->Cast();

	// 描画モードの設定
	GetSdk()->SetDebugMode(true);	
	render->SetRenderMode(true, false);
//	render->EnableRenderAxis(bDebug);
	render->EnableRenderForce(bDebug);
	render->EnableRenderContact(false);			// 接触状態は割り込み処理で動的に変化するので常にfalse

	// カメラ座標の指定
	if(cam && cam->GetFrame()){
		cam->GetFrame()->SetTransform(cameraInfo.view);
	}else{
		window->render->SetViewMatrix(cameraInfo.view.inv());
	}

	// 描画の実行
	if(!window) return;
	GRRenderIf*curRender =  window->GetRender();
	FWSceneIf* curScene = window->GetScene();
	GetSdk()->SwitchRender(curRender);
	GetSdk()->SwitchScene(curScene);
	if(!curRender) return;
	curRender->ClearBuffer();
	curRender->BeginScene();
	if (curScene) curScene->Draw(curRender, GetSdk()->GetDebugMode());	// シーンを描画
	DebugDisplay(render);																			// デバック表示モード
	curRender->EndScene();
 
	glutSwapBuffers(); 
}

void FWHapticAppBase::DebugDisplay(GRDebugRenderIf* render){
	if(!bDebug) return;
	GRLightDesc ld;
	ld.diffuse = Vec4f(1,1,1,1) * 0.8f;
	ld.specular = Vec4f(1,1,1,1) * 0.8f;
	ld.ambient = Vec4f(1,1,1,1) * 0.4f;
	ld.position = Vec4f(1,1,1,0);
	render->PushLight(ld);// 光源の追加
	// ここにデバック表示モードで描画したいものを書く
	//
	render->PopLight();	//	光源の削除
}

FWExpandedPHSolid** FWHapticAppBase::ExpandPHSolidInfo(){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	PHSolidIf** solids = phscene->GetSolids();
	for(int i = (int)expandedPHSolids.size(); i < phscene->NSolids(); i++){
		expandedPHSolids.resize(i + 1);
		expandedPHSolids.back().phSolidIf = solids[i];
	}
	return expandedPHSolids.empty() ? NULL : (FWExpandedPHSolid**)&*expandedPHSolids.begin();
}

void FWHapticAppBase::FindNearestObjectFromHapticPointer(PHSolidIf* hPointer){
	/*
		GJKを使って近傍物体と近傍物体の最近点を取得
		これをすべてのshapeをもつ剛体についてやる
		AABBで力覚ポインタ近傍の物体を絞る
		ここで絞った物体についてGJKを行う．ここで絞ることでGJKをする回数を少なくできる．
		SolidのBBoxレベルでの交差判定(z軸ソート)．交差のおそれの無い組を除外
	*/

	//1. BBoxレベルの衝突判定
	FWExpandedPHSolid** esolids = GetFWExpandedPHSolids();
	int Nesolids = GetNExpandedPHSolids();
	Vec3f dir(0,0,1);
	Edges edges;
	edges.resize(2 * Nesolids);
	Edges::iterator eit = edges.begin();
	for(int i = 0; i <	Nesolids; ++i){
		// ローカル判定をすべてfalseにする
		esolids[i]->flag.bneighbor = false;
		DCAST(PHSolid, esolids[i]->phSolidIf)->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
		Vec3d dPos = esolids[i]->phSolidIf->GetDeltaPosition();
		float dLen = (float) (dPos * dir);
		if (dLen < 0){
			eit[0].edge += dLen;
		}else{
			eit[1].edge += dLen;
		}
		eit[0].index = i; eit[0].bMin = true;
		eit[1].index = i; eit[1].bMin = false;
		eit += 2;
	}
	std::sort(edges.begin(), edges.end());
	//端から見ていって，接触の可能性があるノードの判定をする．
	typedef std::set<int> SolidSet;
	SolidSet cur;																											//	現在のSolidのセット
	bool found = false;

	for(Edges::iterator it = edges.begin(); it != edges.end(); ++it){
		if (it->bMin){																										//	初端だったら，リスト内の物体と判定
			for(SolidSet::iterator itf=cur.begin(); itf != cur.end(); ++itf){
				int f1 = it->index;
				int f2 = *itf;
				if (f1 > f2) std::swap(f1, f2);
				// 近傍物体の仮決定
				if(esolids[f1]->phSolidIf == hPointer){
					esolids[f2]->flag.bneighbor = true;
				}else if(esolids[f2]->phSolidIf == hPointer){
					esolids[f1]->flag.bneighbor = true;
				}
			}
			cur.insert(it->index);
		}else{
			cur.erase(it->index);																						//	終端なので削除．
		}
	}

	// GJKで近傍点を求め，力覚ポインタ近傍の物体を決定する
	// 近傍物体だったらblocalをtrueにし，phSolidにphSolidIfをコピーする
	// blocalがすでにtrueだったらコピー済みなので近傍点だけコピーする
	for(int i = 0; i < Nesolids; i++){
		if(!esolids[i]->flag.bneighbor){
			esolids[i]->flag.bfirstlocal = false;																	//近傍物体でないのでfalseにする
			esolids[i]->flag.blocal = false;
			continue;																									// 近傍でなければ次へ
		}

		CDConvex* a = DCAST(CDConvex, esolids[i]->phSolidIf->GetShape(0));					// 剛体が持つ凸形状
		CDConvex* b = DCAST(CDConvex, hPointer->GetShape(0));									// 力覚ポインタの凸形状
		Posed a2w, b2w;																								// 剛体のワールド座標
		/*	if(esolids[i]->flag.blocal) 	a2w = esolids[i]->phSolidIf.GetPose();						// blocalがtrueなら最新の情報でやる
		else */								a2w = esolids[i]->phSolidIf->GetPose();
		b2w = hPointer->GetPose();																				// 力覚ポインタのワールド座標																									// pa:剛体の近傍点，pb:力覚ポインタの近傍点（ローカル座標）
		Vec3d pa = Vec3d(0.0, 0.0, 0.0);																		// PHSolidの近傍点
		Vec3d pb = Vec3d(0.0, 0.0, 0.0);																		// HapticPointerの近傍点
		FindClosestPoints(a, b, a2w, b2w, pa, pb);															// GJKで近傍点の算出
		Vec3d wa = a2w * pa;																						// 剛体近傍点のワールド座標
		Vec3d wb = b2w * pb;																						// 力覚ポインタ近傍点のワールド座標
		Vec3d a2b = wb - wa;																						// 剛体から力覚ポインタへのベクトル
		Vec3d normal = a2b.unit();
		if(a2b.norm() < localRange){																			// 近傍点間距離から近傍物体を絞る
			if(a2b.norm() < 0.01){																				// 力覚ポインタと剛体がすでに接触していたらCCDGJKで法線を求める		
				pa = pb = Vec3d(0.0, 0.0, 0.0);																// 変数のリセット
				Vec3d dir = -1.0 * esolids[i]->syncInfo.neighborPoint.face_normal;				
				if(dir == Vec3d(0.0, 0.0, 0.0) ){																
					dir = -(hPointer->GetCenterPosition() - wa);											// dirが(0 , 0, 0ならPHSolidの近傍点からHapticPointerの重心までにする
				}
				double dist = 0.0;
				int cp = ContFindCommonPoint(a, b, a2w, b2w, dir, -DBL_MAX, 1, normal, pa, pb, dist);
				if(cp != 1){
					ContFindCommonPointSaveParam(a, b, a2w, b2w, dir, -DBL_MAX, 1, normal, pa, pb, dist);
					DSTR << "ContFindCommonPoint do not find contact point" << std::endl;
				}
			}
			if(!esolids[i]->flag.blocal){																			// 初めて最近傍物体になった時
				esolids[i]->flag.bfirstlocal = true;
				esolids[i]->haSolid = *DCAST(PHSolid, esolids[i]->phSolidIf);						// シーンが持つ剛体の中身を力覚プロセスで使う剛体（実体）としてコピーする
				esolids[i]->syncInfo.neighborPoint.face_normal = normal;							// 初めて近傍物体になったので，前回の法線に今回できた法線を上書きする．										
#ifdef _DEBUG
				if (esolids[i]->syncInfo.neighborPoint.face_normal * normal < 0.8){
					DSTR << "Too big change on normal" << normal << std::endl;
				}
#endif
			}
			esolids[i]->flag.blocal = true;																		// 近傍物体なのでblocalをtrueにする
			esolids[i]->syncInfo.neighborPoint.closestPoint = pa;										// 剛体近傍点のローカル座標
			esolids[i]->syncInfo.neighborPoint.pointerPoint = pb;										// 力覚ポインタ近傍点のローカル座標
			esolids[i]->syncInfo.neighborPoint.last_face_normal											// 法線補間のために前回の法線を取っておく
				= esolids[i]->syncInfo.neighborPoint.face_normal;										// 初めて近傍になった時は今回できた法線
			esolids[i]->syncInfo.neighborPoint.face_normal = normal;								// 剛体から力覚ポインタへの法線
		}else{
			//GJKの結果，近傍点距離がlocalRangeよりも大きい場合
			esolids[i]->flag.blocal = false;																		// 近傍物体ではないのでblocalをfalseにする
			esolids[i]->flag.bfirstlocal = false;
		}
	}
}

void FWHapticAppBase::UpdateHapticPointer(PHSolidIf* hPointer, PHSolid hInterface){
	hPointer->SetFramePosition(hInterface.GetFramePosition());				
//	hPointer->SetFramePosition(cameraInfo.view.Rot() * phpointer.GetFramePosition());		// (未実装)cameraInfo.view.Rot()をかけて力覚ポインタの操作をカメラを回転にあわせる(*力覚も考えないといけん)			
	hPointer->SetOrientation(hInterface.GetOrientation());					
	hPointer->SetVelocity(hInterface.GetVelocity());
	hPointer->SetAngularVelocity(hInterface.GetAngularVelocity());	
	hPointer->SetDynamical(false);
}	

// protected変数へのアクセス
double FWHapticAppBase::GetHapticTimeStep(){
	return hapticTimeStep;
}

double FWHapticAppBase::GetPhysicTimeStep(){
	return physicTimeStep;
}

PHSolid FWHapticAppBase::GetHapticInterface(){
	return hapticInterface;
}

PHSolidIf* FWHapticAppBase::GetHapticPointer(){
	return hapticPointer;
}

FWExpandedPHSolid** FWHapticAppBase::GetFWExpandedPHSolids(){
	return expandedPHSolids.empty() ? NULL : (FWExpandedPHSolid**)&*expandedPHSolids.begin();
}

int FWHapticAppBase::GetNExpandedPHSolids(){
	return (int)expandedPHSolids.size();
}

// フラグを切り替えるための関数
void FWHapticAppBase::SetDebugMode(bool bD){
	bDebug = bD;
}

bool FWHapticAppBase::GetDebugMode(){
	return bDebug;
}

}
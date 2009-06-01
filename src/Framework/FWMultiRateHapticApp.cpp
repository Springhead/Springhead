#include <Framework/FWMultiRateHapticApp.h>
#include <Physics/PHConstraintEngine.h>
#include <sstream>

#ifdef USE_HDRSTOP
#pragma  hdrstop
#endif

namespace Spr{;



FWMultiRateHapticApp::FWMultiRateHapticApp(){
	bSync = false;
	bCalcPhys = true;
	pdt = 50;
	hdt = 1;
}

void FWMultiRateHapticApp::SetHapticProcess(FWHapticProcessBase* process){ hapticProcess = process; }
FWHapticProcessBase* FWMultiRateHapticApp::GetHapticProcess(){ return hapticProcess; }
void FWMultiRateHapticApp::SetPhysicTimeStep(double dt){ pdt = dt; }
double FWMultiRateHapticApp::GetPhysicTimeStep(){ return pdt; }
void FWMultiRateHapticApp::SetHapticTimeStep(double dt){ hdt = dt; }
double FWMultiRateHapticApp::GetHapticTimeStep(){ return hdt; }
void FWMultiRateHapticApp::SetHapticPointer(PHSolidIf* pointer){ 
	hapticPointer = pointer; 
	// 力覚ポインタをspringhead2の接触計算から外す
	// 剛体が増えるたびにContactModeをNONEにしなけらばいけない(増えた剛体と接触計算をしてしまうため)
	GetScene()->GetPHScene()->SetContactMode(hapticPointer, PHSceneDesc::MODE_NONE);
}
PHSolidIf* FWMultiRateHapticApp::GetHapticPointer(){ return hapticPointer; }
int FWMultiRateHapticApp::GetNExpandedPHSolids(){ return expandedPHSolids.size(); }
FWExpandedPHSolid** FWMultiRateHapticApp::GetFWExpandedPHSolids(){ 
	return expandedPHSolids.empty() ? NULL : (FWExpandedPHSolid**)&*expandedPHSolids.begin();
}


void FWMultiRateHapticApp::Init(){	
	GetHapticProcess()->SetHapticInterface(GetHapticInterface());
	SetScene(GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc()));		// Sceneの作成，設定
	ResetScene();
}

void FWMultiRateHapticApp::ResetScene(){
	expandedPHSolids.clear();
	GetHapticProcess()->expandedPHSolids.clear();
	bSync = false;
	bCalcPhys=true;
	hapticcount = 1;
	GetHapticProcess()->loopCounter = 1;
	
	PHSceneIf* phscene = GetScene()->GetPHScene();
	phscene->Clear();	
	phscene->SetGravity(gravity);				
	phscene->SetTimeStep(pdt);
	phscene->SetNumIteration(nIter);
	SetHapticTimeStep(GetHapticProcess()->GetHapticTimeStep());
	GetHapticProcess()->SetPhysicTimeStep(GetPhysicTimeStep()); 
	BuildScene();																// 剛体を作成
}

void FWMultiRateHapticApp::Idle(){
	Step();
}

FWExpandedPHSolid** FWMultiRateHapticApp::ExpandPHSolidInfo(){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	PHSolidIf** solids = phscene->GetSolids();
	for(int i = (int)expandedPHSolids.size(); i < phscene->NSolids(); i++){
		expandedPHSolids.resize(i + 1);
		expandedPHSolids.back().phSolidIf = solids[i];
	}
	return expandedPHSolids.empty() ? NULL : (FWExpandedPHSolid**)&*expandedPHSolids.begin();
}

void FWMultiRateHapticApp::FindNearestObjectFromHapticPointer(PHSolidIf* hPointer){
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
		((PHSolid*)esolids[i]->phSolidIf)->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
//		DCAST(PHSolid, esolids[i]->phSolidIf)->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
		cout << esolids[i]->phSolidIf->GetFramePosition() << endl;
		FWExpandedPHSolid esolid;
		esolid.phSolidIf = GetHapticPointer();
		Vec3d d = esolid.phSolidIf->GetDeltaPosition();
		cout << d << endl;


		Vec3d dPos = ((PHSolid*)esolids[i]->phSolidIf)->GetDeltaPosition();
//		Vec3d dPos = esolids[i]->phSolidIf->GetDeltaPosition();
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

void FWMultiRateHapticApp::UpdateHapticPointer(){	
	PHSolid reproHapticPointer = GetHapticProcess()->GetHapticPointer();
	GetHapticPointer()->SetFramePosition(reproHapticPointer.GetFramePosition());				
//	GetHapticPointer()->SetFramePosition(cameraInfo.view.Rot() * reproHapticPointer.GetFramePosition());		// (未実装)cameraInfo.view.Rot()をかけて力覚ポインタの操作をカメラを回転にあわせる(*力覚も考えないといけん)			
	GetHapticPointer()->SetOrientation(reproHapticPointer.GetOrientation());					
	GetHapticPointer()->SetVelocity(reproHapticPointer.GetVelocity());
	GetHapticPointer()->SetAngularVelocity(reproHapticPointer.GetAngularVelocity());	
	GetHapticPointer()->SetDynamical(false);
}

void FWMultiRateHapticApp::SyncHapticProcess(){
		FWHapticProcessBase* hprocess = GetHapticProcess();
	if(bSync){
		// 力覚ポインタの同期
		UpdateHapticPointer();

		// 力覚プロセス->物理プロセス
		// 力覚プロセスでの近傍物体のシミュレーション結果を物理プロセスに反映させる
		for(unsigned i = 0; i < hprocess->expandedPHSolids.size(); i++){
		// blocalがtrue，blocalがな場合は結果を反映させる
			if(expandedPHSolids[i].flag.blocal && !expandedPHSolids[i].flag.bfirstlocal){
				SpatialVector b = (expandedPHSolids[i].syncInfo.motionCoeff.b + 
					(expandedPHSolids[i].syncInfo.motionCoeff.curb - expandedPHSolids[i].syncInfo.motionCoeff.lastb)) *  GetPhysicTimeStep();
				Vec3d v = hprocess->expandedPHSolids[i].haSolid.GetVelocity() + b.v();
				hprocess->expandedPHSolids[i].phSolidIf->SetVelocity(v);
				Vec3d w = hprocess->expandedPHSolids[i].haSolid.GetAngularVelocity() + b.w();
				hprocess->expandedPHSolids[i].phSolidIf->SetAngularVelocity(w);
				hprocess->expandedPHSolids[i].phSolidIf->SetCenterPosition(hprocess->expandedPHSolids[i].haSolid.GetCenterPosition());
				hprocess->expandedPHSolids[i].phSolidIf->SetOrientation(hprocess->expandedPHSolids[i].haSolid.GetOrientation());
				expandedPHSolids[i].syncInfo.neighborPoint.test_force_norm = hprocess->expandedPHSolids[i].syncInfo.neighborPoint.test_force_norm;
			}
		}

		// 物理プロセス->力覚プロセス
		// シーンで新しく生成された剛体を格納
		for(int i = hprocess->expandedPHSolids.size(); i < expandedPHSolids.size(); i++){
			hprocess->expandedPHSolids.resize(hprocess->expandedPHSolids.size() + 1);
			hprocess->expandedPHSolids.back() = expandedPHSolids[i];
		}
		for(unsigned i = 0; i < hprocess->expandedPHSolids.size(); i++){
			// 初めて取得した近傍物体のみ行う
			if(expandedPHSolids[i].flag.bfirstlocal){
				hprocess->expandedPHSolids[i].haSolid = expandedPHSolids[i].haSolid;
				expandedPHSolids[i].flag.bfirstlocal = false;
			}
			hprocess->expandedPHSolids[i].flag.blocal = expandedPHSolids[i].flag.blocal;
			hprocess->expandedPHSolids[i].syncInfo = expandedPHSolids[i].syncInfo;
		}

		// 物理プロセスで使用する刻み時間
		//GetPHScene()->SetTimeStep(0.001f * (float)hcount);
		//dt = 0.001f * (float)hcount;
		hprocess->loopCounter = 0;

		// 同期終了のフラグ
		bSync = false;
	}
#define COUNT_MAX 300
	if(hprocess->loopCounter > COUNT_MAX) {
		DSTR << "too many step" << std::endl;
		hprocess->loopCounter = 0;
		bSync = false;
	}
	hprocess->loopCounter++;
	hapticcount++;
}

}
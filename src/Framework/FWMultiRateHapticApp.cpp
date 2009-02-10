#include <Framework/FWMultiRateHapticApp.h>
#include <sstream>

#ifdef USE_HDRSTOP
#pragma  hdrstop
#endif

namespace Spr{;

FWMultiRateHapticApp::FWMultiRateHapticApp(){}

void FWMultiRateHapticApp::InitCameraView(){
	std::istringstream issView(
		"((0.9996 0.0107463 -0.0261432 -0.389004)"
		"(-6.55577e-010 0.924909 0.380188 5.65711)"
		"(0.0282657 -0.380037 0.92454 13.7569)"
		"(     0      0      0      1))"
	);
	issView >> cameraInfo.view;
}

void FWMultiRateHapticApp::Idle(){
	Step();
}

void FWMultiRateHapticApp::SyncHapticProcess(){}
void FWMultiRateHapticApp::ExpandPHSolidInfo(){}

void FWMultiRateHapticApp::FindNearestObjectFromHapticPointer(){
	///*
	//	GJKを使って近傍物体と近傍物体の最近点を取得
	//	これをすべてのshapeをもつ剛体についてやる
	//	AABBで力覚ポインタ近傍の物体を絞る
	//	ここで絞った物体についてGJKを行う．ここで絞ることでGJKをする回数を少なくできる．
	//	SolidのBBoxレベルでの交差判定(z軸ソート)．交差のおそれの無い組を除外
	//*/
	//
	////1. BBoxレベルの衝突判定
	//std::vector<ExpandedPHSolid>* esolids = GetExpandedPHSolids();
	//esolids->[0];
//	size_t N = expandedObjects.size();
//	Vec3f dir(0,0,1);
//	Edges edges;
//	edges.resize(2 * N);
//	Edges::iterator eit = edges.begin();
//	for(int i = 0; i < (int)N; ++i){
//		// ローカル判定をすべてfalseにする
//		expandedObjects[i].flag.bneighbor = false;
//		DCAST(PHSolid, expandedObjects[i].phSolidIf)->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
//		Vec3d dPos = expandedObjects[i].phSolidIf->GetDeltaPosition();
//		float dLen = (float) (dPos * dir);
//		if (dLen < 0){
//			eit[0].edge += dLen;
//		}else{
//			eit[1].edge += dLen;
//		}
//		eit[0].index = i; eit[0].bMin = true;
//		eit[1].index = i; eit[1].bMin = false;
//		eit += 2;
//	}
//	sort(edges.begin(), edges.end());
//	//端から見ていって，接触の可能性があるノードの判定をする．
//	typedef set<int> SolidSet;
//	SolidSet cur;							//	現在のSolidのセット
//	bool found = false;
//
//	for(Edges::iterator it = edges.begin(); it != edges.end(); ++it){
//		if (it->bMin){						//	初端だったら，リスト内の物体と判定
//			for(SolidSet::iterator itf=cur.begin(); itf != cur.end(); ++itf){
//				int f1 = it->index;
//				int f2 = *itf;
//				if (f1 > f2) std::swap(f1, f2);
//				// 近傍物体として決定
//				if(expandedObjects[f1].phSolidIf == soPointer){
//					expandedObjects[f2].flag.bneighbor = true;
//				}else if(expandedObjects[f2].phSolidIf == soPointer){
//					expandedObjects[f1].flag.bneighbor = true;
//				}
//			}
//			cur.insert(it->index);
//		}else{
//			cur.erase(it->index);			//	終端なので削除．
//		}
//	}
//
//	for(size_t i = 0; i < expandedObjects.size(); i++){
//		if(expandedObjects[i].flag.bneighbor) continue;
//		expandedObjects[i].flag.bfirstlocal = false;			//近傍物体でないのでfalseにする
//		expandedObjects[i].flag.blocal = false;
//	}
//
//	// GJKで近傍点を求め，力覚ポインタ最近傍の物体を決定する
//	// 最近傍物体だったらblocalをtrueにし，phSolidにphSolidIfをコピーする
//	// blocalがすでにtrueだったらコピー済みなので近傍点だけコピーする
//	for(unsigned i = 0; i < (int)expandedObjects.size(); i++){
//		if(!expandedObjects[i].flag.bneighbor) continue;															// 近傍でなければ次へ
//		CDConvex* a = DCAST(CDConvex, expandedObjects[i].phSolidIf->GetShape(0));		// 剛体が持つ凸形状
//		CDConvex* b = DCAST(CDConvex, soPointer->GetShape(0));									// 力覚ポインタの凸形状
//		Posed a2w, b2w;																								// 剛体のワールド座標
//*		if(expandedObjects[i].blocal) 	a2w = expandedObjects[i].phSolid.GetPose();				// blocalがtrueなら最新の情報でやる
//		else */								a2w = expandedObjects[i].phSolidIf->GetPose();
//		b2w = soPointer->GetPose();																			// 力覚ポインタのワールド座標
//		Vec3d pa ,pb;																									// pa:剛体の近傍点，pb:力覚ポインタの近傍点（ローカル座標）
//		pa = pb = Vec3d(0.0, 0.0, 0.0);
//		FindClosestPoints(a, b, a2w, b2w, pa, pb);															// GJKで近傍点の算出
//		Vec3d wa = a2w * pa;																						// 剛体近傍点のワールド座標
//		Vec3d wb = b2w * pb;																						// 力覚ポインタ近傍点のワールド座標
//		Vec3d a2b = wb - wa;																						// 剛体から力覚ポインタへのベクトル
//		Vec3d normal = a2b.unit();
//		if(a2b.norm() < range){																					// 近傍点までの長さから近傍物体を絞る
//			if(a2b.norm() < 0.01){																				// 力覚ポインタと剛体がすでに接触していたらCCDGJKで法線を求める		
//				pa = pb = Vec3d(0.0, 0.0, 0.0);
//				Vec3d dir = -1.0 * expandedObjects[i].syncInfo.neighborPoint.face_normal;
//				if(dir == Vec3d(0.0, 0.0, 0.0) ){
//					dir = -(soPointer->GetCenterPosition() - wa);
//				}
//				double dist = 0.0;
//				int cp = ContFindCommonPoint(a, b, a2w, b2w, dir, -DBL_MAX, 1, normal, pa, pb, dist);
//				if(cp != 1){
//					ContFindCommonPointSaveParam(a, b, a2w, b2w, dir, -DBL_MAX, 1, normal, pa, pb, dist);
//					DSTR << "ContFindCommonPoint do not find contact point" << endl;
//				}
//			}
//			if(!expandedObjects[i].flag.blocal){																			// 初めて最近傍物体になった時
//				expandedObjects[i].flag.bfirstlocal = true;
//				expandedObjects[i].phSolid = *DCAST(PHSolid, expandedObjects[i].phSolidIf);			// シーンが持つ剛体の中身を力覚プロセスで使う剛体（実体）としてコピーする
//				expandedObjects[i].syncInfo.neighborPoint.face_normal = normal;							// 初めて最近傍物体になったので，前回の法線に今回できた法線を上書きする．										
//#ifdef _DEBUG
//				if (expandedObjects[i].syncInfo.neighborPoint.face_normal * normal < 0.8){
//					DSTR << "Too big change on normal" << normal << std::endl;
//				}
//#endif
//			}
//			expandedObjects[i].flag.blocal = true;																		// 最近傍物体なのでblocalをtrueにする
//			expandedObjects[i].syncInfo.neighborPoint.closestPoint = pa;										// 剛体近傍点のローカル座標
//			expandedObjects[i].syncInfo.neighborPoint.pointerPoint = pb;										// 力覚ポインタ近傍点のローカル座標
//			expandedObjects[i].syncInfo.neighborPoint.last_face_normal = expandedObjects[i].syncInfo.neighborPoint.face_normal;					// 前回の法線(法線の補間に使う)，初めて近傍になった時は今回できた法線
//			expandedObjects[i].syncInfo.neighborPoint.face_normal = normal;								// 剛体から力覚ポインタへの法線
//		}else{
//			expandedObjects[i].flag.blocal = false;																		// 最近傍物体ではないのでblocalをfalseにする
//			expandedObjects[i].flag.bfirstlocal = false;
//		}
//	}
}
}
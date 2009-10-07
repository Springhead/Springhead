/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Framework/FWInteractAdaptee.h>
#include <Framework/FWInteractScene.h>
#include <Physics/PHConstraintEngine.h>
#include <iostream>

namespace Spr{;

/** FWHapticLoopBaseの実装 */
///////////////////////////////////////////////////////////////////////////////////

void FWHapticLoopBase::Clear(){
	GetIASolids()->clear();
	loopCount = 1;
}

/** FWInteractAdapteeの実装 */
///////////////////////////////////////////////////////////////////////////////////
FWInteractAdaptee::FWInteractAdaptee(){}
/// IAScene関係
void FWInteractAdaptee::SetIAScene(FWInteractScene* iScene){ interactScene = iScene; }
FWInteractScene* FWInteractAdaptee::GetIAScene(){ return interactScene; }			
PHSceneIf* FWInteractAdaptee::GetPHScene(){ return GetIAScene()->GetScene()->GetPHScene(); }

/// HapticLoop関係
FWHapticLoopBase* FWInteractAdaptee::GetHapticLoop(){ return NULL; }
void FWInteractAdaptee::SetHMode(FWHapticMode hMode) {GetHapticLoop()->hmode = hMode;}

/// IAPointer関係(IASceneを介して取得)
FWInteractPointer* FWInteractAdaptee::GetIAPointer(int i){ return GetIAScene()->GetIAPointer(i)->Cast(); }
FWInteractPointers* FWInteractAdaptee::GetIAPointers(){ return GetIAScene()->GetIAPointers(); }
int FWInteractAdaptee::NIAPointers(){ return GetIAScene()->NIAPointers(); }

/// IASolid関係(IASceneを介して取得)
FWInteractSolid* FWInteractAdaptee::GetIASolid(int i){ return GetIAScene()->GetIASolid(i); }
FWInteractSolids* FWInteractAdaptee::GetIASolids(){ return GetIAScene()->GetIASolids(); }
int FWInteractAdaptee::NIASolids(){ return GetIAScene()->NIASolids(); }

void FWInteractAdaptee::UpdateSolidList(){
	PHSceneIf* phScene = GetPHScene(); 
	PHSolidIf** phSolids = phScene->GetSolids();
	for(int i = NIASolids(); i < phScene->NSolids(); i++){
		GetIASolids()->resize(i + 1);
		GetIASolids()->back().sceneSolid = phSolids[i]->Cast();
		for(int j = 0; j < NIAPointers(); j++){
			GetIAPointer(j)->interactInfo.resize(i + 1);
		}
	}
}

void FWInteractAdaptee::NeighborObjectFromPointer(){
	// GJKを使って近傍物体と近傍物体の最近点を取得
	// これをすべてのshapeをもつ剛体についてやる
	for(int i = 0; i < NIASolids(); i++){
		int lCount = 0;					///< flag.blocalの数
		int fCount = 0;					///< flag.bfirstlocalの数
		PHSolid* phSolid = GetIASolid(i)->sceneSolid;
		/// IAPointerの数だけやる
		for(int j = 0; j < NIAPointers(); j++){
			FWInteractPointer* iPointer = GetIAPointer(j);
			FWInteractInfo* iaInfo = &GetIAPointer(j)->interactInfo[i];
			PHSolid* soPointer = GetIAPointer(j)->pointerSolid->Cast();

			iaInfo->flag.bneighbor = false;				// ローカル可能性の初期化
			/// Solidが他のポインタであった場合
			for(int k = 0; k < NIAPointers(); k++){
				if(phSolid == GetIAPointer(k)->pointerSolid->Cast()) phSolid = NULL;
			}
			if (soPointer != phSolid && phSolid){
				/* AABBで力覚ポインタ近傍の物体を絞る
				   ここで絞った物体についてGJKを行う．ここで絞ることでGJKをする回数を少なくできる．
				*/
				/// 1. BBoxレベルの衝突判定(Sweep & Prune)
				Vec3d range = Vec3d(1, 1, 1) * iPointer->GetLocalRange();
				Vec3d pMin = soPointer->GetPose() * soPointer->bbox.GetBBoxMin() - range;		// PointerのBBoxの最小値(3軸)
				Vec3d pMax = soPointer->GetPose() * soPointer->bbox.GetBBoxMax() + range;		// PointerのBBoxの最大値(3軸)
				Vec3d soMin = phSolid->GetPose().Pos() + phSolid->bbox.GetBBoxMin();		// SolidのBBoxの最小値(3軸)
				Vec3d soMax = phSolid->GetPose().Pos() + phSolid->bbox.GetBBoxMax();		// SolidのBBoxの最大値(3軸)
				/// 3軸で判定
				int isLocal = 3;		//< いくつの軸で交差しているかどうか
				for(int i = 0; i < 3; ++i){
					int in = 0;
					/// ポインタのエッジ間にソリッドのエッジがあったら交差
					if(pMin[i] <= soMin[i] && soMin[i] <= pMax[i]) in++; 
					if(pMin[i] <= soMax[i] && soMax[i] <= pMax[i]) in++; 
					/// ソリッドのエッジ間にポインタの最大値があったら交差
					if(soMin[i] <= pMin[i] && pMin[i] <= soMax[i]) in++;
					if(soMin[i] <= pMax[i] && pMax[i] <= soMax[i]) in++;
					/// inが1以上ならその軸で交差
					if(in > 0) isLocal++;
#if 0
					DSTR << i << " pMin[i] = " << pMin[i] << "  soMin[i] = " << soMin[i] << "  pMax[i] = " << pMax[i] << std::endl;
					DSTR << i << " pMin[i] = "  << pMin[i] << "  soMax[i] = " << soMax[i] << "  pMax[i] = " << pMax[i] << std::endl;
					DSTR << i << " soMin[i] = " << soMin[i] << "  pMin[i] = " << pMin[i] << "  soMax[i] = " << soMax[i] << std::endl;
					DSTR << i << " soMin[i] = " << soMin[i] << "  pMax[i] = " << pMax[i] << "  soMax[i] = " << soMax[i] << std::endl;
#endif
				}
//				DSTR << "isLocal" << isLocal <<  std::endl;
				/// 2.近傍の可能性がある物体は詳細判定(GJKへ)
				if(isLocal > 2){
					iaInfo->flag.bneighbor = true;
					UpdateInteractSolid(i, GetIAPointer(j));
				}
			}
			/// bneighborかつblocalであればlCount++
			if(iaInfo->flag.bneighbor){
				if(iaInfo->flag.blocal){
					lCount++;
					/// さらにbfirstlocalであればfCount++
					if(iaInfo->flag.bfirstlocal){
						fCount++;
						iaInfo->flag.bfirstlocal = false;
					}
				}else{
					iaInfo->flag.bfirstlocal = false;							
					iaInfo->flag.blocal = false;													
				}
			}else{
				/// 近傍物体でないのでfalseにする
				iaInfo->flag.bfirstlocal = false;							
				iaInfo->flag.blocal = false;									
			}
		}	// end IAPointerの数だけやる
		/// 初シミュレーションの処理フラグをtrueにする
		FWInteractSolid* inSolid = GetIASolid(i);
		if(fCount > 0){
			inSolid->bfirstSim = true;	
			inSolid->copiedSolid = *inSolid->sceneSolid;	// シーンが持つ剛体の中身を力覚プロセスで使う剛体（実体）としてコピーする
		}
		/// シミュレーションの処理フラグをtrueにする
		if(lCount > 0){
			inSolid->bSim = true;					
		}else{
			/// それ以外は全てfalseにする
			inSolid->bSim = false;					
			inSolid->bfirstSim = false;
		}
	}
//	DSTR << "-------------------------------------------------" << std::endl;
}

void FWInteractAdaptee::UpdateInteractSolid(int index, FWInteractPointer* iPointer){
	PHSolid* phSolid = GetIASolid(index)->sceneSolid;
	PHSolid* soPointer = iPointer->GetPointerSolid()->Cast();

	FWInteractInfo* iaInfo = &iPointer->interactInfo[index]; 
	if (!phSolid->NShape()==0){													///< 形状を持たない剛体の場合は行わない
		CDConvexIf* a = DCAST(CDConvexIf, phSolid->GetShape(0));				///< 剛体が持つ凸形状
		CDConvexIf* b = DCAST(CDConvexIf, soPointer->GetShape(0));				///< 力覚ポインタの凸形状
		Posed a2w = phSolid->GetPose();											///< 剛体の姿勢
		Posed b2w = soPointer->GetPose();										///< 力覚ポインタの姿勢
		Vec3d dir = -1.0 * iaInfo->neighborInfo.face_normal;
		Vec3d cp = phSolid->GetCenterPosition();								///< 剛体の重心
		Vec3d normal;															///< 剛体から力覚ポインタへの法線(ワールド座標)
		Vec3d pa, pb;															///< pa:剛体の近傍点，pb:力覚ポインタの近傍点（ローカル座標）

		/// GJKを使った近傍点探索
		double r = FindNearestPoint(a, b, a2w, b2w, cp, dir, normal, pa, pb);	
		/// 近傍点までの長さから近傍物体を絞る
		if(r < iPointer->GetLocalRange()){
			/// 初めて最近傍物体になった場合
			if(iaInfo->flag.blocal == false){																
				iaInfo->flag.bfirstlocal = true;	
				iaInfo->neighborInfo.face_normal = normal;	// 初めて近傍物体になったので，前回の法線に今回できた法線を上書きする．										
				#ifdef _DEBUG
					if (iaInfo->neighborInfo.face_normal * normal < 0.8){
						DSTR << "Too big change on normal = " << normal << std::endl;
					}
				#endif
			}
			/// 初めて近傍または継続して近傍だった場合
			iaInfo->flag.blocal = true;								// 近傍物体なのでblocalをtrueにする
			iaInfo->neighborInfo.closest_point = pa;				// 剛体近傍点のローカル座標
			iaInfo->neighborInfo.pointer_point = pb;				// 力覚ポインタ近傍点のローカル座標
			iaInfo->neighborInfo.last_face_normal = iaInfo->neighborInfo.face_normal;		// 前回の法線(法線の補間に使う)，初めて近傍になった時は今回できた法線
			iaInfo->neighborInfo.face_normal = normal;				// 剛体から力覚ポインタへの法線
		}else{
			/// 近傍物体ではないのでfalseにする
			iaInfo->flag.bneighbor = false;
			iaInfo->flag.bfirstlocal = false;						
			iaInfo->flag.blocal = false;																
		}
	}
}

double FWInteractAdaptee::FindNearestPoint(const CDConvexIf* a, const CDConvexIf* b,
											const Posed& a2w, const Posed& b2w, const Vec3d pc, Vec3d& dir, 
											Vec3d& normal, Vec3d& pa, Vec3d& pb){
	/// GJKで近傍点を求め，力覚ポインタ最近傍の物体を決定する
	CDConvex* ca = DCAST(CDConvex, a);
	CDConvex* cb = DCAST(CDConvex, b);
	FindClosestPoints(ca, cb, a2w, b2w, pa, pb);	///< GJKで近傍点の算出
	Vec3d wa = a2w * pa;							///< 剛体近傍点のワールド座標
	Vec3d wb = b2w * pb;							///< 力覚ポインタ近傍点のワールド座標
	Vec3d a2b = wb - wa;							///< 剛体から力覚ポインタへのベクトル
	normal = a2b.unit();
	//DSTR << "dir" << dir<<std::endl;
	/// 力覚ポインタと剛体がすでに接触していたらCCDGJKで法線を求める
	if(a2b.norm() < 0.01){									
		pa = pb = Vec3d(0.0, 0.0, 0.0);
		/// dirが潰れてしまっている場合は剛体重心から近傍点へのベクトルとする
		if(dir == Vec3d(0.0, 0.0, 0.0) ){
			dir =-( wa - pc);
			//DSTR << "dir not find"<<std::endl;
		}
#if 0
		///Sphereの場合，GJKの結果を使わない
		if(DCAST(CDSphereIf,a)) dir =-( wa - pc);
#endif

		double dist = 0.0;
		/// CCDGJKの実行
			//DSTR << "dir="<< dir << std::endl;
		
extern bool bGJKDebug;
		bGJKDebug = false;
//		bCCDGJKDebug = true;
		int cp = ContFindCommonPoint(ca, cb, a2w, b2w, dir, -DBL_MAX, 1, normal, pa, pb, dist);
		bGJKDebug = false;
		/// CCDGJKが失敗した場合の処理
		if(cp != 1){
			ContFindCommonPointSaveParam(ca, cb, a2w, b2w, dir, -DBL_MAX, 1, normal, pa, pb, dist);
			DSTR << "ContFindCommonPoint do not find contact point" << std::endl;
		}
	}
	return a2b.norm();
}


}
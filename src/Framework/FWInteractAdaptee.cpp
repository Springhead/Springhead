#include <Framework/FWInteractAdaptee.h>
#include <Framework/FWInteractScene.h>
#include <Physics/PHConstraintEngine.h>
#include <iostream>

namespace Spr{;

/** FWHapticLoopBaseの実装 */
///////////////////////////////////////////////////////////////////////////////////

void FWHapticLoopBase::Clear(){
	GetINSolids()->clear();
	loopCount = 1;
}

/** FWInteractAdapteeの実装 */
///////////////////////////////////////////////////////////////////////////////////
FWInteractAdaptee::FWInteractAdaptee(){}
/// INScene関係
void FWInteractAdaptee::SetINScene(FWInteractScene* iScene){ interactScene = iScene; }
FWInteractScene* FWInteractAdaptee::GetINScene(){ return interactScene; }			
PHSceneIf* FWInteractAdaptee::GetPHScene(){ return GetINScene()->GetScene()->GetPHScene(); }

/// HapticLoop関係
FWHapticLoopBase* FWInteractAdaptee::GetHapticLoop(){ return NULL; }

/// INPointer関係(INSceneを介して取得)
FWInteractPointer* FWInteractAdaptee::GetINPointer(int i){ return GetINScene()->GetINPointer(i)->Cast(); }
FWInteractPointers* FWInteractAdaptee::GetINPointers(){ return GetINScene()->GetINPointers(); }
int FWInteractAdaptee::NINPointers(){ return GetINScene()->NINPointers(); }

/// INSolid関係(INSceneを介して取得)
FWInteractSolid* FWInteractAdaptee::GetINSolid(int i){ return GetINScene()->GetINSolid(i); }
FWInteractSolids* FWInteractAdaptee::GetINSolids(){ return GetINScene()->GetINSolids(); }
int FWInteractAdaptee::NINSolids(){ return GetINScene()->NINSolids(); }

void FWInteractAdaptee::UpdateSolidList(){
	PHSceneIf* phScene = GetPHScene(); 
	PHSolidIf** phSolids = phScene->GetSolids();
	for(int i = NINSolids(); i < phScene->NSolids(); i++){
		GetINSolids()->resize(i + 1);
		GetINSolids()->back().sceneSolid = phSolids[i]->Cast();
		for(int j = 0; j < NINPointers(); j++){
			GetINPointer(j)->interactInfo.resize(i + 1);
		}
	}
}

void FWInteractAdaptee::NeighborObjectFromPointer(){
	// GJKを使って近傍物体と近傍物体の最近点を取得
	// これをすべてのshapeをもつ剛体についてやる
	for(int i = 0; i < NINSolids(); i++){
		int lCount = 0;					///< flag.blocalの数
		int fCount = 0;					///< flag.bfirstlocalの数
		PHSolid* phSolid = GetINSolid(i)->sceneSolid;
		/// INPointerの数だけやる
		for(int j = 0; j < NINPointers(); j++){
			FWInteractPointer* inPointer = GetINPointer(j);
			FWInteractInfo* inInfo = &GetINPointer(j)->interactInfo[i];
			PHSolid* soPointer = GetINPointer(j)->pointerSolid->Cast();

			inInfo->flag.bneighbor = false;				// ローカル可能性の初期化
			/// Solidが他のポインタであった場合
			for(int k = 0; k < NINPointers(); k++){
				if(phSolid == GetINPointer(k)->pointerSolid->Cast()) phSolid = NULL;
			}
			if (soPointer != phSolid && phSolid){
				/* AABBで力覚ポインタ近傍の物体を絞る
				   ここで絞った物体についてGJKを行う．ここで絞ることでGJKをする回数を少なくできる．
				*/
				/// 1. BBoxレベルの衝突判定(Sweep & Prune)
				Vec3d range = Vec3d(1, 1, 1) * inPointer->GetLocalRange();
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
					inInfo->flag.bneighbor = true;
					UpdateInteractSolid(i, GetINPointer(j));
				}
			}
			/// bneighborかつblocalであればlCount++
			if(inInfo->flag.bneighbor){
				if(inInfo->flag.blocal){
					lCount++;
					/// さらにbfirstlocalであればfCount++
					if(inInfo->flag.bfirstlocal){
						fCount++;
						inInfo->flag.bfirstlocal = false;
					}
				}else{
					inInfo->flag.bfirstlocal = false;							
					inInfo->flag.blocal = false;													
				}
			}else{
				/// 近傍物体でないのでfalseにする
				inInfo->flag.bfirstlocal = false;							
				inInfo->flag.blocal = false;									
			}
		}	// end INPointerの数だけやる
		/// 初シミュレーションの処理フラグをtrueにする
		FWInteractSolid* inSolid = GetINSolid(i);
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

void FWInteractAdaptee::UpdateInteractSolid(int index, FWInteractPointer* inPointer){
	PHSolid* phSolid = GetINSolid(index)->sceneSolid;
	PHSolid* soPointer = inPointer->GetPointerSolid()->Cast();

	FWInteractInfo* inInfo = &inPointer->interactInfo[index]; 
	if (!phSolid->NShape()==0){													///< 形状を持たない剛体の場合は行わない
		CDConvexIf* a = DCAST(CDConvexIf, phSolid->GetShape(0));				///< 剛体が持つ凸形状
		CDConvexIf* b = DCAST(CDConvexIf, soPointer->GetShape(0));				///< 力覚ポインタの凸形状
		Posed a2w = phSolid->GetPose();											///< 剛体の姿勢
		Posed b2w = soPointer->GetPose();										///< 力覚ポインタの姿勢
		Vec3d dir = -1.0 * inInfo->neighborInfo.face_normal;
		Vec3d cp = phSolid->GetCenterPosition();								///< 剛体の重心
		Vec3d normal;															///< 剛体から力覚ポインタへの法線(ワールド座標)
		Vec3d pa, pb;															///< pa:剛体の近傍点，pb:力覚ポインタの近傍点（ローカル座標）

		/// GJKを使った近傍点探索
		double r = FindNearestPoint(a, b, a2w, b2w, cp, dir, normal, pa, pb);	
		/// 近傍点までの長さから近傍物体を絞る
		if(r < inPointer->GetLocalRange()){
			/// 初めて最近傍物体になった場合
			if(inInfo->flag.blocal == false){																
				inInfo->flag.bfirstlocal = true;	
				inInfo->neighborInfo.face_normal = normal;	// 初めて近傍物体になったので，前回の法線に今回できた法線を上書きする．										
				#ifdef _DEBUG
					if (inInfo->neighborInfo.face_normal * normal < 0.8){
						DSTR << "Too big change on normal = " << normal << std::endl;
					}
				#endif
			}
			/// 初めて近傍または継続して近傍だった場合
			inInfo->flag.blocal = true;								// 近傍物体なのでblocalをtrueにする
			inInfo->neighborInfo.closest_point = pa;				// 剛体近傍点のローカル座標
			inInfo->neighborInfo.pointer_point = pb;				// 力覚ポインタ近傍点のローカル座標
			inInfo->neighborInfo.last_face_normal = inInfo->neighborInfo.face_normal;		// 前回の法線(法線の補間に使う)，初めて近傍になった時は今回できた法線
			inInfo->neighborInfo.face_normal = normal;				// 剛体から力覚ポインタへの法線
		}else{
			/// 近傍物体ではないのでfalseにする
			inInfo->flag.bneighbor = false;
			inInfo->flag.bfirstlocal = false;						
			inInfo->flag.blocal = false;																
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
		
extern bool bCCDGJKDebug;
		bCCDGJKDebug = false;
//		bCCDGJKDebug = true;
		int cp = ContFindCommonPoint(ca, cb, a2w, b2w, dir, -DBL_MAX, 1, normal, pa, pb, dist);
		bCCDGJKDebug = false;
		/// CCDGJKが失敗した場合の処理
		if(cp != 1){
			ContFindCommonPointSaveParam(ca, cb, a2w, b2w, dir, -DBL_MAX, 1, normal, pa, pb, dist);
			DSTR << "ContFindCommonPoint do not find contact point" << std::endl;
		}
	}
	return a2b.norm();
}


}
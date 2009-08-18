#include <Framework/FWInteractAdaptee.h>
#include <Framework/FWInteractScene.h>
#include <Physics/PHConstraintEngine.h>
#include <iostream>

namespace Spr{;

/** FWHapticLoopBaseの実装 */
///////////////////////////////////////////////////////////////////////////////////

void FWHapticLoopBase::Clear(){
	GetInteractSolids()->clear();
	loopCount = 1;
}

/** FWInteractAdapteeの実装 */
///////////////////////////////////////////////////////////////////////////////////
FWInteractAdaptee::FWInteractAdaptee(){}
FWInteractScene* FWInteractAdaptee::GetInteractScene(){
	return interactScene; 
}										
FWHapticLoopBase* GetHapticLoop(){
	return NULL; 
}
FWInteractSolid* FWInteractAdaptee::GetInteractSolid(int i){
	if(0 <= i && i < NInteractSolids()) return &interactSolids[i];
	return NULL;
}
FWInteractSolids* FWInteractAdaptee::GetInteractSolids(){
	return &interactSolids;
}
int FWInteractAdaptee::NInteractSolids(){
	return (int)interactSolids.size();
}
void FWInteractAdaptee::UpdateSolidList(){
	PHSceneIf* phScene = GetInteractScene()->GetScene()->GetPHScene(); 
	PHSolidIf** solids = phScene->GetSolids();
	FWInteractPointers* iPointers = GetInteractScene()->GetInteractPointers();
	for(int i = (int)GetInteractSolids()->size(); i < phScene->NSolids(); i++){
		GetInteractSolids()->resize(i + 1);
		GetInteractSolids()->back().sceneSolid = solids[i]->Cast();
		DSTR << GetInteractSolids()->size() << std::endl;
		for(int j = 0; j < (int)iPointers->size(); j++){
			FWInteractPointer* ip = iPointers->at(j)->Cast();
			FWInteractInfo info;
			ip->interactInfo.resize(i + 1);
//			DSTR << ip->interactInfo.size() << std::endl;
		}
	}
}

void FWInteractAdaptee::NeighborObjectFromPointer(){
	// GJKを使って近傍物体と近傍物体の最近点を取得
	// これをすべてのshapeをもつ剛体についてやる
	// AABBで力覚ポインタ近傍の物体を絞る
	// ここで絞った物体についてGJKを行う．ここで絞ることでGJKをする回数を少なくできる．
	// SolidのBBoxレベルでの交差判定(z軸ソート)．交差のおそれの無い組を除外		
	//1. BBoxレベルの衝突判定
	FWInteractSolid* iSolid;
	FWInteractPointer* iPointer;
	FWInteractInfo* iInfo;

	Vec3f dir(0,0,1);
	Edges edges;
	edges.resize(2 * NInteractSolids());
	Edges::iterator eit = edges.begin();
	int pN = GetInteractScene()->NInteractPointers();
	for(int i = 0; i < NInteractSolids(); ++i){
		for(int j = 0; j < pN; j++){
			iPointer = GetInteractScene()->GetInteractPointer(j)->Cast();
			iInfo = &iPointer->interactInfo[i];
			iInfo->flag.bneighbor = false;
		}
		iSolid = GetInteractSolid(i);
		iSolid->sceneSolid->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
		Vec3d dPos = iSolid->sceneSolid->GetDeltaPosition();
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
	sort(edges.begin(), edges.end());

	//端から見ていって，接触の可能性があるノードの判定をする．
	typedef std::set<int> SolidSet;
	SolidSet cur;							//	現在のSolidのセット
	bool found = false;
	int nP = GetInteractScene()->NInteractPointers();
	for(int i = 0; i < nP; i++){
		iPointer = GetInteractScene()->GetInteractPointer(i)->Cast();
		PHSolid* soPointer = DCAST(PHSolid, iPointer->GetPointerSolid());
		for(Edges::iterator it = edges.begin(); it != edges.end(); ++it){
			///	初端だったら，リスト内の物体と判定
			if (it->bMin){						
				for(SolidSet::iterator itf=cur.begin(); itf != cur.end(); ++itf){
					int f1 = it->index;
					int f2 = *itf;
					if (f1 > f2) std::swap(f1, f2);
					/// 近傍物体の可能性あり，詳細判定へ
					if(GetInteractSolid(f1)->sceneSolid == soPointer){
						iPointer->interactInfo[f2].flag.bneighbor = true;
						UpdateInteractSolid(f2, iPointer);
					}else if(GetInteractSolid(f2)->sceneSolid == soPointer){
						iPointer->interactInfo[f1].flag.bneighbor = true;
						UpdateInteractSolid(f1, iPointer);
					}
				}
				cur.insert(it->index);
			}else{
				///	終端なので削除．
				cur.erase(it->index);	
			}
		}
	}
	
	for(int i = 0; i < NInteractSolids(); i++){
		int lCount = 0;					///< flag.blocalの数
		int fCount = 0;					///< flag.bfirstlocalの数									
		iSolid = GetInteractSolid(i);
		for(int j = 0; j < nP; j++){
			iPointer = GetInteractScene()->GetInteractPointer(j)->Cast();
			FWInteractInfo* iInfo = &iPointer->interactInfo[i];
			/// bneighborかつblocalであればlCount++
			if(iInfo->flag.bneighbor && iInfo->flag.blocal){				
				lCount++;
				/// さらにbfirstlocalであればfCount++
				if(iInfo->flag.bfirstlocal){
					fCount++;
					DSTR << fCount << std::endl;
				}
			}else{
				/// 近傍物体でないのでfalseにする
				iInfo->flag.bfirstlocal = false;							
				iInfo->flag.blocal = false;									
			}
		}
		/// 初シミュレーションの処理フラグをtrueにする
		if(fCount > 0){
			iSolid->bfirstSim = true;	
			iSolid->copiedSolid = *iSolid->sceneSolid;	// シーンが持つ剛体の中身を力覚プロセスで使う剛体（実体）としてコピーする
		}
		/// シミュレーションの処理フラグをtrueにする
		if(lCount > 0){
			iSolid->bSim = true;					
		}else{
			/// それ以外は全てfalseにする
			iSolid->bSim = false;					
			iSolid->bfirstSim = false;
		}
	}
}

void FWInteractAdaptee::UpdateInteractSolid(int index, FWInteractPointer* iPointer){
	FWInteractInfo* iInfo = &iPointer->interactInfo[index]; 
	PHSolidIf* phSolid = GetInteractSolid(index)->sceneSolid->Cast();
	PHSolidIf* soPointer = iPointer->GetPointerSolid();
	CDConvexIf* a = DCAST(CDConvexIf, phSolid->GetShape(0));				///< 剛体が持つ凸形状
	CDConvexIf* b = DCAST(CDConvexIf, soPointer->GetShape(0));				///< 力覚ポインタの凸形状
	Posed a2w = phSolid->GetPose();											///< 剛体の姿勢
	Posed b2w = soPointer->GetPose();										///< 力覚ポインタの姿勢
	Vec3d dir = -1.0 * iInfo->neighborInfo.face_normal;
	Vec3d cp = phSolid->GetCenterPosition();								///< 剛体の重心
	Vec3d normal;															///< 剛体から力覚ポインタへの法線(ワールド座標)
	Vec3d pa, pb;															///< pa:剛体の近傍点，pb:力覚ポインタの近傍点（ローカル座標）

	/// GJKを使った近傍点探索
	double r = FindNearestPoint(a, b, a2w, b2w, dir, cp, normal, pa, pb);	

	/// 近傍点までの長さから近傍物体を絞る
	if(r < iPointer->GetLocalRange()){
		/// 初めて最近傍物体になった場合
		if(!iInfo->flag.blocal){																
			iInfo->flag.bfirstlocal = true;													
			iInfo->neighborInfo.face_normal = normal;	// 初めて近傍物体になったので，前回の法線に今回できた法線を上書きする．										
			#ifdef _DEBUG
				if (iInfo->neighborInfo.face_normal * normal < 0.8){
					DSTR << "Too big change on normal = " << normal << std::endl;
				}
			#endif
		}
		/// 初めて近傍または継続して近傍だった場合
		iInfo->flag.blocal = true;								// 近傍物体なのでblocalをtrueにする
		iInfo->neighborInfo.closest_point = pa;					// 剛体近傍点のローカル座標
		iInfo->neighborInfo.pointer_point = pb;					// 力覚ポインタ近傍点のローカル座標
		iInfo->neighborInfo.last_face_normal = iInfo->neighborInfo.face_normal;		// 前回の法線(法線の補間に使う)，初めて近傍になった時は今回できた法線
		iInfo->neighborInfo.face_normal = normal;				// 剛体から力覚ポインタへの法線
	}else{
		/// 近傍物体ではないのでfalseにする
		iInfo->flag.bneighbor = false;
		iInfo->flag.bfirstlocal = false;						
		iInfo->flag.blocal = false;																
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
	/// 力覚ポインタと剛体がすでに接触していたらCCDGJKで法線を求める
	if(a2b.norm() < 0.01){									
		pa = pb = Vec3d(0.0, 0.0, 0.0);
		/// dirが潰れてしまっている場合は剛体重心から近傍点へのベクトルとする
		if(dir == Vec3d(0.0, 0.0, 0.0) ){
			dir = wa - pc;
		}
		double dist = 0.0;
		/// CCDGJKの実行
		int cp = ContFindCommonPoint(ca, cb, a2w, b2w, dir, -DBL_MAX, 1, normal, pa, pb, dist);
		/// CCDGJKが失敗した場合の処理
		if(cp != 1){
			ContFindCommonPointSaveParam(ca, cb, a2w, b2w, dir, -DBL_MAX, 1, normal, pa, pb, dist);
			DSTR << "ContFindCommonPoint do not find contact point" << std::endl;
		}
	}
	return a2b.norm();
}


}
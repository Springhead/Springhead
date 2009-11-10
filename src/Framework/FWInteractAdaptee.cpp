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

void FWHapticLoopBase::SetRenderedForce(HIBaseIf* hi, bool bForce, SpatialVector f){
	if(bForce){
		if(DCAST(HIForceInterface6DIf, hi)){
			HIForceInterface6DIf* hif = hi->Cast();
			hif->SetForce(f.v(), Vec3d());
			#ifdef TORQUE
				hif->SetForce(outForce.v(), outForce.w());
			#endif
		}else{
			HIForceInterface3DIf* hif = hi->Cast();
			hif->SetForce(f.v());
		}
	}else{
		if(DCAST(HIForceInterface6DIf, hi)){
			HIForceInterface6DIf* hif = hi->Cast();
			hif->SetForce(Vec3d(), Vec3d());
			#ifdef TORQUE
				hif->SetForce(Vec3d(), Vec3d());
			#endif
		}else{
			HIForceInterface3DIf* hif = hi->Cast();
			hif->SetForce(Vec3d());
		}		
	}
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
void FWInteractAdaptee::InitIAAdaptee(){
	GetHapticLoop()->SetIAAdaptee(this);
}
void FWInteractAdaptee::SetHMode(FWHapticMode hMode) { GetHapticLoop()->hmode = hMode; }

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

			/// Solidが他のポインタであった場合
			for(int k = 0; k < NIAPointers(); k++){
				if(phSolid == GetIAPointer(k)->pointerSolid->Cast()) phSolid = NULL;
			}
			if (soPointer != phSolid && phSolid){
				/* AABBで力覚ポインタ近傍の物体を絞る
				   ここで絞った物体についてGJKを行う．ここで絞ることでGJKをする回数を少なくできる．
				*/
				/// 1. BBoxレベルの衝突判定(Sweep & Prune)
				/// sceneSolidのローカル座標系にそろえて判定を行う
				Vec3d range = Vec3d(1, 1, 1) * iPointer->GetLocalRange();
				Posed solidPose = phSolid->GetPose() * phSolid->GetShapePose(0);
				Posed pointerPose = soPointer->GetPose() * soPointer->GetShapePose(0);
				Vec3f soMax = phSolid->bbox.GetBBoxMax();												//sceneSolidのBBoxの最大値(3軸) sceneSolid座標系
				Vec3f soMin = phSolid->bbox.GetBBoxMin();												//sceneSolidのBBoxの最小値(3軸) sceneSolid座標系
				Vec3d pMin = solidPose.Inv() * pointerPose * soPointer->bbox.GetBBoxMin() - range;		// PointerのBBoxの最小値(3軸) sceneSolid座標系
				Vec3d pMax = solidPose.Inv() * pointerPose * soPointer->bbox.GetBBoxMax() + range;		// PointerのBBoxの最大値(3軸) sceneSolid座標系
				/// 3軸で判定
				int isLocal = 0;		//< いくつの軸で交差しているかどうか
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
#if 1
					DSTR << i << " pMin[i] = " << pMin[i] << "  soMin[i] = " << soMin[i] << "  pMax[i] = " << pMax[i] << std::endl;
					DSTR << i << " pMin[i] = "  << pMin[i] << "  soMax[i] = " << soMax[i] << "  pMax[i] = " << pMax[i] << std::endl;
					DSTR << i << " soMin[i] = " << soMin[i] << "  pMin[i] = " << pMin[i] << "  soMax[i] = " << soMax[i] << std::endl;
					DSTR << i << " soMin[i] = " << soMin[i] << "  pMax[i] = " << pMax[i] << "  soMax[i] = " << soMax[i] << std::endl;
					DSTR << "isLocal" << isLocal <<  std::endl;
#endif
				}
				/// 2.近傍物体と判定．接触解析および状態更新
				if(isLocal > 2){
					UpdateInteractSolid(i, GetIAPointer(j));
					lCount++;		//< ローカルなのでlCount++
					if(iaInfo->flag.bfirstlocal){
						fCount++;	//< firstLocalなのでfCount++;
						iaInfo->flag.bfirstlocal = false;
					}
				}else{
					iaInfo->flag.bfirstlocal = false;							
					iaInfo->flag.blocal = false;				
				}
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
		Posed a2w = phSolid->GetPose() * phSolid->GetShapePose(0);											///< 剛体の姿勢
		Posed b2w = soPointer->GetPose() * soPointer->GetShapePose(0);										///< 力覚ポインタの姿勢
		Vec3d dir = -1.0 * iaInfo->neighborInfo.face_normal;
		Vec3d cp = phSolid->GetCenterPosition();								///< 剛体の重心
		Vec3d normal;															///< 剛体から力覚ポインタへの法線(ワールド座標)
		Vec3d pa, pb;															///< pa:剛体の近傍点，pb:力覚ポインタの近傍点（ローカル座標）

		/// GJKを使った近傍点探索
		int found = 0;
		found = FindNearestPoint(a, b, a2w, b2w, cp, dir, normal, pa, pb);							
		
		/// 接触解析(susa実装中)
		Vec3d a2b = b2w * pb - a2w * pa;
		std::vector<Vec3d> section;
		Vec3d commonPoint;
		if(found == 1){
			/// FindClosestPointで終わった場合
			/// 力覚ポインタと剛体が離れているので，近づけて接触解析
			b2w.Pos() -= a2b;
			commonPoint = a2w * pa;
			FindSectionVertex(phSolid, soPointer, a2w, b2w, pa, pb, normal, commonPoint, section);
		}else if(found == 2){
			/// ContFindCommonPointで終わった場合
			///	既に接触している状態なので，そのまま接触解析
			commonPoint = a2w * pa + 0.5 * a2b;
			FindSectionVertex(phSolid, soPointer, a2w, b2w, pa, pb, normal, commonPoint, section);
		}
		iaInfo->neighborInfo.section = section;

		if(found > 0){
			/// 初めて最近傍物体になった場合
			if(iaInfo->flag.blocal == false){																
				iaInfo->flag.bfirstlocal = true;	
				iaInfo->neighborInfo.face_normal = normal;	//< 初めて近傍物体になったので，前回の法線に今回できた法線を上書きする．										
				#ifdef _DEBUG
					if (iaInfo->neighborInfo.face_normal * normal < 0.8){
						DSTR << "Too big change on normal = " << normal << std::endl;
					}
				#endif
			}
			/// 初めて近傍または継続して近傍だった場合
			iaInfo->flag.blocal = true;								//< 近傍物体なのでblocalをtrueにする
			iaInfo->neighborInfo.closest_point = pa;				//< 剛体近傍点のローカル座標
			iaInfo->neighborInfo.pointer_point = pb;				//< 力覚ポインタ近傍点のローカル座標
			iaInfo->neighborInfo.last_face_normal = iaInfo->neighborInfo.face_normal;		//< 前回の法線(法線の補間に使う)，初めて近傍になった時は今回できた法線
			iaInfo->neighborInfo.face_normal = normal;				//< 剛体から力覚ポインタへの法線
		}else{
			/// ここには入らない可能あり，要検証(susa)
			/// 近傍物体ではないのでfalseにする
			iaInfo->flag.bfirstlocal = false;						
			iaInfo->flag.blocal = false;																
		}
	}
}

int FWInteractAdaptee::FindNearestPoint(const CDConvexIf* a, const CDConvexIf* b,
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

	if(a2b.norm() >= 0.01){
		return 1;	// FindClosestPointsで見つけた 
	}else{
		/// 力覚ポインタと剛体がすでに接触していたらCCDGJKで法線を求める
		pa = pb = Vec3d(0.0, 0.0, 0.0);
		/// dirが潰れてしまっている場合は剛体重心から近傍点へのベクトルとする
		if(dir == Vec3d(0.0, 0.0, 0.0) ){
			dir =-( wa - pc);
		}

		/// CCDGJKの実行	
extern bool bGJKDebug;
		bGJKDebug = false;

		double dist = 0.0;
		int cp = ContFindCommonPoint(ca, cb, a2w, b2w, dir, -DBL_MAX, 1, normal, pa, pb, dist);

		bGJKDebug = false;
		/// CCDGJKが失敗した場合の処理
		if(cp != 1){
			ContFindCommonPointSaveParam(ca, cb, a2w, b2w, dir, -DBL_MAX, 1, normal, pa, pb, dist);
			DSTR << "ContFindCommonPoint do not find contact point" << std::endl;
		}
		return 2;	// CCDGJKで見つけた
	}
}


void FWInteractAdaptee::FindSectionVertex(PHSolid* solid0, PHSolid* solid1, const Posed shapePoseW0, const Posed shapePoseW1,
										  Vec3d pa, Vec3d pb, const Vec3d normal,
										  const Vec3d commonPoint, std::vector<Vec3d>& section){
	/// 力覚ポインタと剛体との接触部分解析
	// PHConstraintEngine.hにあるメンバ関数PHShapePairForLCP::EnumVertex()を改変
	// 本当はCDDetectorImp.hのクラスCDContactAnalysisFaceを使うべき？

	// 相対速度をみて2Dの座標系を決める。
	FPCK_FINITE(solid0->pose);
	FPCK_FINITE(solid1->pose);
	Vec3d v0 = solid0->GetPointVelocity(solid0->pose * pa);
	Vec3d v1 = solid1->GetPointVelocity(solid1->pose * pb);
	Matrix3d local;	//	contact coodinate system 接触の座標系
	local.Ex() = normal;
	local.Ey() = v1-v0;
	local.Ey() -= local.Ey() * normal * normal;
	if (local.Ey().square() > 1e-6){
		local.Ey().unitize(); 
	}else{
		if (Square(normal.x) < 0.5) local.Ey()= (normal ^ Vec3f(1,0,0)).unit();
		else local.Ey() = (normal ^ Vec3f(0,1,0)).unit();
	}
	local.Ez() =  local.Ex() ^ local.Ey();

	//	面と面が触れる場合があるので、接触が凸多角形や凸形状になることがある。
	//	切り口を求める。まず、それぞれの形状の切り口を列挙
	CDCutRing cutRing(commonPoint, local);	//	commonPointならば、それを含む面で切れば、必ず切り口の中になる。
	//	両方に切り口がある場合．(球などないものもある)
	CDConvex* convex0 = solid0->GetShape(0)->Cast();
	bool found = convex0->FindCutRing(cutRing, shapePoseW0);
	int nLine0 = cutRing.lines.size();
	CDConvex* convex1 = solid1->GetShape(0)->Cast();
	if (found) found = convex1->FindCutRing(cutRing, shapePoseW1);
	int nLine1 = cutRing.lines.size() - nLine0;
	section.clear();
	if (found){
		//	2つの切り口のアンドをとって、2物体の接触面の形状を求める。
		cutRing.MakeRing();		
		if (cutRing.vtxs.begin != cutRing.vtxs.end && !(cutRing.vtxs.end-1)->deleted){
			CDQHLine<CDCutLine>* vtx = cutRing.vtxs.end-1;
			do{
				assert(finite(vtx->dist));
				Vec3d pos;
				pos.sub_vector(1, Vec2d()) = vtx->normal / vtx->dist;
				pos = cutRing.local * pos;
				section.push_back(pos);
				vtx = vtx->neighbor[0];
			} while (vtx!=cutRing.vtxs.end-1);
		}
	}
	if (section.size() == 0){	//	ひとつも追加していない＝切り口がなかった or あってもConvexHullが作れなかった．
		//	きっと1点で接触している．
		section.push_back(commonPoint);
	}
#if 0
	DSTR << "commonPoint" << commonPoint << std::endl;
	DSTR << "section" << std::endl;
	for(int k = 0; k < section.size(); k++)		DSTR << section[k] << std::endl;
	DSTR << "--------------------------------" << std::endl;
#endif
}

Vec3d* FWInteractAdaptee::GetProxyPoint(){
	Vec3d DisplayProxy[2] = {Vec3d(5,0,0),Vec3d(5,0,0)};
	return DisplayProxy;
}

Vec3d* FWInteractAdaptee::GetForce(){
	Vec3d DisplayForce[2] = {Vec3d(0,0,0),Vec3d(0,0,0)};
	return DisplayForce;
}

bool FWInteractAdaptee::GetContactFlag(){
	return false;
}

}

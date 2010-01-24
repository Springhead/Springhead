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
#include <Collision/CDDetectorImp.h>
#include <iostream>

namespace Spr{;

/** FWHapticLoopBaseの実装 */
///////////////////////////////////////////////////////////////////////////////////

void FWHapticLoopBase::Clear(){
	GetIASolids()->clear();
	loopCount = 1;
}

#define TORQUE
void FWHapticLoopBase::SetRenderedForce(HIBaseIf* hi, bool bForce, SpatialVector f){
	if(bForce){
		if(DCAST(HIForceInterface6DIf, hi)){
			HIForceInterface6DIf* hif = hi->Cast();
			hif->SetForce(f.v(), Vec3d());
			#ifdef TORQUE
				hif->SetForce(f.v(), f.w());
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
			// 接触点の初期化
			iaInfo->neighborInfo.solid_section.clear();
			iaInfo->neighborInfo.pointer_section.clear();

			/// Solidが他のポインタであった場合
			for(int k = 0; k < NIAPointers(); k++){
				if(phSolid == GetIAPointer(k)->pointerSolid->Cast()) phSolid = NULL;
			}
			if (soPointer != phSolid && phSolid){
				/* AABBで力覚ポインタ近傍の物体を絞る
				   ここで絞った物体についてGJKを行う．ここで絞ることでGJKをする回数を少なくできる．
				*/
				/// 1. BBoxレベルの衝突判定(Sweep & Prune)
				/// sceneSolidのshape座標系にそろえて判定を行う
				Vec3d range = Vec3d(1, 1, 1) * iPointer->GetLocalRange();
				//Posed solidPose = phSolid->GetPose()*phSolid->GetShapePose(0) ;
				//Posed pointerPose = soPointer->GetPose() * soPointer->GetShapePose(0);
				Vec3f soMax = phSolid->GetShapePose(0).Inv() * phSolid->bbox.GetBBoxMax();		//sceneSolidのBBoxの最大値(3軸) sceneSolidのshape座標系
				Vec3f soMin = phSolid->GetShapePose(0).Inv() * phSolid->bbox.GetBBoxMin();		//sceneSolidのBBoxの最小値(3軸) sceneSolidのshape座標系
				Vec3d pMin = soPointer->GetShapePose(0) * soPointer->bbox.GetBBoxMin() ;		// PointerのBBoxの最小値(3軸) pointerのshape座標系
				Vec3d pMax = soPointer->GetShapePose(0) * soPointer->bbox.GetBBoxMax() ;		// PointerのBBoxの最大値(3軸) pointerのshape座標系

				//ポインターのBBoxの形が球状出ない場合にBBoxの最大値最小値が座標変換により変わってしまう
				//そこで，BBoxの大きさを大きくとる
				double pmin = pMin[0],pmax = pMax[0];
				for(int i=1;i<3;i++){
					if(pmin>pMin[i]) pmin = pMin[i];
					if(pmax<pMax[i]) pmax = pMax[i];
				}
				pMin =Vec3d(pmin,pmin,pmin); // PointerのBBoxの最小値(3軸) pointerのshape座標系(3軸で一番最小の値)
				pMax =Vec3d(pmax,pmax,pmax); // PointerのBBoxの最大値(3軸) pointerのshape座標系(3軸で一番最大の値)

				pMin = phSolid->GetShapePose(0).Inv() * phSolid->GetPose().Inv() * soPointer->GetPose() * pMin - range;		// PointerのBBoxの最小値(3軸) sceneSolidのshape座標系
				pMax = phSolid->GetShapePose(0).Inv() * phSolid->GetPose().Inv() * soPointer->GetPose() * pMax + range;		// PointerのBBoxの最大値(3軸) sceneSolidのshape座標系
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
#if 0
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
			inSolid->b = inSolid->lastb = SpatialVector();
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
		Posed a2w = phSolid->GetPose() * phSolid->GetShapePose(0);				///< 剛体の姿勢
		Posed b2w = soPointer->GetPose() * soPointer->GetShapePose(0);			///< 力覚ポインタの姿勢
		Posed shapePoseW0 = phSolid->GetShapePose(0);
		Posed shapePoseW1 = soPointer->GetShapePose(0);
		Vec3d dir = -1.0 * iaInfo->neighborInfo.face_normal;
		Vec3d cp = phSolid->GetCenterPosition();								///< 剛体の重心
		Vec3d normal;															///< 剛体から力覚ポインタへの法線(ワールド座標)
		Vec3d pa, pb;															///< pa:剛体の近傍点，pb:力覚ポインタの近傍点（ローカル座標）

		/// GJKを使った近傍点探索
		int found = 0;
		found = FindNearestPoint(a, b, a2w, b2w, cp, dir, normal, pa, pb);		
		/// 判定後の後処理(ローカルシミュレーションするかどうかのフラグを立てる)
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
			iaInfo->neighborInfo.closest_point = pa;				//< 剛体近傍点
			iaInfo->neighborInfo.pointer_point = pb;				//< 力覚ポインタ近傍点
			iaInfo->neighborInfo.common_point = (phSolid->GetPose() * pa + soPointer->GetPose() * pb)/2; 
			iaInfo->neighborInfo.last_face_normal = iaInfo->neighborInfo.face_normal;		//< 前回の法線(法線の補間に使う)，初めて近傍になった時は今回できた法線
			iaInfo->neighborInfo.face_normal = normal;				//< 剛体から力覚ポインタへの法線
		}

		/// 接触解析(susa実装中)
// #define BT_COLLISION
//#define CUT_RING
//#define ROUND_NEAREST_POINTS
#define CONTACT_ANALYSIS

#ifdef CONTACT_ANALYSIS		
		iaInfo->neighborInfo.intersection_vertices.clear();
		if(found == 1)	iaInfo->neighborInfo.intersection_vertices.push_back(pb);
		if(found == 2)	AnalyzeContactResion(phSolid, soPointer, pa, pb, &iaInfo->neighborInfo);
#endif

#ifdef BT_COLLISION
		CompareCurrentContactPoint(phSolid, soPointer, pa, pb, &iaInfo->neighborInfo);
#endif

#ifdef ROUND_NEAREST_POINTS
		FindPenetratingPoints(phSolid, soPointer, pa, pb, &iaInfo->neighborInfo);
#endif

#ifdef CUT_RING
		iaInfo->neighborInfo.intersection_vertices.clear();
		/// Muller Preparataの方法で切り口を求め，接触点を取得
		std::vector<Vec3d> section;
		Vec3d commonPoint;
		Vec3d a2b = b2w * pb - a2w * pa;
		if(found == 1){
			/// FindClosestPointで終わった場合
			/// 力覚ポインタと剛体が離れているので，近づけて接触解析
			b2w.Pos() -= a2b;
			commonPoint = a2w * pa;
			FindSectionVertex(phSolid, soPointer, a2w, b2w, pa, pb, normal, commonPoint, section);
			for(size_t k = 0; k  < section.size(); k++){
				iaInfo->neighborInfo.solid_section.push_back(a2w.Inv() * section[k]);
				iaInfo->neighborInfo.pointer_section.push_back(b2w.Inv() * section[k]);
			}
		}else if(found == 2){
#if 0
			///// ContFindCommonPointで終わった場合
			/////	既に接触している状態なので，そのまま接触解析
			//commonPoint = a2w * pa + 0.5 * a2b;
			//FindSectionVertex(phSolid, soPointer, a2w, b2w, pa, pb, normal, commonPoint, section);
			//for(int k = 0; k  < section.size(); k++){
			//	// commonpointが侵入量により変化してしまうため
			//	// 近傍点が載ってるところに面を動かす
			//	iaInfo->neighborInfo.solid_section.push_back(a2w.Inv() * (section[k] - 0.5 * a2b));
			//	iaInfo->neighborInfo.pointer_section.push_back(b2w.Inv() * (section[k] + 0.5 * a2b));
			//}
			commonPoint = a2w * pa;
			FindSectionVertex(phSolid, soPointer, a2w, b2w, pa, pb, normal, commonPoint, section);
			for(size_t k = 0; k  < section.size(); k++){
				// commonpointが侵入量により変化してしまうため
				// 近傍点が載ってるところに面を動かす
				iaInfo->neighborInfo.solid_section.push_back(a2w.Inv() * (section[k]));
				iaInfo->neighborInfo.pointer_section.push_back(b2w.Inv() * (section[k] + a2b));
			}
#else
			iaInfo->neighborInfo.last_section_depth = iaInfo->neighborInfo.section_depth;
			iaInfo->neighborInfo.section_depth = a2b.norm();
			commonPoint = a2w * pa + 0.5 * a2b;
			FindSectionVertex(phSolid, soPointer, a2w, b2w, pa, pb, normal, commonPoint, section);
			for(size_t k = 0; k  < section.size(); k++){
				iaInfo->neighborInfo.solid_section.push_back(a2w.Inv() * (section[k] - 0.5 * a2b));
				iaInfo->neighborInfo.pointer_section.push_back(b2w.Inv() * (section[k]));
			}
			if(section.size() == 3){
				//iaInfo->neighborInfo.solid_section.push_back(pa);
				//iaInfo->neighborInfo.pointer_section.push_back(pb);
				//DSTR << "ほげ" << std::endl;
			}
			DSTR <<iaInfo->neighborInfo.solid_section.size() << std::endl;
#endif
		}
		//DSTR << iaInfo->neighborInfo.intersection_vertices.size() << std::endl;
#endif

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

	if(a2b.norm() > 0.001){
		return 1;	// FindClosestPointsで見つけた
	}else{
		/// 力覚ポインタと剛体がすでに接触していたらCCDGJKで法線を求める
		pa = pb = Vec3d(0.0, 0.0, 0.0);
		/// dirが潰れてしまっている場合は剛体重心から近傍点へのベクトルとする
		if(dir == Vec3d(0.0, 0.0, 0.0) ){
			dir =-( wa - pc);
			DSTR << "おふ" << std::endl;
		}

		/// CCDGJKの実行	
extern bool bGJKDebug;
		bGJKDebug = false;

		double dist = 0.0;
		int cp = ContFindCommonPoint(ca, cb, a2w, b2w, dir, -DBL_MAX, 1, normal, pa, pb, dist);
		bGJKDebug = false;
		/// CCDGJKが失敗した場合の処理
		if(cp != 1){
			static int cont = 0;
			ContFindCommonPointSaveParam(ca, cb, a2w, b2w, dir, -DBL_MAX, 1, normal, pa, pb, dist);
			DSTR << cont << "ContFindCommonPoint do not find contact point" << std::endl;
			cont++;
		}
		return 2;	// CCDGJKで見つけた
	}
}

// 力覚ポインタと剛体の接触形状(ボリュームの頂点)を取得
extern bool bUseContactVolume;
void FWInteractAdaptee::AnalyzeContactResion(PHSolid* solida, PHSolid* solidb, Vec3d pa, Vec3d pb, NeighborInfo* nInfo){
	CDShapePair sp;
	sp.shape[0] = solida->GetShape(0)->Cast();
	sp.shape[1] = solidb->GetShape(0)->Cast();
	sp.commonPoint = nInfo->common_point; 
	sp.shapePoseW[0] = solida->GetPose();
	sp.shapePoseW[1] = solidb->GetPose();

	static CDContactAnalysis analyzer;
	
	bUseContactVolume = true;
	analyzer.FindIntersection(&sp);
	bUseContactVolume = false;

	// 形状の頂点を取ってくる
	for(CDQHPlane< CDContactAnalysisFace >* it = analyzer.planes.begin; it != analyzer.planes.end; ++it){
		if(it->deleted) continue;
		Vec3d point = it->normal/it->dist + sp.commonPoint;
	
/// ALLかSELECTIONのどちらかを選ぶ
#define SELECTION	// 中間表現の上にに載っている点は接触点としない
//#define ALL		// 接触形状の頂点を全て，接触点とする
#ifdef SELECTION
		double dot = (point - solida->GetPose() * pa) * nInfo->face_normal;
		if(dot < -1e-5){
			nInfo->intersection_vertices.push_back(solidb->GetPose().Inv() * point);
		}
#endif
#ifdef ALL
		nInfo->intersection_vertices.push_back(solidb->GetPose().Inv() * point);
#endif
	}	
}

void FWInteractAdaptee::CompareCurrentContactPoint(PHSolid* solida, PHSolid* solidb, Vec3d pa, Vec3d pb, NeighborInfo* nInfo){
	DSTR << "--------------------------"<< std::endl;
	// キャッシュの更新，侵入していないものは外す，さらに距離が離れているものも外す
	double breakingThreshold = 0.05;
	std::vector<Vec3d> temp_solid_section;
	std::vector<Vec3d> temp_pointer_section;
	for(size_t i = 0; i < nInfo->solid_section.size(); i++){
		Vec3d veca = solida->GetPose() * nInfo->solid_section[i];
		Vec3d vecb = solidb->GetPose() * nInfo->pointer_section[i];
		Vec3d vec = vecb - veca;
		
		// 許容距離内
		Vec3d normal = nInfo->face_normal;
		Vec3d dir = (vec * normal) / pow(normal.norm(), 2) * normal; 
		Vec3d projb2a = vecb + dir;
		double dist = (projb2a - veca).norm();
		if(dist < breakingThreshold * 2){
			DSTR << dist << std::endl;
			if(vec * nInfo->face_normal < 0 && vec.norm() < breakingThreshold){
				// 侵入している場合
				temp_solid_section.push_back(nInfo->solid_section[i]);
				temp_pointer_section.push_back(nInfo->pointer_section[i]);
			}
		}
	}
	nInfo->solid_section = temp_solid_section;
	nInfo->pointer_section = temp_pointer_section;
	// 新たに取得した近傍点対とキャッシュを比較
	int state = 0;
	for(size_t i = 0; i < nInfo->solid_section.size(); i++){
		double arange = (nInfo->solid_section[i] - pa).norm();		// 剛体の近傍点間距離
		double brange = (nInfo->pointer_section[i] - pb).norm();	// ポインタの近傍点間距離
		if(arange < 1e-1 && brange < 1e-1){
			state  = 1;
				//DSTR << "hoge" << std::endl;
			break;
		}
	}
	// 新しい点をキャッシュに加える
	if(state == 0){
		nInfo->solid_section.push_back(pa);
		nInfo->pointer_section.push_back(pb);
	}
	DSTR << nInfo->solid_section.size() << std::endl;
	// キャッシュに5点以上保存されている場合は侵入量が一番浅いのを消す
	if(nInfo->solid_section.size() > 4){
		Vec3d wa = solida->GetPose() * pa;
		Vec3d wb = solidb->GetPose() * pb;
		Vec3d normal = nInfo->face_normal;
		int index = -1;
		double dis = 10e5;
		// 最新の法線にキャッシュの近傍点間ベクトルを射影して，ノルムが小さいものを探す
		for(size_t i = 0; i < nInfo->solid_section.size(); i++){
			Vec3d wa_old = solida->GetPose() * nInfo->solid_section[i];
			Vec3d wb_old = solidb->GetPose() * nInfo->pointer_section[i];
			Vec3d vec = wb_old - wa_old;  
			Vec3d vec_ortho = (vec * normal) / pow(normal.norm(), 2) * normal;
			double vec_norm = vec_ortho.norm();
			if(dis > vec_norm){
				dis = vec_norm;
				index = (int)i;
			}
		}
		// ノルムが小さいものを外し，キャッシュを再構築
		temp_solid_section.clear();
		temp_pointer_section.clear();
		for(size_t i = 0; i < nInfo->solid_section.size(); i++){
			if(i == index) continue;
			temp_solid_section.push_back(nInfo->solid_section[i]);
			temp_pointer_section.push_back(nInfo->pointer_section[i]);
		}
		nInfo->solid_section = temp_solid_section;
		nInfo->pointer_section = temp_pointer_section;
	}
		DSTR << nInfo->solid_section.size() << std::endl;
}

void FWInteractAdaptee::FindPenetratingPoints(PHSolid* solida, PHSolid* solidb, Vec3d pa, Vec3d pb, NeighborInfo* nInfo){
	using namespace GJK;
	// サポートポイントidを比較して，同値なら削る
	int qidtemp[4];
	for(int i = 0; i < nSupport; i++)	qidtemp[i] = q_id[ids[i]];
	std::sort(qidtemp, qidtemp + nSupport);
	std::vector<int> qids;
	int index = -1;
	for(int i = 0; i < nSupport; i++){
		if(index != qidtemp[i]){
			qids.push_back(qidtemp[i]);
			index = qidtemp[i];
		}
	}
	// サポートポイントの隣の頂点を探す
	CDConvex* convexa = solida->GetShape(0)->Cast();
	CDConvex* convexb = solidb->GetShape(0)->Cast();
	std::vector<int> temppoints;
	for(size_t i = 0; i < qids.size(); i++){
		std::vector<int>& pointsb = convexb->FindNeighbors(qids[i]);
		for(size_t j = 0; j < pointsb.size(); j++){
			temppoints.push_back(pointsb[j]);
			std::vector<int>& pointsb2 = convexb->FindNeighbors(pointsb[j]);
			for(size_t k = 0; k < pointsb2.size(); k++){
				temppoints.push_back(pointsb2[k]);
			}
		}
	}

	// 隣の頂点でかぶっているものを削る
	std::sort(temppoints.begin(), temppoints.end());
	index = -1;
	std::vector<int> neighbors;
	for(int i = 0; i < (int)temppoints.size(); i++){
		if(index != temppoints[i]){
			neighbors.push_back(temppoints[i]);
			index = temppoints[i];
		}
	}

	// 頂点が有効かどうか判定，有効ならば保つ
	Vec3f* basea = convexa->GetBase();
	Vec3f* baseb = convexb->GetBase();
	// はじめに近傍点を追加しておく
	//nInfo->solid_section.push_back(nInfo->closest_point);
	//nInfo->pointer_section.push_back(nInfo->pointer_point);
	for(size_t i = 0; i < neighbors.size(); i++){
		Vec3f lvb = baseb[neighbors[i]].data;
		Vec3d wvb = solidb->GetPose() *	lvb;

		double ip = nInfo->face_normal * (wvb - solida->GetPose() * nInfo->closest_point);
		if(ip > 0.05) continue;
		Vec3d ortho = ip * nInfo->face_normal;
		Vec3d wva = wvb - ortho;
		nInfo->solid_section.push_back(solida->GetPose().Inv() * wva);
		nInfo->pointer_section.push_back(lvb);
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
	int nLine0 = (int)cutRing.lines.size();
	CDConvex* convex1 = solid1->GetShape(0)->Cast();
	if (found) found = convex1->FindCutRing(cutRing, shapePoseW1);
	int nLine1 = (int)cutRing.lines.size() - nLine0;
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
	//CSVOUT << section.size() << std::endl;
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

Vec3d FWInteractAdaptee::GetPicCenterPosition(){
	Vec3d centerPosition = Vec3d(0,0,0);
	return centerPosition;
}

}
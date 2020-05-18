/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Collision/CDDetectorImp.h>
#include <Collision/CDConvexMesh.h>
#include <Collision/CDBox.h>
#include <Collision/CDSphere.h>
#include <Foundation/UTQPTimer.h>

namespace Spr {;
const double epsilon = 1e-16;
const double epsilon2 = epsilon*epsilon;

UTQPTimer qpTimerForCollision;
UTLongLong& coltimePhase1 = UTPerformanceMeasureIf::GetInstance("Collision")->Count("P1");
UTLongLong& coltimePhase2 = UTPerformanceMeasureIf::GetInstance("Collision")->Count("P2");
UTLongLong& coltimePhase3 = UTPerformanceMeasureIf::GetInstance("Collision")->Count("P3");
bool bUseContactVolume = true;

int s_methodSW = 0;			//	0=通常,1=加速,2=Gino
int s_accelThreshold = 24;	//	加速適用の頂点数閾値
//衝突判定メソッドのインターフェース
int FindCommonPointInterface(const CDConvex* a, const CDConvex* b,
	const Posed& a2w, const Posed& b2w, const Vec3d& dir, double start, double end,
	Vec3d& normal, Vec3d& pa, Vec3d& pb, double& dist)
{
	int res;
	Vec3d v;
	switch (s_methodSW)
	{
	case 1:
		if (a->GetVtxCount() < s_accelThreshold || b->GetVtxCount() < s_accelThreshold) {
			res = ContFindCommonPoint(a, b, a2w, b2w, dir, start, end, normal, pa, pb, dist);
		}
		else {
			res = ContFindCommonPointAccel(a, b, a2w, b2w, dir, start, end, normal, pa, pb, dist);
		}
		break;
	case 2:
		res = ContFindCommonPointGino(a, b, a2w, b2w, dir, start, end, normal, pa, pb, dist);
		break;
	case 3:	
		assert(0);
		qpTimerForCollision.CountNS();
		res = FindCommonPoint(a, b, a2w, b2w, v, pa, pb);
		if (res >= 1) {
			v.clear();
			CalcEPA(v, a, b, a2w, b2w, pa, pb);
			dist = v.norm();
			normal = v.unit();
		}
		break;
	default:
		res = ContFindCommonPoint(a, b, a2w, b2w, dir, start, end, normal, pa, pb, dist);
		break;
	}
	return res;
}
bool CDShapePair::Detect(unsigned ct, const Posed& pose0, const Posed& pose1){
	shapePoseW[0] = pose0;
	shapePoseW[1] = pose1;
	Vec3d sep;
	bool rv = FindCommonPoint(shape[0], shape[1], shapePoseW[0], shapePoseW[1], sep, closestPoint[0], closestPoint[1]);
	if (rv){
		commonPoint = shapePoseW[0] * closestPoint[0];
		if (lastContactCount == unsigned(ct-1))
			 state = CONTINUE;
		else state = NEW;
		lastContactCount = ct;
	}else{
		state = NONE;
	}
	return rv;
}

void SaveShape(std::ostream& file, CDShape* a);
CDConvex* LoadShape(std::istream& file, PHSdkIf* sdk);

/**	DetectCotinuously2()の 基本方針 @ 2011.10.31
・前のステップ（速度の向きに少し余分に戻しても良い）から、
	回転だけさせた所で
	1. 接触していない場合
		速度の向きに接触するまですすめる

	2.接触している場合
		- 以前の法線があればそれを使う
		- 以前の法線も無ければ、６方向に動かして、移動量が少ないものを仮法線として採用。
*/

//	BoxStackで、HIT_COUNTを試した結果：
//	hits: 75962 21608 1731 255 284 76 84 / 100000
//	#define HIT_COUNT

bool CDShapePair::ContDetect(unsigned ct, const Posed& pose0, const Posed& pose1, const Vec3d& delta0, const Vec3d& delta1, double dt){
#ifdef HIT_COUNT
	static int hits[7];
#endif
	//	for debug dump
	Vec3d lastNormal = normal;
	int lastLCC = lastContactCount;
	shapePoseW[0] = pose0;
	shapePoseW[1] = pose1;	
	if (lastContactCount == unsigned(ct-1) ){	//	継続した接触の場合
		//	法線向きに判定するとどれだけ戻ると離れるか調べる．
		//　end = -epsilonとすることで、侵入していない状態(dist == 0)も接触に含むようにした(2012/5/22susa)
		double dist;
		int res=FindCommonPointInterface(shape[0], shape[1], shapePoseW[0], shapePoseW[1], 
			-normal, -DBL_MAX, -epsilon, normal, closestPoint[0], closestPoint[1], dist);
		if (res <= 0) {	//	範囲内では、接触していない場合
			return false;
		}
		depth = -dist;
		center = commonPoint = shapePoseW[0] * closestPoint[0] - 0.5*normal*depth;
		goto found;
	}else{										//	初めての接触の場合
		//	並進の移動分を戻す
		Vec3d delta = delta1-delta0;
		double end = delta.norm();
		Vec3d dir;
		if (end > epsilon){	//	速度がある場合
			dir= delta / end;
			shapePoseW[0].Pos() -= delta0;
			shapePoseW[1].Pos() -= delta1;
			double dist;
			int res=FindCommonPointInterface(shape[0], shape[1], shapePoseW[0], shapePoseW[1], 
				dir, 0, end, normal, closestPoint[0], closestPoint[1], dist);
			if (res == 0 || res==-1) return false;	//	前の位置から速度の向きに動かしても接触しない場合
			if (res == 1){	//	今回の移動で接触していれば
				double toi = dist / end;
				shapePoseW[0].Pos() += toi*delta0;
				shapePoseW[1].Pos() += toi*delta1;
				center = commonPoint = shapePoseW[0] * closestPoint[0];
				depth = -(1-toi) * delta * normal;
				if (depth <= 0){
					//	deltaの向きに進んで行って、接触した法線が normalだから、
					//	normal * delta < 0になるはずだが、かする場合、計算誤差で>=0になることがある。
//					DSTR << "depth:" << depth << " delta * normal >= 0" << std::endl;
					return false;
				}
#ifdef HIT_COUNT
				hits[0] ++;
#endif
				goto found;
			}
		}
		//	速度がないか、回転しただけで接触したか、最初から接触していたかの場合、ここに来る。
		//	まず、並進位置を現在位置に戻す
		shapePoseW[0].Pos() = pose0.Pos();
		shapePoseW[1].Pos() = pose1.Pos();	

		//	なんとか少ない移動量で侵入を解消できるような法線を見つける。
		//	以前の法線の向きで判定してみて、侵入が移動量(end)より小さければその向きを採用
		static Vec3d tmpN[6];
		double dist;
		if (lastNormal.square() > epsilon) tmpN[0] = lastNormal;
		else tmpN[0] = Vec3d(0,1,0);
		int res=FindCommonPointInterface(shape[0], shape[1], shapePoseW[0], shapePoseW[1], 
			-tmpN[0], -DBL_MAX, 0, normal, closestPoint[0], closestPoint[1], dist);
		if (res <= 0) return false;
#if 1
		int foundId = 0;
		double minD = dist;
		if (-dist > end){	//	侵入量が大きかったので、他の向きを試す。
			tmpN[1] = -tmpN[0];
			tmpN[2] = Vec3d(1,0,0) ^ tmpN[0];
			if (tmpN[2].square() < 0.2) tmpN[2] = Vec3d(0,0,1) ^ tmpN[0];
			tmpN[2].unitize();
			tmpN[3] = -tmpN[2];
			tmpN[4] = tmpN[0] ^ tmpN[2];
			tmpN[5] = -tmpN[4];
			for(int i=1; i<6; ++i){
				int res=FindCommonPointInterface(shape[0], shape[1], shapePoseW[0], shapePoseW[1], 
					-tmpN[i], -DBL_MAX, 0, normal, closestPoint[0], closestPoint[1], dist);
				if (res <=0) return false;
				if (-dist <= end){	//	この向きに決定
					foundId = i;
					break;
				}
				if (-dist < -minD){
					foundId = i;
					minD = dist;
				}
			}
		}
#ifdef HIT_COUNT
		hits[foundId+1] ++;
#endif
		//	tmpN[foundId]を仮法線として、接触位置・侵入量・法線を求める
		res=FindCommonPointInterface(shape[0], shape[1], shapePoseW[0], shapePoseW[1], 
			-tmpN[foundId], -DBL_MAX, 0, normal, closestPoint[0], closestPoint[1], dist);
		if (res <= 0) return false;	//	法線の向きに離してから現在位置まで近づけても接触が起きない場合なので、接触なし。
#else
 #error
 // Test for shallowest penetration was needed for rotation collision.
#endif
		depth = -dist;
		center = commonPoint = shapePoseW[0] * closestPoint[0] - 0.5*normal*depth;
		if (depth > 5 || depth < 0){
			DSTR << "depth:" << depth << std::endl;
			//assert(0);
		}
		goto found;
	}
found:;
	if (lastContactCount == unsigned(ct-1)){
		state = CONTINUE;
	}else{
		state = NEW;
/*		static bool bShow = false;
		if (bShow){
			DSTR << "New contact: " << shape[0]->GetName() << "-" << 
				shape[1]->GetName() << std::endl;
		}
*/
#ifdef HIT_COUNT
		DSTR << "hits:";
		int sum=0;
		for(int i=0; i<sizeof(hits)/sizeof(hits[0]); ++i){
			DSTR << " " << hits[i];
			sum += hits[i];
		}
		DSTR << " / " << sum << std::endl;
#endif
	}
	//	debug dump
	if (depth > 5 || depth < 0){
		//DSTR << "depth=" << depth << std::endl;
		//assert(0);

		// trueを返すと不正な接触が生成され数ステップ後に発散することが多いので，
		// 対処療法だがfalseを返して接触が生成されないようにする
		// 本来はここに来ないようにするのがベスト　tazz
		return false;	
	}
	lastContactCount = ct;
	return true;
}


void CDShapePair::CalcNormal(){
#if 0
	if (state == NEW){
		//	凸形状の中心を離す向きを仮法線にする．
		normal = shapePoseW[1]*shape[1]->CalcCenterOfMass() - shapePoseW[0]*shape[0]->CalcCenterOfMass();
		double norm = normal.norm();
		if (norm > epsilon){
			normal /= norm;
		}else if (norm > 1e-200){
			normal /= norm;
			normal.unitize();
		}else{
			for(int i=0; i<3; ++i){
				normal[i] = normal[i] == 0 ? 0 : (normal[i]>0 ? 1 : -1);
			}
			norm = normal.norm();
			if (norm == 0) normal = Vec3d(0,1,0);
			else normal /= norm;
		}
	}
	//	前回の法線の向きに動かして，最近傍点を求める
	Vec3d n = normal;
#endif
#if 0
	// 前の方式　連続で判定している→動くがstaticでなくなる
	int res = FindCommonPointInterface(shape[0], shape[1], shapePoseW[0], shapePoseW[1], 
		-normal, -DBL_MAX, 0, normal, closestPoint[0], closestPoint[1], depth);
#elif 0
	//昔の方式　衝突を上手くとれない時があった
	if (state == NEW) {
		//	新たな接触の場合は，法線を積分して初期値を求める
		normal = iNormal;
		depth = 1e-2;
	}
	int count = 0;
	int res = 0;
	while (count < 100) {
		depth *= 2;						//	余裕を見て，深さの2倍動かす
		Posed trans = shapePoseW[1];			//	動かす行列
		trans.Pos() += depth * normal;
		Vec3d v;
		FindClosestPoints(shape[0], shape[1], shapePoseW[0], trans, v, closestPoint[0], closestPoint[1]);
		center = shapePoseW[0] * closestPoint[0];
		n = trans *closestPoint[1] - center;
		if (n.square() > 1e-10) {
			res = 1;
			break;
		}
		count++;
	}
	depth = depth - n.norm();			//	動かした距離 - 2点の距離
	normal = n.unit();
	center += 0.5f*depth*normal;
#else
	//EPAで衝突法線を求める
	qpTimerForCollision.CountNS();
	Vec3d n = normal;
	CalcEPA(n, shape[0], shape[1], shapePoseW[0], shapePoseW[1], closestPoint[0], closestPoint[1]);
	depth = -1 * n.norm();
	if (depth*depth > 0) {
		normal = n / -depth;
	}
	else {
		normal = (shapePoseW[1].Pos() - shapePoseW[0].Pos()).unit();
	}
	qpTimerForCollision.Accumulate(coltimePhase2);
#if 0
	if (res <= 0){
		DSTR << "Error in CalcNormal(): res:" << res << "dist:" << depth << n << std::endl;
		ContFindCommonPointSaveParam(shape[0], shape[1], shapePoseW[0], shapePoseW[1], 
			-n, -DBL_MAX, 0, n, closestPoint[0], closestPoint[1], depth);
	}
#endif
	depth *= -1;
	center = shapePoseW[0] * closestPoint[0];
	center -= 0.5f*depth*normal;				//	必ず、center *= -1なのか？なぜ？ 
#endif	
}



void CDContactAnalysisFace::Print(std::ostream& os) const {
	os << normal << "*" << dist;
}

Vec3f CDContactAnalysisFace::CommonVtx(int i){
	/**car2.xでdualPlanes[i]->distが0.000000000になる**/
	double dist = dualPlanes[i]->dist;
#ifdef _DEBUG
	if (dist < CD_EPSILON){
		DSTR << "Error: dist " << dist << std::endl;
		DSTR << "normal:" << dualPlanes[i]->normal << std::endl;
		Vec3d posAv;
		for(int j=0; j<3; ++j){
			DSTR << "Point" << j << ":" << dualPlanes[i]->vtx[j]->GetPos() << std::endl;
			posAv += dualPlanes[i]->vtx[j]->GetPos();
		}
		DSTR << "PosAve:" << posAv << std::endl;
		Vec3d pos0 = dualPlanes[i]->vtx[0]->GetPos();
		Vec3d pos1 = dualPlanes[i]->vtx[1]->GetPos();
		Vec3d pos2 = dualPlanes[i]->vtx[2]->GetPos();
		Vec3d a = pos1 - pos0;
		Vec3d b = pos2 - pos0;
		DSTR << "a:" << a << "b:" << b << std::endl;
		Vec3d normal = a ^ b;
		assert(normal.norm());
		normal.unitize();
		dist = pos0 * normal;
	}
#endif
	return Vec3f(dualPlanes[i]->normal / dist);
}

bool CDContactAnalysisFace::CalcDualVtx(Vec3f* base){
	normal = (base[face->vtxs[1]] - base[face->vtxs[0]]) ^ 
		(base[face->vtxs[2]] - base[face->vtxs[0]]);
	DEBUG_EVAL( if ( normal.norm() < CD_EPSILON || !finite(normal.norm()) ){
		DSTR << "normal is too small." << std::endl; } 
	)
	normal.unitize();
	dist = normal * base[face->vtxs[0]];
#if 1	//	debug	hase
	if (dist < -1e-3){
		DSTR << "Error: distance=" << dist << " < 0" << std::endl;
		return false; 
	}
#endif
	if (dist < CD_EPSILON) dist = CD_EPSILON;
	if (dist > CD_INFINITE) dist = CD_INFINITE;
	return true;
}

//----------------------------------------------------------------------------
//	CDContactAnalysis
//

//void CDContactAnalysis::CalcDistance(const CDFaces::iterator it, const Vec3f* base, const Vec3d common, Vec3f &normal, float &dist){
//	normal = (base[it->vtxs[1]] - base[it->vtxs[0]]) ^ 
//		(base[it->vtxs[2]] - base[it->vtxs[0]]);
//	DEBUG_EVAL( if ( normal.norm() < CD_EPSILON || !finite( normal.norm()) ){
//		DSTR << "normal is too small." << std::endl; } 
//	)
//	normal.unitize();
//	dist = normal * (base[it->vtxs[0]] - common);
//}
//
//#define COMMON_EPSILON 10e-5
//bool CDContactAnalysis::CorrectCommonPoint(CDShapePair* cp){
//	//DSTR << "------" << std::endl;
//	if (!bUseContactVolume) return 0;
//
//	if (DCAST(CDConvexMesh, cp->shape[0]) && DCAST(CDConvexMesh, cp->shape[1])){
//		CDConvexMesh* poly[2];
//		poly[0] = (CDConvexMesh*) cp->shape[0];
//		poly[1] = (CDConvexMesh*) cp->shape[1];
//		Vec3f* base[2];
//		for(int i=0; i<2; ++i){
//			Posed afw = cp->shapePoseW[i];
//			tvtxs[i].resize(poly[i]->base.size());
//			for(unsigned v=0; v<tvtxs[i].size(); ++v){
//				tvtxs[i][v] = afw * poly[i]->base[v];
//			}
//			base[i] = &*tvtxs[i].begin();
//		}
//
//		Vec3d common = cp->commonPoint;
//		Vec3d dirBuffer = Vec3d();
//		for(int i = 0; i<2; i++){	
//			for(CDFaces::iterator it = poly[i]->faces.begin();
//				it != poly[i]->faces.begin() + poly[i]->nPlanes; ++it){
//				Vec3f normal = Vec3f();
//				float dist = 0.0f;
//				CalcDistance(it, base[i], common, normal, dist);
//				//DSTR << "dist = " << dist << std::endl;
//				if(dist <= 0){
//					//DSTR << "start =" << common << std::endl;
//					
//					Vec3d dir = normal + dirBuffer;
//					dir.unitize();
//					dir = -1 * dir;
//
//					float dot = dir * normal; 
//					Vec3f p = -normal * (abs(dist) + COMMON_EPSILON);
//					float k = 0.0f;
//					if(abs(normal[0]) > 0){
//						k = p[0] / (dot * normal[0]);
//					}else if(abs(normal[1]) > 0){
//						k = p[1] / (dot * normal[1]);
//					}else{
//						k = p[2] / (dot * normal[2]);
//					}
//					common += k * dir;
//
//					if(i == 0){
//						for(CDFaces::iterator ite = poly[i]->faces.begin();
//							ite == it; ite++){
//							CalcDistance(ite, base[i], common, normal, dist);
//							//DSTR << "		" <<dist << std::endl;
//							if(dist <= 0) dirBuffer += normal;
//						}
//					}else{
//						int j = 0;
//						for(CDFaces::iterator ite = poly[j]->faces.begin();
//							ite != poly[j]->faces.begin() + poly[j]->nPlanes; ite++){
//							CalcDistance(ite, base[j], common, normal, dist);
//							//DSTR << "		" <<dist << std::endl;
//							if(dist <= 0) dirBuffer += normal;		
//						}
//						j = 1;
//						for(CDFaces::iterator ite = poly[j]->faces.begin();
//							ite != it + 1; ite++){
//							CalcDistance(ite, base[j], common, normal, dist);
//							//DSTR << "		" <<dist << std::endl;
//							if(dist <= 0) dirBuffer += normal;		
//						}
//					}
//
//				}
//			}
//		}
//		////DSTR << "---" << std::endl;
//		//for(int i=0; i<2; ++i){
//		//	for(CDFaces::iterator it = poly[i]->faces.begin();
//		//		it != poly[i]->faces.begin() + poly[i]->nPlanes; it++){
//		//		Vec3f normal;
//		//		float dist;
//		//		CalcDistance(it, base[i], common, normal, dist);
//		//		//DSTR << dist << std::endl;
//		//		if(dist < CD_EPSILON){
//		//			//DSTR << "		" << dist << std::endl;
//		//			return 0;
//		//		}
//		//	}
//		//}
//		correctionCommonPoint = common;
//		return 1;
//	}
//	//DSTR << "You must use Convex Mesh to Correct Common Point" << std::endl;
//	return 0;
//}

#define CONTACT_ANALYSIS_BUFFER	2000
CDContactAnalysis::VtxBuffer CDContactAnalysis::vtxBuffer(CONTACT_ANALYSIS_BUFFER);
CDContactAnalysis::Vtxs CDContactAnalysis::vtxs(CONTACT_ANALYSIS_BUFFER);
CDQHPlanes<CDContactAnalysisFace> CDContactAnalysis::planes(CONTACT_ANALYSIS_BUFFER);
CDContactAnalysisFace** CDContactAnalysis::FindIntersection(CDShapePair* cp){
	planes.Clear();
	vtxBuffer.clear();
	vtxs.clear();
	if (bUseContactVolume){
		std::vector<Vec3f> normals[2];
		std::vector<float> dists[2];
		isValid = true;
		for(int i=0; i<2; ++i){
			Posed afw = cp->shapePoseW[i];
			afw.Pos() -= cp->commonPoint;
			if (DCAST(CDConvexMesh, cp->shape[i])){
				CDConvexMesh* poly = (CDConvexMesh*) cp->shape[i];
				normals[i].resize(poly->nPlanes);
				dists[i].resize(poly->nPlanes);
				for(int j=0; j<(int)normals[i].size(); ++j){
					normals[i][j] = afw.Ori() * poly->faces[j].normal;
					dists[i][j] = normals[i][j] * (afw * poly->base[poly->faces[j].vtxs[0]]);
				}
			} else if (DCAST(CDBox, cp->shape[i])){
				CDBox* box = (CDBox*) cp->shape[i];
				normals[i].resize(6);
				dists[i].resize(6);
				for(int j=0; j<(int)normals[i].size(); ++j){
					normals[i][j] = afw.Ori() * box->qfaces[j].normal;
					dists[i][j] = normals[i][j] * (afw * box->base[box->qfaces[j].vtxs[0]]);
				}
			} else {
				isValid = false;
			}
		}
		if (isValid){
			for(int i=0; i<2; ++i){
				for(int j=0; j<(int)normals[i].size(); ++j){
					#if 1	//	debug	hase
						if (dists[i][j] < -1e-3){
							DSTR << "Error: distance=" << dists[i][j] << " < 0" << std::endl;
							DSTR << "Common Local: " << cp->shapePoseW[i].Inv() * cp->commonPoint 
								<< std::endl;
							assert(0);
						}
					#endif
					if (dists[i][j] < CD_EPSILON) dists[i][j] = CD_EPSILON;
					if (dists[i][j] > CD_INFINITE) dists[i][j] = CD_INFINITE;
					// VC2010でvtxBuffer.push_back(CDContactAnalysisFace());
					// とするとvectorでinvalid floting point overflowがでる
					// 2012/1/16 susa
					CDContactAnalysisFace caf;
					vtxBuffer.push_back(caf);
					vtxBuffer.back().id = i;
					vtxBuffer.back().normal = normals[i][j];
					vtxBuffer.back().dist = dists[i][j];
				}
			}
			for(VtxBuffer::iterator it=vtxBuffer.begin(); it != vtxBuffer.end(); ++it){
				vtxs.push_back(&*it);
			}
			planes.CreateConvexHull(&*vtxs.begin(), &*vtxs.begin() + vtxs.size());
			for(CDContactAnalysisFace** it = &*vtxs.begin(); it != &*vtxs.begin() + vtxs.size(); ++it){
				(*it)->dualPlanes.clear();
			}
			for(CDQHPlane<CDContactAnalysisFace>* it = planes.begin; it != planes.end; ++it){
				if(it->deleted) continue;
				for(int i=0; i!=3; ++i){
					it->vtx[i]->dualPlanes.push_back(it);
				}
			}
			for(CDContactAnalysisFace** itv = &*vtxs.begin(); itv != planes.vtxBegin; ++itv){
				for(CDContactAnalysisFace::DualPlanes::iterator itp = (*itv)->dualPlanes.begin();
					itp != (*itv)->dualPlanes.end(); ++itp){
					CDContactAnalysisFace::DualPlanes::iterator next = itp;
					++next;
					int i;
					for(i=0; i<3; ++i){
						if ((*itp)->vtx[i] == *itv) break;
					}
					for(CDContactAnalysisFace::DualPlanes::iterator it2 = next;
						it2 != (*itv)->dualPlanes.end(); ++it2){
						if ((*it2) == (*itp)->neighbor[i]){
							std::swap(*next, *it2);
							goto nextFace;
						}
					}
					nextFace:;
				}
			}
		}else{
			isValid = false;
		}
		#if 0
		DSTR << "ConvexHullに使用された頂点:" << std::endl;
		int i=1;
		for(CDFace** f = vtxs.begin(); f != planes.vtxBegin; ++f){
			DSTR << "p" << i++ << ":\t" << 1/(*f)->dist << (*f)->normal << std::endl;
			Sleep(1);
		}
		DSTR << "ConvexHullを構成する面:" << std::endl;
		i=1;
		for(CDQHPlane<CDFace>* p = planes.begin; p != planes.end; ++p){
			if (p->deleted) continue;
			DSTR << "v"  << i++ << ":\t" << p->normal/p->dist + cp->commonPoint << std::endl;
			Sleep(1);
		}
		#endif
	}else{	//	bUseContactVolume
		isValid = false;
	}
	return planes.vtxBegin;
}
void CDContactAnalysis::IntegrateNormal(CDShapePair* cp){
	cp->iNormal = Vec3d();
	if (isValid){	//	両方ポリゴンの場合
		double areaForCenter=0;
		for(Vtxs::iterator it = vtxs.begin(); it != vtxs.end(); ++it){
			CDContactAnalysisFace& qhVtx = **it;
			if (qhVtx.NCommonVtx() < 3) continue;
			Vec3f p0 = qhVtx.CommonVtx(0);
			Vec3f p1;
			Vec3f p2 = qhVtx.CommonVtx(1);
			for(unsigned i=2; i<qhVtx.NCommonVtx(); ++i){
				p1 = p2;
				p2 = qhVtx.CommonVtx(i);
				Vec3d n = (p2-p0) ^ (p1-p0);
				if (qhVtx.id==0){
					cp->iNormal += n;
				}else{
					cp->iNormal -= n;
				}
				areaForCenter += n.norm();
			}
		}
	}else{
		//	球かどうか調べる．
		CDSphere* sp[2];
		Vec3f center[2];
		for(int i=0; i<2; ++i){
			sp[i] = DCAST(CDSphere, cp->shape[i]);
			if (sp[i]) center[i] = cp->shapePoseW[i].Pos();
		}
		if (sp[0] && sp[1]){	//	両方球の場合
			cp->iNormal = (center[1] - center[0]).unit();
		}else{
			if (sp[0]){
				cp->iNormal = (cp->commonPoint - center[0]).unit();
			}else if (sp[1]){
				cp->iNormal = (center[1] - cp->commonPoint).unit();
			}else{
//				assert(0);	//	知らない凸形状．
				//cp->iNormal = (af[1].Pos() - af[0].Pos()).unit();
				cp->iNormal = (cp->shapePoseW[1].Pos() - cp->shapePoseW[0].Pos()).unit();
			}
		}
	}
	if (cp->iNormal.square() < 1e-20){
		DEBUG_EVAL( DSTR << "iNormal error."; )
//		if (lastINormal.square() > 1e-20){
//			cp->iNormal = lastINormal;
//		}else{
			cp->iNormal = Vec3f(0,1,0);
			DEBUG_EVAL( DSTR << "Set (0,1,0)"; )
//		}
		DEBUG_EVAL( DSTR << std::endl; )
	}
	cp->iNormal.unitize();
#ifdef _DEBUG
	if (!finite(cp->iNormal.norm())){
		DSTR << "Error: iNormal is " << cp->iNormal << std::endl;
	}
#endif
}


void CDContactAnalysis::CalcNormal(CDShapePair* cp){
	if (cp->state == cp->NEW) {
		//	新たな接触の場合は，法線を積分して初期値を求める
		IntegrateNormal(cp);
		cp->normal = cp->iNormal;
		cp->depth = 1e-2;
	}
	//	前回の法線の向きに動かして，最近傍点を求める
	Vec3d n = cp->normal;
	int res = FindCommonPointInterface(cp->shape[0], cp->shape[1], cp->shapePoseW[0], cp->shapePoseW[1], 
		-cp->normal, -DBL_MAX, 0, cp->normal, cp->closestPoint[0], cp->closestPoint[1], cp->depth);
	// 離れた向きで検出された時のエラー表示
	// 頻繁に表示されるのでコメントアウトしました susa
	//if (res <= 0){
	//	DSTR << "Error in CDContactAnalysis::CalcNormal(): res:" << res << "dist:" << cp->depth << -n << std::endl;
	//	DSTR << "cp:" << cp->shapePoseW[0]*cp->closestPoint[0] << cp->shapePoseW[1]*cp->closestPoint[1] << std::endl; 
	//	ContFindCommonPointSaveParam(cp->shape[0], cp->shape[1], cp->shapePoseW[0], cp->shapePoseW[1], 
	//		-n, -DBL_MAX, 0, n, cp->closestPoint[0], cp->closestPoint[1], cp->depth);
	//}
	cp->depth *= -1;
	cp->center = cp->shapePoseW[0] * cp->closestPoint[0];
	cp->center -= 0.5f*cp->depth*cp->normal;
}

	
/*
void CDContactAnalysis::Draw(CDShapePair& cp, Posed afw, SGScene* s){
	GRRender* render;
	s->GetRenderers().Find(render);
	if (!render || !render->CanDraw()) return;
	render->SetModelMatrix(afw);
	GRMaterialData mat(
		Vec4f(0, 0, 1, 1),
		Vec4f(0, 0, 1, 1),
		Vec4f(0, 0, 1, 1),
		Vec4f(0, 0, 1, 1),
		0.0f);
	render->SetDepthFunc(GRRender::DF_ALWAYS);
	render->SetMaterial(mat);
	for(Vtxs::iterator it = vtxs.begin(); it != vtxs.end(); ++it){
		CDFace& face = **it;
		if (face.dualPlanes.size() < 3) continue;
		Vec3f vbuf[3];
		vbuf[0] = face.dualPlanes[0]->normal / face.dualPlanes[0]->dist + cp->commonPoint;
		vbuf[2] = face.dualPlanes[1]->normal / face.dualPlanes[1]->dist + cp->commonPoint;

		for(unsigned int i=2; i<face.dualPlanes.size(); ++i){
			vbuf[1] = vbuf[2];
			vbuf[2] = face.dualPlanes[i]->normal / face.dualPlanes[i]->dist + cp->commonPoint;
			render->DrawDirect(GRRender::TRIANGLES, vbuf, 3);
		}
	}
	GRMaterialData mat2(
		Vec4f(0, 1, 1, 1),
		Vec4f(0, 1, 1, 1),
		Vec4f(0, 1, 1, 1),
		Vec4f(0, 1, 1, 1),
		0.0f);
	render->SetMaterial(mat2);
	Vec3f vtx(cp->commonPoint);
	render->SetLineWidth(3);
	render->DrawDirect(GRRender::POINTS, &vtx, 1);
	render->SetDepthFunc(GRRender::DF_LESS);
}
*/
//-----------------------------------------------------------------------------
//	接触判定関数，center, extent はローカル(フレームの)座標系で指定
//
#define ABS(x) ((x)>0 ? (x) : -(x))
bool FASTCALL BBoxIntersection(Posed poseA, Vec3f centerA, Vec3f extentA,
					  Posed poseB, Vec3f centerB, Vec3f extentB){ 
	Matrix3f postureA, postureB;
	poseA.Ori().ToMatrix(postureA);
	poseB.Ori().ToMatrix(postureB);


    // compute difference of box centers, D = C1-C0
    Vec3f kD = postureA*centerA - postureB*centerB;

    float aafC[3][3];     // matrix C = A^T B, c_{ij} = Dot(A_i,B_j)
    float aafAbsC[3][3];  // |c_{ij}|
    float afAD[3];        // Dot(A_i,D)
    float fR0, fR1, fR;   // interval radii and distance between centers
    float fR01;           // = R0 + R1
    
    // axis C0+t*A0
    aafC[0][0] = postureA.Ex()*(postureB.Ex());
    aafC[0][1] = postureA.Ex()*(postureB.Ey());
    aafC[0][2] = postureA.Ex()*(postureB.Ez());
    afAD[0] = postureA.Ex()*(kD);
    aafAbsC[0][0] = ABS(aafC[0][0]);
    aafAbsC[0][1] = ABS(aafC[0][1]);
    aafAbsC[0][2] = ABS(aafC[0][2]);
    fR = ABS(afAD[0]);
    fR1 = extentB[0]*aafAbsC[0][0]+extentB[1]*aafAbsC[0][1]+extentB[2]*aafAbsC[0][2];
    fR01 = extentA[0] + fR1;
    if ( fR > fR01 )
        return false;

    // axis C0+t*A1
    aafC[1][0] = postureA.Ey()*(postureB.Ex());
    aafC[1][1] = postureA.Ey()*(postureB.Ey());
    aafC[1][2] = postureA.Ey()*(postureB.Ez());
    afAD[1] = postureA.Ey()*(kD);
    aafAbsC[1][0] = ABS(aafC[1][0]);
    aafAbsC[1][1] = ABS(aafC[1][1]);
    aafAbsC[1][2] = ABS(aafC[1][2]);
    fR = ABS(afAD[1]);
    fR1 = extentB[0]*aafAbsC[1][0]+extentB[1]*aafAbsC[1][1]+extentB[2]*aafAbsC[1][2];
    fR01 = extentA[1] + fR1;
    if ( fR > fR01 )
        return false;

    // axis C0+t*A2
    aafC[2][0] = postureA.Ez()*(postureB.Ex());
    aafC[2][1] = postureA.Ez()*(postureB.Ey());
    aafC[2][2] = postureA.Ez()*(postureB.Ez());
    afAD[2] = postureA.Ez()*(kD);
    aafAbsC[2][0] = ABS(aafC[2][0]);
    aafAbsC[2][1] = ABS(aafC[2][1]);
    aafAbsC[2][2] = ABS(aafC[2][2]);
    fR = ABS(afAD[2]);
    fR1 = extentB[0]*aafAbsC[2][0]+extentB[1]*aafAbsC[2][1]+extentB[2]*aafAbsC[2][2];
    fR01 = extentA[2] + fR1;
    if ( fR > fR01 )
        return false;

    // axis C0+t*B0
    fR = ABS(postureB.Ex()*(kD));
    fR0 = extentA[0]*aafAbsC[0][0]+extentA[1]*aafAbsC[1][0]+extentA[2]*aafAbsC[2][0];
    fR01 = fR0 + extentB[0];
    if ( fR > fR01 )
        return false;

    // axis C0+t*B1
    fR = ABS(postureB.Ey()*(kD));
    fR0 = extentA[0]*aafAbsC[0][1]+extentA[1]*aafAbsC[1][1]+extentA[2]*aafAbsC[2][1];
    fR01 = fR0 + extentB[1];
    if ( fR > fR01 )
        return false;

    // axis C0+t*B2
    fR = ABS(postureB.Ez()*(kD));
    fR0 = extentA[0]*aafAbsC[0][2]+extentA[1]*aafAbsC[1][2]+extentA[2]*aafAbsC[2][2];
    fR01 = fR0 + extentB[2];
    if ( fR > fR01 )
        return false;

    // axis C0+t*A0xB0
    fR = ABS(afAD[2]*aafC[1][0]-afAD[1]*aafC[2][0]);
    fR0 = extentA[1]*aafAbsC[2][0] + extentA[2]*aafAbsC[1][0];
    fR1 = extentB[1]*aafAbsC[0][2] + extentB[2]*aafAbsC[0][1];
    fR01 = fR0 + fR1;
    if ( fR > fR01 )
        return false;

    // axis C0+t*A0xB1
    fR = ABS(afAD[2]*aafC[1][1]-afAD[1]*aafC[2][1]);
    fR0 = extentA[1]*aafAbsC[2][1] + extentA[2]*aafAbsC[1][1];
    fR1 = extentB[0]*aafAbsC[0][2] + extentB[2]*aafAbsC[0][0];
    fR01 = fR0 + fR1;
    if ( fR > fR01 )
        return false;

    // axis C0+t*A0xB2
    fR = ABS(afAD[2]*aafC[1][2]-afAD[1]*aafC[2][2]);
    fR0 = extentA[1]*aafAbsC[2][2] + extentA[2]*aafAbsC[1][2];
    fR1 = extentB[0]*aafAbsC[0][1] + extentB[1]*aafAbsC[0][0];
    fR01 = fR0 + fR1;
    if ( fR > fR01 )
        return false;

    // axis C0+t*A1xB0
    fR = ABS(afAD[0]*aafC[2][0]-afAD[2]*aafC[0][0]);
    fR0 = extentA[0]*aafAbsC[2][0] + extentA[2]*aafAbsC[0][0];
    fR1 = extentB[1]*aafAbsC[1][2] + extentB[2]*aafAbsC[1][1];
    fR01 = fR0 + fR1;
    if ( fR > fR01 )
        return false;

    // axis C0+t*A1xB1
    fR = ABS(afAD[0]*aafC[2][1]-afAD[2]*aafC[0][1]);
    fR0 = extentA[0]*aafAbsC[2][1] + extentA[2]*aafAbsC[0][1];
    fR1 = extentB[0]*aafAbsC[1][2] + extentB[2]*aafAbsC[1][0];
    fR01 = fR0 + fR1;
    if ( fR > fR01 )
        return false;

    // axis C0+t*A1xB2
    fR = ABS(afAD[0]*aafC[2][2]-afAD[2]*aafC[0][2]);
    fR0 = extentA[0]*aafAbsC[2][2] + extentA[2]*aafAbsC[0][2];
    fR1 = extentB[0]*aafAbsC[1][1] + extentB[1]*aafAbsC[1][0];
    fR01 = fR0 + fR1;
    if ( fR > fR01 )
        return false;

    // axis C0+t*A2xB0
    fR = ABS(afAD[1]*aafC[0][0]-afAD[0]*aafC[1][0]);
    fR0 = extentA[0]*aafAbsC[1][0] + extentA[1]*aafAbsC[0][0];
    fR1 = extentB[1]*aafAbsC[2][2] + extentB[2]*aafAbsC[2][1];
    fR01 = fR0 + fR1;
    if ( fR > fR01 )
        return false;

    // axis C0+t*A2xB1
    fR = ABS(afAD[1]*aafC[0][1]-afAD[0]*aafC[1][1]);
    fR0 = extentA[0]*aafAbsC[1][1] + extentA[1]*aafAbsC[0][1];
    fR1 = extentB[0]*aafAbsC[2][2] + extentB[2]*aafAbsC[2][0];
    fR01 = fR0 + fR1;
    if ( fR > fR01 )
        return false;

    // axis C0+t*A2xB2
    fR = ABS(afAD[1]*aafC[0][2]-afAD[0]*aafC[1][2]);
    fR0 = extentA[0]*aafAbsC[1][2] + extentA[1]*aafAbsC[0][2];
    fR1 = extentB[0]*aafAbsC[2][1] + extentB[1]*aafAbsC[2][0];
    fR01 = fR0 + fR1;
    if ( fR > fR01 )
        return false;

    return true;
}

}

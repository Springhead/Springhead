/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Collision.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "CDDetectorImp.h"
#include "CDQuickHull3DImp.h"
#include <float.h>

namespace Spr {;
const double epsilon = 1e-8;
const double epsilon2 = epsilon*epsilon;

bool bUseContactVolume=true;

bool CDShapePair::Detect(unsigned ct, CDConvex* s0, CDConvex* s1, const Posed& pose0, const Posed& pose1){
	shape[0] = s0;
	shape[1] = s1;
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
bool CDShapePair::DetectContinuously(unsigned ct, 
	const Posed& pose0, const Vec3d& delta0, const Vec3d& cog0, const Vec3d& dAng0,
	const Posed& pose1, const Vec3d& delta1, const Vec3d& cog1, const Vec3d& dAng1){
	shapePoseW[0] = pose0;
	shapePoseW[1] = pose1;
	
	if (lastContactCount == unsigned(ct-1) ){	
		//	２回目以降の接触の場合
		shapePoseW[0].Pos() += delta0;
		shapePoseW[1].Pos() += delta1;	//	最初から現在の位置に移動させる

		double dist;
		Vec3d dir = -normal * 1e-8;	//	法線向きに判定するとどれだけ戻ると離れるか分かる．
		int res=ContFindCommonPoint(shape[0], shape[1], shapePoseW[0], shapePoseW[1], dir, normal, closestPoint[0], closestPoint[1], dist);

		if (res <= 0) {	//	範囲内では、接触していない場合
			return false;
		}
		if (dist > 0){	//	距離 > 0 = 離れている
			return false;	//	法線方向に進めないと接触しない場合．
		}
		depth = dist * dir * normal;
		center = commonPoint = shapePoseW[0] * closestPoint[0] - 0.5*normal*depth;
	}else{
		//	初めての接触の場合
		Vec3d delta = delta1-delta0;
		double toi;
		if (delta.square() > epsilon2){	//	 速度がある場合
			double dist;
			Vec3d dir = delta;
			int res=ContFindCommonPoint(shape[0], shape[1], shapePoseW[0], shapePoseW[1], dir, normal, closestPoint[0], closestPoint[1], dist);
			//	res==-1:	range内では接触していないが将来接触する可能性がある．	
			//	res==-2:	range内では接触していないが過去していた可能性がある．
			if (res <= 0) return false;

			double rangeLen = delta * dir;		//	dirはContFindCommonPoint内で正規化されるのでノルムは1
			toi = dist / rangeLen;
			if (toi > 1) return false;	//	接触時刻がこのステップより未来．
			if (toi >= 0){	//	今回の移動で接触していれば
				shapePoseW[0].Pos() += toi*delta0;
				shapePoseW[1].Pos() += toi*delta1;
				center = commonPoint = shapePoseW[0] * closestPoint[0];
				shapePoseW[0].Pos() -= dir*1e-8;	//確実に交差部分を作るため 1e-8余分に動かす
				shapePoseW[1].Pos() += dir*1e-8;	//確実に交差部分を作るため 1e-8余分に動かす
				//	交差部の形状の計算は，衝突時点の位置で行うが，depth は現時点のdepth
				depth = -(1-toi) * delta * normal + 2e-8;
			}else{
				//	とりあえず現在位置で、接触しているかどうか、確認する。
				shapePoseW[0].Pos() += delta0;
				shapePoseW[1].Pos() += delta1;
				int res=ContFindCommonPoint(shape[0], shape[1], shapePoseW[0], shapePoseW[1], -dir, normal, closestPoint[0], closestPoint[1], dist);
				if (res <= 0) return false;	//	接触していないので抜ける。
				if (dist > 0) return false;	//	dist正なら、現在は接触していないので抜ける。
				//	あとはtoi<0の処理に任せる
			}
		}else{
			toi = -1;
			//	現在の位置に移動させる
			shapePoseW[0].Pos() += delta0;
			shapePoseW[1].Pos() += delta1;
		}
		if (toi < 0){
			/*	toi < 0 の場合、次の可能性がある。
				- 回転が原因で接触が起きたため、重心速度に基づくtoiでは、接触検出できない。
				- 速度が小さすぎてtoiが計算できない。
				- 最初から接触していた。
				- ユーザによる非物理移動が原因で接触が起きたため、toiで接触検出できない。

				このような場合は、形状の中心間を結ぶベクトルを仮法線とする。
				仮法線の向きで接触法線を求め、これを本法線とする。
				さらに、本法線の向きで、侵入量と法線、最近傍点を計算する。	*/

			//	仮法線（形状の中心を結ぶ向き）の計算
			normal = shapePoseW[1]*shape[1]->GetCenter() - shapePoseW[0]*shape[0]->GetCenter();
			double norm = normal.norm();
			if (norm > epsilon) normal /= norm;
			else normal = Vec3d(0,1,0);

			//	仮の法線の向きに動かして，法線を更新する。
			Vec3d dir = -normal;
			int res = ContFindCommonPoint(shape[0], shape[1], shapePoseW[0], shapePoseW[1], dir, normal, closestPoint[0], closestPoint[1], depth);
			if (res <= 0) return false;
			if (depth > 0) return false;

			//	法線を更新してもう一度やってみる。
			dir = -normal;
			res = ContFindCommonPoint(shape[0], shape[1], shapePoseW[0], shapePoseW[1], dir, normal, closestPoint[0], closestPoint[1], depth);
			if (res <= 0) return false;
			if (depth > 0) return false;

			depth *= -1;
			center = commonPoint = shapePoseW[0] * closestPoint[0];
			center -= 0.5f*depth*normal;
		}
	}
	if (lastContactCount == unsigned(ct-1)){
		state = CONTINUE;
	}else{
		state = NEW;
		static bool bShow = false;
		if (bShow){
			DSTR << "New contact: " << shape[0]->GetName() << "-" << 
				shape[1]->GetName() << std::endl;
		}
	}
	lastContactCount = ct;
	return true;
}
void CDShapePair::CalcNormal(){
	if (state == NEW){		
		//	物体の重心を離す向きに動かす．
		normal = shapePoseW[1]*shape[1]->GetCenter() - shapePoseW[0]*shape[0]->GetCenter();
		double norm = normal.norm();
		if (norm>epsilon) normal /= norm;
		else normal = Vec3d(0,1,0);
		depth = 1;
	}
	//	前回の法線の向きに動かして，最近傍点を求める
	Vec3d dir = -normal;
	int res = ContFindCommonPoint(shape[0], shape[1], shapePoseW[0], shapePoseW[1], dir, normal, closestPoint[0], closestPoint[1], depth);
	if (res <= 0){
		DSTR << "Error in CalcNormal(): res:" << res << "dist:" << depth << dir << std::endl;
		Vec3d v;
		FindCommonPoint(shape[0], shape[1], shapePoseW[0], shapePoseW[1], v, closestPoint[0], closestPoint[1]);
		DSTR << "v:" << v << std::endl;
		DSTR << "cp:" << shapePoseW[0]*closestPoint[0] << shapePoseW[1]*closestPoint[1] << std::endl; 
		res = ContFindCommonPoint(shape[0], shape[1], shapePoseW[0], shapePoseW[1], dir, normal, closestPoint[0], closestPoint[1], depth);
	}
	depth *= -1;
	center = shapePoseW[0] * closestPoint[0];
	center -= 0.5f*depth*normal;
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
#define CONTACT_ANALYSIS_BUFFER	2000
CDContactAnalysis::VtxBuffer CDContactAnalysis::vtxBuffer(CONTACT_ANALYSIS_BUFFER);
CDContactAnalysis::Vtxs CDContactAnalysis::vtxs(CONTACT_ANALYSIS_BUFFER);
CDQHPlanes<CDContactAnalysisFace> CDContactAnalysis::planes(CONTACT_ANALYSIS_BUFFER);
CDContactAnalysisFace** CDContactAnalysis::FindIntersection(CDShapePair* cp){
	planes.Clear();
	vtxBuffer.clear();
	vtxs.clear();
	if (bUseContactVolume){
		if (DCAST(CDConvexMesh, cp->shape[0]) && DCAST(CDConvexMesh, cp->shape[1])){
			isValid = true;
			CDConvexMesh* poly[2];
			poly[0] = (CDConvexMesh*) cp->shape[0];
			poly[1] = (CDConvexMesh*) cp->shape[1];
			for(int i=0; i<2; ++i){
				Posed afw = cp->shapePoseW[i];
				afw.Pos() -= cp->commonPoint;
				tvtxs[i].resize(poly[i]->base.size());
				for(unsigned v=0; v<tvtxs[i].size(); ++v){
					tvtxs[i][v] = afw * poly[i]->base[v];
				}
				for(CDFaces::iterator it = poly[i]->faces.begin();
					it != poly[i]->faces.begin() + poly[i]->nPlanes; ++it){
					vtxBuffer.push_back(CDContactAnalysisFace());
					vtxBuffer.back().face = &*it;
					vtxBuffer.back().id = i;
					if (!vtxBuffer.back().CalcDualVtx(&*tvtxs[i].begin())){
						DSTR << "Common Local: " << cp->shapePoseW[i].Inv() * cp->commonPoint 
							<< std::endl;
						for(unsigned int v=0; v<poly[i]->base.size(); ++v){
							DSTR << poly[i]->base[v] << std::endl;
						}
						assert(0);
					}
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
/**/
		assert(0);	//	知らない形状
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
	Vec3d dir = -cp->normal;
	int res = ContFindCommonPoint(cp->shape[0], cp->shape[1], cp->shapePoseW[0], cp->shapePoseW[1], dir, cp->normal, cp->closestPoint[0], cp->closestPoint[1], cp->depth);
	if (res <= 0){
		DSTR << "Error in CDContactAnalysis::CalcNormal(): res:" << res << "dist:" << cp->depth << dir << std::endl;
		Vec3d v;
		FindCommonPoint(cp->shape[0], cp->shape[1], cp->shapePoseW[0], cp->shapePoseW[1], v, cp->closestPoint[0], cp->closestPoint[1]);
		DSTR << "v:" << v << std::endl;
		DSTR << "cp:" << cp->shapePoseW[0]*cp->closestPoint[0] << cp->shapePoseW[1]*cp->closestPoint[1] << std::endl; 
		res = ContFindCommonPoint(cp->shape[0], cp->shape[1], cp->shapePoseW[0], cp->shapePoseW[1], dir, cp->normal, cp->closestPoint[0], cp->closestPoint[1], cp->depth);
	}
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

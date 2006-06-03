#include "Collision.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "CDDetectorImp.h"
#include "CDQuickHull3DImp.h"
#include <float.h>

namespace Spr {;
bool bUseContactVolume=true;


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


void CDShapePair::UpdateShapePose(Posed pose0, Posed pose1){
	shapePoseW[0] = pose0 * shape[0]->pose;
	shapePoseW[1] = pose1 * shape[1]->pose;
}

bool CDShapePair::Detect(unsigned ct){	
	Vec3d sep;
	CDConvex* conv[2] = {
		(CDConvex*)(shape[0]->shape),
		(CDConvex*)(shape[1]->shape),
	};

	bool r = FindCommonPoint(conv[0], conv[1], shapePoseW[0], shapePoseW[1],
		sep, closestPoint[0], closestPoint[1]);
	if (r){
		commonPoint = shapePoseW[0] * closestPoint[0];
		if (lastContactCount == unsigned(ct-1)){
			state = CONTINUE;
		}else{
			state = NEW;
		}
		lastContactCount = ct;
	}
	return r;
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
		if (DCAST(CDConvexMesh, cp->shape[0]->shape) && DCAST(CDConvexMesh, cp->shape[1]->shape)){
			isValid = true;
			CDConvexMesh* poly[2];
			poly[0] = (CDConvexMesh*) &*cp->shape[0];
			poly[1] = (CDConvexMesh*) &*cp->shape[1];
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
			planes.CreateConvexHull(&*vtxs.begin(), &*vtxs.end());
			for(CDContactAnalysisFace** it = &*vtxs.begin(); it != &*vtxs.end(); ++it){
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
/**/		CDSphere* sp[2];
		Vec3f center[2];
		for(int i=0; i<2; ++i){
			sp[i] = DCAST(CDSphere, cp->shape[i]->shape);
			//if (sp[i]) center[i] = cp->shapePoseW[i] * sp[i]->center;
			if (sp[i]) center[i] = cp->shapePoseW[i] * cp->shape[i]->pose.Pos();
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
	Vec3d n;			//	求める法線
//	Vec3d closest[2];	//	最近傍点(ローカル系)
	Posed trans;

	if (cp->depth < 1e-2) cp->depth = 1e-2;
	while(1) {
		cp->depth *= 2;						//	余裕を見て，深さの2倍動かす
		trans = cp->shapePoseW[1];			//	動かす行列
		trans.Pos() += cp->depth * cp->normal;
		FindClosestPoints((CDConvex*)cp->shape[0], (CDConvex*)cp->shape[1], cp->shapePoseW[0], trans, cp->closestPoint[0], cp->closestPoint[1]);
		cp->center = cp->shapePoseW[0] * cp->closestPoint[0];
		n = trans * cp->closestPoint[1] - cp->center;
		if (n.square() > 1e-10) break;
	}
	cp->depth = cp->depth - n.norm();			//	動かした距離 - 2点の距離
	cp->normal = n.unit();
	cp->center += 0.5f*cp->depth*cp->normal;
#ifdef _DEBUG
	if (cp->normal * cp->iNormal < 0 || !finite(cp->normal.norm())){
		DSTR << "Error: Wrong normal:" << cp->normal << cp->iNormal << std::endl;
		DSTR << trans;
		DSTR << cp->closestPoint[0] << cp->closestPoint[1] << std::endl;
		FindClosestPoints((CDConvex*)cp->shape[0], (CDConvex*)cp->shape[1], cp->shapePoseW[0], trans, cp->closestPoint[0], cp->closestPoint[1]);
	}
#endif
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
			render->DrawDirect(GRRender::TRIANGLES, vbuf, vbuf+3);
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
	render->DrawDirect(GRRender::POINTS, &vtx, &vtx+1);
	render->SetDepthFunc(GRRender::DF_LESS);
}
*/
//-----------------------------------------------------------------------------
//	接触判定関数，center, extent はローカル(フレームの)座標系で指定
//
#define ABS(x) ((x)>0 ? (x) : -(x))
bool BBoxIntersection(Posed poseA, Vec3f centerA, Vec3f extentA,
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

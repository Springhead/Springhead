/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include <float.h>
#include <Collision/CDDetectorImp.h>
#include <Collision/CDQuickHull2D.h>
#include <Collision/CDQuickHull2DImp.h>
#include <Collision/CDCutRing.h>

using namespace PTM;
using namespace std;
namespace Spr{;

class ContactVertex: public Vec3d{
public:
	static Vec3d ex, ey;
	ContactVertex(){}
	ContactVertex(const Vec3d& v):Vec3d(v){}
	Vec2d GetPos(){
		return Vec2d( (*(Vec3d*)this)*ex, (*(Vec3d*)this)*ey );
	}
};
Vec3d ContactVertex::ex;
Vec3d ContactVertex::ey;

void PHShapePairForLCP::CalcNormal(PHSolid* solid0, PHSolid* solid1){
	const double depthEpsilon = 0.01;
	if (state == NEW){
		normal = solid0->GetPointVelocity(commonPoint) - solid1->GetPointVelocity(commonPoint);
		double norm = normal.norm();
		if (norm<1e-10){
			normal = solid1->GetCenterPosition() - solid0->GetCenterPosition();
			double norm = normal.norm();
			if (norm<1e-10){
				normal = Vec3d(1,0,0);
			}
		}
		normal.unitize();
		depth = depthEpsilon;
	}
	//	前回の法線の向きに動かして，最近傍点を求める
	if (depth < depthEpsilon) depth = depthEpsilon;
	Posed trans;
	Vec3f n;
	while(1) {
		depth *= 2;							//	余裕を見て，深さの2倍動かす
		trans = shapePoseW[1];				//	動かす行列
		trans.Pos() += depth * normal;
		FindClosestPoints(shape[0], shape[1], shapePoseW[0], trans, closestPoint[0], closestPoint[1]);
		center = shapePoseW[0] * closestPoint[0];
		n = trans * closestPoint[1] - center;
		if (n.square() > 1e-5) break;
	}
	depth = depth - n.norm();		//	動かした距離 - 2点の距離
	normal = n.unit();
	center -= 0.5f*depth*normal;
#ifdef _DEBUG
	if (!finite(normal.norm())){
		FindClosestPoints(shape[0], shape[1], shapePoseW[0], trans, closestPoint[0], closestPoint[1]);
	}
#endif
}

// 接触解析．接触部分の切り口を求めて，切り口を構成する凸多角形の頂点をengineに拘束として追加する．	
void PHShapePairForLCP::EnumVertex(PHConstraintEngine* engine, unsigned ct, PHSolidInfoForLCP* solid0, PHSolidInfoForLCP* solid1){
	//	center と normalが作る面と交差する面を求めないといけない．
	//	面の頂点が別の側にある面だけが対象．
	//	quick hull は n log r だから，線形時間で出来ることはやっておくべき．

	//	各3角形について，頂点がどちら側にあるか判定し，両側にあるものを対象とする．
	//	交線を，法線＋数値の形で表現する．
	//	この処理は凸形状が持っていて良い．
	//	＃交線の表現形式として，2次曲線も許す．その場合，直線は返さない
	//	＃2次曲線はMullar＆Preparataには入れないで別にしておく．

	//2Dへの変換がいる．どうする？
	//	適当に速度？
	Vec3d v0 = solid0->solid->GetPointVelocity(center);
	Vec3d v1 = solid1->solid->GetPointVelocity(center);
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
	if (local.det() < 0.99) {
		DSTR << "Error: local coordinate error." << std::endl;
		assert(0);
	}

	// 凸多面同士の場合は、面と面が触れる場合があるので、接触が凸多角形になることがある。
	// 球と凸形状の接触は必ず１点になる。
	CDSphere* sp[2];
	sp[0] = DCAST(CDSphere, shape[0]);	// CDSphereへダイナミックキャスト
	sp[1] = DCAST(CDSphere, shape[1]);
	// 接触解析を行う２つの物体の片方or両方が球の場合
	if (sp[0] || sp[1]) {		
		// 接触点の配列(engine->points)に、球の最進入点と相手の最進入点の中点centerを追加する
		engine->points.push_back(DBG_NEW PHContactPoint(local, this, center, solid0, solid1));
	}
	// 接触解析を行う２つの物体がどちらとも球ではない場合
	else {						
		//	切り口を求める１：切り口を構成する線分の列挙
		CDCutRing cutRing(center, local);
		if (shape[0]->FindCutRing(cutRing, shapePoseW[0]) && shape[1]->FindCutRing(cutRing, shapePoseW[1])){
			//	切り口がある場合（数値誤差で切り口が見つからない場合もある）
			//	切り口を求める２：線分をつないで輪を作る
			cutRing.MakeRing();
			//	cutRing.Print(DSTR);
			//	DSTR << "contact center:" << center << " normal:" << normal << "  vtxs:" << std::endl;
			int nPoint = engine->points.size();
			for(CDQHLine<CDCutLine>* vtx = cutRing.vtxs.begin; vtx!=cutRing.vtxs.end; ++vtx){
				if (vtx->deleted) continue;
				assert(finite(vtx->dist));

				Vec3d pos;
				pos.sub_vector(1, Vec2d()) = vtx->normal / vtx->dist;
				pos = cutRing.local * pos;
				Matrix3d local;
				cutRing.local.Ori().ToMatrix(local);
				engine->points.push_back(DBG_NEW PHContactPoint(local, this, pos, solid0, solid1));
			//	DSTR << "  " << pos << std::endl;
			}
			if (nPoint == engine->points.size()){	//	ひとつも追加していない＝ConvexHullが作れなかった．
				//	きっと1点で接触している．
				engine->points.push_back(DBG_NEW PHContactPoint(local, this, center, solid0, solid1));
			}
		}
	}
}

void PHSolidPairForLCP::OnDetect(PHShapePairForLCP* sp, PHConstraintEngine* engine, unsigned ct, double dt){
	//	法線を求める
	sp->CalcNormal(solid[0]->solid, solid[1]->solid);

	//	交差する2つの凸形状を接触面で切った時の切り口の形を求める
	sp->EnumVertex(engine, ct, solid[0], solid[1]);
}			

//----------------------------------------------------------------------------
// PHConstraintEngine
OBJECT_IMP(PHConstraintEngine, PHEngine);

PHConstraintEngine::PHConstraintEngine(){
	numIteration	= 15;
	correctionRate	= 0.3;
	shrinkRate		= 0.7;
}

PHConstraintEngine::~PHConstraintEngine(){

}

void PHConstraintEngine::Clear(){
	base_type::Clear();
	points.clear();
	joints.clear();
}

PHJoint* PHConstraintEngine::CreateJoint(const PHJointDesc& desc){
	PHJoint* joint;
	switch(desc.type){
	case PHConstraintDesc::HINGEJOINT:
		joint = DBG_NEW PHHingeJoint();
		break;
	case PHConstraintDesc::SLIDERJOINT:
		joint = DBG_NEW PHSliderJoint();
		break;
	case PHConstraintDesc::BALLJOINT:
		joint = DBG_NEW PHBallJoint();
		break;
	case PHConstraintDesc::PATHJOINT:
		joint = DBG_NEW PHPathJoint();
		break;
	case PHConstraintDesc::SPRING:
		joint = DBG_NEW PHSpring();
		break;
	default: assert(false);
	}
	joint->SetScene(GetScene());
	joint->SetDesc(desc);
	return joint;
}

PHJoint* PHConstraintEngine::AddJoint(const PHJointDesc& desc){
	PHJoint* joint = CreateJoint(desc);
	joints.push_back(joint);
	return joint;
}

PHJoint* PHConstraintEngine::AddJoint(PHSolid* lhs, PHSolid* rhs, const PHJointDesc& desc){
	PHSolidInfos<PHSolidInfoForLCP>::iterator islhs, isrhs;
	islhs = solids.Find(lhs);
	isrhs = solids.Find(rhs);
	if(islhs == solids.end() || isrhs == solids.end())
		return NULL;
	
	PHJoint* joint = CreateJoint(desc);
	joint->solid[0] = *islhs;
	joint->solid[1] = *isrhs;
	joints.push_back(joint);

	//関節でつなげられた剛体間の接触は無効化
	//EnableContact(lhs, rhs, false);

	return joint;
}

void PHConstraintEngine::SetupDynamics(double dt){
	PHSolidInfos<PHSolidInfoForLCP>::iterator it;
	for(it = solids.begin(); it != solids.end(); it++)
		(*it)->SetupDynamics(dt);
	points.SetupDynamics(dt, correctionRate, shrinkRate);
	joints.SetupDynamics(dt, correctionRate, shrinkRate);
}
/*void PHConstraintEngine::SetupCorrection(double dt){
	PHSolidInfos<PHSolidInfoForLCP>::iterator it;
	for(it = solids.begin(); it != solids.end(); it++)
		(*it)->SetupCorrection();
	points.SetupCorrection(dt, max_error);
	joints.SetupCorrection(dt, max_error);
}*/
void PHConstraintEngine::IterateDynamics(){
	int count = 0;
	while(true){
		if(count == numIteration){
			//DSTR << "max count." << " iteration count: " << count << " dfsum: " << dfsum << endl;
			break;
		}
		points.IterateDynamics();
		joints.IterateDynamics();

		count++;
	}
}
/*void PHConstraintEngine::IterateCorrection(){
	int count = 0;
	while(true){
		if(count == max_iter_correction){
			//DSTR << "max count." << " iteration count: " << count << " dFsum: " << dFsum << endl;
			break;
		}
		points.IterateCorrection();
		joints.IterateCorrection();

		count++;
	}
}*/

void PHConstraintEngine::UpdateSolids(double dt){
	PHSolidInfos<PHSolidInfoForLCP>::iterator is;
	PHSolidInfoForLCP* info;
	PHSolid* solid;
	Vec3d vnew, wnew;
	for(is = solids.begin(); is != solids.end(); is++){
		info = *is;
		solid = info->solid;
		//velocity update
		vnew = info->v + info->dv0 + info->dv;
		wnew = info->w + info->dw0 + info->dw;
		solid->SetVelocity       (solid->GetOrientation() * vnew);
		solid->SetAngularVelocity(solid->GetOrientation() * wnew);
		//position update
		solid->SetCenterPosition(solid->GetCenterPosition() + solid->GetVelocity() * dt/* + solid->GetOrientation() * info->dV*/);
		solid->SetOrientation(
			(solid->GetOrientation() * Quaterniond::Rot(wnew * dt/* + info->dW*/)).unit()
		);
		//solid->SetOrientation((solid->GetOrientation() + solid->GetOrientation().Derivative(solid->GetOrientation() * is->dW)).unit());
		//solid->SetOrientation((solid->GetOrientation() * Quaterniond::Rot(/*solid->GetOrientation() * */info->dW)).unit());
		solid->SetUpdated(true);
	}
}

#include <windows.h>

void PHConstraintEngine::Step(){
	PHScene* scene = DCAST(PHScene, GetScene());
	unsigned int ct = scene->GetCount();
	double dt = scene->GetTimeStep();
	Dynamics(dt, ct);
	//Correction(dt, ct);
	UpdateSolids(dt);
}
void PHConstraintEngine::Dynamics(double dt, int ct){
	LARGE_INTEGER freq, val[2];
	QueryPerformanceFrequency(&freq);

	//交差を検知
	QueryPerformanceCounter(&val[0]);
	points.clear();
	if(bContactEnabled)
		Detect(ct, dt);
	QueryPerformanceCounter(&val[1]);
	//DSTR << "cd " << (double)(val[1].QuadPart - val[0].QuadPart)/(double)(freq.QuadPart) << endl;
	
	QueryPerformanceCounter(&val[0]);
	SetupDynamics(dt);
	QueryPerformanceCounter(&val[1]);
	//DSTR << "sd " << (double)(val[1].QuadPart - val[0].QuadPart)/(double)(freq.QuadPart) << endl;

	QueryPerformanceCounter(&val[0]);
	IterateDynamics();
	QueryPerformanceCounter(&val[1]);
	//DSTR << "id " << (double)(val[1].QuadPart - val[0].QuadPart)/(double)(freq.QuadPart) << endl;
}

/*void PHConstraintEngine::Correction(double dt, int ct){
	LARGE_INTEGER freq, val[2];
	QueryPerformanceFrequency(&freq);

	QueryPerformanceCounter(&val[0]);
	SetupCorrection(dt);
	QueryPerformanceCounter(&val[1]);
	//DSTR << "sc " << (double)(val[1].QuadPart - val[0].QuadPart)/(double)(freq.QuadPart) << endl;

	QueryPerformanceCounter(&val[0]);
	IterateCorrection();
	QueryPerformanceCounter(&val[1]);
	//DSTR << "ic " << (double)(val[1].QuadPart - val[0].QuadPart)/(double)(freq.QuadPart) << endl;
}*/


}


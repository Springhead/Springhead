/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Collision/CDConvex.h>
#include <Collision/CDBox.h>
#include <Collision/CDDetectorImp.h>

namespace Spr{;

//----------------------------------------------------------------------------
//	CDConvex

CDConvex::CDConvex(){
	bboxReady = false;
	volume = 0;
}

std::vector<int>& CDConvex::FindNeighbors(int vtx){
	static std::vector<int> rv;
	rv.clear();
	return rv;
}

void CDConvex::CalcBBox(Vec3f& bbmin, Vec3f& bbmax, const Posed& pose){
	Matrix3f rot;
	pose.Ori().ToMatrix(rot);
	for(int i=0; i<3; ++i){
		Vec3f v[2];
		Support(v[0], -rot.row(i));
		Support(v[1],  rot.row(i));
		bbmin[i] = std::min(bbmin[i], (float)((pose.Ori() * v[0])[i] + pose.Pos()[i]));
		bbmax[i] = std::max(bbmax[i], (float)((pose.Ori() * v[1])[i] + pose.Pos()[i]));
	}
}

// ++change
int CDConvex::SupportTag(const Vec3f& v, std::vector<Vec3d>& c_vec) const {
	return 0;
}

//----------------------------------------------------------------------------
#ifdef USE_METRICS_BY_BOXEL
void CDConvex::CalcMetricByBoxel(float& volume, Vec3f& center, Matrix3f& inertia){
	volume = 0.0f;
	center.clear();
	inertia = Matrix3f::Zero();

	// ボクセル近似でイナーシャと重心を計算
	Vec3f bbmin( FLT_MAX,  FLT_MAX,  FLT_MAX);
	Vec3f bbmax(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	CalcBBox(bbmin, bbmax, Posed());
	
	boxels.clear();
	float vol = (bbmax.x - bbmin.x) * (bbmax.y - bbmin.y) * (bbmax.z - bbmin.z);
	
	AccumulateBoxels(bbmin, bbmax, vol * 0.00001f);
	if(boxels.empty())
		return;
	
	for(int i = 0; i < (int)boxels.size(); i++){
		volume += boxels[i].volume;
		center += boxels[i].volume * boxels[i].center;
	}
	center /= volume;

	Matrix3f cross;
	for(int i = 0; i < (int)boxels.size(); i++){
		cross = Matrix3f::Cross(center - boxels[i].center);
		inertia += boxels[i].inertia - boxels[i].volume * (cross*cross);
	}

	boxels.clear();
}

void CDConvex::AccumulateBoxels(const Vec3f& bbmin, const Vec3f& bbmax, float eps){
	CDBox box;
	box.SetBoxSize(bbmax - bbmin);
	
	Vec3f c = 0.5 * (bbmin + bbmax);
	Posed boxPose;
	boxPose.Pos() = c;
	
	float vol = box.CalcVolume();

	// 体積がeps以下ならばそこでカットオフ
	if(vol <= eps)
		return;

	//Vec3d v, pa, pb;
	//bool intersect = FindCommonPoint(this, &box, Posed(), boxPose, v, pa, pb);
	
	Vec3f p[8];
	p[0] = Vec3f(bbmin.x, bbmin.y, bbmin.z);
	p[1] = Vec3f(bbmin.x, bbmin.y, bbmax.z);
	p[2] = Vec3f(bbmin.x, bbmax.y, bbmin.z);
	p[3] = Vec3f(bbmin.x, bbmax.y, bbmax.z);
	p[4] = Vec3f(bbmax.x, bbmin.y, bbmin.z);
	p[5] = Vec3f(bbmax.x, bbmin.y, bbmax.z);
	p[6] = Vec3f(bbmax.x, bbmax.y, bbmin.z);
	p[7] = Vec3f(bbmax.x, bbmax.y, bbmax.z);
	bool allInside  = true;
	bool allOutside = true;
	bool centerInside = IsInside(c);
	for(int i = 0; i < 8; i++){
		if(IsInside(p[i]))
			allOutside = false;
		else allInside = false;
	}

	if(allInside){
		boxels.push_back(Boxel(vol, c, box.CalcMomentOfInertia()));
	}
	else if(allOutside && !centerInside){

	}
	else{
		// 再分割
		Vec3f cmin, cmax;
		for(int i = 0; i < 2; i++)for(int j = 0; j < 2; j++)for(int k = 0; k < 2; k++){
			cmin.x = (i == 0 ? bbmin.x : c.x);
			cmax.x = (i == 0 ? c.x : bbmax.x);
			cmin.y = (j == 0 ? bbmin.y : c.y);
			cmax.y = (j == 0 ? c.y : bbmax.y);
			cmin.z = (k == 0 ? bbmin.z : c.z);
			cmax.z = (k == 0 ? c.z : bbmax.z);
			AccumulateBoxels(cmin, cmax, eps);
		}
	}
}
#endif

}

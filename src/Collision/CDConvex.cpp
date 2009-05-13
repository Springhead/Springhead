/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Collision.h"
#include "CDDetectorImp.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

//----------------------------------------------------------------------------
//	CDConvex
void CDConvex::CalcBBox(Vec3f& bbmin, Vec3f& bbmax, const Posed& pose){
	Matrix3f rot;
	pose.Ori().Inv().ToMatrix(rot);
	for(int i=0; i<3; ++i){
		bbmin[i] = std::min(bbmin[i], (float)((pose.Ori() * Support(-rot.col(i)))[i] + pose.Pos()[i]));
		bbmax[i] = std::max(bbmax[i], (float)((pose.Ori() * Support( rot.col(i)))[i] + pose.Pos()[i]));
	}
}

bool CDConvex::IsBoxInside(const Vec3f& bbmin, const Vec3f& bbmax){
	Vec3f p[8];
	p[0] = Vec3f(bbmin.x, bbmin.y, bbmin.z);
	p[1] = Vec3f(bbmin.x, bbmin.y, bbmax.z);
	p[2] = Vec3f(bbmin.x, bbmax.y, bbmin.z);
	p[3] = Vec3f(bbmin.x, bbmax.y, bbmax.z);
	p[4] = Vec3f(bbmax.x, bbmin.y, bbmin.z);
	p[5] = Vec3f(bbmax.x, bbmin.y, bbmax.z);
	p[6] = Vec3f(bbmax.x, bbmax.y, bbmin.z);
	p[7] = Vec3f(bbmax.x, bbmax.y, bbmax.z);
	for(int i = 0; i < 8; i++){
		if(!IsInside(p[i]))
			return false;
	}
	return true;
}

//int g_cutoff;
//int g_count;
//int g_included;

void CDConvex::Analyze(){
	// ボクセル近似でイナーシャと重心を計算
	Vec3f bbmin( FLT_MAX,  FLT_MAX,  FLT_MAX);
	Vec3f bbmax(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	CalcBBox(bbmin, bbmax, Posed());
	
//	g_count = 0;
//	g_cutoff = 0;
//	g_included = 0;
	boxels.clear();
	float vol = (bbmax.x - bbmin.x) * (bbmax.y - bbmin.y) * (bbmax.z - bbmin.z);
	AccumulateBoxels(bbmin, bbmax, vol * 0.001f);
	volume = 0.0f;
	center.clear();
	if(boxels.size()){
		for(int i = 0; i < (int)boxels.size(); i++){
			volume += boxels[i].volume;
			center += boxels[i].volume * boxels[i].pos;
		}
		center /= volume;

	}
	else{
		valid = true;
		return;
	}
	moment.clear();
	Matrix3f offset;
	for(int i = 0; i < (int)boxels.size(); i++){
		OffsetInertia(center - boxels[i].pos, offset);
		moment += boxels[i].moment + boxels[i].volume * offset;
	}

//	DSTR << "subcells: " << g_count << std::endl;
//	DSTR << "cut off: " << g_cutoff << std::endl;
//	DSTR << "included: " << g_included << std::endl;

	valid = true;
}

void CDConvex::OffsetInertia(const Vec3f& c, Matrix3f& I){
	Vec3f c2(c.x*c.x, c.y*c.y, c.z*c.z);
	I[0][0] = c2.y+c2.z; I[0][1] = -c.x*c.y;  I[0][2] = -c.x*c.z;
	I[1][0] = -c.y*c.x;  I[1][1] = c2.x+c2.z; I[1][2] = -c.y*c.z;
	I[2][0] = -c.z*c.x;  I[2][1] = -c.z*c.y;  I[2][2] = c2.x+c2.y;
}

void CDConvex::AccumulateBoxels(const Vec3f& bbmin, const Vec3f& bbmax, float eps){
	Vec3f sz = bbmax - bbmin;
	Vec3f c = 0.5f * (bbmax + bbmin);
	float vol = sz.x * sz.y * sz.z;

//	g_count++;

	// 体積がeps以下ならばそこでカットオフ
	if(vol <= eps){
//		g_cutoff++;
		return;
	}

	CDBox box;
	box.SetBoxSize(bbmax - bbmin);
	Posed boxPose;
	boxPose.Pos() = 0.5 * (bbmin + bbmax);
	Vec3d v, pa, pb;
	bool intersect = FindCommonPoint(this, &box, Posed(), boxPose, v, pa, pb);
	
	if(intersect){
		// bboxが完全に含まれるか調べる
		if(IsBoxInside(bbmin, bbmax)){
			Vec3f sz2(sz.x*sz.x, sz.y*sz.y, sz.z*sz.z);
			Matrix3f I = vol * 1.0f/12.0f * Matrix3f::Diag(sz2.y + sz2.z, sz2.x + sz2.z, sz2.x + sz2.y);
			//Matrix3f offset;
			//OffsetInertia(c, offset);
			boxels.push_back(Boxel());
			boxels.back().pos = c;
			boxels.back().volume = vol;
			boxels.back().moment = I;
			//volume += vol;
			//center += vol * c;
			//moment += I + vol * offset;
//			g_included++;
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
	// 完全に外
	else{

	}
}

}

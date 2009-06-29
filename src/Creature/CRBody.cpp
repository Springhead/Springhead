/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRBody.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

Vec3d CRBody::GetCenterOfMass(){
	/// 重心を求める時に使うi番目までの重心の小計
	double totalWeight = 0;
	/// 重心を求めるときに使うi番目までのブロックの中心座標
	Vec3d  centerPosOfBlocks = Vec3d(0.0, 0.0, 0.0);

	for(int i = 0; i<NSolids(); i++){
		if(solids[i] && solids[i]->GetPHSolid()){
			centerPosOfBlocks = centerPosOfBlocks + solids[i]->GetPHSolid()->GetCenterPosition() * solids[i]->GetPHSolid()->GetMass();
			totalWeight = totalWeight + solids[i]->GetPHSolid()->GetMass();
		}
	}

	return centerPosOfBlocks / totalWeight;
}

double CRBody::GetSumOfMass(){
	/// 重心を求める時に使うi番目までの重心の小計
	double totalWeight = 0;

	for(int i = 0; i<NSolids(); i++){
		if(solids[i])
			totalWeight = totalWeight + solids[i]->GetPHSolid()->GetMass(); 
	}

	return totalWeight;
}

Matrix3d CRBody::CalcBoxInertia(Vec3d boxsize, double mass){
	double i_xx = 1.0 / 12.0 * (boxsize[1] * boxsize[1] + boxsize[2] * boxsize[2]) * mass;
	double i_yy = 1.0 / 12.0 * (boxsize[2] * boxsize[2] + boxsize[0] * boxsize[0]) * mass;
	double i_zz = 1.0 / 12.0 * (boxsize[0] * boxsize[0] + boxsize[1] * boxsize[1]) * mass;

	return Matrix3d(i_xx, 0.0,  0.0, 
					0.0,  i_yy, 0.0, 
					0.0,  0.0,  i_zz);
}

ObjectIf* CRBody::GetChildObject(size_t i) {
	if (i < solids.size()) {
		return solids[i];
	} else if (i < solids.size() + joints.size()) {
		return joints[i - solids.size()];
	} else {
		return NULL;
	}
}

bool CRBody::AddChildObject(ObjectIf* o){
	CRSolidIf* s = DCAST(CRSolidIf, o);
	if (s){
		if (std::find(solids.begin(), solids.end(), s) == solids.end()){
			solids.push_back(s);
			DCAST(SceneObject, s)->SetScene(DCAST(CRBodyIf,this)->GetScene());
			return true;
		}
	}

	CRJointIf* j = DCAST(CRJointIf, o);
	if (j){
		if (std::find(joints.begin(), joints.end(), j) == joints.end()){
			joints.push_back(j);
			DCAST(SceneObject, j)->SetScene(DCAST(CRBodyIf,this)->GetScene());
			return true;
		}
	}

	return false;
}

bool CRBody::DelChildObject(ObjectIf* o){
	CRSolidIf* s = DCAST(CRSolidIf, o);
	if (s){
		CRSolids::iterator it = std::find(solids.begin(), solids.end(), s);
		if(it != solids.end()){
			solids.erase(it);
			return true;
		}
	}

	CRJointIf* j = DCAST(CRJointIf, o);
	if (j){
		CRJoints::iterator it = std::find(joints.begin(), joints.end(), j);
		if(it != joints.end()){
			joints.erase(it);
			return true;
		}
	}

	return false;
}

}
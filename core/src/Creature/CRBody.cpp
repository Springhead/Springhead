/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRBody.h>
#include <Physics/PHSolid.h>

namespace Spr{


//-------------------------------------------------------------------------------------------------

CRBoneIf* CRBody::FindByLabel(UTString label) {
	LabelMap::iterator it = labelMap.find(label);
	if (it != labelMap.end()) {
		return (*it).second;
	} else {
		for (size_t i=0; i<bones.size(); ++i) {
			if (label == bones[i]->GetLabel()) {
				labelMap[label] = bones[i];
				return bones[i];
			}
		}
	}
	return NULL;
}

Vec3d CRBody::GetCenterOfMass(){
	/// 重心を求める時に使うi番目までの重心の小計
	double totalWeight = 0;
	/// 重心を求めるときに使うi番目までのブロックの中心座標
	Vec3d  centerPosOfBlocks = Vec3d(0.0, 0.0, 0.0);

	for(int i = 0; i<NBones(); i++){
		if(bones[i] && bones[i]->GetPHSolid()){
			centerPosOfBlocks = centerPosOfBlocks + bones[i]->GetPHSolid()->GetCenterPosition() * bones[i]->GetPHSolid()->GetMass();
			totalWeight = totalWeight + bones[i]->GetPHSolid()->GetMass();
		}
	}

	return centerPosOfBlocks / totalWeight;
}

double CRBody::GetSumOfMass(){
	/// 重心を求める時に使うi番目までの重心の小計
	double totalWeight = 0;

	for(int i = 0; i<NBones(); i++){
		if(bones[i])
			totalWeight = totalWeight + bones[i]->GetPHSolid()->GetMass(); 
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

bool CRBody::AddChildObject(ObjectIf* o){
	CRBoneIf* b = DCAST(CRBoneIf, o);
	if (b){
		if (std::find(bones.begin(), bones.end(), b) == bones.end()){
			bones.push_back(b);
			DCAST(SceneObject, b)->SetScene(DCAST(CRBodyIf,this)->GetScene());
			if (std::string(b->GetLabel()) != "") {
				labelMap[std::string(b->GetLabel())] = b;
			}
			return true;
		}
	}

	return false;
}

bool CRBody::DelChildObject(ObjectIf* o){
	CRBoneIf* b = o->Cast();
	if (b) {
		for (LabelMap::iterator it = labelMap.begin(); it!=labelMap.end(); ++it) {
			if (it->second == b) {
				labelMap.erase(it);
				break;
			}
		}
		{
			CRBones::iterator it = std::find(bones.begin(), bones.end(), b);
			if (it != bones.end()) { bones.erase(it); }
		}
	}

	return false;
}

}
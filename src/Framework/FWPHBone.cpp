/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Foundation/UTLoadHandler.h>
#include <Foundation/UTLoadContext.h>
#include <Foundation/UTPath.h>
#include <FileIO/FISdk.h>
#include <FileIO/FIFileX.h>
#include <Graphics/GRFrame.h>
#include <Graphics/GRMesh.h>
#include <Graphics/GRSphere.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRScene.h>
#include <Physics/PHSolid.h>
#include <Physics/PHJoint.h>
#include <Framework/FWOldSpringheadNodeHandler.h>
#include <Framework/FWObject.h>
#include <Framework/FWScene.h>
#include <Framework/FWSdk.h>
#include <Collision/CDConvexMesh.h>
#include <Framework/FWPHBone.h>

#include <Framework/FWOldSpringheadNode.h>
namespace SprOldSpringhead{
using namespace Spr;

FWPHBone::FWPHBone(){
}

void FWPHBone::FWPHBoneCreate(){
	//desc
	desc.mass = 0.05;
	desc.inertia = 0.033 * Matrix3d::Unit();
	CDBoxDesc dBox;
	//soBone�̍쐬
	soBone.push_back(phScene->CreateSolid(desc));
	int Num=soBone.size()-1;
	{
		soBone[Num]->SetDynamical(false);
		soBone[Num]->SetGravity(false);
		soBone[Num]->SetFramePosition(Vec3d(2*Num, 0.0, 0.0));
	}
	//shapeBone�̍쐬	
	{
		dBox.boxsize = Vec3f(1.0,0.5,1.5);
		shapeBone.push_back(XCAST(fwSdk->GetPHSdk()->CreateShape(dBox)));
	}

	soBone[Num]->AddShape(shapeBone[Num]);
}
void FWPHBone::DisplayBonePoint(){
	CDSphereDesc dSphere;
	CDSphereIf* shapeSphere;
	std::vector<PHSolidIf*> soSphere;
	//soSphere�̍쐬
	for(int i=0; i<bonePoint.size(); ++i){
		soSphere.push_back(phScene->CreateSolid(desc));
		{
			soSphere[i]->SetDynamical(false);
			soSphere[i]->SetFramePosition(bonePoint[i]);
		}
		//shapeBone�̍쐬	
		{
			dSphere.radius=0.1;
			shapeSphere=XCAST(fwSdk->GetPHSdk()->CreateShape(dSphere));
		}
		soSphere[i]->AddShape(shapeSphere);
	}
}

void FWPHBone::SetAffine(std::vector<Affinef> a){
	af.swap(a);
	Vec3d BonePoint;
	if (af.size()){
		for(int i=0 ;i<af.size(); ++i){
			BonePoint=af[i]*Vec3d(0.0,0.0,0.0);
			bonePoint.push_back(BonePoint);
			//DSTR << i << std::endl << bonePoint[i] << std::endl;
		}
	}
}
void FWPHBone::Clear(){
	af.clear();
	soBone.clear();
	shapeBone.clear();
}

}

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FWPHBONE_H
#define FWPHBONE_H

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

#include <Framework/FWOldSpringheadNode.h>

namespace SprOldSpringhead{
using namespace Spr;

class FWPHBone{
private:
	std::vector<Affinef> af;
	std::vector<PHSolidIf*> soBone;
	std::vector<CDBoxIf*> shapeBone;
	std::vector<Vec3d> bonePoint;
	PHSolidDesc	desc;
	FWSdkIf* fwSdk;
	PHScene* phScene;

public:
	FWPHBone();
	void FWPHBoneCreate();
	void DisplayBonePoint();
	void SetPHScne(PHScene* s){phScene=s;}
	void SetfwSdk(FWSdkIf* s){fwSdk=s;}
	void SetAffine(std::vector<Affinef> a);
	void Clear();
};

}
#endif

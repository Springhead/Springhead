/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FWBONE_H
#define FWBONE_H

#include <Springhead.h>
#include <Framework/SprFWObject.h>
#include <Foundation/Object.h>
#include <Foundation/Scene.h>
#include <Framework/FWObject.h>
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

namespace Spr{;

	//struct BoneJointData{
	//	BoneJointData();
	//	double K;
	//	double D1;
	//	double D2;
	//	double yieldStress;
	//	double hardnessRate;
	//	Vec3f SocketPos;
	//	Vec3f PlugPos;
	//};

	class FWBone : public FWObject{
	private:
		Vec3d		centerPoint;
		double		length;
		CDBoxIf*	shapeBone;
		Affinef		transformAffine;
		Affinef		worldTransformAffine;
	public:
		FWBone();
		void Sync();
    };

}

#endif

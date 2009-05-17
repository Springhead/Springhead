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
class FWBone;
class BoneJoint;
class FWBoneCreate;

	class BoneJoint{
	public:
		BoneJoint();
		double K;
		double D1;
		double D2;
		double yieldStress;
		double hardnessRate;
		Vec3f SocketPos;
		Vec3f PlugPos;
	};

	class FWBone : public FWObject{
	private:

	public:
		Vec3d		centerPoint;
		double		length;
		CDRoundConeIf*	shapeBone;
		Affinef		worldTransformAffine;
		FWBone*		parentBone;
		PHJointIf*  joint;
		BoneJoint	jointData;
		FWBone();
		void Sync();
    };

	class FWBoneCreate{
	private:
		GRMesh*						mesh;
		FWSdkIf*					fwSdk;
		PHScene*					phScene;
		PHSceneIf*					phSceneIf;
		std::vector<FWObjectIf*>	fwObject;
		std::vector<FWBone*>		bone;
		FWBone*						bone_;
		std::vector<PHSolidIf*>		soBone;
		std::vector<CDRoundConeIf*>		shapeBone;
		std::vector<PHJointIf*>		joint;
		//FWBoneCreateを使うのに必要な値を設定するためのインタフェース
		void		SetMesh(GRMesh* m){mesh=m;};
		GRMesh*		GetMesh(){return mesh;};
		void		SetPHScene(PHScene* p){phScene=p;};
		PHScene*	GetPHScene(){return phScene;};
		//FWBoneに情報を代入するための関数
		void		SetFWBone();
		bool		BoneDetector(GRFrameIf* frame1,GRFrameIf* frame2);
		Vec3d		BonePosition(GRFrameIf* frame1,GRFrameIf* frame2);
		double		BoneLength(GRFrameIf* frame1,GRFrameIf* frame2);
		//CDBoxIf*	BoneShape(GRFrameIf* frame1,GRFrameIf* frame2);
		CDRoundConeIf* BoneShapeCone(GRFrameIf* frame1,GRFrameIf* frame2);
		FWBone*		ParentBone(GRFrameIf* frame1);
		//FWBoneに代入された情報を使ってシーンを作成する関数
		void		GenerateBone();
		void		SetBoneJoint();
		void		ContactCanceler();
		void		FWPHBoneCreate();
		void		FWJointCreate();
		void		FWSkinMeshAdapt();
	public:
		void		Boot(GRMesh* m,PHScene* s);

	};
		
}

#endif

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FWBONE_H
#define FWBONE_H

#include <Framework/SprFWBone.h>
#include <Framework/FWObject.h>

namespace Spr{;

class GRMesh;

class PHScene;
struct PHSceneIf;

class BoneJoint;
class FWBone;
class FWBoneCreate;
struct FWSdkIf;

class BoneJoint{
public:
	BoneJoint();
	double K;
	double D1;
	double D2;
	double yieldStress;
	double hardnessRate;
	Posed  SocketPos;
	Posed  PlugPos;
};

class FWBone : public FWObject, public FWBoneDesc{
public:
	SPR_OBJECTDEF(FWBone);
	ACCESS_DESC(FWBone);
	FWBoneDesc desc;
	FWBone(const FWBoneDesc& d=FWBoneDesc()); // コンストラクタ

	Vec3d			centerPoint;
	double			length;
	CDRoundConeIf*	shapeBone;
	Affinef			worldTransformAffine;
	Affinef			TransformAffine;
	FWBone*			parentBone;
	PHJointIf*		joint;
	BoneJoint		jointData;
		
	PHSolidIf*		GetPHSolid(){ return phSolid; }
	void			SetPHSolid(PHSolidIf* s){ phSolid = s; }
	GRFrameIf*		GetGRFrame(){ return grFrame; }
	void			SetGRFrame(GRFrameIf* f){ grFrame = f; }
	CDRoundConeIf*	GetShape(){return shapeBone;}
	void			SetShapeRadius(double r1,double r2){
						shapeBone->SetWidth(Vec2f(r1,r2));
					}
	PHJointIf*		GetJoint(){return joint;} 
	void			SetJointKDD2(double K,double D, double D2);
		
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
	Posed		BonePose(FWBone* Bone);
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

	void		FWAxis(Posed pose);
public:
	void		Boot(GRMesh* m,PHScene* s);

};
		
}

#endif

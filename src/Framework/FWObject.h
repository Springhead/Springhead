/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FWOBJECT_H
#define FWOBJECT_H

#include <Framework/SprFWObject.h>
#include <Foundation/Object.h>

namespace Spr{;

class FWObject : public SceneObject, public FWObjectDesc {
public:
	SPR_OBJECTDEF(FWObject);
	ACCESS_DESC(FWObject);
	FWObject(const FWObjectDesc& d=FWObjectDesc()); // コンストラクタ

	//
	UTRef< PHSolidIf > phSolid;
	UTRef< GRFrameIf > grFrame;
	double			   solidLength;

	//
	virtual SceneObjectIf* CloneObject(); 
	virtual bool		AddChildObject(ObjectIf* o);
	virtual ObjectIf* GetChildObject(size_t pos);
	virtual size_t NChildObject() const;

	PHSolidIf*	GetPHSolid(){ return phSolid; }
	void		SetPHSolid(PHSolidIf* s){ phSolid = s; }
	GRFrameIf*	GetGRFrame(){ return grFrame; }
	void		SetGRFrame(GRFrameIf* f){ grFrame = f; }
	double		GetSolidLength(){ return solidLength; }
	void		SetSolidLength(double l){ solidLength = l; }
	bool		LoadMesh(const char* filename, const IfInfo* ii = NULL, GRFrameIf* frame = NULL);
	void		GenerateCDMesh(GRFrameIf* frame = NULL, const PHMaterial& mat = PHMaterial());
	virtual void Sync(bool ph_to_gr);
};

class FWBoneObject : public FWObject, public FWBoneObjectDesc {
public:
	SPR_OBJECTDEF(FWBoneObject);
	ACCESS_DESC(FWBoneObject);
	FWBoneObjectDesc desc;
	FWBoneObject(const FWBoneObjectDesc& d=FWBoneObjectDesc()); // コンストラクタ
	//
	UTRef< PHJointIf > phJoint;
	UTRef< GRFrameIf > endFrame;
	Posed sockOffset;
	//
	virtual bool AddChildObject(ObjectIf* o);	
	virtual ObjectIf* GetChildObject(size_t pos);
	virtual size_t NChildObject() const;
	virtual void Sync();
	virtual void Modify();

	PHJointIf*	GetPHJoint(){ return phJoint; }
	void		SetPHJoint(PHJointIf* j){ phJoint = j; }
	GRFrameIf*	GetEndFrame(){ return endFrame; }
	void		SetEndFrame(GRFrameIf* f){ endFrame = f; }
	
	void		 SetAdaptType(int t){AdaptType = (FWBoneObjectAdaptType)t;}
};

class FWStructure: public SceneObject, public FWStructureDesc{
private:
	std::vector<FWBoneObjectIf*> fwBones;
public:
	SPR_OBJECTDEF(FWStructure);
	ACCESS_DESC(FWStructure);

	FWStructureDesc desc;
	FWStructure(const FWStructureDesc& d=FWStructureDesc()); // コンストラクタ

	FWBoneObjectIf* GetBone(int n);			//FWBoneObjectを取得する
	FWBoneObjectIf* GetBone(const char* name);	//FWBoneObjectを名前で取得する
	int				GetBoneSize();			//FWBoneObjectの数を取得する
	void			SetPose(Posed p);		//FWBoneObjectの位置を設定する(fwBones[0]を基準とする)
	void AddBone(FWBoneObjectIf* o);

	virtual bool		AddChildObject(ObjectIf* o);
};

}

#endif

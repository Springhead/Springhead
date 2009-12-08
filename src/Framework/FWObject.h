/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FWOBJECT_H
#define FWOBJECT_H

#include <Springhead.h>
#include <Framework/SprFWObject.h>
#include <Foundation/Object.h>
#include <Foundation/Scene.h>

namespace Spr{;

class FWObject : public SceneObject, public FWObjectDesc {
public:
	SPR_OBJECTDEF(FWObject);
	ACCESS_DESC(FWObject);
	FWObjectDesc desc;
	FWObject(const FWObjectDesc& d=FWObjectDesc()); // コンストラクタ
	//
	UTRef< PHSolidIf > phSolid;
	UTRef< GRFrameIf > grFrame;
	double			   solidLength;
	//
	virtual SceneObjectIf* CloneObject(); 
	virtual bool		AddChildObject(ObjectIf* o);
	virtual PHSolidIf*	GetPHSolid(){ return phSolid; }
	virtual void		SetPHSolid(PHSolidIf* s){ phSolid = s; }
	virtual GRFrameIf*	GetGRFrame(){ return grFrame; }
	virtual void		SetGRFrame(GRFrameIf* f){ grFrame = f; }
	virtual double		GetSolidLength(){ return solidLength; }
	virtual void		SetSolidLength(double l){ solidLength = l; }


	ObjectIf* GetChildObject(size_t pos){
		if (pos==0) if (phSolid) return phSolid; else return grFrame;
		if (pos==1) if (phSolid) return grFrame; else return NULL;
		return NULL;
	}
	size_t NChildObject() const {
		return phSolid ? (grFrame ? 2 : 1) : (grFrame ? 1 : 0);
	}

	virtual void Sync();
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
	virtual bool		AddChildObject(ObjectIf* o);
	virtual PHJointIf*	GetPHJoint(){ return phJoint; }
	virtual void		SetPHJoint(PHJointIf* j){ phJoint = j; }
	virtual GRFrameIf*	GetEndFrame(){ return endFrame; }
	virtual void		SetEndFrame(GRFrameIf* f){ endFrame = f; }
	//
	ObjectIf* GetChildObject(size_t pos){
		bool objs[] = {phSolid!=NULL, grFrame!=NULL, phJoint!=NULL, endFrame!=NULL};
		int cnt = -1;
		int i=0;
		for (; i<4; ++i) {
			if (objs[i]) { cnt++; }
			if (cnt==pos) { break; }
		}
		if (i == 0) { return phSolid;  }
		if (i == 1) { return grFrame;  }
		if (i == 2) { return phJoint;  }
		if (i == 3) { return endFrame; }
		return NULL;
	}
	size_t NChildObject() const {
		bool objs[] = {phSolid!=NULL, grFrame!=NULL, phJoint!=NULL, endFrame!=NULL};
		int cnt = 0;
		for (int i=0; i<4; ++i) {
			if (objs[i]) { cnt++; }
		}
		return cnt;
	}

	virtual void Sync();
	virtual void Modify();
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
	FWBoneObjectIf* GetBone(char* name);	//FWBoneObjectを名前で取得する
	int				GetBoneSize();			//FWBoneObjectの数を取得する
	void			SetPose(Posed p);		//FWBoneObjectの位置を設定する(fwBones[0]を基準とする)
	void AddBone(FWBoneObjectIf* o);

	virtual bool		AddChildObject(ObjectIf* o);
};

}

#endif

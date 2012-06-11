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
	FWObject(const FWObjectDesc& d=FWObjectDesc());

	UTRef< PHSolidIf > phSolid;
	UTRef< GRFrameIf > grFrame;
	UTRef< PHJointIf > phJoint;
	UTRef< GRFrameIf > childFrame;

	Posed sockOffset;

	// ----- ----- ----- ----- -----

	// 複製
	virtual SceneObjectIf* CloneObject(); 

	// 子要素
	virtual bool       AddChildObject(ObjectIf* o);
	virtual ObjectIf*  GetChildObject(size_t pos);
	virtual size_t     NChildObject() const;

	// Getter・Setter
	PHSolidIf*	GetPHSolid(){ return phSolid; }
	void		SetPHSolid(PHSolidIf* s){ phSolid = s; }

	GRFrameIf*	GetGRFrame(){ return grFrame; }
	void		SetGRFrame(GRFrameIf* f){ grFrame = f; }

	PHJointIf*	GetPHJoint(){ return phJoint; }
	void		SetPHJoint(PHJointIf* j){ phJoint = j; }

	GRFrameIf*	GetChildFrame(){ return childFrame; }
	void		SetChildFrame(GRFrameIf* f){ childFrame = f; }

	// 同期オプション
	void		SetSyncSource(FWObjectDesc::FWObjectSyncSource syncSrc) { syncSource = syncSrc; }
	FWObjectDesc::FWObjectSyncSource GetSyncSource() { return syncSource; }

	void		EnableAbsolute(bool bAbs) { bAbsolute = bAbs; }
	bool		IsAbsolute() { return bAbsolute; }

	// グラフィックス用メッシュに関するヘルパ
	bool		LoadMesh(const char* filename, const IfInfo* ii = NULL, GRFrameIf* frame = NULL);
	void		GenerateCDMesh(GRFrameIf* frame = NULL, const PHMaterial& mat = PHMaterial());

	// 同期
	void		Sync();

	// モディファイ
	void		Modify();
};

}

#endif

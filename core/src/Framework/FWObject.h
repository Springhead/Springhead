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
#include "../Foundation/Object.h"

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
	virtual PHSolidIf*	GetPHSolid(){ return phSolid; }
	virtual void		SetPHSolid(PHSolidIf* s){ phSolid = s; }

	virtual GRFrameIf*	GetGRFrame(){ return grFrame; }
	virtual void		SetGRFrame(GRFrameIf* f){ grFrame = f; }

	virtual PHJointIf*	GetPHJoint(){ return phJoint; }
	virtual void		SetPHJoint(PHJointIf* j){ phJoint = j; }

	virtual GRFrameIf*	GetChildFrame(){ return childFrame; }
	virtual void		SetChildFrame(GRFrameIf* f){ childFrame = f; }

	// 同期オプション
	virtual void		SetSyncSource(FWObjectDesc::FWObjectSyncSource syncSrc) { syncSource = syncSrc; }
	virtual FWObjectDesc::FWObjectSyncSource GetSyncSource() { return syncSource; }

	virtual void		EnableAbsolute(bool bAbs) { bAbsolute = bAbs; }
	virtual bool		IsAbsolute() { return bAbsolute; }

	// グラフィックス用メッシュに関するヘルパ
	virtual bool		LoadMesh(const char* filename, const IfInfo* ii = NULL, GRFrameIf* frame = NULL);
	virtual void		GenerateCDMesh(GRFrameIf* frame = NULL, const PHMaterial& mat = PHMaterial());

	// 同期
	virtual void		Sync();
	virtual void		Sync2(); //小野原追加

	// モディファイ
	virtual void		Modify();
};

}

#endif

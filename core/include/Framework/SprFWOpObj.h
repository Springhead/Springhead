/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#ifndef SPRFWOPOBJ_H
#define SPRFWOPOBJ_H

#include <Framework/SprFWObject.h>

namespace Spr{;


struct FWOpObjDesc :public FWObjectDesc{

	float fwPSize;
};

struct FWOpObjIf : FWObjectIf{
	SPR_IFDEF(FWOpObj);
	void Blend();

	///	子オブジェクトの数
	 size_t NChildObject() const;
	///	子オブジェクトの取得
	 ObjectIf* GetChildObject(size_t pos);
	///	子オブジェクトの追加
	 bool AddChildObject(ObjectIf* o);
	 void CreateOpObjWithRadius(float r);
	 void CreateOpObj();
	 ObjectIf* GetOpObj();
	 ObjectIf* GetGRMesh();
};

}
#endif
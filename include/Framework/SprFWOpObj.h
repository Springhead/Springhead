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
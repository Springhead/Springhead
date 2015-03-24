#ifndef FWOPOBJ_H
#define FWOPOBJ_H


#include <Framework\SprFWOpObj.h>
//#include "FWObject.h"
#include <Framework/FWObject.h>
#include "FrameworkDecl.hpp"
#include <Graphics\GRMesh.h>
#include <Physics/PHOpObj.h>
namespace Spr{ ;
//class GRMesh;
//class PHOpObj;

class FWOpObj: public FWObject{
	SPR_OBJECTDEF(FWOpObj);
	//SPR_DECLMEMBEROF_FWOpObjDesc;
public :
	UTRef<GRMesh> grMesh;
	UTRef<PHOpObj> opObj;
	float fwPSize;

	FWOpObj(const FWOpObjDesc& d = FWOpObjDesc());

	//GRMeshから、Opオブジェクトを作る
	void CreateOpObj();
	//頂点をGraphicsに反映する
	void Blend();
	void Sync();
	///	子オブジェクトの数
	virtual size_t NChildObject() const;
	///	子オブジェクトの取得
	virtual ObjectIf* GetChildObject(size_t pos);
	///	子オブジェクトの追加
	virtual bool AddChildObject(ObjectIf* o);

};

}
#endif
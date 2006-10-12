/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef GRSphere_H
#define GRSphere_H

#include <SprGraphics.h>
#include "GRFrame.h"

namespace Spr{;

class GRMaterial;
/**	@class	GRSphere
    @brief	グラフィックスシーングラフでの球オブジェクトを表す． */
class GRSphere: public InheritGRVisual<GRSphereIf, GRVisual>, public GRSphereDesc{
	unsigned int list;				///< ディスプレイリストの識別子
	GRRenderIf* render;
	
	/// ディスプレイリストを作成．
	void CreateList(GRRenderIf* r);
public:
	OBJECT_DEF(GRSphere);
	ACCESS_DESC(GRSphere);
	
	UTRef<GRMaterial> material;		///< マテリアル
	
	GRSphere(const GRSphereDesc& desc=GRSphereDesc());
	~GRSphere();
	void Render(GRRenderIf* r);
	void Rendered(GRRenderIf* r);
	virtual bool AddChildObject(ObjectIf* o);
};
}
#endif

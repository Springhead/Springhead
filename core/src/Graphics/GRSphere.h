/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef GRSphere_H
#define GRSphere_H

#include <Graphics/SprGRSphere.h>
#include <Graphics/GRFrame.h>
#include <Graphics/GRRender.h>

namespace Spr{;

class GRMaterial;
/**	@class	GRSphere
    @brief	グラフィックスシーングラフでの球オブジェクトを表す． */
class GRSphere: public GRVisual, public GRSphereDesc{
	unsigned int list;				///< ディスプレイリストの識別子
	GRRenderIf* render;
	
	/// ディスプレイリストを作成．
	void CreateList(GRRenderIf* r);
public:
	SPR_OBJECTDEF(GRSphere);
	ACCESS_DESC(GRSphere);
	
	UTRef<GRMaterial> material;		///< マテリアル
	
	GRSphere(const GRSphereDesc& desc=GRSphereDesc());
	~GRSphere();
	void Render(GRRenderIf* r);
	void Rendered(GRRenderIf* r);
	virtual bool AddChildObject(ObjectIf* o);
	virtual void SetMaterial(GRMaterialIf* mat) { material = mat->Cast(); }
	virtual GRMaterialIf* GetMaterial() { return material->Cast(); }
	void SetRender(GRRenderIf* r) {render = r;}
};
}
#endif

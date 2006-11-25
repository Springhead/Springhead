/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef GRFrame_H
#define GRFrame_H

#include <SprGraphics.h>
#include "IfStubGraphics.h"

namespace Spr{;

/**	@class	GRVisual
    @brief	 */
class GRVisual: public NamedObject, GRVisualIfInit{
public:
	OBJECT_DEF_ABST(GRVisual);
	virtual void Render(GRRenderIf* render){}
	virtual void Rendered(GRRenderIf* render){}
};

/**	@class	GRFrame
    @brief	グラフィックスシーングラフのツリーのノード 座標系を表す */
class GRFrame: public GRVisual, GRFrameIfInit, public GRFrameDesc{
public:
	OBJECT_DEF(GRFrame);
	ACCESS_DESC(GRFrame);
	GRFrame* parent;
	typedef std::vector< UTRef<GRVisual> > GRVisuals;
	GRVisuals children;
	GRFrame(const GRFrameDesc& desc=GRFrameDesc());


	virtual GRFrameIf* GetParent(){ return parent->GetIf(); }
	virtual void SetParent(GRFrameIf* fr);
	virtual void SetNameManager(NameManager* n);
	virtual GRSceneIf* GetScene();
	virtual bool AddChildObject(ObjectIf* v);
	virtual bool DelChildObject(ObjectIf* v);

	virtual void Render(GRRenderIf* r);
	virtual void Rendered(GRRenderIf* r);
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t pos);
	virtual Affinef GetWorldTransform(){ if (parent) return parent->GetWorldTransform() * transform; return transform; }
	virtual Affinef GetTransform(){ return transform; }
	virtual void SetTransform(Affinef& af){ transform = af; }
	void Print(std::ostream& os) const ;
};
}
#endif

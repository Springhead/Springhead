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
class GRVisual: public NamedObject, public GRVisualIfInit{
public:
	OBJECTDEF_ABST(GRVisual, NamedObject);
	virtual void Render(GRRenderIf* render){}
	virtual void Rendered(GRRenderIf* render){}
};

/**	@class	GRFrame
    @brief	グラフィックスシーングラフのツリーのノード 座標系を表す */
class GRFrame: public GRVisual, public GRFrameIfInit, public GRFrameDesc{
public:
	OBJECTDEF(GRFrame, GRVisual);
	ACCESS_DESC(GRFrame);
	GRFrame* parent;
	typedef std::vector< UTRef<GRVisualIf> > GRVisualIfs;
	GRVisualIfs children;
	GRFrame(const GRFrameDesc& desc=GRFrameDesc());

	virtual GRFrameIf* GetParent(){ return parent->Cast(); }
	virtual void SetParent(GRFrameIf* fr);
	virtual int NChildren(){ return (int)children.size(); }
	virtual GRVisualIf** GetChildren(){ return children.empty() ? NULL : (GRVisualIf**)&*children.begin(); }
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
	virtual void SetTransform(const Affinef& af){ transform = af; }
	void Print(std::ostream& os) const ;
};


class GRAnimation: public SceneObject, public GRAnimationIfInit, public GRAnimationDesc{
public:
	OBJECTDEF(GRAnimation, SceneObject);
	bool activated;
	float duration;
	float weight;
	float weightTo;
	float time;
	GRAnimation();
	///	時刻を設定する
	void SetTime(float time);
};

class GRAnimationSet: public SceneObject, public GRAnimationSetIfInit{
	typedef std::vector< UTRef<GRAnimation> > Animations;
	Animations animations;
	Animations activeAnimations;
public:
	OBJECTDEF(GRAnimationSet, SceneObject);
	ObjectIf* GetChildObject(size_t p);
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
	virtual int NChildObject();
	virtual void Activate(UTString name, 
		float weightStart=1, float weightEnd=1, float duration=-1);
	virtual void Deactivate(UTString name);
	///	時刻を設定する
	void SetTime(float time);
};


}
#endif

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
class GRVisual: public SceneObject, public GRVisualIfInit{
public:
	OBJECTDEF_ABST(GRVisual, SceneObject);
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

	GRSceneIf* GetScene(){return DCAST(GRSceneIf, GRVisual::GetScene());}
	
	virtual GRFrameIf* GetParent(){ return parent->Cast(); }
	virtual void SetParent(GRFrameIf* fr);
	virtual int NChildren(){ return (int)children.size(); }
	virtual GRVisualIf** GetChildren(){ return children.empty() ? NULL : (GRVisualIf**)&*children.begin(); }
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
	GRAnimation(GRAnimationDesc& d = GRAnimationDesc()){}
	///
	struct Target{
		GRFrameIf* target;
		Affinef initalTransform;
	};
	typedef std::vector<Target> Targets;
	///	変換対象フレーム
	Targets targets;
	///	
	virtual void BlendPose(float time, float weight);
	///
	virtual void ResetPose();
	///
	virtual void LoadInitialPose();
	///	
	virtual bool AddChildObject(ObjectIf* v);
};

class GRAnimationSet: public SceneObject, public GRAnimationSetIfInit{
	typedef std::vector< UTRef<GRAnimation> > Animations;
	Animations animations;
	std::vector<GRFrame*> roots;

public:
	OBJECTDEF(GRAnimationSet, SceneObject);
	GRAnimationSet(GRAnimationSetDesc& d = GRAnimationSetDesc()){}
	///	子オブジェクト(animations)を返す
	ObjectIf* GetChildObject(size_t p);
	///	GRAnimationの追加
	virtual bool AddChildObject(ObjectIf* o);
	///	GRAnimationの削除
	virtual bool DelChildObject(ObjectIf* o);
	///	GRAnimationの数
	virtual int NChildObject();

	///	指定の時刻の変換に重みをかけて、ボーンをあらわすターゲットのフレームに適用する。
	virtual void BlendPose(float time, float weight);
	///	ボーンをあらわすターゲットのフレームの行列を初期値に戻す．
	virtual void ResetPose();
	///
	virtual void LoadInitialPose();
};

class GRAnimationController: public SceneObject, GRAnimationControllerIfInit{
public:
	typedef std::map<UTString, UTRef<GRAnimationSet>, UTStringLess> Sets;
	Sets sets;
	OBJECTDEF(GRAnimationController, SceneObject);
	GRAnimationController(const GRAnimationControllerDesc& d = GRAnimationControllerDesc()){}
	///	指定の時刻の変換に重みをかけて、ボーンをあらわすターゲットのフレームに適用する。
	virtual void BlendPose(UTString name, float time, float weight);
	///	フレームの変換行列を初期値に戻す．
	virtual void ResetPose();
	///	フレームの変換行列を初期値に戻す．
	virtual void LoadInitialPose();

	///	GRAnimationの追加
	virtual bool AddChildObject(ObjectIf* o);
	///	GRAnimationの削除
	virtual bool DelChildObject(ObjectIf* o);
	///	GRAnimationの数
	virtual int NChildObject();
	///	GRAnimationの取得
	ObjectIf* GetChildObject(size_t p);
};

}//	namespace Spr
#endif

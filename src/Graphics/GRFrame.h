/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef GRFrame_H
#define GRFrame_H

#include <Graphics/SprGRScene.h>
#include "../Foundation/Object.h"
#include <map>

namespace Spr{;

/**	@class	GRVisual
    @brief	 */
class GRVisual: public SceneObject{
public:
	SPR_OBJECTDEF_ABST(GRVisual);
	bool	enabled;

public:
	virtual void Render(GRRenderIf* render){}
	virtual void Rendered(GRRenderIf* render){}
	void Enable(bool on = true){ enabled = on; }
	bool IsEnabled(){ return enabled; }

	GRVisual(){ enabled = true; }
};

class GRLight;
class GRMaterial;
class GRMesh;
class GRFrame;

/**	@class	GRFrame
    @brief	グラフィックスシーングラフのツリーのノード 座標系を表す */
class GRFrame: public GRVisual, public GRFrameDesc{
public:
	SPR_OBJECTDEF(GRFrame);
	ACCESS_DESC(GRFrame);
	GRFrame* parent;
	typedef std::vector< UTRef<GRVisualIf> > GRVisualIfs;
	GRVisualIfs children;

	/// レンダリング順序がクリティカルなものを個別保持
	std::vector<GRLight*>		lights;
	std::vector<GRMaterial*>	materials;
	std::vector<GRMesh*>		meshes;
	std::vector<GRFrame*>		frames;
	std::vector<GRVisual*>		miscs;

	GRFrame(const GRFrameDesc& desc=GRFrameDesc());

	// インタフェースの実装
	GRSceneIf*   GetScene(){return DCAST(GRSceneIf, GRVisual::GetScene());}
	GRFrameIf*   GetParent(){ return parent->Cast(); }
	void         SetParent(GRFrameIf* fr);
	int          NChildren(){ return (int)children.size(); }
	GRVisualIf** GetChildren(){ return children.empty() ? NULL : (GRVisualIf**)&*children.begin(); }
	Affinef      GetWorldTransform(){ if (parent) return parent->GetWorldTransform() * transform; return transform; }
	Affinef      GetTransform(){ return transform; }
	void         SetTransform(const Affinef& af){ transform = af; }
	bool         CalcBBox(Vec3f& bbmin, Vec3f& bbmax, const Affinef& aff = Affinef());

	// Objectの仮想関数
	virtual SceneObjectIf* CloneObject(); 
	virtual bool AddChildObject(ObjectIf* v);
	virtual bool DelChildObject(ObjectIf* v);
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t pos);
	virtual void Clear(){children.clear();}
	virtual void Print(std::ostream& os) const ;

	// GRVisualの仮想関数
	virtual void Render(GRRenderIf* r);
	virtual void Rendered(GRRenderIf* r);
	
	// Keyframe Blending based on Radial Basis Function 
	std::vector< PTM::VVector<float> > kfPositions;
	PTM::VVector<float> kfAffines[4][4], kfCoeffs[4][4];
	void AddRBFKeyFrame(PTM::VVector<float> pos);
	void BlendRBF(PTM::VVector<float> pos);
};

/**	@class	GRDummyFrame
    @brief	表示しないコンテナ．Visualをしまっておいて，後でプログラムから使うために使う　*/
class GRDummyFrame: public GRVisual, public GRDummyFrameDesc{
public:
	SPR_OBJECTDEF(GRDummyFrame);
	ACCESS_DESC(GRDummyFrame);
	typedef std::vector< UTRef<GRVisualIf> > GRVisualIfs;
	GRVisualIfs children;
	GRDummyFrame(const GRDummyFrameDesc& desc=GRDummyFrameDesc());
	GRSceneIf* GetScene(){return DCAST(GRSceneIf, GRVisual::GetScene());}
	
	virtual bool AddChildObject(ObjectIf* v);
	virtual bool DelChildObject(ObjectIf* v);
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t pos);
};

class GRAnimation: public SceneObject, public GRAnimationDesc{
public:
	SPR_OBJECTDEF(GRAnimation);
	GRAnimation(const GRAnimationDesc& d = GRAnimationDesc()){}
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
	virtual void BlendPose(float time, float weight, bool add);
	///
	virtual void ResetPose();
	///
	virtual void LoadInitialPose();
	///	
	virtual bool AddChildObject(ObjectIf* v);
	///
	virtual GRAnimationKey GetAnimationKey(int n);
	///	
	virtual int NAnimationKey();
	/// 現在のGRFrameからGRKeyを作成する tは時刻を指定
	void SetCurrentPose(float t);
	/// GRKeyを時刻順にソートする
	void SortGRKey();
	/// 時刻tのGRKeyを削除する
	void DeletePose(float t);
	/// アニメーションの最終時間を取得する
	float GetLastKeyTime();
};

class GRAnimationSet: public SceneObject{
	typedef std::vector< UTRef<GRAnimation> > Animations;
	Animations animations;
	std::vector<GRFrame*> roots;
	float lastKeyTime;

public:
	SPR_OBJECTDEF(GRAnimationSet);
	GRAnimationSet(const GRAnimationSetDesc& d = GRAnimationSetDesc()){}
	///	子オブジェクト(animations)を返す
	ObjectIf* GetChildObject(size_t p);
	///	GRAnimationの追加
	virtual bool AddChildObject(ObjectIf* o);
	///	GRAnimationの削除
	virtual bool DelChildObject(ObjectIf* o);
	///	GRAnimationの数
	size_t NChildObject() const;

	///	指定の時刻の変換に重みをかけて、ボーンをあらわすターゲットのフレームに適用する。
	virtual void BlendPose(float time, float weight);
	///	指定の時刻の変換に重みをかけて、ボーンをあらわすターゲットのフレームに適用する。
	virtual void BlendPose(float time, float weight, bool add);
	///	ボーンをあらわすターゲットのフレームの行列を初期値に戻す．
	virtual void ResetPose();
	///
	virtual void LoadInitialPose();
	/// 現在のGRFrameからポーズを作成する tは時刻を指定
	void SetCurrentAnimationPose(float t);
	/// 時刻tのGRKeyを削除する
	void DeleteAnimationPose(float t);
	/// GRKeyからアニメーションの最終時間を設定する
	void UpdateLastKeyTime();
	/// アニメーションの最終時間を取得する
	float GetLastKeyTime();
};

class GRAnimationController: public SceneObject{
public:
	typedef std::map<UTString, UTRef<GRAnimationSet>, UTStringLess> Sets;
	Sets sets;
	float buffer[16];
	SPR_OBJECTDEF(GRAnimationController);
	GRAnimationController(const GRAnimationControllerDesc& d = GRAnimationControllerDesc()){}
	///	指定の時刻の変換に重みをかけて、ボーンをあらわすターゲットのフレームに適用する。
	virtual void BlendPose(UTString name, float time, float weight);
	///	指定の時刻の変換に重みをかけて、ボーンをあらわすターゲットのフレームに適用する。
	virtual void BlendPose(UTString name, float time, float weight, bool add);
	///	フレームの変換行列を初期値に戻す．
	virtual void ResetPose();
	///	フレームの変換行列を初期値に戻す．
	virtual void LoadInitialPose();

	///	GRAnimationの追加
	virtual bool AddChildObject(ObjectIf* o);
	///	GRAnimationの削除
	virtual bool DelChildObject(ObjectIf* o);
	///	GRAnimationの数
	virtual size_t NChildObject() const;
	///	GRAnimationの取得
	ObjectIf* GetChildObject(size_t p);
	///	GRAnimationSetの取得
	GRAnimationSetIf* GetAnimationSet(size_t p);
};

}//	namespace Spr
#endif

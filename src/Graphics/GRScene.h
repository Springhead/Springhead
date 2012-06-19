/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef GRSCENE_H
#define GRSCENE_H

#include <Graphics/SprGRScene.h>
#include <Graphics/GRFrame.h>
#include <Graphics/GRRender.h>
#include <Foundation/Scene.h>

namespace Spr {;

class GRCamera;
class GRAnimationController;

/**	シーングラフのトップノード．光源・視点を持つ．
	レンダラとシーングラフの関係が深いため，
	レンダラが違うとシーングラフはそのまま使用できない．
	シーングラフは，たとえばレンダラがOpenGLなら，displayList
	を持っているし，D3Dならば ID3DXMeshを持っている．
*/
class SPR_DLL GRScene:public Scene, public GRSceneDesc{
	SPR_OBJECTDEF(GRScene);
protected:
	///	ルートノード
	UTRef<GRFrame> world;
	///	カメラ
	UTRef<GRCamera> camera;
	///	アニメーションコントローラ
	UTRef<GRAnimationController> animationController;
public:
	///	コンストラクタ
	GRScene(const GRSceneDesc& desc=GRSceneDesc());
	void Init();
	///	デストラクタ
	~GRScene(){}

	GRSdkIf* GetSdk();

	///	シーンを空にする．
	void Clear();

	virtual GRVisualIf* CreateVisual(const IfInfo* info, const GRVisualDesc& desc, GRFrameIf* parent = NULL);
	virtual GRFrameIf* GetWorld(){return world->Cast();}
	virtual void		SetCamera(const GRCameraDesc& desc);
	virtual GRCameraIf* GetCamera(){return camera->Cast();}
	virtual GRAnimationControllerIf* GetAnimationController(){return animationController->Cast();}
	virtual bool AddChildObject(ObjectIf* o);
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t pos);
	virtual void Render(GRRenderIf* r);

	void* GetDescAddress(){ return (GRSceneDesc*)this; }
	friend class GRFrame;
};

}
#endif

/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef GRSCENE_H
#define GRSCENE_H
#include <Graphics/SprGRScene.h>
#include <Foundation/Scene.h>
#include <Graphics/GRFrame.h>
#include <Graphics/GRRender.h>

namespace Spr {;

class GRCamera;
/**	シーングラフのトップノード．光源・視点を持つ．
	レンダラとシーングラフの関係が深いため，
	レンダラが違うとシーングラフはそのまま使用できない．
	シーングラフは，たとえばレンダラがOpenGLなら，displayList
	を持っているし，D3Dならば ID3DXMeshを持っている．
*/
class SPR_DLL GRScene:public Scene, GRSceneIfInit, public GRSceneDesc{
	OBJECT_DEF(GRScene);
protected:
	///	ルートノード
	UTRef<GRFrame> world;
	///	カメラ
	UTRef<GRCamera> camera;
public:
	///	コンストラクタ
	GRScene(const GRSceneDesc& desc=GRSceneDesc());
	void Init();
	///	デストラクタ
	~GRScene(){}

	GRSdkIf* GetSdk();

	///	シーンを空にする．
	void Clear();

	virtual GRFrameIf* CreateFrame(const GRFrameDesc& desc);
	virtual GRFrameIf* GetWorld(){return world->GetIf();}
	virtual GRCameraIf* GetCamera(){return camera->GetIf();}
	virtual bool AddChildObject(ObjectIf* o);
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t pos);
	virtual void Render(GRRenderIf* r);

	void* GetDescAddress(){ return (GRSceneDesc*)this; }
	friend class GRFrame;
};

}
#endif

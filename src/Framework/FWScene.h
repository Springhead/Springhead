#ifndef FWSCENE_H
#define FWSCENE_H

#include <Framework/SprFWScene.h>
#include <Framework/SprFWObject.h>
#include <Foundation/Object.h>
#include <Foundation/Scene.h>
#include "IfStubFramework.h"

namespace Spr{;


///	FWSceneのファクトリ
class FWSceneFactory : public FactoryBase {
public:
	const IfInfo* GetIfInfo() const {
		return FWSceneIf::GetIfInfoStatic();
	}
	ObjectIf* Create(const void* desc, const ObjectIf*){
		return CreateFWScene();
	}
};

///	GraphicsとPhysicsのSceneをまとめたシーングラフ全体
class FWScene : public Scene, public FWSceneIfInit, public FWSceneDesc {
public:
	OBJECTDEF(FWScene, Scene);
	ACCESS_DESC(FWScene);
	typedef std::vector< UTRef<FWObjectIf> > FWObjects;
	FWObjects fwObjects;						///<	物理とグラフィックスのオブジェクトのリンク
	UTRef<PHSceneIf> phScene;					///<	物理シミュレーション用のシーン
	UTRef<GRSceneIf> grScene;					///<	グラフィックス用のシーン
	typedef std::vector< UTRef<HIForceDevice6D> > FWHumanInterfaces;
	//	hase TBW
	//	FWHumanInterfaces humanInterfaces;			///<	ユーザインタフェース．[0]がカメラ．HIForceDevice6Dのクラス名は変更する予定．
	enum HumanInterfacePurposeId{
		HI_CAMERACONTROLLER,
		HI_OBJECTMANIPULATOR0,
		HI_OBJECTMANIPULATOR1,
		HI_OBJECTMANIPULATOR2,
		HI_OBJECTMANIPULATOR3,
	};
	//
	FWScene(const FWSceneDesc& d=FWSceneDesc()); // コンストラクタ
	//
	PHSceneIf* GetPHScene(){ return phScene; }
	GRSceneIf* GetGRScene(){ return grScene; }
	///	オブジェクトの位置・姿勢を同期
	void Sync();
	void Step();
	void Draw(GRRenderIf* grRender, bool debug=false);
	HIForceDevice6D* GetHumanInterface(size_t pos);
	//
	virtual ObjectIf* CreateObject(const IfInfo* info, const void* desc);
	virtual bool AddChildObject(ObjectIf* o);
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t pos);
	virtual NamedObjectIf* FindObject(UTString name, UTString cls);
	virtual void AddHumanInterface(HIForceDevice6D* d);
};


}

#endif

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FWSCENE_H
#define FWSCENE_H

#include <Springhead.h>
#include <Framework/SprFWScene.h>
#include <Framework/SprFWObject.h>
#include <Framework/SprFWBone.h>
#include <Foundation/Object.h>
#include <Foundation/Scene.h>

namespace Spr{;

class FWSdk;

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
class FWScene : public Scene, public FWSceneDesc {
public:
	SPR_OBJECTDEF(FWScene);
	ACCESS_DESC(FWScene);
	typedef std::vector< UTRef<FWObjectIf> > FWObjects;
	typedef std::vector< UTRef<FWBoneIf> >	 FWBones;
	typedef std::vector< UTRef<FWStructureIf> >	 FWStructures;
	FWSdk*	sdk;								///<	親SDKへの参照
	FWObjects fwObjects;						///<	物理とグラフィックスのオブジェクトのリンク
	FWBones	  fwBones;							///<	物理とグラフィックスのBoneのリンク
	FWStructures fwStructures;					///<	BoneObjectの集合体であるFWStructureへのリンク
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
	/// コンストラクタ
	FWScene(const FWSceneDesc& d=FWSceneDesc());
	
	PHSceneIf*	GetPHScene(){ return phScene; }
	void		SetPHScene(PHSceneIf* s){ phScene = s; }
	GRSceneIf*	GetGRScene(){ return grScene; }
	void		SetGRScene(GRSceneIf* s){ grScene = s; }
	FWObjectIf* CreateFWObject();
	int NObject()const{return (int)fwObjects.size();}
	FWObjectIf** GetObjects(){return (FWObjectIf**)&*fwObjects.begin();}

	void		Sync();
	void		Step();
	void		Draw(GRRenderIf* grRender, bool debug=false);

	HIForceDevice6D*	GetHumanInterface(size_t pos);
	void				AddHumanInterface(HIForceDevice6D* d);
	void				SetFWBones(FWBoneIf* b);
	std::vector< UTRef<FWBoneIf> > GetFWBones();
	void				CreateFWStructure();
	void				AddFWStructure(FWStructureIf* o);
	FWStructureIf*		GetFWStructure();
	FWStructureIf*		GetFWStructure(int n);
	size_t				NFWStructure();

	//
	virtual ObjectIf* CreateObject(const IfInfo* info, const void* desc);
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t pos);
	virtual NamedObjectIf* FindObject(UTString name, UTString cls);
	
};


}

#endif

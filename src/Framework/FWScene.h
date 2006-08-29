#ifndef FWSCENE_H
#define FWSCENE_H

#include <Framework/SprFWScene.h>
#include <Framework/SprFWObject.h>
#include <Foundation/Object.h>
#include <Foundation/Scene.h>

namespace Spr{

	class FWSceneFactory : public FactoryBase {
	public:
		const IfInfo* GetIfInfo() const {
			return FWSceneIf::GetIfInfoStatic();
		}
		UTRef<ObjectIf> Create(const void* desc, const ObjectIf*){
			return CreateFWScene();
		}
	};
	
	
	class FWScene : public InheritScene<FWSceneIf, Scene>, public FWSceneDesc {
    public:
		OBJECT_DEF(FWScene);
		ACCESS_DESC(FWScene);
		std::vector< UTRef<FWObjectIf> > fwObjects;
		UTRef<PHSceneIf> phScene;
		UTRef<GRSceneIf> grScene;
		//
		FWScene(const FWSceneDesc& d=FWSceneDesc()); // コンストラクタ
		//
		PHSceneIf* GetPHScene(){ return phScene; }
		GRSceneIf* GetGRScene(){ return grScene; }
		///	オブジェクトの位置・姿勢を同期
		void Sync();
		//
		virtual ObjectIf* CreateObject(const IfInfo* info, const void* desc);
		virtual bool AddChildObject(ObjectIf* o);
		virtual size_t NChildObject() const;
		virtual ObjectIf* GetChildObject(size_t pos);
	};

}

#endif

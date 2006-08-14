#ifndef FWSCENE_H
#define FWSCENE_H

#include <Framework/SprFWScene.h>
#include <Foundation/Object.h>
#include <Foundation/Scene.h>

namespace Spr{

	class FWSceneFactory : public SdkFactoryBase {
	public:
		const IfInfo* GetIfInfo() const {
			return FWSceneIf::GetIfInfoStatic();
		}
		ObjectIf* Create(const void* desc){
			return CreateFWScene();
		}
	};

	class FWScene : public InheritScene<FWSceneIf, Scene>, public FWSceneDesc {
    public:
		OBJECT_DEF(FWScene);
		ACCESS_DESC(FWScene);
		std::vector<ObjectIf*> objs;
		//
		FWScene(const FWSceneDesc& d=FWSceneDesc()); // コンストラクタ
		~FWScene(); // デストラクタ
		//
		void Link(PHSceneIf* phScene, GRSceneIf* grScene);
		//
		virtual bool AddChildObject(ObjectIf* o);
		virtual size_t NChildObject() const;
		virtual ObjectIf* GetChildObject(size_t pos);
	};

}

#endif

#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include <stdlib.h>

namespace Spr{;

struct Sdks{
	typedef std::vector< PHSdkIf* > Cont;
	Cont* cont;

	Sdks(){
#if defined _DEBUG && _MSC_VER			
		// メモリリークチェッカ
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif		
		cont = DBG_NEW Cont;
	}
	~Sdks(){
		while(cont->size()){
			PHSdkIf* sdk = cont->back();
			cont->erase(cont->end()-1);
			sdk->DelRef();
			if (sdk->RefCount()==0){
				delete sdk;
			}
		}
		delete cont;
	}
};
static Sdks sdks;
PHSdkIf* SPR_CDECL CreatePHSdk(){
	PHSdkIf* rv = DBG_NEW PHSdk;
	sdks.cont->push_back(rv);
	rv->AddRef();
	return rv;
}

//----------------------------------------------------------------------------
//	PHSdk
OBJECTIMP(PHSdk, NameManager);
IF_IMP(PHSdk, NameManager);
PHSdk::~PHSdk(){
	for(Sdks::Cont::iterator it = sdks.cont->begin(); it != sdks.cont->end(); ++it){
		if (*it == this){
			sdks.cont->erase(it);
			DelRef();
			break;
		}
	}
}

PHSceneIf* PHSdk::CreateScene(const PHSceneDesc& desc){
	PHScene* rv = DBG_NEW PHScene(this, desc);
	rv->SetNameManager(this);
	scenes.push_back(rv);
	return rv;
}
PHSceneIf* PHSdk::CreateScene(){
	return CreateScene(PHSceneDesc());
}
int PHSdk::NScene(){
	return scenes.size();
}
PHSceneIf** PHSdk::GetScenes(){
	return (PHSceneIf**)&*scenes.begin();
}

CDShapeIf* PHSdk::CreateShape(const CDShapeDesc& desc){
	if (desc.type == CDShapeDesc::CONVEXMESH){
		CDShape* s = DBG_NEW CDConvexMesh((const CDConvexMeshDesc&)desc);
		s->SetNameManager(this);
		shapes.push_back(s);
		return s;
	}else if (desc.type == CDShapeDesc::SPHERE){
		CDShape* s = DBG_NEW CDSphere((const CDSphereDesc&)desc);
		s->SetNameManager(this);
		shapes.push_back(s);
		return s;
	}else{
		DSTR << "Error: Unknown shape type " << desc.type << std::endl;
		return NULL;
	}
}
int PHSdk::NShape(){
	return shapes.size();
}
CDShapeIf** PHSdk::GetShapes(){
	return (CDShapeIf**)&*shapes.begin();
}
ObjectIf* PHSdk::CreateObject(const IfInfo* info, const void* desc){
	if (info->Inherit(CDShapeIf::GetIfInfoStatic())){
		return CreateShape(*(CDShapeDesc*)desc);
	}else if(info == PHSceneIf::GetIfInfoStatic()){
		return CreateScene(*(const PHSceneDesc*)desc);
	}
	return NULL;
}

}

#include "Physics.h"

#pragma hdrstop
//	メモリリークチェッカ
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


namespace Spr{;

struct Sdks{
	typedef std::vector< PHSdkIf* > Cont;
	Cont* cont;

	Sdks(){
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
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
PHSdkIf* _cdecl CreatePHSdk(){
	PHSdkIf* rv = DBG_NEW PHSdk;
	sdks.cont->push_back(rv);
	rv->AddRef();
	return rv;
}

//----------------------------------------------------------------------------
//	PHSdk
OBJECTIMP(PHSdk, Object);
IF_IMP(PHSdk, Object);
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
	PHSceneIf* rv = DBG_NEW PHScene(this, desc);
	scenes.push_back(rv);
	return rv;
}
PHSceneIf* PHSdk::CreateScene(){
	return CreateScene(PHSceneDesc());
}
int PHSdk::GetNScene(){
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
	}else{
		DSTR << "Error: Unknown shape type " << desc.type << std::endl;
		return NULL;
	}
}
int PHSdk::GetNShape(){
	return shapes.size();
}
CDShapeIf** PHSdk::GetShapes(){
	return (CDShapeIf**)&*shapes.begin();
}
ObjectIf* PHSdk::CreateObject(const IfInfo* info, const void* desc){
	if (info == CDShapeIf::GetIfInfoStatic()){
		return CreateShape(*(const CDShapeDesc*)desc);
	}else if(info == PHSceneIf::GetIfInfoStatic()){
		return CreateScene(*(const PHSceneDesc*)desc);
	}
	return NULL;
}
void PHSdk::Print(std::ostream& os) const{
	Object::Print(os);
	int w = os.width();
	for(size_t i=0; i<scenes.size(); ++i){
		os.width(w+2);
		scenes[i]->Print(os);
	}
	os.width(w);
}

}

#include "FileIO.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#include "FINodeHandler.h"
#include "FIFileContext.h"
namespace Spr{;

#define REGISTER(x)	\
	FINodeHandlers::theNodeHandlers->insert(DBG_NEW x);

UTRef<FINodeHandlers> FINodeHandlers::theNodeHandlers;

template <class T>
class FINodeHandlerImp: public FINodeHandler{
public:
	typedef T DESC;
	FINodeHandlerImp(const char* t){
		type = t;
	}
	template <class S>
	void Get(S*& s, FIFileContext* ctx){
		s = NULL;
		for(int i=ctx->objects.size()-1; i>=0 && !s; --i){
			s = ICAST(S, ctx->objects[i]);
		}
		assert(s);
	}
	void Load(FIFileContext* ctx){
		T* desc = (T*)ctx->datas.Top()->data;
		Load(*desc, ctx);
	}
	virtual void Load(T& t, FIFileContext* ctx)=0;
};

class FINodeHandlerFIInactiveSolids: public FINodeHandlerImp<FIInactiveSolids>{
public:
	struct Task:public FIFileContext::Task{
		FIInactiveSolids desc;
		PHSceneIf* scene;
		Task(const FIInactiveSolids& d, PHSceneIf* s):desc(d), scene(s){}
		void Execute(FIFileContext* ctx){
			for(unsigned i=0; i<desc.solids.size(); ++i){
				PHSolidIf* s1;
				scene->FindObject(s1, desc.solids[i]);
				for(unsigned j=i+1; j<desc.solids.size(); ++j){
					PHSolidIf* s2;
					scene->FindObject(s2, desc.solids[j]);
					scene->EnableContact(s1, s2, false);
				}
			}
		}
	};

	FINodeHandlerFIInactiveSolids(): FINodeHandlerImp<DESC>("FIInactiveSolids"){}
	void Load(FIInactiveSolids& desc, FIFileContext* ctx){
		PHSceneIf* scene;
		Get(scene, ctx);
		ctx->postTasks.push_back(new Task(desc, scene));
	}
};

void RegisterNodeHandlers(){
	if (!FINodeHandlers::theNodeHandlers)
		FINodeHandlers::theNodeHandlers = DBG_NEW FINodeHandlers;
	REGISTER(FINodeHandlerFIInactiveSolids);
}


}

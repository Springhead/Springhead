/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "FileIO.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#include "FINodeHandler.h"
#include "FILoadContext.h"
namespace Spr{;


class FINodeHandlerFIInactiveSolids: public FINodeHandlerImp<FIInactiveSolids>{
public:
	struct Task:public FILoadContext::Task{
		FIInactiveSolids desc;
		PHSceneIf* scene;
		Task(const FIInactiveSolids& d, PHSceneIf* s):desc(d), scene(s){}
		void Execute(FILoadContext* ctx){
			for(unsigned i=0; i<desc.solids.size(); ++i){
				PHSolidIf* s1;
				scene->FindObject(s1, desc.solids[i]);
				for(unsigned j=i+1; j<desc.solids.size(); ++j){
					PHSolidIf* s2;
					scene->FindObject(s2, desc.solids[j]);
					//scene->EnableContact(s1, s2, false);
					scene->SetContactMode(s1, s2, PHSceneDesc::MODE_NONE);

				}
			}
		}
	};

	FINodeHandlerFIInactiveSolids(): FINodeHandlerImp<Desc>("FIInactiveSolids"){}
	void Load(FIInactiveSolids& desc, FILoadContext* ctx){
		PHSceneIf* scene;
		Get(scene, ctx);
		ctx->postTasks.push_back(new Task(desc, scene));
	}
};

extern void RegisterOldSpringheadNodeHandlers();
void RegisterNodeHandlers(FINodeHandlers* handler){
	handler->insert(DBG_NEW FINodeHandlerFIInactiveSolids);
}


}

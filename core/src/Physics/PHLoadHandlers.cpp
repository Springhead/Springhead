/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Foundation/UTLoadContext.h>
#include <Foundation/UTLoadHandler.h>
#include <Physics/SprPHNodeHandlers.h>
#include <Physics/SprPHScene.h>

namespace Spr{;

class PHLoadHandlerPHInactiveSolids: public UTLoadHandlerImp<PHInactiveSolids>{
public:
	struct Task:public UTLoadTask{
		PHInactiveSolids desc;
		PHSceneIf* scene;
		Task(const PHInactiveSolids& d, PHSceneIf* s):desc(d), scene(s){}
		void Execute(UTLoadContext* ctx){
			for(unsigned i=0; i<desc.solids.size(); ++i){
				PHSolidIf* s1;
				scene->FindObject(s1, desc.solids[i]);
				for(unsigned j=i+1; j<desc.solids.size(); ++j){
					PHSolidIf* s2;
					scene->FindObject(s2, desc.solids[j]);
					scene->SetContactMode(s1, s2, PHSceneDesc::MODE_NONE);
				}
			}
		}
	};

	PHLoadHandlerPHInactiveSolids(): UTLoadHandlerImp<Desc>("PHInactiveSolids"){}
	void Load(PHInactiveSolids& desc, UTLoadContext* ctx){
		PHSceneIf* scene;
		Get(scene, ctx);
		ctx->postTasks.push_back(new Task(desc, scene));
	}
};
void SPR_CDECL PHRegisterLoadHandlers(){
	UTLoadHandlerDbPool::Get("Physics")->insert( DBG_NEW PHLoadHandlerPHInactiveSolids );
}

}

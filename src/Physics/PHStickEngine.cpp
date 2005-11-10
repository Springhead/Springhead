#include "Physics.h"
#pragma hdrstop
#include "PHStickEngine.h"

namespace Spr{;

//----------------------------------------------------------------------------
//	PHStick
//
SGOBJECTIMP(PHStick, SGObject);
bool PHStick::AddChildObject(SGObject* o, SGScene* s){
	SGFrame* fr = DCAST(SGFrame, o);
	if (!fr && DCAST(PHSolid, o)) fr = ((PHSolid*)o)->GetFrame();
	if (fr){
		if(!frame[0]){
			frame[0] = fr;
		}else if (!frame[1]){
			frame[1] = fr;
		}else{
			frame[0] = frame[1];
			frame[1] = fr;
		}
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------
//	PHStickEngine
//
SGOBJECTIMP(PHStickEngine, SGBehaviorEngine);
bool PHStickEngine::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHStick, o)){
		sticks.push_back((PHStick*)o);
		return true;
	}
	return false;
}
void PHStickEngine::After(SGScene* scene, CDFramePairWithRecord* fp){
	PHContactEngine::FrameRecord* fr[2] = {
		(PHContactEngine::FrameRecord*)&*fp->frame[0]->records[contactEngine->GetFrameRecordPos()],
		(PHContactEngine::FrameRecord*)&*fp->frame[1]->records[contactEngine->GetFrameRecordPos()]
	};
	PHStick* s = (PHStick*)&*fp->records[framePairRecordPos];
	PHContactEngine::FramePairRecord* cfpr= (PHContactEngine::FramePairRecord*)&*fp->records[contactEngine->GetFramePairRecordPos()];

	if (cfpr->reflexForce.square() > 1e-8){
		Vec3f normal = cfpr->reflexForce.unit();
		Vec3f pos = (cfpr->reflexForce ^ cfpr->reflexTorque) / cfpr->reflexForce.square();
		fr[0]->solid->AddForce(s->force * normal, pos);
		fr[1]->solid->AddForce(-s->force * normal, pos);
	}
}

void PHStickEngine::Clear(SGScene* s){
	listenerPos = -1;
	framePairRecordPos = -1;
	sticks.clear();
	contactEngine = NULL;
}

void PHStickEngine::Loaded(SGScene* scene){
	for(unsigned int i=0; i<sticks.size(); ++i){
		for(int j=0; j<2; ++j){
			PHSolid* solid = DCAST(PHSolid, sticks[i]->frame[j]);
			if (solid) sticks[i]->frame[j] = solid->GetFrame();
		}
	}
	contactEngine = NULL;
	scene->GetBehaviors().Find(contactEngine);
	if (!contactEngine){
		DSTR << "PHStickEngine requires PHContactEngine." << std::endl;
		return;
	}
	contactEngine->Loaded(scene);
	CDCollisionEngine* ce;
	scene->GetBehaviors().Find(ce);
	assert(ce);
	listenerPos = ce->AddListener(this);
	framePairRecordPos = ce->ReserveFramePairRecord();
	for(unsigned i=0; i<sticks.size(); ++i){
		int f1 = ce->GetFrameID(sticks[i]->frame[0]);
		int f2 = ce->GetFrameID(sticks[i]->frame[1]);
		ce->AddActive(f1, f2, listenerPos);
		CDFramePairWithRecord* fpr=ce->GetFramePair(f1, f2);
		if (!fpr) fpr=ce->GetFramePair(f2, f1);
		fpr->records[framePairRecordPos] = sticks[i];
	}
	ce->Init();
};

class PHStickLoader:public FIObjectLoader<PHStick>{
	virtual bool LoadData(FILoadScene* ctx, PHStick* s){
		ctx->docs.Top()->GetWholeData(s->force);
		return true;
	}
};
DEF_REGISTER_LOADER(PHStick);

class PHStickEngineLoader:public FIObjectLoader<PHStickEngine>{
	virtual bool LoadData(FILoadScene* ctx, PHStickEngine* se){
		return true;
	}
};
class PHStickEngineSaver:public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHStickEngine"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHStickEngine* se = (PHStickEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("StickEngine", se);
		ctx->docs.back()->AddChild(doc);
		for(PHStickEngine::Sticks::iterator it = se->sticks.begin(); it != se->sticks.end(); ++it){
			FIDocNodeBase* docStick = ctx->CreateDocNode("Stick");
			docStick->SetWholeData((*it)->force);
			doc->AddChild(docStick);
			docStick->AddChild(ctx->CreateDocNode("REF", (*it)->frame[0]));
			docStick->AddChild(ctx->CreateDocNode("REF", (*it)->frame[1]));
		}
	}
};
DEF_REGISTER_BOTH(PHStickEngine);

}

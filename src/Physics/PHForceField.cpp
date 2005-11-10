#include "Physics.h"
#pragma hdrstop
#include "PHForceField.h"

using namespace std;
namespace Spr{

/////////////////////////////////////////////////////////////
//PHGravityEngine/Loader/Saver
SGOBJECTIMP(PHGravityEngine, SGBehaviorEngine);

void PHGravityEngine::Step(SGScene*)
{
	ApplyForce();
}

bool PHGravityEngine::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHSolid, o)){
		solids.push_back((PHSolid*)o);
		return true;
	}
	return false;
}

void PHGravityEngine::ApplyForce()
{
	PHSolids::iterator it;
	PHSolid* solid;
	for(it = solids.begin(); it != solids.end(); it++){
		solid = *it;
		solid->AddForce((float)solid->GetMass() * accel);
	}
}


///////////////////////////////////////////////////////////////////
class PHGravityEngineLoader : public FIObjectLoader<PHGravityEngine>{
public:
	bool LoadData(FILoadScene* ctx, PHGravityEngine* gravity){		
		//d—Í‰Á‘¬“x‚ðŽæ“¾
		ctx->docs.Top()->GetWholeData(gravity->accel);
		gravity->accel.z *= -1;
		return true;
	}
};

class PHGravityEngineSaver : public FIBaseSaver{
public:
	UTString GetType() const{ return "PHGravityEngine";}
	void Save(FISaveScene* ctx, SGObject* arg){
		PHGravityEngine* gravity= (PHGravityEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("GravityEngine", gravity);
		ctx->docs.back()->AddChild(doc);
		gravity->accel.z *= -1;
		doc->SetWholeData(gravity->accel);
		gravity->accel.z *= -1;
		for(PHSolids::iterator it = gravity->solids.begin(); it != gravity->solids.end(); it++)
			doc->AddChild(ctx->CreateDocNode("REF", *it));
	}
};
DEF_REGISTER_BOTH(PHGravityEngine);

}
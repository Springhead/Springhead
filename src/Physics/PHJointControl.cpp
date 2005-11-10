#include "Physics.h"
#pragma hdrstop
#include "PHJointControl.h"

using namespace PTM;
namespace Spr{;

SGOBJECTIMP(PHJointControl, SGObject);

size_t PHJointControl::NReferenceObjects(){
	return joints.size();
}
SGObject* PHJointControl::ReferenceObject(size_t i){
	if (0 <= i && i < joints.size()) return joints[i];
	return NULL;
}
void PHJointControl::Loaded(SGScene* scene){
	for(TJoints::iterator it = joints.begin(); it != joints.end(); ++it){
		SGObject* obj = *it;
		PHJointPid* pid = PHJointPid::Find(DCAST(PHJoint1D, obj), scene);
		if (pid){
			*it = pid;
			break;
		}
	}
}
bool PHJointControl::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHJointPid, o)){
		joints.push_back((PHJointPid*)o);
		return true;
	}
	if (DCAST(PHJoint1D, o)){
		joints.push_back((PHJointPid*)o);
		return true;
	}
	return false;
}
bool PHJointControl::DelChildObject(SGObject* o, SGScene* s){
	TJoints::iterator it = std::find(joints.begin(), joints.end(), UTRef<PHJointPid>((PHJointPid*)o));
	if (it != joints.end()){
		joints.erase(it);
		return true;
	}
	return false;
}
void PHJointControl::Step(float pos){
	float v = (end-start) * 0.5f * float(1.0+cos((pos+(phase/360)) * 2 * M_PI)) + start;
	for(unsigned i=0; i<joints.size(); ++i){
		joints[i]->goal = (float)Rad(v);
	}
}

SGOBJECTIMP(PHJointControlEngine, SGBehaviorEngine);

PHJointControlEngine::PHJointControlEngine(){
	time = 0.0f;
}

void PHJointControlEngine::Step(SGScene* scene){
	time += (float)(scene->GetTimeStep() * vel);
	if (time > period) time -= period;
	for(unsigned i=0; i<jcs.size(); ++i){
		jcs[i]->Step(time / period);
	}
}
size_t PHJointControlEngine::NChildObjects(){
	return jcs.size();
}
SGObject* PHJointControlEngine::ChildObject(size_t i){
	if (0 <= i && i < jcs.size()) return jcs[i];
	return NULL;
}
bool PHJointControlEngine::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHJointControl, o)){
		jcs.push_back((PHJointControl*)o);
		return true;
	}
	return false;
}
bool PHJointControlEngine::DelChildObject(SGObject* o, SGScene* s){
	TJointControls::iterator it = std::find(jcs.begin(), jcs.end(), UTRef<PHJointControl>((PHJointControl*)o));
	if (it != jcs.end()){
		jcs.erase(it);
		return true;
	}
	return false;
}
void PHJointControlEngine::Loaded(SGScene* s){
	for(unsigned i=0; i<jcs.size(); ++i){
		jcs[i]->Loaded(s);
	}
}


typedef float FLOAT;
class PHJointControlLoader : public FIObjectLoader<PHJointControl>
{
public:
	PHJointControlLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(FLOAT);
		db->REG_RECORD_PROTO(XJointControl);
	}
	bool LoadData(FILoadScene* ctx, PHJointControl* jc){		
		ctx->docs.Top()->GetWholeData(*(XJointControl*)jc);
		return true;
	}
};

class PHJointControlSaver : public FIObjectSaver<PHJointControl>
{
protected:
	void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, PHJointControl* jc){
		doc->SetWholeData(*(XJointControl*)jc);
	}
};
DEF_REGISTER_BOTH(PHJointControl);

class PHJointControlEngineLoader : public FIObjectLoader<PHJointControlEngine>
{
public:
	PHJointControlEngineLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(FLOAT);
		db->REG_RECORD_PROTO(XJointControlEngine);
	}
	bool LoadData(FILoadScene* ctx, PHJointControlEngine* jc){		
		ctx->docs.Top()->GetWholeData(*(XJointControlEngine*)jc);
		return true;
	}
};

class PHJointControlEngineSaver : public FIObjectSaver<PHJointControlEngine>
{
protected:
	void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, PHJointControlEngine* jc){
		doc->SetWholeData(*(XJointControlEngine*)jc);
	}
};

DEF_REGISTER_BOTH(PHJointControlEngine);

}

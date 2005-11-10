#include "Physics.h"
#pragma hdrstop
#include "PHJointPid.h"

using namespace PTM;
namespace Spr{;

SGOBJECTIMP(PHJointPid, SGBehaviorEngine);

void PHJointPid::Print(std::ostream& os) const {
	int w = os.width();
	os.width(0);
	os << UTPadding(w);
	os << "<" << GetTypeInfo()->ClassName() << " " << GetName();
	if (joint) os << "j=" << joint->GetName();
	os << ">" << std::endl;
	os.width(w);
}
size_t PHJointPid::NReferenceObjects(){
	return joint ? 1 : 0;
}
SGObject* PHJointPid::ReferenceObject(size_t i){
	if (i == 0) return joint;
	return NULL;
}
bool PHJointPid::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHJointBase, o)){
		joint = (PHJointBase*)o;
		return true;
	}
	return false;
}
bool PHJointPid::DelChildObject(SGObject* o, SGScene* s){
	if (joint == o){
		joint = NULL;
		return true;
	}
	return false;
}
void PHJointPid::Step(SGScene* s){
	double now = type ? joint->GetJointVelocity(axis) : joint->GetJointPosition(axis);
	double e = goal - now;
	if (type==0) joint->LimitAngle(e);
	double ed;
	double goalVel;
	if (dGoal == FLT_MAX){	//	未設定なら計算
		goalVel = (goal - lastGoal) / s->GetTimeStep();
	}else{					//	設定されていればそれを使用
		goalVel = dGoal;
		dGoal = FLT_MAX;
	}
	if (type==0){
		ed = goalVel - joint->GetJointVelocity(axis);
	}else{
		ed = goalVel - joint->GetJointAccel(axis);
	}
	integratedError += float(e * s->GetTimeStep());
	pTorque = float(proportional*e);
	iTorque = float(integral*integratedError);
	dTorque = float(differential*ed);
	double tq = pTorque + iTorque + dTorque;
	joint->AddJointTorque(tq, axis);
	lastGoal = goal;
}


class PHJointPidState:public SGBehaviorState{
public:
	SGOBJECTDEF(PHJointPidState);
	float integratedError;
	float lastGoal;
	float goal;
	float pTorque, iTorque, dTorque;
};
SGOBJECTIMP(PHJointPidState, SGBehaviorState);

void PHJointPid::LoadState(const SGBehaviorStates& states){
	PHJointPidState* pState = DCAST(PHJointPidState, states.GetNext());
	lastGoal = pState->lastGoal;
	integratedError = pState->integratedError;
	goal = pState->goal;
	pTorque = pState->pTorque;
	iTorque = pState->iTorque;
	dTorque = pState->dTorque;
}
void PHJointPid::SaveState(SGBehaviorStates& states) const{
	UTRef<PHJointPidState> state = new PHJointPidState;
	states.push_back(state);
	state->lastGoal = lastGoal;
	state->integratedError = integratedError;
	state->goal = goal;	
	state->pTorque = pTorque;
	state->iTorque = iTorque;
	state->dTorque = dTorque;
}

typedef float FLOAT;
DEF_RECORD(XJointPid,{
	GUID Guid(){ return WBGuid("395862A3-064C-445c-A969-72488CDD6A78"); }
	DWORD axis;		//	軸の番号
	DWORD type;		//	0:位置制御， 1:速度制御
	FLOAT prop;		//	PID の P 比例係数
	FLOAT inte;		//	PID の I 積分係数
	FLOAT diff;		//	PID の D 微分係数
	FLOAT goal;		//	PIDの目標値
});


class PHJointPidLoader : public FIObjectLoader<PHJointPid>
{
public:
	PHJointPidLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(FLOAT);
		db->REG_FIELD(DWORD);
		db->REG_RECORD_PROTO(XJointPid);
	}
	bool LoadData(FILoadScene* ctx, PHJointPid* pid){		
		XJointPid x;
		ctx->docs.Top()->GetWholeData(x);
		pid->axis = x.axis;
		pid->type = x.type;
		pid->proportional = x.prop;
		pid->integral = x.inte;
		pid->differential = x.diff;
		pid->goal = x.goal;
		return true;
	}
};

class PHJointPidSaver : public FIObjectSaver<PHJointPid>
{
protected:
	void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, PHJointPid* pid){
		XJointPid x;
		ctx->docs.Top()->GetWholeData(x);
		x.axis = pid->axis;
		x.type = pid->type;
		x.prop = pid->proportional;
		x.inte = pid->integral;
		x.diff =  pid->differential;;
		x.goal = pid->goal;
		doc->SetWholeData(x);
	}
};
DEF_REGISTER_BOTH(PHJointPid);

PHJointPid* PHJointPid::Find(PHJoint1D* j, SGScene* scene){
	if (!j || !scene) return NULL;
	for(SGBehaviorEngines::const_iterator itb = scene->GetBehaviors().begin(); itb != scene->GetBehaviors().end(); ++itb){
		PHJointPid* pid = DCAST(PHJointPid, (*itb));
		if (pid && pid->joint == j){
			return pid;
		}
	}
	return NULL;
}



//-----------------------------------------------------------------------------

SGOBJECTIMP(PHJointBallPid, SGBehaviorEngine);

void PHJointBallPid::Print(std::ostream& os) const {
	int w = os.width();
	os.width(0);
	os << UTPadding(w);
	os << "<" << GetTypeInfo()->ClassName() << " " << GetName();
	if (joint) os << "j=" << joint->GetName();
	os << ">" << std::endl;
	os.width(w);
}

size_t PHJointBallPid::NReferenceObjects(){
	return joint ? 1 : 0;
}
SGObject* PHJointBallPid::ReferenceObject(size_t i){
	if (i == 0) return joint;
	return NULL;
}
bool PHJointBallPid::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHJointBall, o)){
		joint = (PHJointBall*)o;
		return true;
	}
	return false;
}
bool PHJointBallPid::DelChildObject(SGObject* o, SGScene* s){
	if (joint == o){
		joint = NULL;
		return true;
	}
	return false;
}
void PHJointBallPid::Step(SGScene* s){
	Quaternionf now = joint->position;
	Quaternionf qtE = now.inv() * goal;
	Vec3f e = qtE.rotation_half();
	Vec3f goalVel;
	if (dGoal.X() == FLT_MAX){
		goalVel = (lastGoal.inv() * goal).rotation() / s->GetTimeStep();
	}else{
		goalVel = dGoal;
		dGoal.X() = FLT_MAX;
	}
	Vec3f ed = goalVel - joint->velocity;
	integratedError += e * (float)s->GetTimeStep();
	pTorque = proportional*e;
	iTorque = integral*integratedError;
	dTorque = differential*ed;
	Vec3f tq = pTorque + iTorque + dTorque;
	joint->AddTorque(tq);
	lastGoal = goal;
}

class PHJointBallPidState:public SGBehaviorState{
public:
	SGOBJECTDEF(PHJointBallPidState);
	Vec3f integratedError;
	Quaternionf goal;
	Quaternionf lastGoal;
	Vec3f pTorque, iTorque, dTorque;
};
SGOBJECTIMP(PHJointBallPidState, SGBehaviorState);

void PHJointBallPid::LoadState(const SGBehaviorStates& states){
	PHJointBallPidState* pState = DCAST(PHJointBallPidState, states.GetNext());
	lastGoal = pState->lastGoal;
	integratedError = pState->integratedError;
	goal = pState->goal;
	pTorque = pState->pTorque;
	iTorque = pState->iTorque;
	dTorque = pState->dTorque;
}
void PHJointBallPid::SaveState(SGBehaviorStates& states) const{
	UTRef<PHJointBallPidState> state = new PHJointBallPidState;
	states.push_back(state);
	state->lastGoal = lastGoal;
	state->integratedError = integratedError;
	state->goal = goal;	
	state->pTorque = pTorque;
	state->iTorque = iTorque;
	state->dTorque = dTorque;
}

typedef float FLOAT;
typedef Quaternionf Quaternion;
DEF_RECORD(XJointBallPid,{
	GUID Guid(){ return WBGuid("C5C8B104-45AC-40ec-9C85-FF0007714BE5"); }
	FLOAT prop;			//	PID の P 比例係数
	FLOAT inte;			//	PID の I 積分係数
	FLOAT diff;			//	PID の D 微分係数
	Quaternion goal;	//	PIDの目標値
});

class PHJointBallPidLoader : public FIObjectLoader<PHJointBallPid>
{
public:
	PHJointBallPidLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(FLOAT);
		db->REG_FIELD(Quaternion);
		db->REG_RECORD_PROTO(XJointBallPid);
	}
	bool LoadData(FILoadScene* ctx, PHJointBallPid* pid){		
		XJointBallPid x;
		ctx->docs.Top()->GetWholeData(x);
		pid->proportional = x.prop;
		pid->integral = x.inte;
		pid->differential = x.diff;
		pid->goal = x.goal;
		return true;
	}
};

class PHJointBallPidSaver : public FIObjectSaver<PHJointBallPid>
{
protected:
	void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, PHJointBallPid* pid){
		XJointBallPid x;
		ctx->docs.Top()->GetWholeData(x);
		x.prop = pid->proportional;
		x.inte = pid->integral;
		x.diff =  pid->differential;;
		x.goal = pid->goal;
		doc->SetWholeData(x);
	}
};
DEF_REGISTER_BOTH(PHJointBallPid);

PHJointBallPid* PHJointBallPid::Find(PHJointBall* j, SGScene* scene){
	if (!j || !scene) return NULL;
	for(SGBehaviorEngines::const_iterator itb = scene->GetBehaviors().begin(); itb != scene->GetBehaviors().end(); ++itb){
		PHJointBallPid* pid = DCAST(PHJointBallPid, (*itb));
		if (pid && pid->joint == j){
			return pid;
		}
	}
	return NULL;
}

}

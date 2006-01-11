#include "Physics.h"
#pragma hdrstop
#include <float.h>

using namespace PTM;
namespace Spr{

///////////////////////////////////////////////////////////////////
//	PHSolid
OBJECTIMP(PHSolid, Object);

PHSolid::PHSolid(const PHSolidDesc& desc):PHSolidDesc(desc){
	integrationMode = PHINT_SIMPLETIC;
}

void PHSolid::CalcBBox(){
	Vec3f bboxMin = Vec3f(FLT_MAX, FLT_MAX, FLT_MAX);
	Vec3f bboxMax = Vec3f(-FLT_MAX,-FLT_MAX,-FLT_MAX);
	for(CDShapes::iterator it=shapes.begin(); it != shapes.end(); ++it){
		(*it)->CalcBBox(bboxMin, bboxMax);
	}
	if (bboxMin.X() == FLT_MAX){
		bbox.SetBBoxMinMax(Vec3f(0,0,0), Vec3f(-1,-1,-1));
	}else{
		bbox.SetBBoxMinMax(bboxMin, bboxMax);
	}
//	DSTR << GetName() << bbox.GetBBoxMin() << bbox.GetBBoxMax() << std::endl;
}
void PHSolid::GetBBoxSupport(const Vec3f& dir, float& minS, float& maxS){
		bbox.GetSupport(GetOrientation().Inv()*dir, minS, maxS);
		float c = Vec3f(GetFramePosition()) * dir;
		minS += c;
		maxS += c;
}

void PHSolid::Step(){
	force = nextForce;
	torque = nextTorque;
	nextForce.clear();
	nextTorque.clear();

	PHScene* s = OCAST(PHScene, GetScene());
	double dt = s->GetTimeStep();
	assert(GetIntegrationMode() != PHINT_NONE);
#ifdef _DEBUG
	if (!_finite(velocity.norm()) || velocity.norm() > 100 || angVelocity.norm() > 100){	
		DSTR << "Warning: solid '" << GetName() << "' has a very fast velocity. v:" << velocity << "w:" << angVelocity << std::endl;
	}
#endif
	//k1 = f(y);
	//k2 = f(y + k1 * h / 2);
	//k3 = f(y + k2 * h / 2);
	//k4 = f(y + k3);
	//y += (k1 + 2 * k2 + 2 * k3 + k4) * h / 6;
	//	積分計算
	Vec3d dv, dw;
	switch(GetIntegrationMode()){
	case PHINT_EULER:
		//平行移動量の積分
		SetCenterPosition(GetCenterPosition() + velocity * dt);
		velocity += force * (dt / mass);
		//角速度からクウォータニオンの時間微分を求め、積分、正規化
		pose.ori += pose.ori.Derivative(angVelocity) * dt;
		pose.ori.unitize();
		torque		= pose.ori.Conjugated() * torque;			//トルクと角速度をローカル座標へ
		angVelocity = pose.ori.Conjugated() * angVelocity;
		angVelocity += Euler(inertia, torque, angVelocity) * dt;	//オイラーの運動方程式
		torque = pose.ori * torque;						//トルクと角速度をワールドへ
		angVelocity = pose.ori * angVelocity;
		break;
	case PHINT_ARISTOTELIAN:{
		SetCenterPosition(GetCenterPosition() + velocity * dt);
		velocity = force / mass;		//速度は力に比例する
		Vec3d tq = pose.ori.Conjugated() * torque;	//トルクをローカルへ
		angVelocity = pose.ori * (inertia_inv * tq);	//角速度はトルクに比例する
		//クウォータニオンを積分、正規化
		pose.ori += pose.ori.Derivative(angVelocity) * dt;
		pose.ori.unitize();
		}break;
	case PHINT_SIMPLETIC:{
		//	x(dt) = x(0) + dt*v(0)/m
		//	v(dt) = v(0) + dt*f(dt)
		//回転量の積分
		torque		= pose.ori.Conjugated() * torque;				//	トルクと角速度をローカル座標へ
		angVelocity = pose.ori.Conjugated() * angVelocity;

		dw = Euler(inertia, torque, angVelocity) * dt;			//角速度変化量
		angVelocity += dw;										//角速度の積分
		Quaterniond dq = Quaterniond::Rot(angVelocity * dt);
		Vec3d dp = pose.ori * (dq*(-center) - (-center));
		pose.ori = pose.ori * dq;
		pose.ori.unitize();
		torque = pose.ori * torque;									//トルクと角速度をワールドへ
		angVelocity = pose.ori * angVelocity;
		//平行移動量の積分
		velocity += force * (dt / mass);								//	速度の積分
		SetCenterPosition(GetCenterPosition() + velocity * dt + dp);	//	位置の積分
		}break;
	case PHINT_ANALYTIC:{
		//回転量の積分
		//回転は解析的に積分できないので、形式的に↑の公式を回転の場合に当てはめる
		torque		= pose.ori.Conjugated() * torque;					//トルクと角速度をローカル座標へ
		angVelocity = pose.ori.Conjugated() * angVelocity;
		dw = Euler(inertia, torque, angVelocity) * dt;			//角速度変化量

		Quaterniond dq = Quaterniond::Rot((angVelocity+0.5*dw) * dt);
		Vec3d dp = pose.ori * (dq*(-center) - (-center));
		pose.ori = pose.ori * dq;
		pose.ori.unitize();

		angVelocity += dw;										//角速度の積分
		torque = pose.ori * torque;									//トルクと角速度をワールドへ
		angVelocity = pose.ori * angVelocity;
		//平行移動量の積分（解析解に一致）
		dv = force * (dt / mass);									//速度変化量
		SetCenterPosition(GetCenterPosition() + (velocity+0.5*dv) * dt + dp);	//	位置の積分
		velocity += dv;												//速度の積分
		}break;
	case PHINT_RUNGEKUTTA2:
		//平行移動量の積分(まじめにルンゲクッタ公式に従っても結果は同じ＝解析解)
		dv = force * (dt / mass);
		SetCenterPosition(GetCenterPosition() + (velocity + 0.5 * dv) * dt);
		velocity += dv;
		//回転量の計算
		//回転は解析的に積分できないので、ルンゲクッタ公式を使う
		torque		= pose.ori.Conjugated() * torque;					//トルクと角速度をローカル座標へ
		angVelocity = pose.ori.Conjugated() * angVelocity;
		_angvel[0]	= angVelocity;
		_angacc[0]	= Euler(inertia, torque, _angvel[0]);
		_angvel[1]	= _angvel[0] + _angacc[0] * dt;
		_angacc[1]	= Euler(inertia, torque, _angvel[1]);
		pose.ori += pose.ori.Derivative(pose.ori * (_angvel[0] + _angvel[1]) / 2.0) * dt;
		pose.ori.unitize();
		angVelocity = pose.ori * (angVelocity + ((_angacc[0] + _angacc[1]) * (dt / 2.0)));
		torque = pose.ori * torque;
		break;
	case PHINT_RUNGEKUTTA4:
		//平行移動量の積分(まじめにルンゲクッタ公式に従っても結果は同じ＝解析解)
		dv = force * (dt / mass);
		SetCenterPosition(GetCenterPosition() + (velocity + 0.5 * dv) * dt);
		velocity += dv;
		//回転量の計算
		_angvel[0]	= angVelocity;
		_angacc[0]	= Euler(inertia, torque, _angvel[0]);
		_angvel[1]	= _angvel[0] + _angacc[0] * (dt / 2.0);
		_angacc[1]	= Euler(inertia, torque, _angvel[1]);
		_angvel[2]	= _angvel[0] + _angacc[1] * (dt / 2.0); 
		_angacc[2]	= Euler(inertia, torque, _angvel[2]);
		_angvel[3]	= _angvel[0] + _angacc[2] * dt;
		_angacc[3]	= Euler(inertia, torque, _angvel[3]);
		pose.ori += pose.ori.Derivative(pose.ori * (_angvel[0] + 2.0 * (_angvel[1] + _angvel[2]) + _angvel[3]) / 6.0) * dt;
		pose.ori.unitize();
		angVelocity = pose.ori * (angVelocity + (_angacc[0] + 2.0 * (_angacc[1] + _angacc[2]) + _angacc[3]) * (dt / 6.0));
		torque = pose.ori * torque;
		
		break;
	}
}
void PHSolid::AddTorque(Vec3d t){
	nextTorque += t; 
}
void PHSolid::AddForce(Vec3d f){
	nextForce += f;
}

void PHSolid::AddForce(Vec3d f, Vec3d r){
	nextTorque += (r - pose*center) ^ f;
	nextForce += f;
}

/*void PHSolid::AddForceLocal(Vec3d f, Vec3d r){
	torque += cross(pose.ori * (r - center), f);
	force += f;
}*/


void PHSolid::AddShape(CDShapeIf* shape){
	shapes.push_back(OCAST(CDShape,shape));
	CalcBBox();
	PHPenaltyEngine* pe;
	((PHScene*)GetScene())->engines.Find(pe);
	pe->Init();
}

void PHSolid::SetGravity(bool bOn){
	PHScene* ps = (PHScene*)GetScene();
	PHGravityEngine* ge;
	ps->engines.Find(ge);
	if (bOn == true){
		if (ge->solids.Find(this)){
			ge->solids.push_back(this);
		}
	}else{
		ge->solids.Erase(this);
	}
}
int PHSolid::GetNShapes(){
	return shapes.size();
}
CDShapeIf** PHSolid::GetShapes(){
	return (CDShapeIf**)(void*)&shapes.front();
}

//----------------------------------------------------------------------------
//	PHSolidContainer
//
OBJECTIMP(PHSolidContainer, PHEngine);
bool PHSolidContainer::AddChildObject(Object* o, PHScene* s){
	if (DCAST(PHSolid, o)){
		solids.push_back((PHSolid*)o);
		return true;
	}
	return false;
}
bool PHSolidContainer::DelChildObject(Object* o, PHScene* s){
	PHSolid* so = DCAST(PHSolid, o);
	if (so){
		solids.Erase(so);
		return true;
	}
	return false;
}

void  PHSolidContainer::Step(){
	for(PHSolids::iterator it = solids.begin(); it != solids.end(); ++it){
		(*it)->Step();
	}
}


}

#include <Springhead.h>
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

void PHSolid::Step(){
	force = nextForce;
	torque = nextTorque;
	nextForce.clear();
	nextTorque.clear();

	PHScene* s = ACAST(PHScene, scene);
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
		orientation += orientation.derivative(angVelocity) * dt;
		orientation.unitize();
		torque		= orientation.conjugated() * torque;			//トルクと角速度をローカル座標へ
		angVelocity = orientation.conjugated() * angVelocity;
		angVelocity += Euler(inertia, torque, angVelocity) * dt;	//オイラーの運動方程式
		torque = orientation * torque;						//トルクと角速度をワールドへ
		angVelocity = orientation * angVelocity;
		break;
	case PHINT_ARISTOTELIAN:{
		SetCenterPosition(GetCenterPosition() + velocity * dt);
		velocity = force / mass;		//速度は力に比例する
		Vec3d tq = orientation.conjugated() * torque;	//トルクをローカルへ
		angVelocity = orientation * (inertia_inv * tq);	//角速度はトルクに比例する
		//クウォータニオンを積分、正規化
		orientation += orientation.derivative(angVelocity) * dt;
		orientation.unitize();
		}break;
	case PHINT_SIMPLETIC:{
		//	x(dt) = x(0) + dt*v(0)/m
		//	v(dt) = v(0) + dt*f(dt)
		//回転量の積分
		torque		= orientation.conjugated() * torque;				//	トルクと角速度をローカル座標へ
		angVelocity = orientation.conjugated() * angVelocity;

		dw = Euler(inertia, torque, angVelocity) * dt;			//角速度変化量
		angVelocity += dw;
		Quaterniond dq = Quaterniond::Rot(angVelocity * dt);
		Vec3d dp = orientation * (dq*(-center) - (-center));
		orientation = orientation * dq;
		orientation.unitize();
		angVelocity += dw;										//角速度の積分
		torque = orientation * torque;									//トルクと角速度をワールドへ
		angVelocity = orientation * angVelocity;
		//平行移動量の積分
		velocity += force * (dt / mass);								//	速度の積分
		SetCenterPosition(GetCenterPosition() + velocity * dt + dp);	//	位置の積分
		}break;
	case PHINT_ANALYTIC:{
		//回転量の積分
		//回転は解析的に積分できないので、形式的に↑の公式を回転の場合に当てはめる
		torque		= orientation.conjugated() * torque;					//トルクと角速度をローカル座標へ
		angVelocity = orientation.conjugated() * angVelocity;
		dw = Euler(inertia, torque, angVelocity) * dt;			//角速度変化量

		Quaterniond dq = Quaterniond::Rot((angVelocity+0.5*dw) * dt);
		Vec3d dp = orientation * (dq*(-center) - (-center));
		orientation = orientation * dq;
		orientation.unitize();

		angVelocity += dw;										//角速度の積分
		torque = orientation * torque;									//トルクと角速度をワールドへ
		angVelocity = orientation * angVelocity;
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
		torque		= orientation.conjugated() * torque;					//トルクと角速度をローカル座標へ
		angVelocity = orientation.conjugated() * angVelocity;
		_angvel[0]	= angVelocity;
		_angacc[0]	= Euler(inertia, torque, _angvel[0]);
		_angvel[1]	= _angvel[0] + _angacc[0] * dt;
		_angacc[1]	= Euler(inertia, torque, _angvel[1]);
		orientation += orientation.derivative(orientation * (_angvel[0] + _angvel[1]) / 2.0) * dt;
		orientation.unitize();
		angVelocity = orientation * (angVelocity + ((_angacc[0] + _angacc[1]) * (dt / 2.0)));
		torque = orientation * torque;
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
		orientation += orientation.derivative(orientation * (_angvel[0] + 2.0 * (_angvel[1] + _angvel[2]) + _angvel[3]) / 6.0) * dt;
		orientation.unitize();
		angVelocity = orientation * (angVelocity + (_angacc[0] + 2.0 * (_angacc[1] + _angacc[2]) + _angacc[3]) * (dt / 6.0));
		torque = orientation * torque;
		
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
	nextTorque += (r - (orientation*center+position)) ^ f;
	nextForce += f;
}

/*void PHSolid::AddForceLocal(Vec3d f, Vec3d r){
	torque += cross(orientation * (r - center), f);
	force += f;
}*/

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

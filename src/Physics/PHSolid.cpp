#include "Physics.h"
#pragma hdrstop
#include <float.h>

#define _DEBUG

using namespace PTM;
namespace Spr{

///////////////////////////////////////////////////////////////////
//	PHSolid
OBJECTIMP(PHSolid, Object);
IF_IMP(PHSolid, Object);

PHSolid::PHSolid(const PHSolidDesc& desc):PHSolidDesc(desc){
	integrationMode = PHINT_SIMPLETIC;
	inertia_inv = inertia.inv();
}
CDShapeIf* PHSolid::CreateShape(const CDShapeDesc& desc){
	CDShapeIf* rv = ICAST(PHSceneIf, GetScene())->CreateShape(desc);
	AddShape(rv);
	return rv;
}
ObjectIf* PHSolid::CreateObject(const IfInfo* info, const void* desc){
	if (CDConvexMeshIf::GetIfInfoStatic() == info){
		return CreateShape(*(CDConvexMeshDesc*)desc);
	}
	return NULL;
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

	//既に他のエンジンによって更新が成された場合は積分を行わない
	if(IsUpdated())
		return;

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
	Vec3d dv, dw;				//<	速度・角速度の変化量
	Vec3d	_angvel[4];			//<	数値積分係数
	Vec3d	_angacc[4];
	switch(GetIntegrationMode()){
	case PHINT_EULER:
		//平行移動量の積分
		SetCenterPosition(GetCenterPosition() + velocity * dt);
		velocity += force * (dt / mass);
		//角速度からクウォータニオンの時間微分を求め、積分、正規化
		pose.Ori() += pose.Ori().Derivative(angVelocity) * dt;
		pose.Ori().unitize();
		torque		= pose.Ori().Conjugated() * torque;			//トルクと角速度をローカル座標へ
		angVelocity = pose.Ori().Conjugated() * angVelocity;
		angVelocity += Euler(inertia, torque, angVelocity) * dt;	//オイラーの運動方程式
		torque = pose.Ori() * torque;						//トルクと角速度をワールドへ
		angVelocity = pose.Ori() * angVelocity;
		break;
	case PHINT_ARISTOTELIAN:{
		SetCenterPosition(GetCenterPosition() + velocity * dt);
		velocity = force / mass;		//速度は力に比例する
		Vec3d tq = pose.Ori().Conjugated() * torque;	//トルクをローカルへ
		angVelocity = pose.Ori() * (inertia_inv * tq);	//角速度はトルクに比例する
		//クウォータニオンを積分、正規化
		pose.Ori() += pose.Ori().Derivative(angVelocity) * dt;
		pose.Ori().unitize();
		}break;
	case PHINT_SIMPLETIC:{
		//	x(dt) = x(0) + dt*v(0)/m
		//	v(dt) = v(0) + dt*f(dt)
		//回転量の積分
		torque		= pose.Ori().Conjugated() * torque;				//	トルクと角速度をローカル座標へ
		angVelocity = pose.Ori().Conjugated() * angVelocity;

		dw = Euler(inertia, torque, angVelocity) * dt;			//角速度変化量
		angVelocity += dw;										//角速度の積分
		Quaterniond dq = Quaterniond::Rot(angVelocity * dt);
		Vec3d dp = pose.Ori() * (dq*(-center) - (-center));
		pose.Ori() = pose.Ori() * dq;
		pose.Ori().unitize();
		torque = pose.Ori() * torque;									//トルクと角速度をワールドへ
		angVelocity = pose.Ori() * angVelocity;
		//平行移動量の積分
		velocity += force * (dt / mass);								//	速度の積分
		SetCenterPosition(GetCenterPosition() + velocity * dt + dp);	//	位置の積分
		}break;
	case PHINT_ANALYTIC:{
		//回転量の積分
		//回転は解析的に積分できないので、形式的に↑の公式を回転の場合に当てはめる
		torque		= pose.Ori().Conjugated() * torque;					//トルクと角速度をローカル座標へ
		angVelocity = pose.Ori().Conjugated() * angVelocity;
		dw = Euler(inertia, torque, angVelocity) * dt;			//角速度変化量

		Quaterniond dq = Quaterniond::Rot((angVelocity+0.5*dw) * dt);
		Vec3d dp = pose.Ori() * (dq*(-center) - (-center));
		pose.Ori() = pose.Ori() * dq;
		pose.Ori().unitize();

		angVelocity += dw;										//角速度の積分
		torque = pose.Ori() * torque;									//トルクと角速度をワールドへ
		angVelocity = pose.Ori() * angVelocity;
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
		torque		= pose.Ori().Conjugated() * torque;					//トルクと角速度をローカル座標へ
		angVelocity = pose.Ori().Conjugated() * angVelocity;
		_angvel[0]	= angVelocity;
		_angacc[0]	= Euler(inertia, torque, _angvel[0]);
		_angvel[1]	= _angvel[0] + _angacc[0] * dt;
		_angacc[1]	= Euler(inertia, torque, _angvel[1]);
		pose.Ori() += pose.Ori().Derivative(pose.Ori() * (_angvel[0] + _angvel[1]) / 2.0) * dt;
		pose.Ori().unitize();
		angVelocity = pose.Ori() * (angVelocity + ((_angacc[0] + _angacc[1]) * (dt / 2.0)));
		torque = pose.Ori() * torque;
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
		pose.Ori() += pose.Ori().Derivative(pose.Ori() * (_angvel[0] + 2.0 * (_angvel[1] + _angvel[2]) + _angvel[3]) / 6.0) * dt;
		pose.Ori().unitize();
		angVelocity = pose.Ori() * (angVelocity + (_angacc[0] + 2.0 * (_angacc[1] + _angacc[2]) + _angacc[3]) * (dt / 6.0));
		torque = pose.Ori() * torque;
		
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
	torque += cross(pose.Ori() * (r - center), f);
	force += f;
}*/


void PHSolid::AddShape(CDShapeIf* shape){
	shapes.push_back(OCAST(CDShape,shape));
	CalcBBox();
	//接触エンジンをinvalidate
	PHScene* scene = OCAST(PHScene,GetScene());
	assert(scene);
	switch(scene->contactSolver){
	case PHScene::SOLVER_PENALTY:{
		PHPenaltyEngine* pe;
		scene->engines.Find(pe);
		if(pe) pe->Invalidate();
	}break;
	case PHScene::SOLVER_CONSTRAINT:{
		PHConstraintEngine* ce;
		scene->engines.Find(ce);
		if(ce) ce->Invalidate();
	}break;
	}
}

void PHSolid::SetGravity(bool bOn){
	PHScene* ps = OCAST(PHScene,GetScene());
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
int PHSolid::NShape(){
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
	for(PHSolids::iterator it = solids.begin(); it != solids.end(); ++it)
		(*it)->Step();
}

//----------------------------------------------------------------------------
//	PHSolidInitializer
//
OBJECTIMP(PHSolidInitializer, PHEngine);
bool PHSolidInitializer::AddChildObject(Object* o, PHScene* s){
	if (DCAST(PHSolid, o)){
		solids.push_back((PHSolid*)o);
		return true;
	}
	return false;
}
bool PHSolidInitializer::DelChildObject(Object* o, PHScene* s){
	PHSolid* so = DCAST(PHSolid, o);
	if (so){
		solids.Erase(so);
		return true;
	}
	return false;
}
void PHSolidInitializer::Step(){
	for(PHSolids::iterator it = solids.begin(); it != solids.end(); it++)
		(*it)->SetUpdated(false);
}	

}

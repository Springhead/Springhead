/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHSolid.h>
#include <Physics/PHScene.h>
#include <Physics/PHTreeNode.h>
#include <Physics/PHForceField.h>
#include <Physics/PHPenaltyEngine.h>
#include <Physics/PHConstraintEngine.h>
#include <Physics/PHHapticEngine.h>
#include <Collision/CDShape.h>

#include <float.h>

using namespace PTM;
namespace Spr{;


///////////////////////////////////////////////////////////////////
//	PHSolid

PHSolid::PHSolid(const PHSolidDesc& desc, SceneIf* s):PHSolidDesc(desc),
	integrationMode(PHINT_SIMPLETIC),
	treeNode(NULL),
	engine(NULL){
	minv = 1.0;
	Iinv = GetInertiaInv();
	Minv.clear();
	bFrozen         = false;
	bboxReady       = false;
	aabbReady       = false;
	velocityNorm = 0;
	angVelocityNorm = 0;
	femIndex = 0;
	if (s){ SetScene(s); }
}

SceneObjectIf* PHSolid::CloneObject(){
	PHSolidDesc desc;
	PHSolidIf* origin = DCAST(PHSolidIf,this);
	origin ->GetDesc(&desc);
	PHSceneIf* s = DCAST(PHSceneIf, GetScene());
	PHSolidIf* clone = s->CreateSolid(desc);
	for (unsigned int i=0; i < origin->NChildObject(); ++i) {
		clone->AddChildObject(origin->GetChildObject(i));
	}
	return clone;
}
void PHSolid::SetGravity(bool bOn){
	PHScene* s = DCAST(PHScene, GetScene());
	PHGravityEngine* ge;
	s->engines.Find(ge);
	if(bOn)
		 ge->AddChildObject(Cast());
	else ge->DelChildObject(Cast());
}

CDShapeIf* PHSolid::CreateAndAddShape(const IfInfo*  info, const CDShapeDesc& desc){
	CDShapeIf* rv = DCAST(PHScene, GetScene())->CreateShape(info, desc);
	if (rv){
		AddShape(rv);
	}
	return rv;
}
ObjectIf* PHSolid::CreateObject(const IfInfo* info, const void* desc){
	ObjectIf* rv = SceneObject::CreateObject(info, desc);
	if (!rv){
		if (info->Inherit(CDShapeIf::GetIfInfoStatic())){
			rv = DCAST(PHScene, GetScene())->CreateShape(info, *(Spr::CDShapeDesc*)desc);
		}
	}
	return rv;
}
bool PHSolid::AddChildObject(ObjectIf* obj){
	if (DCAST(PHFrameIf, obj)){
		AddFrame(DCAST(PHFrameIf, obj));
		return true;
	}
	if (DCAST(CDShapeIf, obj)){
		AddShape(DCAST(CDShapeIf, obj));
		return true;
	}
	return false;
}
bool PHSolid::DelChildObject(ObjectIf* obj){
	if (DCAST(PHFrameIf, obj)){
		PHFrameIf* f = obj->Cast();
		for(int i=0; i<NFrame(); ++i){
			if (GetFrame(i) == f)
				RemoveShape(i);
			i--;
		}
		return true;
	}
	if (DCAST(CDShapeIf, obj)){
		RemoveShape(DCAST(CDShapeIf, obj));
		return true;
	}
	return false;
}
/*SpatialVector PHSolid::GetAcceleration() const {
	return dv / ((PHScene*)GetScene())->GetTimeStep(); 
}*/
Vec3d PHSolid::GetDeltaPosition() {
	PHScene* s = DCAST(PHScene, nameManager);
	return velocity * s->GetTimeStep();
}
Vec3d PHSolid::GetDeltaPosition(const Vec3d& p) {
	PHScene* s = DCAST(PHScene, nameManager);
	double dt = s->GetTimeStep();
	Quaterniond rot = Quaterniond::Rot(angVelocity*dt);
	return velocity*dt + (rot*(p-center) - p);
}

bool PHSolid::CalcBBox(){
	if(bboxReady) return false;

	bool changed = false;
	for(int i = 0; i < (int)frames.size(); i++)
		changed |= frames[i]->CalcBBox();

	if(!changed)
		return false;

	float inf = std::numeric_limits<float>::max();
	Vec3f bbmin = Vec3f( inf,  inf,  inf);
	Vec3f bbmax = Vec3f(-inf, -inf, -inf);
	
	for(int i = 0; i < (int)frames.size(); i++){
		frames[i]->CalcBBox();
		bbmin.element_min(frames[i]->bbBody.GetBBoxMin());
		bbmax.element_max(frames[i]->bbBody.GetBBoxMax());
	}

	if(bbmin.x == inf)
		 bbLocal.SetBBoxMinMax(Vec3f(0,0,0), Vec3f(-1,-1,-1));
	else bbLocal.SetBBoxMinMax(bbmin, bbmax);

	bboxReady = true;
	return true;
}
void PHSolid::CalcAABB(){
	//if(aabbReady)
	//	return;

	bool changed = CalcBBox();
	if(!changed && aabbReady)
		return;

	float inf = std::numeric_limits<float>::max();
	Vec3f bbmin = Vec3f( inf,  inf,  inf);
	Vec3f bbmax = Vec3f(-inf, -inf, -inf);
	
	for(int i = 0; i < (int)frames.size(); i++){
		frames[i]->CalcAABB();
		bbmin.element_min(frames[i]->bbWorld[0].GetBBoxMin());
		bbmax.element_max(frames[i]->bbWorld[0].GetBBoxMax());
	}

	if(bbmin.x == inf)
		 bbWorld.SetBBoxMinMax(Vec3f(0,0,0), Vec3f(-1,-1,-1));
	else bbWorld.SetBBoxMinMax(bbmin, bbmax);

	//poseChanged = false;
	aabbReady = true;
}
void PHSolid::GetBBox(Vec3d& bbmin, Vec3d& bbmax, bool world){
	if(world){
		CalcAABB();
		bbmin = bbWorld.GetBBoxMin();
		bbmax = bbWorld.GetBBoxMax();
	}
	else{
		CalcBBox();
		bbmin = bbLocal.GetBBoxMin();
		bbmax = bbLocal.GetBBoxMax();
	}
}
void PHSolid::GetBBoxSupport(const Vec3f& dir, float& minS, float& maxS){
	CalcBBox();
	bbLocal.GetSupport(GetOrientation().Inv()*dir, minS, maxS);
	float c = Vec3f(GetFramePosition()) * dir;
	minS += c;
	maxS += c;
}

bool PHSolid::IsArticulated(){
	return treeNode && treeNode->IsEnabled();
}

void PHSolid::UpdateCacheLCP(double dt){
	if(mass != 0)
		minv = GetMassInv();
	Iinv = GetInertiaInv();
	Minv.clear();
	Minv.vv() = Matrix3d::Unit() * minv;
	Minv.ww() = Iinv;

	PHSceneIf* scene = GetScene()->Cast();

	// 力・モーメントの制限
	double fmax = scene->GetMaxForce ();
	double tmax = scene->GetMaxMoment();
	double fn = nextForce .norm();
	double tn = nextTorque.norm();
	if(fn > fmax){
		DSTR << "solid external force exceeded limit: " << fn << std::endl;
		nextForce *= (fmax/fn);
	}
	if(tn > tmax){
		DSTR << "solid external moment exceeded limit: " << tn << std::endl;
		nextTorque *= (tmax/tn);
	}

	Quaterniond qc = GetOrientation().Conjugated();
	f.v() = qc * nextForce  ;
	f.w() = qc * nextTorque ;
	v.v() = qc * velocity   ;
	v.w() = qc * angVelocity;

	// ツリーに属する場合はPHRootNode::SetupDynamicsでdvが計算される
	if(IsArticulated())return;
	
	if(IsDynamical() && !IsStationary() && !IsFrozen()){
		dv0.v() = minv * f.v() * dt;
		dv0.w() = Iinv * (f.w() - v.w() % (GetInertia() * v.w())) * dt;
	}
	else{
		dv0.clear();
	}
	dv.clear();
	dV.clear();
}
/*
void PHSolid::UpdateCachePenalty(int c){
	if ((unsigned)c == count) return;
	count = c;
	vel = GetVelocity();
	angVel = GetAngularVelocity();
	lastPos = pos;
	pos = GetFramePosition();
	lastOri = ori;
	ori = GetOrientation();
	cog = ori * GetCenterOfMass() + pos;
}
*/

void PHSolid::SetVelocity(const Vec3d& v){
	velocity     = v;
	velocityNorm = v.norm();
	SetFrozen(false);

	PHSceneIf* scene = GetScene()->Cast();
	if (scene){
		double vmax = scene->GetMaxVelocity();

		if(velocityNorm > vmax){
			DSTR << "solid " << GetName() << " velocity exceeded limit: " << velocityNorm << std::endl;
			velocity *= (vmax/velocityNorm);
			velocityNorm = vmax;
		}
	}
}

void PHSolid::SetAngularVelocity(const Vec3d& av){
	angVelocity     = av;
	angVelocityNorm = av.norm();
	SetFrozen(false);

	PHSceneIf* scene = GetScene()->Cast();
	if (scene){
		double wmax = scene->GetMaxAngularVelocity();

		if(angVelocityNorm > wmax){
			DSTR << "solid " << GetName() << " ang.velocity exceeded limit: " << angVelocityNorm << std::endl;
			angVelocity *= (wmax/angVelocityNorm);
			angVelocityNorm = wmax;
		}
	}
}

void PHSolid::UpdateVelocity(double* dt){
	if(!IsDynamical())
		return;
	if(IsStationary())
		return;
	if(IsFrozen())
		return;
	if(IsUpdated())
		return;
		
	SpatialVector vold = v;
	v += dv0 + dv;

	// 空気抵抗係数をかける
	PHSceneIf* scene = GetScene()->Cast();
	v.v() *= scene->GetAirResistanceRateForVelocity();
	v.w() *= scene->GetAirResistanceRateForAngularVelocity();

	// 速度更新
	SetVelocity       (GetOrientation() * v.v());
	SetAngularVelocity(GetOrientation() * v.w());

	// 位置更新のステップ幅を計算
	double dpmax = scene->GetMaxDeltaPosition();
	double dqmax = scene->GetMaxDeltaOrientation();
	if(velocityNorm * (*dt) > dpmax)
		*dt = dpmax/velocityNorm;
	if(angVelocityNorm * (*dt) > dqmax)
		*dt = dqmax/angVelocityNorm;

	// ステートの加速度を更新
	accel    = dv.v() * scene->GetTimeStepInv();
	angAccel = dv.w() * scene->GetTimeStepInv();
	
	// フリーズ判定．速度が一定以下になると，積分などの処理をやめる．
	if(!IsFrozen()){
		if(vold.square() < engine->freezeThreshold && v.square() < engine->freezeThreshold){
			SetFrozen(true);
		}
	}
}

void PHSolid::UpdatePosition(double dt){
	if(IsFrozen())
		return;
	if(IsStationary())
		return;
	if(IsUpdated())
		return;
	
	// 移動後の質量中心位置
	Vec3d pc = GetCenterPosition() + pose.Ori() * (v.v() * dt + dV.v());
	// 向きの変化
	Vec3d dq = v.w() * dt + dV.w();
	pose.Ori() = (pose.Ori() * Quaterniond::Rot(dq)).unit();
	// 質量中心位置から座標原点位置を求める
	pose.Pos() = pc - pose.Ori() * center;

	// 形状の位置と向きを更新
	Posed pose_prev;
	for(int i = 0; i < (int)frames.size(); i++){
		pose_prev = frames[i]->pose_abs;
		frames[i]->pose_abs = pose * frames[i]->pose;
		frames[i]->delta    = frames[i]->pose_abs.Pos() - pose_prev.Pos();
	}

	aabbReady = false;
	SetUpdated(true);
}

void PHSolid::Step(){
	force  = nextForce;
	torque = nextTorque;
	nextForce .clear();
	nextTorque.clear();

	// 既に他のエンジンによって更新が成された場合は積分を行わない
	if(IsUpdated   ()) return;
	// 静止物体はスキップ
	if(IsStationary()) return;

	PHScene* s = DCAST(PHScene, GetScene());
	double dt = s->GetTimeStep();
	assert(GetIntegrationMode() != PHINT_NONE);

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
		angVelocity = pose.Ori() * (Iinv * tq);	//角速度はトルクに比例する
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
	default:			
		/* DO NOTHING */		
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
	nextForce  += f;
}

void PHSolid::AfterSetDesc(){
	SetInertia(inertia);
	SceneObject::AfterSetDesc();
}

void PHSolid::CompInertia(){
	mass = 0.0;
	center.clear();
	
	for(int i = 0; i < (int)frames.size(); i++){
		frames[i]->CompInertia();
		mass += frames[i]->mass;
		center += frames[i]->mass * (frames[i]->pose * frames[i]->center);
	}
	
	if(mass < 1.0e-12)
		DSTR << "too small mass!" << std::endl;
	else
		center *= (1.0 / mass);
	
	Matrix3d R;
	Matrix3f cross;
	inertia.clear();
	for(int i = 0; i < (int)frames.size(); i++){
		frames[i]->pose.Ori().ToMatrix(R);
		cross = Matrix3f::Cross(center - frames[i]->pose * frames[i]->center);
		inertia += R * frames[i]->inertia * R.trans() - (float)frames[i]->mass * (cross*cross);
	}

	SetMass(mass);
	SetInertia(inertia);
}

Matrix3d PHSolid::GetGlobalInertia() {
	Matrix3d R, inertialGlobal;
	GetOrientation().ToMatrix(R);
	inertialGlobal = R * GetInertia() * R.trans();
	return inertialGlobal;
}

PHTreeNodeIf* PHSolid::GetTreeNode(){
	if(treeNode)
		return treeNode->Cast();
	return NULL;
}

//----------------------------------------------------------------------------
//	PHSolidContainer
//
PHSolidContainer::PHSolidContainer(){
}

bool PHSolidContainer::AddChildObject(ObjectIf* o){
	PHSolid* s = o->Cast();
	if (s && std::find(solids.begin(), solids.end(), s) == solids.end()){
		solids.push_back(s);
		return true;
	}
	return false;
}
bool PHSolidContainer::DelChildObject(ObjectIf* o){
	PHSolid* so = o->Cast();
	if (so){
		solids.erase(find(solids.begin(), solids.end(), so));
		return true;
	}
	return false;
}

void PHSolidContainer::Reset(){
	for(PHSolidRefs::iterator it = solids.begin(); it != solids.end(); ++it){
		(*it)->SetUpdated(false);
	}
}

void PHSolidContainer::Step(){
	for(PHSolidRefs::iterator it = solids.begin(); it != solids.end(); ++it){
		(*it)->Step();
	}
}

//----------------------------------------------------------------------------
//	PHSolidInitializer
//
void PHSolidInitializer::Step(){
	container->Reset();	
}

}

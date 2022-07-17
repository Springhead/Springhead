/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHIKActuator.h>
#include <Physics/PHHingeJoint.h>
#include <Physics/PHBallJoint.h>
#include <Physics/PHSpring.h>

using namespace std;
namespace Spr{;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// IKActuator

bool PHIKActuator::AddChildObject(ObjectIf* o){
	PHIKEndEffector* endeffector = o->Cast();
	if (endeffector) {
		DCAST(PHSceneIf,GetScene())->GetIKEngine()->AddChildObject(endeffector->Cast());
		this->eef = endeffector;
		endeffector->SetParentActuator(DCAST(PHIKActuatorIf, this));
		return true;
	}

	PHIKActuator* actuator = o->Cast();
	if (actuator) {
		DSTR << this->GetName() << " <= " << actuator->GetName() << std::endl;

		DCAST(PHSceneIf, GetScene())->GetIKEngine()->AddChildObject(actuator->Cast());
		// 自分の子にする
		children.push_back(actuator);
		this->bActuatorAdded = true;
		// 自分の子に自分が親であることを教える
		actuator->parent = this;
		for (int j=0; j<(int)actuator->descendant.size(); ++j) {
			// 新たな子の子孫を紹介してもらう
			descendant.push_back(actuator->descendant[j]);
			// 新たな子の子孫に自分が祖先である事を教える
			actuator->descendant[j]->ascendant.push_back(this);
			actuator->descendant[j]->bActuatorAdded = true;
			for (int i=0; i<(int)ascendant.size(); ++i) {
				// 自分の祖先に新たな子の事を伝える
				ascendant[i]->descendant.push_back(actuator->descendant[j]);
				ascendant[i]->bActuatorAdded = true;
				// 新たな子に自分の祖先の事を伝える
				actuator->descendant[j]->ascendant.push_back(ascendant[i]);
			}
		}
		return true;
	}

	return false;
}

ObjectIf* PHIKActuator::GetChildObject(size_t pos){
	if (pos==0) {
		if (eef!=NULL) {
			return eef->Cast();
		} else {
			if (children.size() > 0) {
				return children[pos]->Cast();
			} else {
				return NULL;
			}
		}
	} else {
		if (eef!=NULL) { pos--; }
		if (children.size() > pos) {
			return children[pos]->Cast();
		} else {
			return NULL;
		}
	}
}

size_t PHIKActuator::NChildObject() const{
	return( children.size() + (eef==NULL ? 0 : 1) );
}

// --- --- --- --- ---
void PHIKActuator::SetupMatrix(){
	if (this->bEnabled) {
		// --- --- --- --- ---
		// 変数の初期化

		// Ｊ（ヤコビアン）
		for (int nDesc=0; nDesc<(int)descendant.size(); ++nDesc) {
			PHIKEndEffector* childEef = descendant[nDesc]->eef; if (childEef==NULL) { continue; }
			if (this->bNDOFChanged || (childEef->bNDOFChanged && childEef->bEnabled) || this->bActuatorAdded) {
				Mj[childEef->number].resize(childEef->ndof, this->ndof);
			}
			Mj[childEef->number].clear();
		}

		// 引き戻し速度
		if (this->bNDOFChanged) {
			domega_pull.resize(this->ndof);
		}
		domega_pull.clear();

		// ω、τ
		if (this->bNDOFChanged) {
			omega.resize(this->ndof);
			omega2.resize(this->ndof);
			omega_prev.resize(ndof);
			tau.resize(ndof);
		}
		omega.clear();
		omega2.clear();
		omega_prev.clear();
		tau.clear();
	}
}

void PHIKActuator::CalcAllJacobian(){
	for (int nDesc=0; nDesc<(int)descendant.size(); ++nDesc) {
		PHIKEndEffector* childEef = descendant[nDesc]->eef;
		if (childEef==NULL || !(childEef->bEnabled)) { continue; }
		CalcJacobian(childEef);
	}
}

void PHIKActuator::PrepareSolve(){
	if (!bEnabled) { return; }

	omega.clear();
	omega_prev.clear();
	omega2.clear();
}

void PHIKActuator::ProceedSolve(){
	omega_prev  = omega;

	for (int i=0; i<ndof; ++i) {
		double delta_epsilon = 0;

		// δ
		for (int nLink=0; nLink < NLinks(); ++nLink) {
			if (Link(nLink)==this || !(Link(nLink)->IsEnabled())) { continue; }
			int act_n = Link(nLink)->number;
			if (gamma.find(act_n) != gamma.end()) {
				for (int k=0; k<Link(nLink)->ndof; ++k) {
					delta_epsilon   += ( (gamma[act_n][i][k]) * (Link(nLink)->omega[k]) );
				}
			}
		}

		// ε
		for (int k=0; k<ndof; ++k) {
			if (k!=i) {
				if (gamma.find(number) != gamma.end()) {
					delta_epsilon   += ( (gamma[number][i][k]) * (omega[k])  );
				}
			}
		}

		// ωの更新
		double invAlpha = 0;
		if (alpha[i]!=0) {
			invAlpha = (1.0 / alpha[i]);
		} else {
			invAlpha = 1e+20;
		}
		omega[i]  = invAlpha * (beta[i]  - delta_epsilon);
	}

	// 後処理
	AfterProceedSolve();
}

void PHIKActuator::FK()  {
	Posed soParentPose = (parent) ? parent->solidTempPose : joint->GetSocketSolid()->GetPose();
	Posed socketPose; joint->GetSocketPose(socketPose);
	Posed plugPose;   joint->GetPlugPose(plugPose);
	Posed jto = Posed(); jto.Ori() = jointTempOri;

	if (DCAST(PHIKSpringActuatorIf, this)) { jto = jointTempPose; }

	solidTempPose = soParentPose * socketPose * jto * plugPose.Inv();

	Vec3d soParentVel  = (parent) ? parent->solidVelocity        : joint->GetSocketSolid()->GetVelocity();
	Vec3d soParentAVel = (parent) ? parent->solidAngularVelocity : joint->GetSocketSolid()->GetAngularVelocity();

	Vec3d jointTempAngVel = Vec3d();
	PHIKBallActuator*  ba = this->Cast(); if (ba) { jointTempAngVel = ba->jointVelocity; }
	PHIKHingeActuator* ha = this->Cast(); if (ha) { jointTempAngVel = Vec3d(0,0,ha->jointVelocity); }
	jointTempAngVel = soParentPose.Ori() * socketPose.Ori() * jointTempAngVel;

	solidVelocity        = soParentVel
						 + PTM::cross(soParentAVel, soParentPose.Ori() * socketPose.Pos())
						 + PTM::cross(soParentAVel+jointTempAngVel, -(solidTempPose.Ori() * plugPose.Pos()));
	solidAngularVelocity = soParentAVel + jointTempAngVel;

	//DSTR << this->GetName() << "(" << this << ")" << std::endl;
	//DSTR << this->GetName() << "sock : " << joint->GetSocketSolid()->GetName() << "(" << joint->GetSocketSolid() << ")" << " : " << joint->GetSocketSolid()->GetPose() << std::endl;
	//DSTR << this->GetName() << "sockpose : " << socketPose << std::endl;
	//DSTR << this->GetName() << "plug : " << joint->GetPlugSolid()->GetName() << "(" << joint->GetPlugSolid() << ")" << " : " << joint->GetPlugSolid()->GetPose() << std::endl;
	//DSTR << this->GetName() << "plugpose : " << plugPose << std::endl;

	for (size_t i=0; i<children.size(); ++i) {
		children[i]->FK();
	}

	if (eef) {
		eef->solidTempPose = solidTempPose;
	}
}

// --- --- --- --- ---
bool PHIKBallActuator::AddChildObject(ObjectIf* o){
	PHBallJointIf* jo = o->Cast();
	if (jo) {
		this->joint = jo;
		return true;
	}
	return PHIKActuator::AddChildObject(o);
}

ObjectIf* PHIKBallActuator::GetChildObject(size_t pos){
	if (pos == 0 && this->joint != NULL) { return this->joint; }
	if (this->joint != NULL) {
		return PHIKActuator::GetChildObject(pos - 1);
	} else {
		return PHIKActuator::GetChildObject(pos);
	}
	return NULL;
}

size_t PHIKBallActuator::NChildObject() const{
	if (this->joint != NULL) { return 1 + PHIKActuator::NChildObject(); }
	return PHIKActuator::NChildObject();
}

void PHIKBallActuator::BeforeSetupMatrix(){
	// <!!>
	if (ndof != 3) {
		ndof = 3;
		bNDOFChanged = true;
	}
}

void PHIKBallActuator::BeforeCalcAllJacobian(){
	CalcAxis();
}

void PHIKBallActuator::CalcAxis(){
	e[0] = Vec3d(1,0,0);
	e[1] = Vec3d(0,1,0);
	e[2] = Vec3d(0,0,1);
}

void PHIKBallActuator::CalcJacobian(PHIKEndEffector* endeffector){
	int n = endeffector->number;

	// アクチュエータ回転 <=> エンドエフェクタ位置
	if (endeffector->bPosition){
		// 関節の回転中心
		PHBallJoint* j = DCAST(PHBallJoint,joint);
		Posed soParentPose = (parent) ? parent->GetSolidTempPose() : joint->GetSocketSolid()->GetPose();

		// <!!> IKActuatorがついてないjointが間に入るとd.poseSocketがずれる
		PHBallJointDesc d; j->GetDesc(&d);
		Vec3d Pj = soParentPose * d.poseSocket * Vec3d(0,0,0);

		// エンドエフェクタ位置
		Vec3d Pe = endeffector->solidTempPose * endeffector->targetLocalPosition;

		// 外積ベクトルからヤコビアンを求める
		for (int i = 0; i < ndof; ++i) {
			Vec3d v = PTM::cross(e[i],(Pe-Pj)); //Change here
			/*
			if (endeffector->GetBaseSolid() != NULL)
			{
				PHSolidIf* bSolid = endeffector->GetBaseSolid();
				DSTR << bSolid << std::endl;
				Vec3d v1 = PTM::cross(e[i], (Pe - Pj));
				Quaterniond  q = bSolid->GetPose().Ori();
				Vec3d v = q.Inv() * v1;
				Mj[n][0][i] = v[0];  Mj[n][1][i] = v[1];  Mj[n][2][i] = v[2];
			}
			else
			{
				Vec3d v = PTM::cross(e[i], (Pe - Pj));
				Mj[n][0][i] = v[0];  Mj[n][1][i] = v[1];  Mj[n][2][i] = v[2];
			}
			*/
			Mj[n][0][i] = v[0];  Mj[n][1][i] = v[1];  Mj[n][2][i] = v[2];
		}
	}

	// アクチュエータ回転 <=> エンドエフェクタ回転
	if (endeffector->bOrientation){
		int stride = (endeffector->bPosition ? 3 : 0);

		// 単位行列
		for (int i=0; i<3; ++i) {
			for (int j=0; j<ndof; ++j) {
				Mj[n][i+stride][j] = ((i==j) ? 1 : 0);
			}
		}
	}

	// 重み付け
	size_t ndof_eef=0;
	ndof_eef += (endeffector->bPosition    ? 3 : 0);
	ndof_eef += (endeffector->bOrientation ? 3 : 0);
	for (size_t i=0; i<ndof_eef; ++i) {
		for (size_t j=0; j<(size_t)ndof; ++j) {
			Mj[n][i][j] *= sqsaib;
		}
	}
}

void PHIKBallActuator::CalcPullbackVelocity() {
	Matrix3d m(e[0], e[1], e[2]);

	Vec3d pullback = Vec3d();
	
	// 標準姿勢へののPullback
	pullback += (pullbackTarget * jointTempOri.Inv()).RotationHalf() * pullbackRate;

	// <!!> トルク最小化IKの概念テスト。実際にはもう少しきめ細やかな実装が必要になりそう
	// pullback += -(DCAST(PHBallJointIf, joint)->GetMotorForce() * 0.001);

	// <!!> 本当はLimit成分を入れたほうが良い気がする

	// <!!> Pullback量が一定以下になるよう制限する．
	if (pullback.norm() > Rad(10)) {
		pullback = pullback.unit() * Rad(10);
	}

	Posed soParentPose = (parent) ? parent->GetSolidTempPose() : joint->GetSocketSolid()->GetPose();
	Posed socketPose; joint->GetSocketPose(socketPose);

	Vec3d pullback_ = m.inv() * (soParentPose.Ori() * (socketPose.Ori() * pullback));

	for (size_t i=0; i<(size_t)ndof; ++i) { domega_pull[i] = pullback_[i]; }

	Posed soParentPullbackPose = (parent) ? parent->GetSolidPullbackPose() : joint->GetSocketSolid()->GetPose();
	Posed plugPose;   joint->GetPlugPose(plugPose);
	Posed jpt = Posed(); jpt.Ori() = pullbackTarget;
	solidPullbackPose = soParentPullbackPose * socketPose * jpt * plugPose.Inv();
}

void PHIKBallActuator::Move(){  //here
	if (!bEnabled) { return; }
	
	/*
	Vec3d dir = (jointTempOri.RotationHalf() - jointTempOriIntp);
	double limit = DCAST(PHSceneIf,GetScene())->GetIKEngine()->GetMaxActuatorVelocity();
	if (dir.norm() > limit) { dir = dir.unit() * limit; }
	jointTempOriIntp += dir;
	if (jointVelocity.norm() > limit) { jointVelocity = jointVelocity.unit() * limit; }
	DCAST(PHBallJoint,joint)->SetTargetPosition(Quaterniond::Rot(jointTempOriIntp));
	DCAST(PHBallJoint, joint)->SetTargetVelocity(jointVelocity);
	
	int intpRate = DCAST(PHSceneIf, GetScene())->GetIKEngine()->GetIntpRate();
	if (intpRate > 0) {
		if (intpRate == 50) jointTempOriIntp = DCAST(PHBallJoint, joint)->GetTargetPosition().RotationHalf();
		float s = intpRate / 50;
		jointTempOri = Quaterniond::Rot(s * jointTempOriIntp + (1 - s) * jointTempOri.RotationHalf());
	}
	*/

	DCAST(PHBallJoint, joint)->SetTargetPosition(jointTempOri);
	DCAST(PHBallJoint,joint)->SetTargetVelocity(jointVelocity);
	//DCAST(PHBallJoint, joint)->SetOffsetForce(jointTempTorque);//add here
	return;
}

void PHIKBallActuator::ApplyExactState(bool reverse) {
	if (!reverse) {
		solidTempPose        = joint->GetPlugSolid()->GetPose();
		jointTempOri         = DCAST(PHBallJointIf,joint)->GetPosition();
	} else {
		joint->GetPlugSolid()->SetPose(solidTempPose);
		joint->GetPlugSolid()->SetVelocity(solidVelocity);
		joint->GetPlugSolid()->SetAngularVelocity(solidAngularVelocity);
	}
}

bool PHIKBallActuator::LimitTempJoint() {
	PHBallJointConeLimitIf* limit = DCAST(PHBallJointIf,joint)->GetLimit()->Cast(); 
	if (limit) {
		Vec2d range; limit->GetSwingRange(range);
		Vec3d limitDir = limit->GetLimitDir();
		Vec3d   dir  = (jointTempOri * Vec3d(0, 0, 1)).unit();
		Vec3d  axis  = PTM::cross(limitDir, dir);
		if (axis.norm() > 1e-5) {
			axis.unitize();
			double angle = acos(PTM::dot(limitDir, dir));

			if (range[1] <= angle) {
				Quaterniond pullback = Quaterniond::Rot( axis * (range[1] - angle) );
				jointTempOri  = pullback * jointTempOri;
				jointVelocity = Vec3d();
				return true;
			}
		}
	}
	PHBallJointIndependentLimitIf* iLimit = DCAST(PHBallJointIf, joint)->GetLimit()->Cast();
	if (iLimit) {
		Vec3d euler;
		jointTempOri.ToEuler(euler);
		euler = Vec3d(euler.z, euler.x, euler.y);
		bool over = false;
		Vec3d correct = Vec3d();
		for (int i = 0; i < 3; i++) {
			Vec2d range; iLimit->GetLimitRangeN(i, range);
			if (euler[i] <= range[0]) {
				over = true;
				correct[i] = range[0] - euler[i];
			}
			else if (range[1] <= euler[i]) {
				over = true;
				correct[i] = range[1] - euler[i];
			}
		}
		if (over) {
			Quaterniond pullback;
			pullback.FromEuler(Vec3d(correct.y, correct.z, correct.x));
			jointTempOri = pullback * jointTempOri;
			jointVelocity = Vec3d();
			return true;
		}
	}
	return false;
}

void PHIKBallActuator::MoveTempJoint() {
	// ----- 位置 -----
	// 回転軸ベクトルにする
	Vec3d  w = Vec3d();
	for (int i=0; i<ndof; ++i) { w += ( omega[i]*sqsaib ) * e[i]; }

	// 関節座標系にする
	Posed soParentPose = (parent) ? parent->GetSolidTempPose() : joint->GetSocketSolid()->GetPose();
	Posed socketPose; joint->GetSocketPose(socketPose);
	w = (soParentPose * socketPose).Inv().Ori() * w;

	jointTempOri  = Quaterniond::Rot(w) * jointTempOri;

	// ----- 速度 -----
	jointVelocity = w  * (1/DCAST(PHSceneIf,GetScene())->GetTimeStep());
}

// --- --- --- --- ---
bool PHIKHingeActuator::AddChildObject(ObjectIf* o){
	PHHingeJointIf* jo = o->Cast();
	if (jo) {
		this->joint = jo;
		return true;
	}
	return PHIKActuator::AddChildObject(o);
}

ObjectIf* PHIKHingeActuator::GetChildObject(size_t pos){
	if (pos == 0 && this->joint != NULL) { return this->joint; }
	if (this->joint != NULL) {
		return PHIKActuator::GetChildObject(pos - 1);
	} else {
		return PHIKActuator::GetChildObject(pos);
	}
	return NULL;
}

size_t PHIKHingeActuator::NChildObject() const{
	if (this->joint != NULL) { return 1 + PHIKActuator::NChildObject(); }
	return PHIKActuator::NChildObject();
}

void PHIKHingeActuator::CalcJacobian(PHIKEndEffector* endeffector){
	int n = endeffector->number;
	PHHingeJoint* j = DCAST(PHHingeJoint,joint);
	Posed soParentPose = (parent) ? parent->GetSolidTempPose() : joint->GetSocketSolid()->GetPose();

	// アクチュエータ回転 <=> エンドエフェクタ位置
	if (endeffector->bPosition){
		Vec3d Pm = soParentPose * j->Xj[0].r;
		Vec3d Rm = soParentPose.Ori() * j->Xj[0].q * Vec3d(0,0,1);

		Vec3d Pe = endeffector->solidTempPose * endeffector->targetLocalPosition;

		Vec3d M3 = PTM::cross((Pm-Pe), Rm);
		for (int i=0; i<3; ++i) {
			Mj[n][i][0] = M3[i];
		}
	}

	// アクチュエータ回転 <=> エンドエフェクタ回転
	if (endeffector->bOrientation){
		int stride = (endeffector->bPosition ? 3 : 0);

		// ヒンジの回転軸ベクトル
		Vec3d Rm = soParentPose.Ori() * j->Xj[0].q * Vec3d(0,0,1);
		for (int i=0; i<3; ++i) {
			Mj[n][i+stride][0] = Rm[i];
		}
	}

	// 重み付け
	size_t ndof_eef=0;
	ndof_eef += (endeffector->bPosition    ? 3 : 0);
	ndof_eef += (endeffector->bOrientation ? 3 : 0);
	for (size_t i=0; i<ndof_eef; ++i) {
		for (size_t j=0; j<(size_t)ndof; ++j) {
			Mj[n][i][j] *= sqsaib;
		}
	}
}

void PHIKHingeActuator::CalcPullbackVelocity() {
	double pullbacked = jointTempAngle - pullbackRate * (jointTempAngle - pullbackTarget);

	// リミット成分
	PH1DJointLimitIf* limit = DCAST(PHHingeJointIf,joint)->GetLimit();
	if (limit) {
		Vec2d range; limit->GetRange(range);
		if (range[0] < range[1]) {
			pullbacked = max(range[0], min(pullbacked, range[1]));
		}
	}

	domega_pull[0] = pullbacked - jointTempAngle;

	// <!!> Pullback量が一定以下になるよう制限する．
	domega_pull[0] = max(Rad(-200), min(domega_pull[0], Rad(200)));

	Posed soParentPullbackPose = (parent) ? parent->GetSolidPullbackPose() : joint->GetSocketSolid()->GetPose();
	Posed socketPose; joint->GetSocketPose(socketPose);
	Posed plugPose;   joint->GetPlugPose(plugPose);
	Posed jpt = Posed(); jpt.Ori() = Quaterniond::Rot(pullbackTarget, 'z');
	solidPullbackPose = soParentPullbackPose * socketPose * jpt * plugPose.Inv();
}

void PHIKHingeActuator::Move(){
	if (!bEnabled) { return; }
	PHHingeJointIf* hj = joint->Cast();
	
	double limit = DCAST(PHSceneIf,GetScene())->GetIKEngine()->GetMaxActuatorVelocity();
	double diff = jointTempAngle - jointTempAngleIntp;
	if (abs(diff) > limit) { diff = diff / abs(diff) * limit; }
	jointTempAngleIntp += diff;
	jointVelocity = std::max(-limit, std::min(jointVelocity, limit));
	
	hj->SetTargetPosition(jointTempAngleIntp);
	hj->SetTargetVelocity(jointVelocity);
	/*/

	int intpRate = DCAST(PHSceneIf, GetScene())->GetIKEngine()->GetIntpRate();
	if (intpRate > 0) {
		if (intpRate == 50) jointTempAngleIntp = hj->GetTargetPosition();
		float s = intpRate / 50;
		jointTempAngle = (1 - s) * jointTempAngle + s * jointTempAngleIntp;
	}

	hj->SetTargetPosition(jointTempAngle);
	hj->SetTargetVelocity(jointVelocity);
	*/
	return;
}

void PHIKHingeActuator::ApplyExactState(bool reverse) {
	if (!reverse) {
		jointTempAngle = DCAST(PHHingeJointIf,joint)->GetPosition();
		jointTempOri   = Quaterniond::Rot(jointTempAngle, 'z');
		solidTempPose  = joint->GetPlugSolid()->GetPose();
	} else {
		joint->GetPlugSolid()->SetPose(solidTempPose);
		joint->GetPlugSolid()->SetVelocity(solidVelocity);
		joint->GetPlugSolid()->SetAngularVelocity(solidAngularVelocity);
	}
}

bool PHIKHingeActuator::LimitTempJoint() {
	PH1DJointLimitIf* limit = DCAST(PHHingeJointIf,joint)->GetLimit();
	if (limit) {
		Vec2d range; limit->GetRange(range);
		if (range[0] < range[1]) {
			if (jointTempAngle <= range[0]) {
				jointTempAngle = range[0];
				jointTempOri   = Quaterniond::Rot(jointTempAngle, 'z');
				jointVelocity  = 0;
				return true;
			}
			if (range[1] <= jointTempAngle) {
				jointTempAngle = range[1];
				jointTempOri   = Quaterniond::Rot(jointTempAngle, 'z');
				jointVelocity  = 0;
				return true;
			}
		}
	}
	return false;
}

void PHIKHingeActuator::MoveTempJoint() {
	if (!bEnabled) { return; }

	double dTheta = omega[0]*sqsaib;

	jointTempAngle += dTheta;
	if(DCAST(PHHingeJointIf,joint)->IsCyclic()){
		while(jointTempAngle >  M_PI)
			jointTempAngle -= 2 * M_PI;
		while(jointTempAngle < -M_PI)
			jointTempAngle += 2 * M_PI;
	}

	jointTempOri  = Quaterniond::Rot(jointTempAngle, 'z');
	jointVelocity = dTheta  / DCAST(PHSceneIf,GetScene())->GetTimeStep();
}

// --- --- --- --- ---
bool PHIKSpringActuator::AddChildObject(ObjectIf* o) {
	PHSpringIf* jo = o->Cast();
	if (jo) {
		this->joint = jo;
		return true;
	}
	return PHIKActuator::AddChildObject(o);
}

ObjectIf* PHIKSpringActuator::GetChildObject(size_t pos) {
	if (pos == 0 && this->joint != NULL) { return this->joint; }
	if (this->joint != NULL) {
		return PHIKActuator::GetChildObject(pos - 1);
	}
	else {
		return PHIKActuator::GetChildObject(pos);
	}
	return NULL;
}

size_t PHIKSpringActuator::NChildObject() const {
	if (this->joint != NULL) { return 1 + PHIKActuator::NChildObject(); }
	return PHIKActuator::NChildObject();
}

void PHIKSpringActuator::BeforeSetupMatrix() {
	if (ndof != 6) {
		ndof = 6;
		bNDOFChanged = true;
	}
}

void PHIKSpringActuator::BeforeCalcAllJacobian() {
	CalcAxis();
}

void PHIKSpringActuator::CalcAxis() {
	e[0] = Vec3d(1, 0, 0);
	e[1] = Vec3d(0, 1, 0);
	e[2] = Vec3d(0, 0, 1);
}

void PHIKSpringActuator::CalcJacobian(PHIKEndEffector* endeffector) {
	int n = endeffector->number;

	// アクチュエータ位置 <=> エンドエフェクタ位置
	if (endeffector->bPosition) {
		// 単位行列
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				Mj[n][i][j] = ((i == j) ? 1 : 0);
			}
		}
	}

	// アクチュエータ位置 <=> エンドエフェクタ回転
	if (endeffector->bOrientation) {
		int stride = (endeffector->bPosition ? 3 : 0);

		// ゼロ行列
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				Mj[n][i + stride][j] = 0;
			}
		}
	}

	// ----- ----- -----

	// アクチュエータ回転 <=> エンドエフェクタ位置
	if (endeffector->bPosition) {
		// 関節の回転中心
		PHSpring* j = DCAST(PHSpring, joint);
		Posed soParentPose = (parent) ? parent->GetSolidTempPose() : joint->GetSocketSolid()->GetPose();

		// <!!> IKActuatorがついてないjointが間に入るとd.poseSocketがずれる
		PHSpringDesc d; j->GetDesc(&d);
		Vec3d Pj = soParentPose * d.poseSocket * Vec3d(0, 0, 0);

		// エンドエフェクタ位置
		Vec3d Pe = endeffector->solidTempPose * endeffector->targetLocalPosition;

		// 外積ベクトルからヤコビアンを求める
		for (int i = 0; i < 3; ++i) {
			Vec3d v = PTM::cross(e[i], (Pe - Pj));
			Mj[n][0][i + 3] = v[0];  Mj[n][1][i + 3] = v[1];  Mj[n][2][i + 3] = v[2];
		}
	}

	// アクチュエータ回転 <=> エンドエフェクタ回転
	if (endeffector->bOrientation) {
		int stride = (endeffector->bPosition ? 3 : 0);

		// 単位行列
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				Mj[n][i + stride][j + 3] = ((i == j) ? 1 : 0);
			}
		}
	}

	// 重み付け
	size_t ndof_eef = 0;
	ndof_eef += (endeffector->bPosition ? 3 : 0);
	ndof_eef += (endeffector->bOrientation ? 3 : 0);
	for (size_t i = 0; i < ndof_eef; ++i) {
		for (size_t j = 0; j < (size_t)ndof; ++j) {
			Mj[n][i][j] *= sqsaib;
		}
	}
}

void PHIKSpringActuator::CalcPullbackVelocity() {
	Matrix3d m(e[0], e[1], e[2]);

	Vec6d pullback = Vec6d();

	// 標準姿勢へののPullback
	pullback.r = (pullbackTarget.Pos() - jointTempPose.Pos()) * pullbackRate;
	pullback.v = (pullbackTarget.Ori() * jointTempPose.Ori().Inv()).RotationHalf() * pullbackRate;

	// <!!> Pullback量が一定以下になるよう制限する．
	if (pullback.r.norm() > 1) {
		pullback.r = pullback.r.unit() * 1;
	}
	if (pullback.v.norm() > Rad(10)) {
		pullback.v = pullback.v.unit() * Rad(10);
	}

	Posed soParentPose = (parent) ? parent->GetSolidTempPose() : joint->GetSocketSolid()->GetPose();
	Posed socketPose; joint->GetSocketPose(socketPose);

	pullback.v = m.inv() * (soParentPose.Ori() * (socketPose.Ori() * pullback.v));

	for (size_t i = 0; i < (size_t)ndof; ++i) { domega_pull[i] = pullback[i]; }
}

void PHIKSpringActuator::Move() {
	if (!bEnabled) { return; }
	DSTR << jointTempPose.Pos() << std::endl;
	DCAST(PHSpring, joint)->SetTargetPosition(jointTempPose.Pos());
	DCAST(PHSpring, joint)->SetTargetOrientation(jointTempPose.Ori());
	DCAST(PHSpring, joint)->SetTargetVelocity(jointVelocity);

	return;
}

void PHIKSpringActuator::ApplyExactState(bool reverse) {
	if (!reverse) {
		solidTempPose = joint->GetPlugSolid()->GetPose();
		jointTempPose = DCAST(PHSpringIf, joint)->GetAbsolutePoseQ(); // <!!> ... だと思うんだけど
	}
	else {
		joint->GetPlugSolid()->SetPose(solidTempPose);
		joint->GetPlugSolid()->SetVelocity(solidVelocity);
		joint->GetPlugSolid()->SetAngularVelocity(solidAngularVelocity);
	}
}

bool PHIKSpringActuator::LimitTempJoint() {
	return false;
}

void PHIKSpringActuator::MoveTempJoint() {
	// ----- 位置 -----
	Vec3d v = Vec3d();
	for (int i = 0; i < 3; ++i) { v[i] = (omega[i] * sqsaib); }
	jointTempPose.Pos() += v;

	// ----- 速度 -----
	jointVelocity.r = v * (1 / DCAST(PHSceneIf, GetScene())->GetTimeStep());

	// ----- 角度 -----
	// 回転軸ベクトルにする
	Vec3d  w = Vec3d();
	for (int i = 0; i < 3; ++i) { w += (omega[i+3] * sqsaib) * e[i]; }

	// 関節座標系にする
	Posed soParentPose = (parent) ? parent->GetSolidTempPose() : joint->GetSocketSolid()->GetPose();
	Posed socketPose; joint->GetSocketPose(socketPose);
	w = (soParentPose * socketPose).Inv().Ori() * w;

	jointTempPose.Ori() = Quaterniond::Rot(w) * jointTempPose.Ori();

	// ----- 角速度 -----
	jointVelocity.v = w * (1 / DCAST(PHSceneIf, GetScene())->GetTimeStep());
}

}

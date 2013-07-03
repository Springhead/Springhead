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

using namespace std;
namespace Spr{;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// IKActuator

bool PHIKActuator::AddChildObject(ObjectIf* o){
	PHIKEndEffector* endeffector = o->Cast();
	if (endeffector) {
		DCAST(PHSceneIf,GetScene())->GetIKEngine()->AddChildObject(endeffector->Cast());
		this->eef = endeffector;
		return true;
	}

	PHIKActuator* actuator = o->Cast();
	if (actuator) {
		DCAST(PHSceneIf,GetScene())->GetIKEngine()->AddChildObject(actuator->Cast());
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

		// α、β
		if (this->bNDOFChanged) {
			alpha.resize(this->ndof);
			beta.resize(this->ndof);
		}
		alpha.clear();
		beta.clear();

		// Γ（自分自身を相手とするΓも含む）
		for (int nLink=0; nLink<NLinks(); ++nLink) {
			if (this->bNDOFChanged || (Link(nLink)->bNDOFChanged && Link(nLink)->bEnabled) || this->bActuatorAdded) {
				gamma[Link(nLink)->number].resize(this->ndof, Link(nLink)->ndof);
			}
			gamma[Link(nLink)->number].clear();
		}

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
			omega_prev.resize(ndof);
			tau.resize(ndof);
		}
		omega.clear();
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

	PHIKEngineIf* engine = DCAST(PHSceneIf,GetScene())->GetIKEngine();

	for (int i=0; i< ndof; ++i) {
		for (int nDesc=0; nDesc<(int)descendant.size(); ++nDesc) {
			PHIKEndEffector* childEef = descendant[nDesc]->eef;
			if (childEef==NULL || !(childEef->bEnabled)) { continue; }

			int eef_n = childEef->number;

			PTM::VVector<double> eef_v;
			childEef->GetTempTarget(eef_v);

			for (int k=0; k < childEef->ndof; ++k) {

				// α、β
				alpha[i] += ( (Mj[eef_n][k][i]/bias) * (Mj[eef_n][k][i]) );
				beta[i]  += ( (Mj[eef_n][k][i]/bias) * (eef_v[k])  );

				// γ[act_y, this] （act_y == this の場合を含む）
				for (int nLink=0; nLink < NLinks(); ++nLink) {
					if (!(Link(nLink)->bEnabled)) { continue; }
					int act_n = Link(nLink)->number;
					for (int j=0; j<Link(nLink)->ndof; ++j) {
						if (Link(nLink)->Mj.find(eef_n) != Link(nLink)->Mj.end()) {
							gamma[act_n][i][j] += ( (Mj[eef_n][k][i]/bias) * (Link(nLink)->Mj[eef_n][k][j] / Link(nLink)->bias) );
						}
					}
				}

			}
		}
	}

	omega.clear();
	omega_prev.clear();
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
	solidTempPose = soParentPose * socketPose * jto * plugPose.Inv();

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
	// 姿勢制御をするエンドエフェクタが無ければ自由度を２に下げる（冗長性回避のため）

	/*
	bool bFound = false;;
	for (int i=0; i<(int)descendant.size(); ++i) {
		if (descendant[i]->eef && descendant[i]->eef->bEnabled && descendant[i]->eef->bOrientation) {
			bFound = true;
		}
	}

	if (bFound) {
		if (ndof != 3) {
			ndof = 3;
			bNDOFChanged = true;
		}
	} else {
		if (ndof != 2) {
			ndof = 2;
			bNDOFChanged = true;
		}
	}
	*/

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

	/*
	Posed soParentPose = (parent) ? parent->solidTempPose : joint->GetSocketSolid()->GetPose();
	PHBallJoint* j = DCAST(PHBallJoint,joint);
	PHBallJointDesc d; j->GetDesc(&d);

	for (int nDesc=0; nDesc<(int)descendant.size(); ++nDesc) {
		PHIKEndEffector* childEef = descendant[nDesc]->eef; if (childEef==NULL) { continue; }
		if (childEef->bEnabled && !childEef->bOrientation) {
			// 関節の回転中心
			Vec3d Pj = soParentPose * d.poseSocket * Vec3d(0,0,0);

			// エンドエフェクタ位置
			Vec3d Pe = childEef->solidTempPose * childEef->targetLocalPosition;

			// 関節回転中心<->エンドエフェクタ 軸
			Vec3d e0 = (Pe - Pj);
			if (e0.norm() != 0){
				e0 = e0 / e0.norm();
			} else {
				e0 = Vec3d(1,0,0);
			}

			// 回転軸を求める
			if (abs(PTM::dot(e0,Vec3d(1,0,0))) > Rad(10)) {
				e[0] = Vec3d(0,1,0);
			} else {
				e[0] = Vec3d(1,0,0);
			}
			e[0] = e[0] - (PTM::dot(e[0],e0) * e0);
			e[0] = e[0] / e[0].norm();
			e[1] = PTM::cross(e0,e[0]);
			e[2] = e0;

			break;
		}
	}
	*/
}

void PHIKBallActuator::CalcJacobian(PHIKEndEffector* endeffector){
	int n = endeffector->number;

	// アクチュエータ回転 <=> エンドエフェクタ位置
	if (endeffector->bPosition){
		// 関節の回転中心
		PHBallJoint* j = DCAST(PHBallJoint,joint);
		Posed soParentPose = (parent) ? parent->solidTempPose : joint->GetSocketSolid()->GetPose();

		PHBallJointDesc d; j->GetDesc(&d);
		Vec3d Pj = soParentPose * d.poseSocket * Vec3d(0,0,0);

		// エンドエフェクタ位置
		Vec3d Pe = endeffector->solidTempPose * endeffector->targetLocalPosition;

		// 外積ベクトルからヤコビアンを求める
		for (int i=0; i<ndof; ++i) {
			Vec3d v = PTM::cross(e[i],(Pe-Pj));
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
}

void PHIKBallActuator::CalcPullbackVelocity(double ratio) {
	Matrix3d m(e[0], e[1], e[2]);

	// ratio = 0.01;

	Vec3d pullback = -(jointTempOri.RotationHalf()) * ratio;

	// <!!> Limit Max Pullback Angle
	if (pullback.norm() > Rad(20)) {
		pullback = pullback.unit() * Rad(20);
	}

	Posed soParentPose = (parent) ? parent->solidTempPose : joint->GetSocketSolid()->GetPose();
	Posed socketPose; joint->GetSocketPose(socketPose);

	Vec3d pullback_ = m.inv() * (soParentPose.Ori() * (socketPose.Ori() * pullback));
	for (size_t i=0; i<ndof; ++i) { domega_pull[i] = pullback_[i]; }

	/*
	Vec3d pbv = (jointTempOri.Inv() * Quaterniond::Rot(rot)).RotationHalf();

	// Jacobian系に変換する
	Matrix3d m(e[0], e[1], e[2]);
	Posed soParentPose = (parent) ? parent->solidTempPose : joint->GetSocketSolid()->GetPose();
	Posed socketPose; joint->GetSocketPose(socketPose);

	domega_pull = m.inv() * soParentPose * socketPose * pbv;
	*/
}

void PHIKBallActuator::Move(){
	if (!bEnabled) { return; }

	DCAST(PHBallJoint,joint)->SetTargetPosition(jointTempOri);
	
	return;
}

void PHIKBallActuator::PullbackTempJoint(double ratio) {
	Vec3d rot = jointTempOri.RotationHalf();
	rot *= (1-ratio);
	jointTempOri = Quaterniond::Rot(rot);
}

void PHIKBallActuator::MoveTempJoint() {
	// 回転軸ベクトルにする
	Vec3d  w = Vec3d();
	for (int i=0; i<ndof; ++i) { w += ( omega[i]/bias) * e[i]; }

	// 関節座標系にする
	Posed soParentPose = (parent) ? parent->solidTempPose : joint->GetSocketSolid()->GetPose();
	Posed socketPose; joint->GetSocketPose(socketPose);
	w = (soParentPose * socketPose).Inv().Ori() * w;

	jointTempOri = Quaterniond::Rot(w) * jointTempOri;
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
	Posed soParentPose = (parent) ? parent->solidTempPose : joint->GetSocketSolid()->GetPose();

	// アクチュエータ回転 <=> エンドエフェクタ位置
	if (endeffector->bPosition){
		Vec3d Pm = soParentPose * j->Xj[0].r;
		Vec3d Rm = soParentPose.Ori() * j->Xj[0].q * Vec3d(0,0,1);

		Vec3d Pe = endeffector->solidTempPose * endeffector->targetLocalPosition;

		Vec3d M3 = PTM::cross((Pm-Pe), Rm);
		for (int i=0; i<3; ++i) {
			Mj[n][i][0] = M3[i];
		}

		/*
		Vec3d Pm = j->solid[0]->GetPose() * j->Xj[0].r;
		Vec3d Rm = j->solid[0]->GetPose().Ori() * j->Xj[0].q * Vec3d(0,0,1);

		Vec3d Pe = endeffector->solid->GetPose() * endeffector->targetLocalPosition;

		Vec3d M3 = PTM::cross((Pm-Pe), Rm);
		for (int i=0; i<3; ++i) {
			Mj[n][i][0] = M3[i];
		}
		*/
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
}

void PHIKHingeActuator::CalcPullbackVelocity(double ratio) {
	double pullbacked = (1-ratio) * jointTempAngle;
	domega_pull[0] = pullbacked - jointTempAngle;
	domega_pull[0] = max(Rad(-10), min(domega_pull[0], Rad(10)));
}

void PHIKHingeActuator::Move(){
	if (!bEnabled) { return; }

	DCAST(PHHingeJoint,joint)->SetTargetPosition(jointTempAngle);

	/*
	double dTheta = omega[0]/bias;
	DCAST(PHHingeJoint,joint)->SetTargetPosition(DCAST(PHHingeJoint,joint)->GetTargetPosition() + dTheta);
	DCAST(PHHingeJoint,joint)->SetTargetVelocity(dTheta / DCAST(PHSceneIf,GetScene())->GetTimeStep());
	*/

	return;
}

void PHIKHingeActuator::PullbackTempJoint(double ratio) {
	jointTempAngle = (1-ratio) * jointTempAngle + ratio * Rad(10);

	if(DCAST(PHHingeJointIf,joint)->IsCyclic()){
		while(jointTempAngle >  M_PI)
			jointTempAngle -= 2 * M_PI;
		while(jointTempAngle < -M_PI)
			jointTempAngle += 2 * M_PI;
	}
	jointTempOri = Quaterniond::Rot(jointTempAngle, 'z');
}

void PHIKHingeActuator::MoveTempJoint() {
	double dTheta = omega[0]/bias;
	jointTempAngle += dTheta;
	if(DCAST(PHHingeJointIf,joint)->IsCyclic()){
		while(jointTempAngle >  M_PI)
			jointTempAngle -= 2 * M_PI;
		while(jointTempAngle < -M_PI)
			jointTempAngle += 2 * M_PI;
	}
	jointTempOri = Quaterniond::Rot(jointTempAngle, 'z');
}

}

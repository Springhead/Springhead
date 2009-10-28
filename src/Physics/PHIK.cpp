/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Physics.h"
#pragma hdrstop
#include "PHIK.h"
#include "Physics/PHJoint.h"

using namespace std;
namespace Spr{

//static std::ofstream *dlog;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// IKEngine
PHIKEngine::PHIKEngine():numIter(25){ 
	bEnabled = false; 
}
void PHIKEngine::Step(){
	//dlog = new std::ofstream("iklog.txt", ios_base::out | ios_base::app);
	//(*dlog) << "----- ----- ----- ----- ----- ----- ----- ----- ----- ----- " << std::endl;

	if (bEnabled) {
		if (nodes.size() > 0 && controlpoints.size() > 0) {

			// ヤコビアンの計算
			for(size_t i=0; i<nodes.size(); ++i){
				DCAST(PHIKNode,nodes[i])->ClearJacobian();
				DCAST(PHIKNode,nodes[i])->CalcAllJacobian();
			}

			// 計算の準備（α・β・γの事前計算）
			for(size_t i=0; i<nodes.size(); ++i){
				nodes[i]->PrepareSolve();
			}

			// 繰り返し計算の実行
			int iter = numIter;
			if (iter < 0) { iter = 200; }
			for(int n=0; n<iter; n++){
				double dErr = 0;
				for(size_t i=0; i<nodes.size(); ++i){
					nodes[i]->ProceedSolve();
					PTM::VVector<double> diff = (DCAST(PHIKNode,nodes[i])->dTheta_prev - DCAST(PHIKNode,nodes[i])->dTheta);
					dErr += (diff.norm() * diff.norm());
				}
				if ((((int)numIter) < 0) && (sqrt(dErr) < 1e-8)) { break; }
			}

			// 関節の動作
			for(size_t i=0; i<nodes.size(); ++i){
				//(*dlog) << "--- w[nd:" << DCAST(PHIKNode,nodes[i])->number << "] ---" << std::endl;
				//(*dlog) << DCAST(PHIKNode,nodes[i])->dTheta << std::endl;

				nodes[i]->Move();
			}

		}
	}
	//dlog->close();
	//delete dlog;
}

void PHIKEngine::Clear(){
	for(size_t i=0; i<nodes.size(); ++i){
		PHIKNode* nd = nodes[i]->Cast();
		delete nd;
	}
	nodes.clear();

	for(size_t i=0; i<controlpoints.size(); ++i){
		PHIKControlPoint* ct = controlpoints[i]->Cast();
		delete ct;
	}
	controlpoints.clear();
}

PHIKNode* PHIKEngine::CreateIKNode(const IfInfo* ii, const PHIKNodeDesc& desc){
	PHIKNode* iknode = NULL;
	if (ii == PHIKSolidIf::GetIfInfoStatic()) {
		iknode = DBG_NEW PHIKSolid();
		DCAST(PHIKSolidIf,iknode)->SetDesc(&desc);
	} else if (ii == PHIKBallJointIf::GetIfInfoStatic()) {
		iknode = DBG_NEW PHIKBallJoint();
		DCAST(PHIKBallJointIf,iknode)->SetDesc(&desc);
	} else if (ii == PHIKHingeJointIf::GetIfInfoStatic()) {
		iknode = DBG_NEW PHIKHingeJoint();
		DCAST(PHIKHingeJointIf,iknode)->SetDesc(&desc);
	}
	return iknode;
}

PHIKControlPoint* PHIKEngine::CreateIKControlPoint(const IfInfo* ii, const PHIKControlPointDesc& desc){
	PHIKControlPoint* ikcontrolpoint = NULL;
	if (ii == PHIKPosCtlIf::GetIfInfoStatic()) {
		ikcontrolpoint = DBG_NEW PHIKPosCtl();
		DCAST(PHIKPosCtlIf,ikcontrolpoint)->SetDesc(&desc);
	} else if (ii == PHIKOriCtlIf::GetIfInfoStatic()) {
		ikcontrolpoint = DBG_NEW PHIKOriCtl();
		DCAST(PHIKOriCtlIf,ikcontrolpoint)->SetDesc(&desc);
	}
	return ikcontrolpoint;
}

bool PHIKEngine::AddChildObject(ObjectIf* o){
	// --- --- --- --- --- --- ---
	// ノードの場合
	PHIKSolidIf* so = o->Cast();
	if(so){
		nodes.push_back(so);
		so->SetNumber(nodes.size()-1);
		return true;
	}

	PHIKBallJointIf* bj = o->Cast();
	if(bj){
		nodes.push_back(bj);
		bj->SetNumber(nodes.size()-1);
		return true;
	}

	PHIKHingeJointIf* hj = o->Cast();
	if(hj){
		nodes.push_back(hj);
		hj->SetNumber(nodes.size()-1);
		return true;
	}

	// --- --- --- --- --- --- ---
	// 制御点の場合
	PHIKPosCtlIf* pc = o->Cast();
	if(pc){
		controlpoints.push_back(pc);
		pc->SetNumber(controlpoints.size()-1);
		return true;
	}

	PHIKOriCtlIf* oc = o->Cast();
	if(oc){
		controlpoints.push_back(oc);
		oc->SetNumber(controlpoints.size()-1);
		return true;
	}

	return false;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// IKControlPoint

void PHIKControlPoint::Enable(bool enable){
	this->isEnabled = enable;
	if (!enable) {
		for(NSetIter node=linkedNodes.begin(); node!=linkedNodes.end(); ++node){
			(*node)->MoveNatural();
		}
	}
}

// --- --- --- --- ---
Vec3d PHIKPosCtl::GetTmpGoal(){
	Vec3d spos = solid->GetPose()*pos;
	Vec3d dir = goal - spos;
	double epsilon = 1.0;
	if (dir.norm() < epsilon) {
		return(dir);
	} else {
		return(dir/dir.norm()*epsilon);
	}
}

// --- --- --- --- ---
Vec3d PHIKOriCtl::GetTmpGoal(){
	Quaterniond qS = solid->GetPose().Ori();
	Quaterniond qG = (goal * qS.Inv());

	double epsilon = Rad(50.0);
	if (qG.Theta() < epsilon) {
		return((qG.Axis() * qG.Theta()));
	} else {
		return((qG.Axis() * epsilon));
	}
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// IKNode

// --- --- --- --- ---
void PHIKNode::SetNDOF(int n){
	ndof = n;

	alpha.resize(ndof); alpha.clear();
	beta.resize(ndof); beta.clear();
	gamma.clear();
	for(NSetIter n_y=linkedNodes.begin(); n_y!=linkedNodes.end(); ++n_y){
		int n_y_n = DCAST(PHIKNode,*n_y)->number;
		gamma[n_y_n].resize(ndof,DCAST(PHIKNode,*n_y)->ndof);
		gamma[n_y_n].clear();
	}
	gamma[number].resize(ndof,ndof); gamma[number].clear();

	dTheta.resize(ndof); dTheta.clear();
	dTheta_prev.resize(ndof); dTheta_prev.clear();
	tau.resize(ndof); tau.clear();
}

void PHIKNode::ClearJacobian(){
	Mj.clear();
}

void PHIKNode::CalcAllJacobian(){
	enableStat = ES_NONE;
	for(CSetIter ctlpt=linkedControlPoints.begin(); ctlpt!=linkedControlPoints.end(); ++ctlpt){
		if (! DCAST(PHIKControlPoint,*ctlpt)->isEnabled) { continue; }

		if (DCAST(PHIKOriCtl,*ctlpt)) {
			// Enabledな姿勢制御点が含まれる
			enableStat = ES_ORI;
		} else {
			// Enabledな位置制御点が含まれる
			if (enableStat==ES_NONE) {
				enableStat = ES_NOORI;
			}
		}
	}

	if (DCAST(PHIKBallJointIf,this)) {
		if (enableStat==ES_NOORI) {
			if (ndof!=2) {
				SetNDOF(2);
			}
		} else if (enableStat==ES_ORI) {
			if (ndof!=3) {
				SetNDOF(3);
			}
		}
	}

	for(CSetIter ctlpt=linkedControlPoints.begin(); ctlpt!=linkedControlPoints.end(); ++ctlpt){
		if (! DCAST(PHIKControlPoint,*ctlpt)->isEnabled) { continue; }

		int n = DCAST(PHIKControlPoint,*ctlpt)->number;
		Mj[n] = CalcJacobian(*ctlpt);

		//(*dlog) << "--- J[nd:" << number << "][cp:" << n << "] ---" << std::endl;
		//(*dlog) << Mj[n] << std::endl;
	}
}

void PHIKNode::PrepareSolve(){
	if (!isEnabled) { return; }

	alpha.clear();
	beta.clear();

	for(NSetIter n_y=linkedNodes.begin(); n_y!=linkedNodes.end(); ++n_y){
		if (!((*n_y)->IsEnabled())) { continue; }
		int n_y_n = DCAST(PHIKNode,*n_y)->number;
		gamma[n_y_n].resize(ndof,DCAST(PHIKNode,*n_y)->ndof);
		gamma[n_y_n].clear();
	}
	gamma[number].resize(ndof,ndof);
	gamma[number].clear();

	for (int i=0; i< ndof; ++i) {
		for(CSetIter c_z=linkedControlPoints.begin(); c_z!=linkedControlPoints.end(); ++c_z){
			if (! DCAST(PHIKControlPoint,*c_z)->isEnabled) { continue; }
			int c_z_n = DCAST(PHIKControlPoint,*c_z)->number;
			Vec3d c_z_v = DCAST(PHIKControlPoint,*c_z)->GetTmpGoal();

			//(*dlog) << "--- v[cp:" << c_z_n << "] ---" << std::endl;
			//(*dlog) << c_z_v << std::endl;

			for (size_t k=0; k<3; ++k) {

				// α、β
				alpha[i] += ( (Mj[c_z_n][k][i]/bias) * (Mj[c_z_n][k][i]) );
				beta[i]  += ( (Mj[c_z_n][k][i]/bias) * (c_z_v[k]) );

				// γ[ny, nx]
				for(NSetIter n_y=linkedNodes.begin(); n_y!=linkedNodes.end(); ++n_y){
					if (!((*n_y)->IsEnabled())) { continue; }
					int n_y_n = DCAST(PHIKNode,*n_y)->number;
					for (int j=0; j<DCAST(PHIKNode,*n_y)->ndof; ++j) {
						if (DCAST(PHIKNode,*n_y)->Mj.find(c_z_n) != DCAST(PHIKNode,*n_y)->Mj.end()) {
							gamma[n_y_n][i][j] += ( (Mj[c_z_n][k][i]/bias) * (DCAST(PHIKNode,*n_y)->Mj[c_z_n][k][j] / DCAST(PHIKNode,*n_y)->bias) );
						}
					}
				}

				// γ[nx, nx]
				for (int j=0; j<ndof; ++j) {
					if (i!=j) {
						//(*dlog) << number << ":" << i << ":" << j << ": += " << (Mj[c_z_n][k][i]/bias) << ", " << (Mj[c_z_n][k][j]/bias) << std::endl;
						gamma[number][i][j] += ( (Mj[c_z_n][k][i]/bias) * (Mj[c_z_n][k][j]/bias) );
					}
				}

			}
		}
	}


	//(*dlog) << "--- alpha[" << number << "] ---" << std::endl;
	//(*dlog) << alpha << std::endl;

	//(*dlog) << "--- beta[" << number << "] ---" << std::endl;
	//(*dlog) << beta << std::endl;

	for(NSetIter n_y=linkedNodes.begin(); n_y!=linkedNodes.end(); ++n_y){
		if (!((*n_y)->IsEnabled())) { continue; }
		int n_y_n = DCAST(PHIKNode,*n_y)->number;
		//(*dlog) << "--- gamma[" << number << "][" << n_y_n << "] ---" << std::endl;
		//(*dlog) << gamma[n_y_n] << std::endl;
	}
	//(*dlog) << "--- gamma[" << number << "][" << number << "] ---" << std::endl;
	//(*dlog) << gamma[number] << std::endl;

	dTheta.clear();
	dTheta_prev.clear();
}

void PHIKNode::ProceedSolve(){
	dTheta_prev = dTheta;

	for (int i=0; i<ndof; ++i) {
		double delta_epsilon = 0;

		// δ
		for(NSetIter n_y=linkedNodes.begin(); n_y!=linkedNodes.end(); ++n_y){
			if (!((*n_y)->IsEnabled())) { continue; }
			int n_y_n = DCAST(PHIKNode,*n_y)->number;
			if (gamma.find(n_y_n) != gamma.end()) {
				for (int k=0; k<DCAST(PHIKNode,*n_y)->ndof; ++k) {
					delta_epsilon += ( (gamma[n_y_n][i][k]) * (DCAST(PHIKNode,*n_y)->dTheta[k]) );
				}
			}
		}

		// ε
		for (int k=0; k<ndof; ++k) {
			if (k!=i) {
				if (gamma.find(number) != gamma.end()) {
					delta_epsilon += ( (gamma[number][i][k]) * (dTheta[k]) );
				}
			}
		}

		// ωの更新
		double iAlpha = 0;
		if (alpha[i]!=0) {
			iAlpha = (1.0 / alpha[i]);
		} else {
			iAlpha = 1e+20;
		}
		dTheta[i] = iAlpha * (beta[i] - delta_epsilon);
	}
}

void PHIKNode::AddControlPoint(PHIKControlPointIf* control){
	NSet* ln = &(DCAST(PHIKControlPoint,control)->linkedNodes);
	for(NSetIter node=ln->begin(); node!=ln->end(); ++node){
		linkedNodes.insert(*node);
		PHIKNodeIf* self = this->Cast();
		if (DCAST(PHIKNode,*node)->linkedNodes.find(self) == DCAST(PHIKNode,*node)->linkedNodes.end()) {
			DCAST(PHIKNode,*node)->linkedNodes.insert(self);
		}
	}
	DCAST(PHIKControlPoint,control)->linkedNodes.insert(this->Cast());
	linkedControlPoints.insert(control);

	gamma.clear();
	for(NSetIter n_y=linkedNodes.begin(); n_y!=linkedNodes.end(); ++n_y){
		int n_y_n = DCAST(PHIKNode,*n_y)->number;
		gamma[n_y_n].resize(ndof,DCAST(PHIKNode,*n_y)->ndof);
		gamma[n_y_n].clear();
	}
	gamma[number].resize(ndof,ndof); gamma[number].clear();
}

PTM::VMatrixRow<double> PHIKNode::CalcJacobian(PHIKControlPointIf* control){
	return PTM::VMatrixRow<double>();
}

// --- --- --- --- ---
PTM::VMatrixRow<double> PHIKSolid::CalcJacobian(PHIKControlPointIf* control){
	PHIKPosCtlIf* cpPos;
	if (cpPos = DCAST(PHIKPosCtlIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,3); M.clear();
		M[0][0]=1; M[1][1]=1; M[2][2]=1;
		return M;
	}
 
	PHIKOriCtlIf* cpOri;
	if (cpOri = DCAST(PHIKOriCtlIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,3);
		Vec3d Pm = solid->GetPose().Pos();
		PHIKOriCtl* cp = DCAST(PHIKOriCtl,cpOri);
		Vec3d Pc = cp->solid->GetPose().Pos();
		double norm = (Pm-Pc).norm();
		M = Matrix3d::Cross((Pm-Pc)/(norm*norm));
		return M;
	}

	return PTM::VMatrixRow<double>();
}

// --- --- --- --- ---
void PHIKBallJoint::CalcAllJacobian(){
	CalcAxis();
	PHIKNode::CalcAllJacobian();
}

void PHIKBallJoint::CalcAxis(){
	e[0] = Vec3d(1,0,0);
	e[1] = Vec3d(0,1,0);
	e[2] = Vec3d(0,0,1);

	for(CSetIter ctlpt=linkedControlPoints.begin(); ctlpt!=linkedControlPoints.end(); ++ctlpt){
		if (! DCAST(PHIKControlPoint,*ctlpt)->isEnabled) { continue; }

		PHIKPosCtlIf* cpPos;
		if (cpPos = DCAST(PHIKPosCtlIf,(*ctlpt))){
			// 関節の回転中心
			PHBallJoint* j = DCAST(PHBallJoint,joint);
			PHBallJointDesc d; j->GetDesc(&d);
			Vec3d Pj = j->solid[0]->GetPose() * d.poseSocket * Vec3d(0,0,0);

			// エンドエフェクタ位置
			PHIKPosCtl* cp = DCAST(PHIKPosCtl,cpPos);
			Vec3d Pc = cp->solid->GetPose() * cp->pos;

			// 関節回転中心<->エンドエフェクタ 軸
			Vec3d e0 = (Pc - Pj);
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

			return;
		}
	}
}

PTM::VMatrixRow<double> PHIKBallJoint::CalcJacobian(PHIKControlPointIf* control){
	PHIKPosCtlIf* cpPos;
	if (cpPos = DCAST(PHIKPosCtlIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,ndof);

		// 関節の回転中心
		PHBallJoint* j = DCAST(PHBallJoint,joint);
		PHBallJointDesc d; j->GetDesc(&d);
		Vec3d Pj = j->solid[0]->GetPose() * d.poseSocket * Vec3d(0,0,0);

		// エンドエフェクタ位置
		PHIKPosCtl* cp = DCAST(PHIKPosCtl,cpPos);
		Vec3d Pc = cp->solid->GetPose() * cp->pos;

		// ヤコビアンを求める
		for (int i=0; i<ndof; ++i) {
			Vec3d v = PTM::cross(e[i],(Pc-Pj));
			M[0][i]=v[0]; M[1][i]=v[1]; M[2][i]=v[2];
		}

		return M;
	}

	PHIKOriCtlIf* cpOri;
	if (cpOri = DCAST(PHIKOriCtlIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,3); M.clear();

		for (int i=0; i<3; ++i) {
			for (int j=0; j<ndof; ++j) {
				if (i==j) {
					M[i][j]=1;
				} else {
					M[i][j]=0;
				}
			}
		}

		return M;
	}

	DSTR << "Invalid Control Point type for this Node" << std::endl;

	return PTM::VMatrixRow<double>();
}

void PHIKBallJoint::Move(){
	if (enableStat==ES_NONE) {
		return;
	}
	if (!isEnabled) {
		return;
	}

	// 回転軸ベクトルにする
	Vec3d dT = Vec3d();
	for (int i=0; i<ndof; ++i) {
		dT += (dTheta[i]/bias) * e[i];
	}

	// Axis-Angle表現にする
	double angle = dT.norm();
	Vec3d axis = dT;
	if (angle != 0) {
		axis = axis / angle;
	} else {
		axis = Vec3d(1,0,0);
	}

	// Quaternionにする
	Quaterniond dQ = Quaterniond::Rot(angle, axis);

	// 関節のローカル座標をもとめる
	PHBallJoint* j = DCAST(PHBallJoint,joint);
	PHBallJointDesc d; j->GetDesc(&d);
	Vec3d Pj = j->solid[0]->GetPose() * d.poseSocket * Vec3d(0,0,0);
	Quaterniond Qj = (j->solid[0]->GetPose() * d.poseSocket).Ori();

	// 関節のローカル座標系にする
	Quaterniond pos = Qj.Inv() * dQ * Qj * joint->GetPosition();

	Vec3d goal = pos.RotationHalf();
	Vec3d orig = jGoal.RotationHalf();

	Vec3d newGoal = (jSpring*orig + spring*goal) * (1/(jSpring + spring));

	pos = Quaterniond::Rot(newGoal.norm(), (newGoal.norm()!=0)?(newGoal.unit()):(Vec3d(1,0,0)));

	// 関節を動かす
	joint->SetSpring(jSpring + spring);
	joint->SetDamper(jDamper + damper);
	joint->SetGoal(pos);
	/*
	Vec3d vel = dT / DCAST(PHSceneIf,GetScene())->GetTimeStep();
	if (vel.norm() > Rad(360)) {
		vel = vel.unit() * Rad(360);
	}
	joint->SetMode(PHBallJointDesc::MODE_VELOCITY);
	joint->SetDesiredVelocity(vel);
	*/

	return;
}

void PHIKBallJoint::MoveNatural(){
	joint->SetSpring(jSpring);
	joint->SetDamper(jDamper);
	joint->SetGoal(jGoal);
}

void PHIKBallJoint::AddControlPoint(PHIKControlPointIf* control){
	if (DCAST(PHIKOriCtlIf,control)) {
		// 姿勢制御に荷担するボールジョイントは自由度を３に引き上げる
		// （デフォルトではエンドエフェクタの位置を変えない回転軸を除いた
		// 　２軸（ユニバーサルジョイント）になっている）
		SetNDOF(3);
	}
	PHIKNode::AddControlPoint(control);
}
// --- --- --- --- ---
PTM::VMatrixRow<double> PHIKHingeJoint::CalcJacobian(PHIKControlPointIf* control){
	PHIKPosCtlIf* cpPos;
	if (cpPos = DCAST(PHIKPosCtlIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,1);
		PHHingeJoint* j = DCAST(PHHingeJoint,joint);
		Vec3d Pm = j->solid[0]->GetPose() * j->Xj[0].r;
		Vec3d Rm = j->solid[0]->GetPose().Ori() * j->Xj[0].q * Vec3d(0,0,1);
		PHIKPosCtl* cp = DCAST(PHIKPosCtl,cpPos);
		Vec3d Pc = cp->solid->GetPose() * cp->pos;
		Vec3d M3 = PTM::cross((Pm-Pc), Rm);
		M[0][0]=M3[0]; M[0][1]=M3[1]; M[0][2]=M3[2];
		return M;
	}
 
	PHIKOriCtlIf* cpOri;
	if (cpOri = DCAST(PHIKOriCtlIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,1); return M; /// 適切なのを返す
	}

	return PTM::VMatrixRow<double>();
}

void PHIKHingeJoint::Move(){
	if (enableStat==ES_NONE) {
		return;
	}
	if (!isEnabled) {
		return;
	}

	static const double Pi = 3.141592653589;

	// 新しい回転角度
	double angle  = joint->GetPosition() + (dTheta[0]/bias);

	double newGoal = (jSpring*jGoal + spring*angle) * (1/(jSpring + spring));

	// トルクを実現するためのオフセットの追加
	/*
	double torque = tau[0];
	newGoal += torque * Rad(16) / joint->GetSpring();
	*/

	// 関節を動かす
	joint->SetSpring(jSpring + spring);
	joint->SetDamper(jDamper + damper);
	joint->SetSpringOrigin(newGoal);
}

void PHIKHingeJoint::MoveNatural(){
	joint->SetSpring(jSpring);
	joint->SetDamper(jDamper);
	joint->SetSpringOrigin(jGoal);
}

}

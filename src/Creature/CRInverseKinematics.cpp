/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRInverseKinematics.h"

#include "Physics/PHBallJoint.h"
#include "Physics/PHHingeJoint.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 制御点

Vec3d CRIKControlPos::GetTmpGoal(){
	Vec3d spos = solid->GetPose()*pos;
	Vec3d dir = goal - spos;
	double epsilon = 0.5;
	// std::cout << dir/dir.norm()*epsilon << std::endl;
	if (dir.norm() < epsilon) {
		// return(Vec3d(0,0,0));
		return(dir);
	} else {
		return(dir/dir.norm()*epsilon);
	}
}

// --- --- --- --- ---
Vec3d CRIKControlOri::GetTmpGoal(){
	Vec3d sorieul; solid->GetPose().Ori().ToEuler(sorieul);
	Vec3d dir = goal - sorieul;
	double epsilon = 0.2;
	// std::cout << dir/dir.norm()*epsilon << std::endl;
	if (dir.norm() < epsilon) {
		// return(Vec3d(0,0,0));
		return(dir);
	} else {
		return(dir/dir.norm()*epsilon);
	}
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 制御対象
Matrix3d CrossMatrix(Vec3d v){
	Matrix3d c = Matrix3d();
	c[0][1] = -v[2];
	c[0][2] =  v[1];
	c[1][0] =  v[2];
	c[1][2] = -v[0];
	c[2][0] = -v[1];
	c[2][1] =  v[0];
	return c;
}

void CRIKMovable::SetNDOF(int n){
	ndof = n;
	iDx.resize(ndof);
	iD.resize(ndof);
	F.resize(ndof, ndof);
	for(unsigned int i=0; i<K.size(); ++i){ K[i].resize(ndof, ndof); }
	value.resize(ndof);
}

void CRIKMovable::PrepareSolve(){
	PTM::VMatrixRow<double> JtJ; JtJ.resize(ndof,ndof); JtJ.clear();
	PTM::VVector<double> Jtx; Jtx.resize(ndof); Jtx.clear();

	for(CSetIter it=linkedControls.begin(); it!=linkedControls.end(); ++it){
		PTM::VMatrixRow<double> J; J.resize(ndof,ndof);
		J = CalcJacobian(*it);
		// std::cout << "J : " << std::endl << J << std::endl;
		JtJ += J.trans() * J;
		Jtx += J.trans() * DCAST(CRIKControl,(*it))->GetTmpGoal();
		// std::cout << "Goal : " << DCAST(CRIKControl,(*it))->GetTmpGoal() << std::endl;
	}
	// std::cout << "JtJ : " << std::endl << JtJ << std::endl;

	for(int i=0; i<ndof; i++){
		iD[i]  = 1.0 / JtJ[i][i];
		iDx[i] = iD[i] * Jtx[i];
	}

	int m=0;
	for(MSetIter mi=linkedMovables.begin(); mi!=linkedMovables.end(); ++mi){
		K[m].clear();
		for(CSetIter ci=linkedControls.begin(); ci!=linkedControls.end(); ++ci){
			K[m] += (CalcJacobian(*ci).trans() * DCAST(CRIKMovable,(*mi))->CalcJacobian(*ci));
		}
		// std::cout << "K[" << m << "] : " << std::endl << K[m] << std::endl;
		++m;
	}
	// std::cout << "iDx : " << iDx << std::endl;
	// std::cout << "iD  : " << iD  << std::endl;
	// std::cout << "F   : " << std::endl << F << std::endl;

	value.clear();
}

void CRIKMovable::ProceedSolve(){
	PTM::VVector<double> Kv; Kv.resize(ndof); Kv.clear();
	int m=0;
	for(MSetIter mi=linkedMovables.begin(); mi!=linkedMovables.end(); ++mi){
		Kv += (K[m] * DCAST(CRIKMovable,(*mi))->value);
		++m;
	}
	PTM::VVector<double> B; B.resize(ndof); B.clear();
	B = F*value + Kv;
	for(int i=0; i<ndof; i++){ B[i] *= iD[i]; }
	value = iDx - B;
}

void CRIKMovable::AddIKControl(CRIKControlIf* control){
	MSet* lm = &(DCAST(CRIKControl,control)->linkedMovables);
	for(MSetIter it=lm->begin(); it!=lm->end(); ++it){
		linkedMovables.insert(*it);
	}
	DCAST(CRIKControl,control)->linkedMovables.insert(this->Cast());
	linkedControls.insert(control);
	K.resize(linkedMovables.size());
	for(unsigned int i=0; i<K.size(); ++i){ K[i].resize(ndof, ndof); }
}

PTM::VMatrixRow<double> CRIKMovable::CalcJacobian(CRIKControlIf* control){
	return PTM::VMatrixRow<double>();
}

// --- --- --- --- ---
PTM::VMatrixRow<double> CRIKMovableSolidPos::CalcJacobian(CRIKControlIf* control){
	CRIKControlPosIf* cpPos;
	if (cpPos = DCAST(CRIKControlPosIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,3); M.clear();
		M[0][0]=1; M[1][1]=1; M[2][2]=1;
		return M;
	}
 
	CRIKControlOriIf* cpOri;
	if (cpOri = DCAST(CRIKControlOriIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,3);
		Vec3d Pm = solid->GetPose().Pos();
		CRIKControlOri* cp = DCAST(CRIKControlOri,cpOri);
		Vec3d Pc = cp->solid->GetPose().Pos();
		double norm = (Pm-Pc).norm();
		M = CrossMatrix((Pm-Pc)/(norm*norm));
		return M;
	}

	return PTM::VMatrixRow<double>();
}

// --- --- --- --- ---
PTM::VMatrixRow<double> CRIKMovableSolidOri::CalcJacobian(CRIKControlIf* control){
	CRIKControlPosIf* cpPos;
	if (cpPos = DCAST(CRIKControlPosIf,control)){
		 PTM::VMatrixRow<double> M; M.resize(3,3); return M; /// 適切なのを返す
	}
 
	CRIKControlOriIf* cpOri;
	if (cpOri = DCAST(CRIKControlOriIf,control)){
		 PTM::VMatrixRow<double> M; M.resize(3,3); return M; /// 適切なのを返す
	}

	return PTM::VMatrixRow<double>();
}

// --- --- --- --- ---
PTM::VMatrixRow<double> CRIKMovableBallJointOri::CalcJacobian(CRIKControlIf* control){
	CRIKControlPosIf* cpPos;
	if (cpPos = DCAST(CRIKControlPosIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,3);
		PHBallJoint* j = DCAST(PHBallJoint,joint);
		PHBallJointDesc d; j->GetDesc(&d);
		Vec3d Pm = j->solid[0]->GetPose() * d.poseSocket * Vec3d(0,0,0);
		CRIKControlPos* cp = DCAST(CRIKControlPos,cpPos);
		Vec3d Pc = cp->solid->GetPose() * cp->pos;
		M = CrossMatrix(Pm-Pc);
		return M;	 
	}

	CRIKControlOriIf* cpOri;
	if (cpOri = DCAST(CRIKControlOriIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,3); M.clear();
		M[0][0]=1; M[1][1]=1; M[2][2]=1;
		return M;
	}

	return PTM::VMatrixRow<double>();
}

// --- --- --- --- ---
PTM::VMatrixRow<double> CRIKMovable3HingeJointOri::CalcJacobian(CRIKControlIf* control){
	CRIKControlPosIf* cpPos;
	if (cpPos = DCAST(CRIKControlPosIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,3);
		PHHingeJoint* j[3] = {DCAST(PHHingeJoint,joint1),DCAST(PHHingeJoint,joint2),DCAST(PHHingeJoint,joint3)};
		for(int i=0; i<3; i++){
			PTM::VMatrixRow<double> J; J.resize(3,1);
			Vec3d Pm = j[i]->solid[0]->GetPose() * j[i]->Xj[0].r;
			Vec3d Rm = j[i]->solid[0]->GetPose().Ori() * j[i]->Xj[0].q * Vec3d(0,0,1);
			CRIKControlPos* cp = DCAST(CRIKControlPos,cpPos);
			Vec3d Pc = cp->solid->GetPose() * cp->pos;
			M[i] = PTM::cross((Pm-Pc), Rm);
			// std::cout << "Pc[" << i << "]: " << Pc << std::endl;
		}
		// std::cout << "M3hinge:" << std::endl << M << std::endl;
		return M;
	}
 
	CRIKControlOriIf* cpOri;
	if (cpOri = DCAST(CRIKControlOriIf,control)){
		 PTM::VMatrixRow<double> M; M.resize(3,3); return M; /// 適切なのを返す
	}

	return PTM::VMatrixRow<double>();
}

void CRIKMovable3HingeJointOri::Move(){
	static const double PI     = 3.141592653589;
	static const double HALFPI = PI / 2.0;
	if (value.norm() < 100.0f) {
		/*
		for (int i=0; i<3; ++i) {
			while (HALFPI  < value[i]) { value[i] -= PI; }
			while (value[i] < -HALFPI) { value[i] += PI; }
		}
		*/
		joint1->SetSpringOrigin(joint1->GetPosition() + value[0]);
		joint2->SetSpringOrigin(joint2->GetPosition() + value[1]);
		joint3->SetSpringOrigin(joint3->GetPosition() + value[2]);
	}
}

// --- --- --- --- ---
PTM::VMatrixRow<double> CRIKMovableHingeJointOri::CalcJacobian(CRIKControlIf* control){
	CRIKControlPosIf* cpPos;
	if (cpPos = DCAST(CRIKControlPosIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,1);
		PHHingeJoint* j = DCAST(PHHingeJoint,joint);
		Vec3d Pm = j->solid[0]->GetPose() * j->Xj[0].r;
		Vec3d Rm = j->solid[0]->GetPose().Ori() * j->Xj[0].q * Vec3d(0,0,1);
		CRIKControlPos* cp = DCAST(CRIKControlPos,cpPos);
		Vec3d Pc = cp->solid->GetPose() * cp->pos;
		Vec3d M3 = PTM::cross((Pm-Pc), Rm);
		M[0][0]=M3[0]; M[0][1]=M3[1]; M[0][2]=M3[2];
		return M;
	}
 
	CRIKControlOriIf* cpOri;
	if (cpOri = DCAST(CRIKControlOriIf,control)){
		 PTM::VMatrixRow<double> M; M.resize(3,1); return M; /// 適切なのを返す
	}

	return PTM::VMatrixRow<double>();
}

void CRIKMovableHingeJointOri::Move(){
	static const double Pi = 3.141592653589;
	double angle = joint->GetPosition() + value[0];
	while (angle > +2*Pi) { angle -= 2*Pi; }
	while (angle < -2*Pi) { angle += 2*Pi; }
	joint->SetSpringOrigin(angle);
}

}

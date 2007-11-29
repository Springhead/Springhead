/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRInverseKinematics.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 制御点
IF_OBJECT_IMP(CRIKControl, SceneObject);

IF_OBJECT_IMP(CRIKControlPos, CRIKControl);
IF_OBJECT_IMP(CRIKControlOri, CRIKControl);

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 制御対象
IF_OBJECT_IMP(CRIKMovable, SceneObject);

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
	for(int i=0; i<K.size(); ++i){ K[i].resize(ndof, ndof); }
	value.resize(ndof);
}

void CRIKMovable::PrepareSolve(){
	PTM::VMatrixRow<double> JtJ; JtJ.resize(ndof,ndof); JtJ.clear();
	PTM::VVector<double> Jtx; Jtx.resize(ndof); Jtx.clear();

	for(CSetIter it=linkedControls.begin(); it!=linkedControls.end(); ++it){
		PTM::VMatrixRow<double> J; J.resize(ndof,ndof);
		J = CalcJacobian(*it);
		JtJ += J.trans() * J;
		Jtx += J.trans() * DCAST(CRIKControl,(*it))->GetGoal();
	}

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
		++m;
	}
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
	for(int i=0; i<K.size(); ++i){ K[i].resize(ndof, ndof); }
}

PTM::VMatrixRow<double> CRIKMovable::CalcJacobian(CRIKControlIf* control){
	return PTM::VMatrixRow<double>();
}

// --- --- --- --- ---
IF_OBJECT_IMP(CRIKMovableSolidPos, CRIKMovable);

PTM::VMatrixRow<double> CRIKMovableSolidPos::CalcJacobian(CRIKControlIf* control){
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
IF_OBJECT_IMP(CRIKMovableSolidOri, CRIKMovable);

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
IF_OBJECT_IMP(CRIKMovableBallJointOri, CRIKMovable);

PTM::VMatrixRow<double> CRIKMovableBallJointOri::CalcJacobian(CRIKControlIf* control){
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
IF_OBJECT_IMP(CRIKMovable3HingeJointOri, CRIKMovable);

PTM::VMatrixRow<double> CRIKMovable3HingeJointOri::CalcJacobian(CRIKControlIf* control){
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
IF_OBJECT_IMP(CRIKMovableHingeJointOri, CRIKMovable);

PTM::VMatrixRow<double> CRIKMovableHingeJointOri::CalcJacobian(CRIKControlIf* control){
	CRIKControlPosIf* cpPos;
	if (cpPos = DCAST(CRIKControlPosIf,control)){
		 PTM::VMatrixRow<double> M; M.resize(3,1); return M; /// 適切なのを返す
	}
 
	CRIKControlOriIf* cpOri;
	if (cpOri = DCAST(CRIKControlOriIf,control)){
		 PTM::VMatrixRow<double> M; M.resize(3,1); return M; /// 適切なのを返す
	}

	return PTM::VMatrixRow<double>();
}

}

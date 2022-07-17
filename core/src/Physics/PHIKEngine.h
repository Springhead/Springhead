/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PH_IKENGINE_H
#define PH_IKENGINE_H

#include <Physics/SprPHIK.h>
#include <Physics/SprPHEngine.h>
#include "PHEngine.h"

#include <Foundation/UTClapack.h>

namespace Spr{;

class PHIKActuator;
class PHIKEndEffector;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// IKエンジン
// 

class PHIKEngine : public PHEngine , public PHIKEngineDesc {
public:
	SPR_OBJECTDEF(PHIKEngine);
	ACCESS_DESC(PHIKEngine);

	// --- --- --- --- ---

	std::vector< UTRef<PHIKActuator> >		actuators;
	std::vector< UTRef<PHIKEndEffector> >	endeffectors;

	// --- --- --- --- ---

	std::vector< int >        strideAct;
	std::vector< int >        strideEff;

	// --- --- --- --- ---

	// uBLAS行列型
	typedef double element_type;
	typedef ublas::vector< element_type >                               vector_type;
	typedef ublas::matrix< element_type, ublas::column_major >          matrix_type;
	typedef ublas::diagonal_matrix< element_type, ublas::column_major > diag_matrix_type;

	// 全体ヤコビアン
	matrix_type J;
	//ublas::mapped_matrix<double> J;

	// 全エンドエフェクタ目標変位
	vector_type V;

	// 全エンドエフェクタ目標力
	vector_type F; //add here

	// 全関節角引き戻し変位
	vector_type Wp;

	// 全関節角変位（IK計算結果）
	vector_type W;
	
	// 全関節角トルク（IK計算結果）
	vector_type T; //add here

	// ラグランジェ乗数
	vector_type l;
	vector_type Vl;
	//matrix_type Jl;
	ublas::mapped_matrix<double> Jl;

	// --- --- --- --- ---

	int     lastM, lastN;
	double  iterCutOffAngVel;

	// --- --- --- --- ---

	int constraintChangedIntpMax = 50;
	int constraintChangedIntpRate;

	// --- --- --- --- --- --- --- --- --- ---

	PHIKEngine(const PHIKEngineDesc& desc = PHIKEngineDesc());

	int  GetPriority() const {return SGBP_INVERSEKINEMATICS;}
	void Step();
	void Clear();
	bool IsEnabled()                          { return this->bEnabled; }
	void Enable(bool bEnabled)                { this->bEnabled = bEnabled; }
	void SetNumIter(int numIter)              { this->numIter  = numIter; }
	int GetNumIter()                          { return (int)(this->numIter); }
	void SetMaxVelocity(double maxVel)        { this->maxVel = maxVel; }
	double GetMaxVelocity()                   { return maxVel; }
	void SetMaxAngularVelocity(double maxAV)  { this->maxAngVel = maxAV; }
	double GetMaxAngularVelocity()            { return maxAngVel; }
	void SetMaxActuatorVelocity(double maxAV) { this->maxActVel = maxAV; }
	double GetMaxActuatorVelocity()           { return maxActVel; }
	void SetRegularizeParam(double epsilon)   { this->regularizeParam = epsilon; }
	double GetRegularizeParam()               { return regularizeParam; }
	void SetIterCutOffAngVel(double epsilon)  { this->iterCutOffAngVel = epsilon; }
	double GetIterCutOffAngVel()              { return iterCutOffAngVel; }

	void SetIntpRate() { constraintChangedIntpRate = constraintChangedIntpMax; }
	int GetIntpRate() { return constraintChangedIntpRate; }

	// --- --- --- --- ---

	void ApplyExactState(bool reverse=false);

	void Prepare(bool second = false);
	void CalcJacobian();
	void IK(bool nopullback = false);
	void LQIK(bool nopullback = false);
	void LagrangeMultiplierIK(bool nopullback = false);
	void Limit();
	void FK();
	void SaveFKResult();
	void Move();

	// --- --- --- --- ---

	PHIKActuator*		CreateIKActuator(const IfInfo* ii, const PHIKActuatorDesc& desc);
	PHIKEndEffector*	CreateIKEndEffector(const PHIKEndEffectorDesc& desc);

	virtual bool		DelChildObject(ObjectIf* o);
	virtual bool		AddChildObject(ObjectIf* o);
};

}

#endif

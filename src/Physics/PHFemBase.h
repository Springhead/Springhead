/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef PH_FEM_BASE_H
#define PH_FEM_BASE_H

#include "Physics/PHFemMeshNew.h"
#include "Foundation/Object.h"
#include "PHScene.h"

namespace Spr{;

/// 計算モジュールの共通部分
class PHFemBase: public SceneObject{
public:
	SPR_OBJECTDEF_ABST(PHFemBase);
	SPR_DECLMEMBEROF_PHFemBaseDesc;
protected:
	PHFemMeshNew* phFemMesh;
public:
	typedef VVector< double > VVectord;
	typedef VMatrixRow< double > VMatrixRd;
	PHFemBase():phFemMesh(NULL){}
	virtual void Init(){}
	virtual void Step(){}

	void SetPHFemMesh(PHFemMeshNew* m);
	PHFemMeshNew* GetPHFemMesh();
	int NVertices();

	/// 二階の時間積分
	/// _M:質量行列、_K:剛性行列、_C:減衰行列、_f:外力、_dt:積分刻み、_xd:変位、_v:速度
	/// 行列版
	virtual void CompInitialCondition(const VMatrixRd& _M, const VMatrixRd& _K, const VMatrixRd& _C,
		const VVectord& _f, VVectord& _x, VVectord& _v, VVectord& _a);
	// 前進オイラー
	virtual void InitExplicitEuler(const VMatrixRd& _M, VMatrixRd& _MInv);
	virtual void ExplicitEuler(const VMatrixRd& _MInv, const VMatrixRd& _K, const VMatrixRd& _C, 
		const VVectord& _f, const double& _dt, VVectord& _xd, VVectord& _v);
	// 後退オイラー
	//virtual void InitImplicitEuler(VMatrixRd& _M, double& _SInv);
	virtual void ImplicitEuler(const VMatrixRd& _M, const VMatrixRd& _K, const VMatrixRd& _C, 
		const VVectord& _f, const double& _dt, VVectord& _xd, VVectord& _v);
	// シンプレクティック
	virtual void InitSimplectic(const VMatrixRd& _M, VMatrixRd& _MInv);
	virtual void Simplectic(const VMatrixRd& _MInv, const VMatrixRd& _K, const VMatrixRd& _C, 
		const VVectord& _f, const double& _dt, VVectord& _xd, VVectord& _v);
	// NewmarkBeta
	virtual void InitNewmarkBeta(const VMatrixRd& _M, const VMatrixRd& _K, const VMatrixRd& _C, 
		const double& _dt, VMatrixRd& _SInv, const double b = 1.0/6.0);
	virtual void NewmarkBeta(const VMatrixRd& _SInv, const VMatrixRd& _K, const VMatrixRd& _C, 
		const VVectord& _f, const double& _dt, VVectord& _xd, VVectord& _v, VVectord& _a, const double b = 1.0 /6.0);
	/// 1自由度版
	// 前進オイラー
	virtual void InitExplicitEuler(const double& _m, double& _sInv);
	virtual void ExplicitEuler(const double& _sInv, const double& _k, const double& _c, 
		const double& _f, const double& _dt, double& _x, double& _v);
	// シンプレクティック
	virtual void InitSimplectic(const double& _m, double& _sInv);
	virtual void Simplectic(const double& _sInv, const double& _k, const double& _c, 
		const double& _f, const double& _dt, double& _x, double& _v);	
	// NewmarkBeta
	virtual void InitNewmarkBeta(const double& _m, const double& _k , const double& _c, const double & _dt, double& _sInv, const double b = 1.0/6.0);
	virtual void NewmarkBeta(const double& _sInv, const double& _k, const double& _c,
		const double& _f, const double& _dt, double& _x, double& _v, double& _a, const double b = 1.0/6.0);
};

}

#endif
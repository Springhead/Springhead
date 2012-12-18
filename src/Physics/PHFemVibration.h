/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef PH_FEM_VIBRATION_H
#define PH_FEM_VIBRATION_H

#include <Physics/PHFemMeshNew.h>
#include <Foundation/Object.h>

namespace Spr{;

class PHFemVibration : public PHFemVibrationDesc, public PHFem{
public:
	SPR_OBJECTDEF(PHFemVibration);
	ACCESS_DESC(PHFemVibration);

	double vdt;
	VMatrixRow< double > matK;		// 全体剛性行列
	VMatrixRow< double > matM;		// 全体質量行列
	VMatrixRow< double > matMInv;	// 全体質量行列の逆行列
	VMatrixRow< double > matC;		// 全体減衰行列
	VVector< double > xlocalInit;	// 初期頂点位置
	VVector< double > xlocal;		// 計算用の頂点位置(u = (x1, ..., xn-1, y1, ..., yn-1, z1, ..., zn-1)
	VVector< double > vlocal;		// 初期頂点速度
	VVector< double > flocal;		// 計算用の外力

	PHFemVibration(const PHFemVibrationDesc& desc = PHFemVibrationDesc());
	/// 初期化
	virtual void Init();
	/// シミュレーションステップ
	virtual void Step();

	/// 時間積分
	virtual void ExplicitEuler();
	virtual void ImplicitEuler();
	virtual void NewmarkBeta(const double b = 1.0/6.0);

	/// 各種設定
	void SetTimeStep(double dt){ vdt = dt; }
	double GetTimeStep(){ return vdt; }
	void SetYoungModulus(double value){ young = value; }
	double GetYoungModulus(){ return young; }
	void SetPoissonsRatio(double value){ poisson = value; }
	double GetPoissonsRatio(){ return poisson; }
	void SetDensity(double value) { density = value; }
	double GetDensity(){ return density; }
	void SetAlpha(double value){ alpha = value; }
	double GetAlpha(){ return alpha; }
	void SetBeta(double value){ beta = value; }
	double GetBeta(){ return beta; }
	// 境界条件を加える
	bool AddBoundaryCondition(int vtxId, Vec3i dof);
	// 境界条件を加える(頂点順）
	bool AddBoundaryCondition(VVector< Vec3i > bcs); 
	// 頂点に力を加える（ワールド座標系）
	bool AddVertexForce(int vtxId, Vec3d fW);
	// 頂点群に力を加える（ワールド座標系）
	bool AddVertexForce(VVector< Vec3d > fWs);
};

}

#endif
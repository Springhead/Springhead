/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprPHFemMeshNew.h
 *	@brief 剛体
*/
#ifndef SPR_PH_FEMMESH_NEW_IF_H
#define SPR_PH_FEMMESH_NEW_IF_H

#include <Foundation/SprObject.h>

/**	\addtogroup gpPhysics	*/
//@{
namespace Spr{;

struct PHSolidIf;
struct PHFemVibrationIf;

///	FemMeshのステート
struct PHFemMeshNewState{};
///	FemMeshのディスクリプタ
struct PHFemMeshNewDesc: public PHFemMeshNewState{
	std::vector<Vec3d> vertices;
	std::vector<int> tets;
	std::vector<int> faces;
};
/// PHFemMeshNewへのインタフェース
struct PHFemMeshNewIf : public SceneObjectIf{
	SPR_IFDEF(PHFemMeshNew);
	void SetPHSolid(PHSolidIf* s);
	PHSolidIf* GetPHSolid();
	PHFemVibrationIf* GetPHFemVibration();
	int NVertices();
	int NFaces();
	double GetTetrahedronVolume(int tetID);
	bool AddDisplacement(int vtxId, Vec3d disW);
	bool AddLocalDisplacement(int vtxId, Vec3d disL);
	bool SetVertexPosition(int vtxId, Vec3d posW);
	bool SetLocalVertexPosition(int vtxId, Vec3d posL);
};

/// Femのデスクリプタ
struct PHFemDesc{};
/// Femの共通計算部分
struct PHFemIf : public SceneObjectIf{
	SPR_IFDEF(PHFem);
};

/// 振動計算のデスクリプタ
struct PHFemVibrationDesc : public PHFemDesc{
	enum INTEGRATION_MODE{
		MODE_EXPLICIT_EULER,
		MODE_IMPLICIT_EULER,
		MODE_NEWMARK_BETA,
		MODE_MODAL_ANALYSIS
	};
	double young;		///< ヤング率(Pa, N/m2)
	double poisson;		///< ポアソン比(*一様な立方体の場合、-1 <= v <= 0.5)
	double density;		///< 密度(kg/m3)
	double alpha;		///< 粘性減衰率
	double beta;		///< 構造減衰率
	PHFemVibrationDesc(){
		// アルミの物性
		// ポアソン比:0.35,ヤング率 70GPa, 密度2.70g/cm3
		// 減衰比は適当に設定
		poisson = 0.35;
		young = 70 * 1e6;
		density =  2.7 * 1e3; 
		alpha = 0.001;
		beta = 0.0001;
	}
};

/// 振動計算
struct PHFemVibrationIf : public PHFemIf{
	SPR_IFDEF(PHFemVibration);
	void SetTimeStep(double dt);
	double GetTimeStep();
	void SetYoungModulus(double value);
	double GetYoungModulus();
	void SetPoissonsRatio(double value);
	double GetPoissonsRatio();
	void SetDensity(double value);
	double GetDensity();
	void SetAlpha(double value);
	double GetAlpha();
	void SetBeta(double value);
	double GetBeta();
	void SetIntegrationMode(PHFemVibrationDesc::INTEGRATION_MODE mode);
	// 境界条件を加える
	bool AddBoundaryCondition(int vtxId, Vec3i dof);
	// 境界条件を加える(頂点順）
	bool AddBoundaryCondition(PTM::VVector< Vec3i > bcs); 
	// 頂点に力を加える（ワールド座標系）
	bool AddVertexForce(int vtxId, Vec3d fW);
	// 頂点群に力を加える（ワールド座標系）
	bool AddVertexForce(PTM::VVector< Vec3d > fWs);
};

//@}

}	//	namespace Spr
#endif
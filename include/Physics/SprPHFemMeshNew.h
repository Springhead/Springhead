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
	double CompTetVolume(int tetID, bool bDeform);
	bool AddVertexDisplacementW(int vtxId, Vec3d disW);
	bool AddVertexDisplacementL(int vtxId, Vec3d disL);
	bool SetVertexPositionW(int vtxId, Vec3d posW);
	bool SetVertexPositionL(int vtxId, Vec3d posL);
};

/// Femのデスクリプタ
struct PHFemBaseDesc{};
/// Femの共通計算部分
struct PHFemBaseIf : public SceneObjectIf{
	SPR_IFDEF(PHFemBase);
};

/// 振動計算のデスクリプタ
struct PHFemVibrationDesc : public PHFemBaseDesc{
	enum ANALYSIS_MODE{
		ANALYSIS_DIRECT,
		ANALYSIS_MODAL
	};
	enum INTEGRATION_MODE{
		INT_EXPLICIT_EULER,
		INT_IMPLICIT_EULER,
		INT_SIMPLECTIC,
		INT_NEWMARK_BETA,
	};
	double young;		///< ヤング率(Pa, N/m2)
	double poisson;		///< ポアソン比(*一様な立方体の場合、-1 <= v <= 0.5)
	double density;		///< 密度(kg/m3)
	double alpha;		///< 粘性減衰率
	double beta;		///< 構造減衰率
	std::vector<int> fixedVertices;	///< 固定頂点のIDs
	PHFemVibrationDesc();
};

/// 振動計算
struct PHFemVibrationIf : public PHFemBaseIf{
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
	void SetAnalysisMode(PHFemVibrationDesc::ANALYSIS_MODE mode);
	void SetIntegrationMode(PHFemVibrationDesc::INTEGRATION_MODE mode);
	// 境界条件を加える
	bool AddBoundaryCondition(int vtxId, Vec3i dof);
	// 境界条件を加える(頂点順）
	bool AddBoundaryCondition(PTM::VVector< Vec3i > bcs); 
	// 頂点に力を加える（ワールド座標系）
	bool AddVertexForceW(int vtxId, Vec3d fW);
	// 頂点群に力を加える（ワールド座標系）
	bool AddVertexForceW(PTM::VVector< Vec3d > fWs);

	bool SetDampingRatio_Wood();
	bool SetDampingRatio_Aluminum();
	bool SetDampingRatio_Plastic();
	void SetbRecomp();
	void Init();
};

//@}

}	//	namespace Spr
#endif
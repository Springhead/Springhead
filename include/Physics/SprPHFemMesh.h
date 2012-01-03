/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprPHFemMesh.h
 *	@brief FEMシミュレーションのための四面体メッシュ
*/
#ifndef SPR_PHFemMeshIf_H
#define SPR_PHFemMeshIf_H

#include <Foundation/SprObject.h>

/**	\addtogroup gpPhysics	*/
//@{
namespace Spr{;

///	FemMeshのステート
struct PHFemMeshState{
};
///	FemMeshのディスクリプタ
struct PHFemMeshDesc: public PHFemMeshState{
	std::vector<Vec3d> vertices;
	std::vector<int> tets;
	std::vector<int> faces;
	PHFemMeshDesc();
	void Init();
};

///	FEM用の四面体メッシュ
struct PHFemMeshIf : public SceneObjectIf{
	SPR_IFDEF(PHFemMesh);
	//int GetSurfaceVertex(int id);
	//int NSurfaceVertices();
	//void SetVertexTc(int id,double temp);
	//Vec3d GetPose(unsigned id);
	//Vec3d GetSufVtxPose(unsigned id);
};

///	FemMeshThermoのディスクリプタ
struct PHFemMeshThermoDesc: public PHFemMeshDesc{
	double roh;		//	密度
	double thconduct;	//	熱伝導率
	double consts;		//	比熱
	double heat_trans;	//	熱伝達率
	PHFemMeshThermoDesc();
	void Init();
};

///	温度のFEM用のメッシュ
struct PHFemMeshThermoIf : public PHFemMeshIf{
	SPR_IFDEF(PHFemMeshThermo);
	int GetSurfaceVertex(int id);
	int NSurfaceVertices();
	void SetVertexTc(int id,double temp);
	void SetVertexTc(int id,double temp,double heatTrans);
	Vec3d GetPose(int id);
	Vec3d GetSufVtxPose(unsigned id);
	unsigned long GetStepCount();		///	カウント１
	unsigned long GetStepCountCyc();		///	カウント１が何週目か	計算式:TotalCount = GetStepCount() + GetStepCountCyc() * (1000 * 1000 * 1000) 
	void SetVertexTemp(unsigned id,double temp);
	void SetvecFAll(unsigned id,double dqdt);
	void SetRohSpheat(double roh,double Cp);		//素材固有の物性
};

//@}

}	//	namespace Spr
#endif

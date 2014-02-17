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
	//std::vector<Spr::PHFemMesh::Edge> GetEdge();
	
};

///	FemMeshThermoのディスクリプタ
struct PHFemMeshThermoDesc: public PHFemMeshDesc{
	double rho;						//	密度
	double thConduct;				//熱伝導率
	double thConduct_x;				//	x方向	熱伝導率
	double thConduct_y;				//	y方向	熱伝導率
	double thConduct_z;				//	z方向	熱伝導率
	double heatTrans;				//熱伝達率			//class 節点には、heatTransRatioが存在する
	double specificHeat;			//比熱
	double radiantHeat;				//熱輻射率（空気への熱伝達率）
	//float	distance[10];			//	中心からの距離	一つ目は0.0
	//float	ondo[10];				//	上記距離の温度
	double initial_temp;			//	均質な初期温度、
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
	unsigned long GetStepCount();					///	カウント１
	unsigned long GetStepCountCyc();				///	カウント１が何週目か	計算式:TotalCount = GetStepCount() + GetStepCountCyc() * (1000 * 1000 * 1000) 
	double GetVertexTemp(unsigned id);				// メッシュ節点の温度を取得
	double GetSufVertexTemp(unsigned id);			// メッシュ表面の節点温度を取得
	void SetVertexTemp(unsigned id,double temp);
	void SetVerticesTempAll(double temp);
	void AddvecFAll(unsigned id,double dqdt);		//セットだと、値をそう入れ替えしそうな名前で危険。実際には、add又は、IH加熱ベクトルのみにSetする。ベクトルにSetする関数を作って、ロードしてもいいと思う。
	void SetvecFAll(unsigned id,double dqdt);		//FAllの成分に加算だが、危険
	void SetRhoSpheat(double rho,double Cp);		//素材固有の物性
	unsigned GetNFace();
	std::vector<Vec3d> GetFaceEdgeVtx(unsigned id);
	Vec3d GetFaceEdgeVtx(unsigned id, unsigned	 vtx);
	Vec2d GetIHbandDrawVtx();
	void CalcIHdqdt_atleast(double r,double R,double dqdtAll,unsigned num);
	void UpdateIHheat(unsigned heating);	//	IH加熱状態の更新
	void UpdateVecF();						//	被加熱物体の熱流束リセット
	void UpdateVecF_frypan();				//	被加熱物体の熱流束リセット
	void DecrMoist();						//	
	void DecrMoist_velo(double vel);						//
	void DecrMoist_vel(double dt);						//
	void InitAllVertexTemp();
	void SetInitThermoConductionParam(
		double thConduct,		// thConduct:熱伝導率
		double rho,				// roh:密度
		double specificHeat,	// specificHeat:比熱 J/ (K・kg):1960
		double heatTrans		// heatTrans:熱伝達率 W/(m^2・K)
		);
	void SetParamAndReCreateMatrix(double thConduct0,double roh0,double specificHeat0);
	double GetArbitraryPointTemp(Vec3d temppos);			//	多分、未使用
	double GetVtxTempInTets(Vec3d temppos);					//	使用している関数	
	void InitVecFAlls();
	double Get_thConduct();
	bool SetConcentricHeatMap(std::vector<double> r, std::vector<double> temp, Vec2d origin);
	void SetThermalEmissivityToVerticesAll(double thermalEmissivity,double thermalEmissivity_const);
	// 熱放射同定関係
	void SetOuterTemp(double temp);
	void SetThermalRadiation(double ems,double ems_const);
	void SetGaussCalcParam(unsigned cyc,double epsilon);
	void InitTcAll(double temp);
	void InitToutAll(double temp);
	void SetWeekPow(double weekPow_);
	void SetIHParamWEEK(double inr_, double outR_, double weekPow_);
	void SetHeatTransRatioToAllVertex(double heatTransR_);
	void AfterSetDesc();												//全行列などを作り直す
	//void ReProduceMat_Vec_ThermalRadiation();
	void SetStopTimespan(double timespan);
	void UpdateMatk_RadiantHeatToAir();
};

//@}

}	//	namespace Spr
#endif

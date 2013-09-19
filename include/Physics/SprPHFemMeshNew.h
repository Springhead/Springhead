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
struct PHFemThermoIf;

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
	PHFemThermoIf* GetPHFemThermo();
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
#ifndef SWIG
	bool AddBoundaryCondition(int vtxId, Vec3i dof);
	// 境界条件を加える(頂点順）Add the voudnary conditions (vertex order)
	bool AddBoundaryCondition(std::vector< Vec3i >& bcs); 
	// 頂点に力を加える（ワールド座標系）  Applying force to the vertex (world coordinate system)
	bool AddVertexForceW(int vtxId, Vec3d fW);
	// 頂点群に力を加える（ワールド座標系） Applying force to a group of vertices (world coordinate system)
	bool AddVertexForceW(std::vector< Vec3d >& fWs);
#endif
	void SetbRecomp();	
	void Init();
};

///FemThermoのデスクリプタ
struct PHFemThermoDesc: public PHFemBaseDesc{
	double rho;						//	密度
	double thConduct;				//熱伝導率
	double heatTrans;				//熱伝達率			//class 節点には、heatTransRatioが存在する
	double specificHeat;			//比熱
	PHFemThermoDesc();
	void Init();
};

///	温度のFEM用のメッシュ
struct PHFemThermoIf : public PHFemBaseIf{
	SPR_IFDEF(PHFemThermo);
	void SetTimeStep(double dt);
	double GetTimeStep();
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
	//int NFaces();
	//std::vector<Vec3d> GetFaceEdgeVtx(unsigned id);
	//Vec3d GetFaceEdgeVtx(unsigned id, unsigned	 vtx);
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
	void InitVecFAlls();
	double GetVtxTempInTets(Vec3d temppos);
	void UpdateVertexTempAll();		//	計算結果としての温度をTVecAllから全節点に更新する
	float calcGvtx(std::string fwfood, int pv, unsigned texture_mode);

};

//@}

}	//	namespace Spr
#endif
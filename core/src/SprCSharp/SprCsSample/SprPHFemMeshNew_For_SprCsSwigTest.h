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
#include <stdio.h>

/**	\addtogroup gpPhysics	*/
//@{
namespace Spr{;

// SwigStructTest を実施するための定義
//
struct CSTestIntrinsic {
	// vector of intrinsic data type
	std::vector<int> vec_int;
	std::vector<int> vec_int2;
	std::vector<unsigned int> vec_unsigned_int;
	std::vector<size_t> vec_size_t;
	std::vector<float> vec_float;
	std::vector<double> vec_double;
	std::vector<std::string> vec_string;

	// array of intrinsic data type
	int ary_int[3];
	int ary_int2[3];
	unsigned int ary_unsigned_int[3];
	size_t ary_size_t[3];
	float ary_float[3];
	double ary_double[3];
	std::string ary_string[3];
	char* ary_char_p[3];
};
struct CSTestStructNested {
	Vec3d vec3d;
	std::vector<Vec3d> vec_vec3d;
	Vec3d ary_vec3d[3];
};
struct CSTestStruct {
	// struct in struct
	Vec3d vec3d;
	CSTestStructNested nested;

	// vector of struct in struct
	std::vector<Vec3d> vec_vec3d;
	std::vector<CSTestStructNested> vec_nested;

	// array of struct in struct
	Vec3d ary_vec3d[3];
	CSTestStructNested ary_nested[3];
};

struct PHSolidIf;
struct PHFemVibrationIf;
struct PHFemThermoIf;
struct PHFemPorousWOMoveIf;

///	FemMeshのステート
struct PHFemMeshNewState{};
///	FemMeshのディスクリプタ
struct PHFemMeshNewDesc: public PHFemMeshNewState{
	std::vector<Vec3d> vertices;
	std::vector<int> tets;
	std::vector<int> faces;
	bool spheric;   //For multiple FEM implementation
};
/// PHFemMeshNewへのインタフェース
struct PHFemMeshNewIf : public SceneObjectIf{
	SPR_IFDEF(PHFemMeshNew);
	void SetPHSolid(PHSolidIf* s);
	PHSolidIf* GetPHSolid();
	PHFemVibrationIf* GetPHFemVibration();
	PHFemThermoIf* GetPHFemThermo();
	PHFemPorousWOMoveIf* GetPHFemPorousWOMove();
	int NVertices();
	int NFaces();
	int NTets();
	void SetVertexUpdateFlags(bool flg);
	void SetVertexUpateFlag(int vid, bool flg);
	double CompTetVolume(int tetID, bool bDeform);
	bool AddVertexDisplacementW(int vtxId, Vec3d disW);
	bool AddVertexDisplacementL(int vtxId, Vec3d disL);
	bool SetVertexPositionW(int vtxId, Vec3d posW);
	bool SetVertexPositionL(int vtxId, Vec3d posL);
	bool SetVertexVelocityL(int vtxId, Vec3d posL);
	Vec3d GetVertexVelocityL(int vtxId);
	Vec3d GetVertexPositionL(int vtxId);
	Vec3d GetVertexDisplacementL(int vtxId);
	Vec3d GetVertexInitalPositionL(int vtxId);
	void SetVelocity(Vec3d v);
	int* GetTetVertexIds(int t);
	int* GetFaceVertexIds(int f);
	Vec3d GetFaceNormal(int f);
	int GetSurfaceVertex(int i);
	int NSurfaceVertices();
	int NSurfaceFace();
	bool CompTetShapeFunctionValue(const int& tetId, const Vec3d& posL, Vec4d& value, const bool& bDeform);
	/// 面から四面体を探す
	int FindTetFromFace(int faceId);
};

/// Femのデスクリプタ
struct PHFemBaseDesc{};
/// Femの共通計算部分
struct PHFemBaseIf : public SceneObjectIf{
	SPR_IFDEF(PHFemBase);
	PHFemMeshNewIf* GetPHFemMesh();
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
	void SetBoundary(int vtxIds);
	void ClearBoundary();
	std::vector<int>  GetBoundary();
	void SetAnalysisMode(PHFemVibrationDesc::ANALYSIS_MODE mode);
	void SetIntegrationMode(PHFemVibrationDesc::INTEGRATION_MODE mode);
	bool AddBoundaryCondition(int vtxId, Vec3i dof);
	void DeleteBoundaryCondition();
	bool FindNeighborTetrahedron(Vec3d posW, int& tetId, Vec3d& cpW, bool bDeform);
	bool SetDamping(int tetId, Vec3d posW, double damp_ratio);
	// 力を加える
	bool AddForce(int tetId, Vec3d posW, Vec3d fW);
	// 形状関数を使って任意の点の変位を取得する
	bool GetDisplacement(int tetId, Vec3d posW, Vec3d& disp, bool bDeform);
	// 形状関数を使って任意の点の速度を取得する
	bool GetVelocity(int tetId, Vec3d posW, Vec3d& vel, bool bDeform);
	// 形状関数を使って任意の点の位置(変化後)を取得する
	bool GetPosition(int tetId, Vec3d posW, Vec3d& pos, bool bDeform);
#ifndef SWIG
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
	//double thConduct;				//熱伝導率
	//double thConduct_x;				//	x方向	熱伝導率
	//double thConduct_y;				//	y方向	熱伝導率
	//double thConduct_z;				//	z方向	熱伝導率
	double heatTrans;				//熱伝達率			//class 節点には、heatTransRatioが存在する
	double specificHeat;			//比熱
	double radiantHeat;				//熱輻射率（空気への熱伝達率）
	//float	distance[10];			//	中心からの距離	一つ目は0.0
	//float	ondo[10];				//	上記距離の温度
	double initial_temp;			//	均質な初期温度、
	PHFemThermoDesc();
	void Init();
	double weekPow_full;
};

///	温度のFEM用のメッシュ
struct PHFemThermoIf : public PHFemBaseIf{
	SPR_IFDEF(PHFemThermo);
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
	Vec2d GetIHbandDrawVtx();
	void CalcIHdqdt_atleast(double r,double R,double dqdtAll,unsigned num);
	void UpdateIHheatband(double xS,double xE,unsigned heatingMODE);//小野原追加
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
	void ReCreateMatrix(double thConduct0);
	void ActivateVtxbeRadiantHeat();			//
	PTM::TMatrixRow<4,4,double> GetKMatInTet(unsigned id);
	void OutputMatKall();
	void IfRadiantHeatTrans();
	void IfRadiantHeatTransSteak();
	float calcGvtx(std::string fwfood, int pv, unsigned texture_mode);
	void SetTimeStep(double dt);
	Vec3d GetVertexNormal(unsigned vtxid);
	void SetVertexHeatTransRatio(unsigned vtxid, double heattransRatio);
	void SetVertexBeRadiantHeat(unsigned vtxid, bool flag);
	double GetVertexArea(unsigned vtxid);
	void SetVertexToofar(unsigned vtxid, bool tooFar);
	bool GetVertexToofar(unsigned vtxid);
	void SetVertexBeCondVtxs(unsigned vtxid, bool becondVtxs);
	void CreateVecFAll();
	void CalcFaceNormalAll();
	void CalcVertexNormalAll();
	void InitFaceNormalAll();
	void InitVertexNormalAll();
	void RevVertexNormalAll();
	void SetWeekPowFULL(double weekPow_full);
	void SetweekPow_FULL(double setweekPow_FULL);
	void setIhRatio(double a);
	void Setems(double setems);
	void Setems_steak(double setems_steak);
	void SetthConduct(double thConduct);
	double GetWeekPowFULL();
	Vec3d GetVertexPose(unsigned vtxid);
	void OutTetVolumeAll();
	int GetTetsV(unsigned tetid, unsigned vtxid);
	void VecFNegativeCheck();
	double GetVecFElem(unsigned vtxid);
	int GetTetVNums(unsigned id,unsigned num);
	double GetInitialTemp();
	void UpdateVertexTempAll();
	void SetThermoCameraScale(double minTemp,double maxTemp);
};


struct PHFemPorousWOMoveDesc: public PHFemBaseDesc{
	double wDiffAir;
	double K;		//浸透係数
	double kc;		//毛管ポテンシャルの係数
	double kp;		//毛管圧力の定数
	double alpha;	//毛管ポテンシャルの係数
	double gamma;	//毛管圧力の係数
	double rhoWater;	//水の密度(g/m^3)
	double rhoOil;	//油の密度(g/m^3)
	double rhowInit;	//含水率の初期値
	double rhooInit;	//含油率の初期値
	double evapoRate;	//蒸発速度定数
	double denatTemp;	//変性温度	頂点がこの温度に達すると、結合水を含水率ベクトルに加える
	double boundWaterRatio;	//結合水の割合 0.15～0.25
	double equilWaterCont;	//減率第1段乾燥期の平衡含水量
	double limitWaterCont;	//限界水分量 恒率乾燥期と減率第1段乾燥期の閾値
	double boundaryThick;	//境膜の厚さ
	double initMassAll;		//食材の初期質量
	double initWaterRatio;	//食材の質量に対する水分質量の初期値
	double initOilRatio;	//食材の質量に対する油質量の初期値
	double shrinkageRatio;	//タンパク質変性時の収縮率
	Vec3d top;
	Vec3d center;
	Vec3d bottom;
	PHFemPorousWOMoveDesc();
	void Init();
};

struct PHFemPorousWOMoveIf: public PHFemBaseIf{
	SPR_IFDEF(PHFemPorousWOMove);
	void SetTimeStep(double dt);
	double GetTimeStep();
	void UpdateVertexRhoWAll();
	void UpdateVertexRhoOAll();
	void SetVertexMw(unsigned vtxid, double mw);
	void SetVertexMo(unsigned vtxid, double mo);
	double GetVertexMw(unsigned vtxid);
	double GetVertexMo(unsigned vtxid);
	double GetVtxWaterInTets(Vec3d temppos);
	double GetVtxOilInTets(Vec3d temppos);
	double GetVertexRhoW(unsigned vtxid);
	double GetVertexRhoO(unsigned vtxid);
	float calcGvtx(std::string fwfood, int pv, unsigned texture_mode);
	void setGravity(Vec3d g);
	double decideWetValue(unsigned faceid);
	double GetVtxSaturation(unsigned vtxid);
};


//@}

}	//	namespace Spr
#endif

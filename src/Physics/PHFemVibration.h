/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef PH_FEM_VIBRATION_H
#define PH_FEM_VIBRATION_H

#include <Physics/PHFemBase.h>
#include <Foundation/Object.h>

namespace Spr{;
using namespace PTM;

class PHFemVibration : public PHFemVibrationDesc, public PHFemBase{
public:
	SPR_OBJECTDEF(PHFemVibration);
	ACCESS_DESC(PHFemVibration);
	typedef double element_type;
	typedef VMatrixRow< element_type > VMatrixRe;
	typedef VVector< double > VVectord;
	#define element_limit std::numeric_limits< element_type >::max_digits10 	// streamの精度

	double vdt;
	PHFemVibrationDesc::ANALYSIS_MODE analysis_mode;
	PHFemVibrationDesc::INTEGRATION_MODE integration_mode;
	bool bRecomp;		// 全体行列が再計算された場合のフラグ
	VMatrixRe matKIni;	// 全体剛性行列の初期値
	VMatrixRe matMIni;	// 全体質量行列の初期値
	VMatrixRe matCIni;	// 全体減衰行列の初期値
	VVectord xdl;		// 頂点変位(u = (u0, v0, w0, ..., un-1, vn-1, wn-1))
	VVectord vl;		// 頂点速度
	VVectord al;		// 頂点加速度（一部の積分ではつかわない）
	VVectord fl;		// 計算用の外力
	VMatrixRe matKp;	// 全体剛性行列の一部
	VMatrixRe matMp;	// 全体質量行列の一部
	VMatrixRe matCp;	// 全体減衰行列の一部
	VVectord xdlp;		// 頂点変位の一部(u = (u0, v0, w0, ..., un-1, vn-1, wn-1))
	VVectord vlp;		// 頂点速度の一部
	VVectord alp;		// 頂点加速度の一部
	VVectord flp;		// 計算用の外力の一部

	//モード座標系　 n:自由度、m:モード次数
	VVectord q;			// モード振動ベクトル(m)
	VVectord qv;		// モード振動速度ベクトル(m)
	VVectord qa;		// モード振動加速度ベクトル(m)
	VVectord qf;		// モード外力(m)
		
	VVectord evalue;	// 固有値(m)
	VMatrixRe evector;	// 固有ベクトル(n*m)
	VVectord ewrad;		// MK系の固有角振動数(m)
	VMatrixRe Mm;		// モード質量行列(m*m)
	VMatrixRe Km;		// モード剛性行列(m*m)
	VMatrixRe Cm;		// モード減衰行列(m*m)
	VMatrixRe SmInv;	// M,K,C行列が変化しない限り定数の行列(時間積分で使う)(m*m)


//	VMatrixRe evalue;	// 固有値
//	VMatrixRe evector;	// 固有ベクトル
	double dampingRatio[2];	// 減衰比
	int nMode;			// モード数

	VVector< int > boundary;	// 境界条件ID

	PHFemVibration(const PHFemVibrationDesc& desc = PHFemVibrationDesc());
	/// 初期化
	virtual void Init();
	virtual void CompStiffnessMatrix();
	virtual void CompMassMatrix();
	virtual void CompRayleighDampingMatrix();
	virtual void CompRayleighDampingMatrixByDampingRatio();

	/// シミュレーションステップ
	virtual void Step();
	// 時間積分方法のスイッチ（行列）
	virtual void InitNumericalIntegration(const VMatrixRe& _M, const VMatrixRe& _K, const VMatrixRe& _C, const double& _dt, VMatrixRe& _SInv);
	virtual void NumericalIntegration(const VMatrixRe& _M, const VMatrixRe& _K, const VMatrixRe& _C, 
		const VVectord& _f, const double& _dt, VVectord& _xd, VVectord& _v, VVectord& _a);
	// 時間積分方法のスイッチ(1自由度)
	virtual void InitNumericalIntegration(const double& _m, const double& _k, const double& _c, const double& _dt, double& _sInv);
	virtual void NumericalIntegration(const double& _sInv, const double& _k, const double& _c, 
		const double& _f, const double& _dt, double& _x, double& _v, double& _a);
	virtual void ModalAnalysis(const VMatrixRe& _M, const VMatrixRe& _K, const VMatrixRe& _C, 
		const VVectord& _f, const double& _dt, bool& bFirst, VVectord& _xd, VVectord& _v, VVectord& _a, const int nmode);
	virtual void InitModalAnalysis(const VMatrixRe& _M, const VMatrixRe& _K, const VMatrixRe& _C, 
		const VVectord& _f, const double& _dt, bool& bFirst, VVectord& _xd, VVectord& _v, VVectord& _a, const int nmode);


	/// 固有値解析(Kx = lamda M x)を解く
	virtual void CompEigenValue(const VMatrixRd& _M, const VMatrixRd& _K, const int start, const int interval, VVectord& e, VMatrixRd& v);
	virtual void CompEigenVibrationFrequency(const VVectord& e, VVectord& w);
	virtual void CompEigenAngularVibrationFrequency(const VVectord& e, VVectord& wrad);
	double CompModalDampingRatio(double wrad);
	void CompRayleighDampingCoeffcient(double wrad[2], double ratio[2], double& a, double& b);
	// Subspace法の実装
	virtual void SubSpace(const VMatrixRe& _K, const VMatrixRe& _M, 
							const int nmode, const double epsilon, VVectord& e, VMatrixRe& v);

	/// 各種設定、計算
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
	void SetBoundary(int value){fixedVertices.push_back(value);}
	std::vector<int> GetBoundary(){return fixedVertices;} 

	void SetbRecomp(){ bRecomp=true; }
	void SetAnalysisMode(PHFemVibrationDesc::ANALYSIS_MODE mode);
	void SetIntegrationMode(PHFemVibrationDesc::INTEGRATION_MODE mode);

	// FemVertexから頂点変位を取得し、計算できる形に変換する
	void GetVerticesDisplacement(VVectord& _xd);
	// FemVertexに頂点変位を加える
	void UpdateVerticesPosition(VVectord& _xd);
	// FemVertexに頂点速度を加える
	void UpdateVerticesVelocity(VVectord& _v);

	std::vector< int > FindVertices(const int vtxId, const Vec3d _vecl);
	// 境界条件を加える(行列と番号）
	bool AddBoundaryCondition(VMatrixRe& mat, const int id);
	// 境界条件を加える(頂点）
	bool AddBoundaryCondition(const int vtxId, const Vec3i dof);
	//境界条件を消す(頂点)
	void DeleteBoundaryCondition(){fixedVertices.pop_back();}
	// 境界条件を加える(頂点順）
	bool AddBoundaryCondition(const std::vector< Vec3i >& bcs);
	// 境界条件に応じて行列の自由度を削減する
	void ReduceMatrixSize(VMatrixRe& mat, const VVector< int >& bc);
	// 境界条件に応じて行列自由度を削減する
	void ReduceMatrixSize(VMatrixRe& _M, VMatrixRe& _K, VMatrixRe& _C, const VVector< int >& bc);
	// 境界条件に応じてベクトルの自由度を削減する
	void ReduceVectorSize(VVectord& r, const VVector< int >& bc);
	// 境界条件に応じてベクトルの自由度を削減する
	void ReduceVectorSize(VVectord& _xd, VVectord& _v, VVectord& _a, VVectord& _f,const VVector< int >& bc);
	// 削減したベクトルを元のベクトルに戻す
	void AssignVector(const VVectord& from, VVectord& to, const VVector< int >& bc);

	// 頂点に力を加える（ローカル座標系）
	bool AddVertexForceL(int vtxId, Vec3d fL);
	// 頂点に力を加える（ワールド座標系）
	bool AddVertexForceW(int vtxId, Vec3d fW);
	// 頂点群に力を加える（ワールド座標系）
	bool AddVertexForceW(std::vector< Vec3d > fWs);
	// 力を加える
	bool AddForce(int tetId, Vec3d posW, Vec3d fW);
	// 振動を抑える
	bool Damping(int tetId, Vec3d posW, double damp_ratio);
	// 減衰係数の設定
	bool SetDampingRatio();

	// 形状関数を使って任意の点の変位を取得する
	bool GetDisplacement(int tetId, Vec3d posW, Vec3d& disp, bool bDeform);

	// 形状関数を使って任意の点の速度を取得する
	bool GetVelocity(int tetId, Vec3d posW, Vec3d& vel, bool bDeform);

	/// 実装中
	bool FindClosestPointOnMesh( const Vec3d& posW, const Vec3d pos[3], Vec3d& cp, double& dist, bool bDeform);
	/// ある点から近い面と面上の点を探す
	bool FindNeighborFaces(Vec3d posW, std::vector< int >& faceIds, std::vector< Vec3d >& cpWs, bool bDeform);
	/// ある点から近い四面体と四面体上の点を探す
	bool FindNeighborTetrahedron(Vec3d posW, int& tetId, Vec3d& cpW, bool bDeform);

	/// 行列のファイル出力
	void MatrixFileOut(VMatrixRe mat, std::string filename);
};

}

#endif
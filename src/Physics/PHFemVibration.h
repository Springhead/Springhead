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
	typedef double element_type;
	typedef VMatrixRow< element_type > VMatrixRe;
	typedef VVector< double > VVectord;
	#define element_limit std::numeric_limits< element_type >::max_digits10 	// streamの精度

	double vdt;
	PHFemVibrationDesc::INTEGRATION_MODE integration_mode;
	VMatrixRe matKIni;	// 全体剛性行列の初期値
	VMatrixRe matMIni;	// 全体質量行列の初期値
	VMatrixRe matMInv;	// 全体質量行列の逆行列
	VMatrixRe matCIni;	// 全体減衰行列の初期値
	VVectord xdl;		// 頂点変位(u = (u0, v0, w0, ..., un-1, vn-1, wn-1))
	VVectord vl;		// 頂点速度
	VVectord fl;		// 計算用の外力
	VMatrixRe matKp;	// 全体剛性行列の一部
	VMatrixRe matMp;	// 全体質量行列の一部
	VMatrixRe matCp;	// 全体減衰行列の一部
	VVectord xdlp;		// 頂点変位の一部(u = (u0, v0, w0, ..., un-1, vn-1, wn-1))
	VVectord vlp;		// 頂点速度の一部
	VVectord flp;		// 計算用の外力の一部

	VVector< int > boundary;	// 境界条件ID

	PHFemVibration(const PHFemVibrationDesc& desc = PHFemVibrationDesc());
	/// 初期化
	virtual void Init();
	//virtual void CompStiffnessMatrix();
	//virtual void CompMassMatrix();
	//virtual void CompDampingMatrix();
	/// シミュレーションステップ
	virtual void Step();

	/// 時間積分
	/// _M:質量行列、_K:剛性行列、_C:減衰行列、_f:外力、_dt:積分刻み、_xd:変位、_v:速度
	virtual void ExplicitEuler(const VMatrixRe& _MInv, const VMatrixRe& _K, const VMatrixRe& _C, 
		const VVectord& _f, const double& _dt, VVectord& _xd, VVectord& _v);
	virtual void ImplicitEuler(const VMatrixRe& _MInv, const VMatrixRe& _K, const VMatrixRe& _C, 
		const VVectord& _f, const double& _dt, VVectord& _xd, VVectord& _v);
	virtual void NewmarkBeta(const VMatrixRe& _M, const VMatrixRe& _K, const VMatrixRe& _C, 
		const VVectord& _f, const double& _dt, VVectord& _xd, VVectord& _v, const double b = 1.0 /6.0);
	virtual void ModalAnalysis(const VMatrixRe& _M, const VMatrixRe& _K, const VMatrixRe& _C, 
		const VVectord& _f, const double& _dt, VVectord& _xd, VVectord& _v, const int nmode);

	/// 固有値解析
	virtual void SubSpace(const VMatrixRe& _M, const VMatrixRe& _K, 
							const int nmode, const double epsilon, VVectord& e, VMatrixRe& v);

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
	void SetIntegrationMode(PHFemVibrationDesc::INTEGRATION_MODE mode);

	// FemVertexから頂点変位を取得し、計算できる形に変換する
	void GetVerticesDisplacement(VVectord& _xd);
	// FemVertexに頂点変位を加える
	void UpdateVerticesPosition(VVectord& _xd);

	std::vector< int > FindVertices(const int vtxId, const Vec3d _vecl);
	// 境界条件を加える(行列と番号）
	bool AddBoundaryCondition(VMatrixRe& mat, const int id);
	// 境界条件を加える(頂点）
	bool AddBoundaryCondition(const int vtxId, const Vec3i dof);
	// 境界条件を加える(頂点順）
	bool AddBoundaryCondition(const VVector< Vec3i > bcs);
	// 境界条件を反映させるための行列を計算する
	void CompBoundaryMatrix(VMatrixRe& _L, VMatrixRe& _R, const VVector< int > bc);
	// 境界条件に応じて行列の自由度を削減する
	void ReduceMatrixSize(VMatrixRe& mat, const VVector< int > bc);
	// 境界条件に応じて行列自由度を削減する
	void ReduceMatrixSize(VMatrixRe& _M, VMatrixRe& _K, VMatrixRe& _C, const VVector< int > bc);
	// 境界条件に応じてベクトルの自由度を削減する
	void ReduceVectorSize(VVectord& r, const VVector< int > bc);
	// 境界条件に応じてベクトルの自由度を削減する
	void ReduceVectorSize(VVectord& _xd, VVectord& _v, VVectord& _f,const VVector< int > bc);
	// ベクトルの自由度を元に戻す
	void GainVectorSize(VVectord& r, const VVector< int > bc);
	// ベクトルの自由度を元に戻す
	void GainVectorSize(VVectord& _xd, VVectord& _v, const VVector< int > bc);

	// 頂点に力を加える（ローカル座標系）
	bool AddVertexForceL(int vtxId, Vec3d fL);
	// 頂点に力を加える（ワールド座標系）
	bool AddVertexForceW(int vtxId, Vec3d fW);
	// 頂点群に力を加える（ワールド座標系）
	bool AddVertexForceW(VVector< Vec3d > fWs);
	// 力を加える
	void AddForce(Vec3d fW, Vec3d posW);

	/// scilabデバック
	bool IsScilabStarted;	/// scilabがスタートしているかどうかのフラグ
	/// scilabで読み込ませるdat形式ファイルを出力
	template < class AD >
	void ScilabFileOut(PTM::MatrixImp<AD>& a, const std::string filename = "scimat.dat"){
		if(!IsScilabStarted){
			DSTR << "Scilab has not started" << std::endl;
			return;
		}		
		ScilabJob("clear;");
		ScilabSetMatrix("A", a);
		std::stringstream str;
		str << "fprintfMat('" << filename << "', A, '%Lf');";
		ScilabJob(str.str().c_str());
	}

	/// 行列式計算
	template < class AD >
	void ScilabDeterminant(PTM::MatrixImp<AD>& a, const std::string name = ""){
		DSTR << "////////////////////////////////////////////////////////////////////////////////////////" << std::endl;
		DSTR << "Scilab Determinant Start." << std::endl;	
		DSTR << "det(" << name << ") springhead2 : " << a.det() << std::endl;
		if(!IsScilabStarted){
			DSTR << "Scilab has not started" << std::endl;
			return;
		}
		ScilabJob("clear;");
		ScilabSetMatrix("A", a);
		ScilabJob("detA = det(A);");
		DSTR << "Determinant of scilab is written in console." << std::endl;
		std::cout << "det("<< name << ") scilab : ";
		ScilabJob("disp(detA);");	
		DSTR << "Scilab Determinant End." << std::endl;	
		DSTR << "////////////////////////////////////////////////////////////////////////////////////////" << std::endl;
	}
	/// 固有値固有ベクトル計算
	void ScilabEigenValueAnalysis(VMatrixRe& _M, VMatrixRe& _K);

	/// 行列のファイル出力
	void MatrixFileOut(VMatrixRe mat, std::string filename);

	/// 実装中
	std::vector< int > FindNeigborTetrahedron(Vec3d pos);
	std::vector< int >  FindNeigborFaces(Vec3d pos);
};

}

#endif
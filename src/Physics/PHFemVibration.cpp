/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHFemVibration.h>
#include <SciLab/SprSciLab.h>

namespace Spr{;

//* 初期化と行列の作成
/////////////////////////////////////////////////////////////////////////////////////////
PHFemVibration::PHFemVibration(const PHFemVibrationDesc& desc){
	SetDesc(&desc);
	//integration_mode = PHFemVibrationDesc::MODE_EXPLICIT_EULER;
	integration_mode = PHFemVibrationDesc::MODE_NEWMARK_BETA;
	//integration_mode = PHFemVibrationDesc::MODE_MODAL_ANALYSIS;
	IsScilabStarted = false;
} 

void PHFemVibration::Init(){
	// Scilabの起動
	IsScilabStarted = ScilabStart();
	if(!IsScilabStarted){
		DSTR << "Scilab can not start." << std::endl;
	}

	DSTR << "Initializing PHFemVibration" << std::endl;
	/// 刻み時間の設定
	PHSceneIf* scene = GetPHFemMesh()->GetScene()->Cast();
	if(scene) vdt = scene->GetTimeStep();
	else vdt = 0.001;

	/// 全体剛性行列、全体質量行列、全体減衰行列の計算
	/// これらはすべてローカル系
	PHFemMeshNew* mesh = GetPHFemMesh();
	int NTets = mesh->tets.size();
	int NVer= NVertices();
	int NDof = NVer * 3;
	// 各全体行列の初期化
	matKIni.resize(NDof,NDof, 0.0);
	matKIni.resize(NDof, NDof, 0.0);
	matMIni.resize(NDof, NDof, 0.0);
	matMInv.resize(NDof, NDof, 0.0);
	matCIni.resize(NDof, NDof, 0.0);
	for(int i = 0; i < NTets; i++){
		// 要素行列の計算
		/// tetが持つ頂点順
		/// 要素剛性行列 u = (u0, u1, ..., u3, v0, v1, ..., v3, w0, w1, ..., w3)として計算
		Vec3d pos[4];
		for(int j = 0; j < 4; j++){
			pos[j] = mesh->vertices[mesh->tets[i].vertexIDs[j]].pos;
			DSTR << mesh->tets[i].vertexIDs[j] << std::endl;
		}
		/// 行列Cの計算（頂点座標に応じて変わる）
		PTM::TMatrixRow< 4,4,double > matCk1;	// matCkの1ブロック分
		for(int j = 0; j < 4; j++){
				matCk1.item(j, 0) = 1.0;
				matCk1.item(j, 1) = pos[j].x;
				matCk1.item(j, 2) = pos[j].y;
				matCk1.item(j, 3) = pos[j].z;
		}
		PTM::TMatrixRow< 4,4,double > matCk1Inv;
		matCk1Inv = matCk1.inv();
		PTM::TMatrixRow< 12, 12, double > matCkInv;
		matCkInv.clear(0.0);
		for(int j = 0; j < 4; j++){
			for(int k = 0; k < 4; k++){
				matCkInv[j][k] = matCk1Inv[j][k];
				matCkInv[j+4][k+4] = matCk1Inv[j][k];
				matCkInv[j+8][k+8] = matCk1Inv[j][k];
			}
		}

		/// 行列B（定数）
		PTM::TMatrixRow< 6,12,double > matB;
		matB.clear(0.0);
		matB[0][1] = 1.0;
		matB[1][6] = 1.0;
		matB[2][11] = 1.0;
		matB[3][2] = 1.0;	matB[3][5] = 1.0;
		matB[4][7] = 1.0;	matB[4][10] = 1.0;
		matB[5][3] = 1.0;	matB[5][9] = 1.0;

		/// 弾性係数行列Dの計算
		/// （ヤング率、ポアソン比に応じてかわる）
		const double E = GetYoungModulus();
		const double v = GetPoissonsRatio();
		const double a = 1 - v;
		const double b = (1 - 2 * v);
		const double c = b / 2;
		double Em;
		if(b == 0.0) Em = DBL_MAX; /// 変形しない。ほんとうは+∞になる。
		else Em = E / (b * (v + 1));
		PTM::TMatrixRow< 6, 6,double > matD;
		matD.clear(0.0);
		matD[0][0] = a; matD[0][1] = v; matD[0][2] = v;
		matD[1][0] = v; matD[1][1] = a; matD[1][2] = v;
		matD[2][0] = v; matD[2][1] = v; matD[2][2] = a;
		matD[3][3] = c;
		matD[4][4] = c;
		matD[5][5] = c;
		matD *= Em;
		
		/// BtDBの計算
		PTM::TMatrixRow< 12, 12,double > matBtDB;
		matBtDB.clear(0.0);
		matBtDB = matB.trans() * matD * matB;

		/// 要素剛性行列の計算
		TMatrixRow< 12, 12,double > matKe; // 要素剛性行列
		matKe.clear(0.0);
		matKe = matCkInv.trans() * matBtDB * matCkInv * mesh->GetTetrahedronVolume(i);	

		/// 質量行列の計算
		TMatrixRow< 12, 12, double > matMe;
		matMe.clear(0.0);
		for(int j = 0; j < 3; j++){
			for(int k = 0; k < 4; k++){
				for(int l = 0; l < 4; l++){
					int id = j * 4;
					matMe[id + k][id + l] = 1.0;
					if(k == l) matMe[id + k][id + l] = 2.0;
				}
			}
		}
		matMe *= GetDensity() * mesh->GetTetrahedronVolume(i) / 20;

		/// 減衰行列（比例減衰）
		TMatrixRow< 12, 12, double > matCe;
		matCe.clear(0.0);
		matCe = GetAlpha() * matMe + GetBeta() * matKe;

		/// 全体行列の計算
		/// 頂点番号順
		/// u = (u0, ..., un-1, v0, ..., vn-1, w0, ..., wn-1)として計算 
		// j:頂点数, k:頂点数
		for(int j = 0; j < 4; j++){
			for(int k = 0; k < 4; k++){
				int id = mesh->tets[i].vertexIDs[j];
				int id2 = mesh->tets[i].vertexIDs[k];
				DSTR << "ID1:ID2 " << id << " : " << id2 << std::endl;
				DSTR << NVer + id << " : " << NVer + id2 << std::endl;
				DSTR << NVer * 2 + id << " : " << NVer * 2 + id2 << std::endl;

				// 全体剛性行列
				matKIni[id][id2] += matKe[j][k];	// uの位置
				matKIni[NVer + id][NVer + id2] += matKe[j+4][k+4];			// vの位置
				matKIni[NVer * 2 + id][NVer * 2 + id2] += matKe[j+8][k+8];	// wの位置

				// 全体質量行列
				matMIni[id][id2] += matMe[j][k];
				matMIni[NVer + id][NVer + id2] += matMe[j+4][k+4];
				matMIni[NVer * 2 + id][NVer * 2 + id2] += matMe[j+8][k+8];

				// 全体減衰行列
				matCIni[id][id2] += matCe[j][k];
				matCIni[NVer + id][NVer + id2] += matCe[j+4][k+4];
				matCIni[NVer * 2 + id][NVer * 2 + id2] += matCe[j+8][k+8];
			}
		}
		// 毎ステップ計算する必要のないものを保存
		matMInv = matMIni.inv();

		//DSTR << "matCk1" << std::endl;	DSTR << matCk1 << std::endl;
		//DSTR << "matCk1Inv" << std::endl;	DSTR << matCk1Inv << std::endl;
		//DSTR << "matCkInv" << std::endl;	DSTR << matCkInv << std::endl;
		//DSTR << "matB" << std::endl;		DSTR << matB << std::endl;
		//DSTR << "matD" << std::endl;		DSTR << matD << std::endl;
		//DSTR << "matBtDB" << std::endl;	DSTR << matBtDB << std::endl;
		DSTR << "matKe" << std::endl;		DSTR << matKe << std::endl;
		DSTR << "det matKe" << std::endl;	DSTR << matKe.det() << std::endl;
		//DSTR << "matMe" << std::endl;		DSTR << matMe << std::endl;
	}
	DSTR << "matKIni" << std::endl;	DSTR << matKIni << std::endl;
	//DSTR << "matMIni" << std::endl;	DSTR << matMIni << std::endl;
	//DSTR << "matCIni" << std::endl;	DSTR << matCIni << std::endl;
	DSTR << "det(matKIni) = " << matKIni.det() << std::endl;
	DSTR << "det(matMIni) = " << matMIni.det() << std::endl;	
	DSTR << "det(matCIni) = " << matCIni.det() << std::endl;

	/// 各種変数の初期化
	xdl.resize(NDof, 0.0);
	vl.resize(NDof, 0.0);
	fl.resize(NDof, 0.0);
	/// FemVertexから変位を取ってくる
	GetVerticesDisplacement(xdl);

	DSTR << "initial xdl" << std::endl;	DSTR << xdl << std::endl;
	DSTR << "initial vl" << std::endl;	DSTR << vl << std::endl;
	DSTR << "Initializing Completed." << std::endl;

	// テストコード

	vdt = 0.001;
	AddBoundaryCondition(0, Vec3d(1, 1, 1));
	//AddBoundaryCondition(5, Vec3d(1, 1, 1));
	//AddVertexForce(1, Vec3d(1000.0, 0.0, 0.0));
	//mesh->AddLocalDisplacement(1, Vec3d(0.1, 0.0, 0.0));
	//matCIni.clear(0.0);
	ScilabEigenValueAnalysis(matMIni, matKIni);

	//VMatrixRd matT;
	//matT.resize(3, 3);
	//matT.clear(0.0);
	//matT[0][0] = 105000;	matT[0][1] = -105000;
	//matT[1][0] = -105000;	matT[1][1] = 175000;	matT[1][2] = -7000;
	//matT[2][0] = -7000;	matT[2][1] = 7000;
	//DSTR << matT.det() << std::endl;

#if 0
	// 固有値のサイラボテスト
	VMatrixRd k;
	k.resize(5, 5, 0.0);
	k.item(0, 0) = 200;		k.item(0, 1) = -100;
	k.item(1, 0) = -100;	k.item(1, 1) = 100;
	k.item(2, 2) = 20;
	k.item(3, 3) = 20;
	k.item(4,4) = 1;

	VMatrixRd m;
	m.resize(5, 5, 0.0);
	m.item(0, 0) = 3;	m.item(0, 2) = 2;
	m.item(1, 1) = 3;	m.item(1, 3) = 2;
	m.item(2, 0) = 2;	m.item(2, 2) = 4;
	m.item(3, 1) = 2;	m.item(3, 3) = 4;
	m.item(4,4) = 1;
	DSTR << "ScilabTest" << std::endl;
	ScilabEigenValueAnalysis(m, k);
#endif
} 

//#define DEBUG
void PHFemVibration::Step(){
	// FemVertexから変位をとってくる
	GetVerticesDisplacement(xdl);

	///積分
	switch(integration_mode){
		case PHFemVibrationDesc::MODE_EXPLICIT_EULER:
			ExplicitEuler(matMInv, matKIni, matCIni,fl,vdt, xdl, vl);
			break;
		case PHFemVibrationDesc::MODE_IMPLICIT_EULER:
			//ImplicitEuler(matMInv, matKIni, matCIni,fl,vdt, xdl, vl);
			break;
		case PHFemVibrationDesc::MODE_NEWMARK_BETA:
			NewmarkBeta(matMIni, matKIni, matCIni,fl,vdt, xdl, vl);
			break;
		case PHFemVibrationDesc::MODE_MODAL_ANALYSIS:
			ModalAnalysis(matMIni, matKIni, matCIni,fl,vdt, xdl, vl, 1);
			break;
		default:
			break;
	}
	fl.clear(0.0);
	// 計算結果をFemVertexに反映
	UpdateVerticesPosition(xdl);

#ifdef DEBUG
	DSTR << "vl updated" << std::endl;	DSTR << vl << std::endl;
	DSTR << "xdl updated" << std::endl;	DSTR << xdl << std::endl;
	DSTR << "///////////////////////////////////////////////" << std::endl;
#endif
}

//* 積分関数
/////////////////////////////////////////////////////////////////////////////////////////
void PHFemVibration::ExplicitEuler(const VMatrixRd& _MInv, const VMatrixRd& _K, const VMatrixRd& _C, 
		const VVectord& _f, const double& _dt, VVectord& _xd, VVectord& _v){
	int NDof = NVertices() * 3;
	VVectord tmp;
	tmp.resize(NDof, 0.0);
	tmp = -1 * (_K * _xd) - (_C * _v) + _f;
	_v += _MInv * tmp * _dt;
	_xd = _v * _dt;
}

// 定式化しなおさないといけない.2013.1.3
void PHFemVibration::ImplicitEuler(const VMatrixRd& _MInv, const VMatrixRd& _K, const VMatrixRd& _C, 
		const VVectord& _f, const double& _dt, VVectord& _xd, VVectord& _v){
#if 0
	// 外力をまだ組み込んでない
	int NDof = GetPHFemMesh()->vertices.size() * 3;
	VMatrixRd E;	// 単位行列
	E.resize(NDof, NDof);
	E.clear(0.0);
	for(int i = 0; i < NDof; i++){
		E[i][i] = 1.0;
	}
	VMatrixRd _Kt;
	_Kt.resize(NDof, NDof);
	_Kt.clear(0.0);	
	VMatrixRd _CtInv;
	_CtInv.resize(NDof, NDof);
	_CtInv.clear(0.0);
	VMatrixRd _DInv;
	_DInv.resize(NDof, NDof);
	_DInv.clear(0.0);

	_Kt = _MInv * _K * _dt;
	_CtInv = (E + _MInv * _C * _dt).inv();
	_DInv = (E + _CtInv * _Kt * vdt).inv();

	xdl = _DInv * (xdl + _CtInv * (vl + _Kt * xdlInit) * vdt);
	vl = _CtInv * (vl - _Kt * (xdl - xdlInit));
	//DSTR << "Integrate" << std::endl;
	//DSTR << "_K" << std::endl;
	//DSTR << _K << std::endl;
	//DSTR << "_CInv" << std::endl;
	//DSTR << _CInv << std::endl;
	//DSTR << "_DInv" << std::endl;
	//DSTR << _DInv << std::endl;
#endif
}

void PHFemVibration::NewmarkBeta(const VMatrixRd& _M, const VMatrixRd& _K, const VMatrixRd& _C, 
		const VVectord& _f, const double& _dt, VVectord& _xd, VVectord& _v,  const double b){
	int NDof = (int)_xd.size();
	static VVectord _a;		// 今回の加速度
	_a.resize(NDof, 0.0);
	static VVectord _al;	// 前回の加速度
	_al.resize(NDof, 0.0);

	VMatrixRd _SInv;
	_SInv.resize(NDof, NDof, 0.0);
	_SInv = (_M + (0.5 * _C * vdt) + (b * _K * pow(vdt, 2))).inv();

	VVectord _Ct;
	_Ct.resize(NDof, 0.0);
	VVectord _Kt;
	_Kt.resize(NDof, 0.0);

	static bool bFirst = true;
	if(bFirst){
		_a = _M.inv() * (- 1 * ( _C * _v) - 1 * (_K * _xd) + _f);
		bFirst = false;
	}

	_Ct = _C * (_v + (0.5 * _a * _dt));
	_Kt = _K * (_xd + (_v * _dt) + (_a * pow(_dt, 2) * (0.5 - b)));
	_al = _a;
	_a = _SInv * (_f - _Ct - _Kt);
	_v += 0.5 * (_al + _a) * _dt;
	_xd = (_v * _dt) + ((0.5 - b) * _al * pow(_dt, 2)) + (_a * b * pow(_dt, 2));
	//DSTR << "Ct" << std::endl;		DSTR << Ct << std::endl;
	//DSTR << "Kt" << std::endl;		DSTR << Kt << std::endl;
	//DSTR << "alocal" << std::endl;	DSTR << alocal << std::endl;
	//DSTR << "_MInv" << std::endl;	DSTR << _MInv << std::endl;
}

void PHFemVibration::ModalAnalysis(const VMatrixRd& _M, const VMatrixRd& _K, const VMatrixRd& _C, 
		const VVectord& _f, const double& _dt, VVectord& _xd, VVectord& _v, const int nmode){
	//DSTR << "//////////////////////////////////" << std::endl;
	// まだ減衰を含んでいない
	// n:自由度、m:モード次数
	static double time = 0.0;		// 経過時間
	static VVectord evalue;			// 固有振動数(m)
	static VMatrixRd evector;		// 固有ベクトル(n*m)
	static VVectord w;				// 固有角振動数(m)
	static VVectord q0;				// モード振動ベクトルの初期値(m)
	static VVectord qv0;			// モード振動速度ベクトルの初期値(m)
	static VMatrixRd fC;	// モード外力の積分cos成分(0列:今回, 1列前回)(m)
	static VMatrixRd fS;	// モード外力の積分sin成分(0列:今回, 1列前回)(m)

	static bool bFirst = true;
	if(bFirst){
		evalue.resize(nmode, 0.0);
		evector.resize(_M.height(), nmode, 0.0);
		SubSpace(_M, _K, nmode, 1e-10, evalue, evector);
		DSTR << "eigenvalue" << std::endl;
		DSTR << evalue << std::endl;
		DSTR << "eigenvector" << std::endl;
		DSTR << evector << std::endl;

		// 初期変位、速度をモード座標系にする
		q0.resize(nmode, 0.0);
		q0 = evector.trans() * _M * _xd;
		qv0.resize(nmode, 0.0);
		qv0 = evector.trans() * matMIni * _v / _dt;
		fC.resize(nmode, 2, 0.0);
		fS.resize(nmode, 2, 0.0);
		w.resize(evalue.size());
		for(int i = 0; i < w.size(); i++){
			w[i] = sqrt(evalue[i]);	// 固有角振動数
		}
		bFirst = false;
	}

	// 積分
	VVectord q;		// 更新後のモード振動ベクトル(m)
	q.resize(nmode, 0.0);
	VVectord qv;	// 更新後のモード振動速度ベクトル(m)
	qv.resize(nmode, 0.0);
	VVectord fM;	// モード外力(m)
	fM.resize(nmode, 0.0);
	fM =  evector.trans() * fl;

	for(int i = 0; i < nmode; i++){
		double wt = w[i] * time;
		double ftemp = fM[i] * cos(wt);
		fC.item(i, 0) += 0.5 * (ftemp + fC.item(i, 1)) * _dt;
		fC.item(i, 1) = ftemp;
		ftemp = fM[i] * sin(wt);
		fS.item(i, 0) += 0.5 * (ftemp + fS.item(i, 1)) * _dt;
		fS.item(i, 1) = ftemp;
		q[i] = q0[i] * cos(wt) + qv0[i] * sin(wt) / w[i] + (fC.item(i, 0) * sin(wt) - fS.item(i, 0) * cos(wt))/ w[i];
	}
	//DSTR << fM << std::endl;
	//DSTR << fC << std::endl;
	//DSTR << fS << std::endl;
	//DSTR << time << std::endl;

	_xd = evector * q;

	time += _dt;
}

//* 計算関数（そのうちTMatirixへ）
/////////////////////////////////////////////////////////////////////////////////////////
/// サブスペース法（同時逆反復法？）
/// VMatrixRd& _M:質量行列（正値対称）, VMatrixRd& _K:剛性行列（正値対称）
/// int nmode:モード次数, double epsilon:収束条件, VVectord& e:固有振動数[Hz], VMatrixRd& v:固有ベクトル(列順）
void PHFemVibration::SubSpace(const VMatrixRd& _M, const VMatrixRd& _K, 
	const int nmode, const double epsilon, VVectord& evalue, VMatrixRd& evector){
	const int size = _M.height();
	if(abs(_K.det()) < 1e-5){
		DSTR << "Stiffness Matrix is not regular matrix." << std::endl;
		return;
	}
	if(nmode > size) assert(0);
	/// 初期化
	// 固有値
	evalue.resize(nmode, 0.0);
	// 固有ベクトル
	evector.resize(size, nmode, 0.0);
	// 初期値ベクトル
	VMatrixRd y;
	y.resize(size, nmode, 0.0);
	VMatrixRd ylast;
	ylast.resize(size, nmode, 0.0);
	VVectord yini;
	yini.resize(size, 1.0);		// 初期値は1.0
	yini.unitize();
	for(int i = 0; i < nmode; i++){
		y.col(i) = yini;
		ylast.col(i) = yini;
	}	

	/// _M, _Kをコレスキー分解
	// _AInvの計算はコレスキー分解値を使ってfor文で計算したほうが速いはず。
	// 今は速さを気にせず逆行列を計算してる。
	VMatrixRd _Mc;
	_Mc.resize(size, size, 0.0);
	cholesky(_M, _Mc);	
	VMatrixRd _Kc;
	_Kc.resize(size, size, 0.0);
	cholesky(_K, _Kc);
	VMatrixRd _AInv;			
	_AInv.resize(size, size, 0.0);
	_AInv = _Mc.trans() * (_Kc.inv()).trans() * _Kc.inv() * _Mc;

	/// 反復計算
	for(int k = 0; k < nmode; k++){
		VVectord z;
		z.resize(size, 0.0);
		int cnt = 0;
		while(1){
			// zの計算
			z = _AInv * y.col(k);	 
			// グラム・シュミットの直交化
			VVectord c;
			c.resize(size, 0.0);
			for(int i = 0; i < k; i++){
				double a = y.col(i) * z;
				c += a * y.col(i);
			}
			y.col(k) = z - c;
			y.col(k).unitize();

			double error = 0.0;
			error = sqrt((ylast.col(k) - y.col(k)) * (ylast.col(k) - y.col(k)));
			ylast.col(k) = y.col(k);
			if(abs(error) < epsilon) break;
			cnt++;
			if(cnt > 1e5){
				DSTR << "Can not converge in subspace" << std::endl;
				break;
			}
		}
		evector.col(k) = _Mc.trans().inv() * y.col(k);		// 固有ベクトル
		evalue[k] = 1.0 / (y.col(k) * _AInv * y.col(k));	// 固有値
	}
}

//* 各種設定関数
/////////////////////////////////////////////////////////////////////////////////////////
void PHFemVibration::SetIntegrationMode(PHFemVibrationDesc::INTEGRATION_MODE mode){
	integration_mode = mode;
}

void PHFemVibration::GetVerticesDisplacement(VVectord& _xd){
	/// FemVertexから変位を取ってくる
	// u = (x1, ..., xn-1, y1, ..., yn-1, z1, ..., zn-1)の順
	int NVer = NVertices();
	for(int i = 0; i < NVer; i++){
		Vec3d disp = GetPHFemMesh()->GetVertexDisplacementL(i);
		for(int j = 0; j < 3; j++){
			_xd[i + NVer * j] = disp[j];
		}
	}	
}

void PHFemVibration::UpdateVerticesPosition(VVectord& _xd){
	/// 計算結果をFemVertexに戻す
	// u = (x1, ..., xn-1, y1, ..., yn-1, z1, ..., zn-1)の順
	int NVer = NVertices();
	PHFemMeshNew* mesh = GetPHFemMesh();
	for(int i = 0; i < NVer; i++){
		Vec3d initialPos = mesh->GetVertexInitPositionL(i);
		for(int j = 0; j < 3; j++){
			GetPHFemMesh()->vertices[i].pos[j] = _xd[i + NVer * j] + initialPos[j];
		}
	}
}

bool PHFemVibration::AddBoundaryCondition(VMatrixRd& mat, const int id){
	int n = mat.height();
	if(id > n - 1) return false;
	mat.vsub_matrix(id, id, 1, n - id).clear(0.0);	// 行をクリア
	mat.vsub_matrix(id, id, n - id, 1).clear(0.0);	// 列をクリア
	mat[id][id] = 1.0;
	return true;
}

bool PHFemVibration::AddBoundaryCondition(const int vtxId, const Vec3i dof){
	int NVer = NVertices();
	if(0 <= vtxId && vtxId <= NVer -1){
		for(int i = 0; i < 3; i++){
			if(dof[i] == 1){
				const int id = vtxId + i * NVer;
				AddBoundaryCondition(matKIni, id);
				AddBoundaryCondition(matCIni, id);
				AddBoundaryCondition(matMIni, id);
				AddBoundaryCondition(matMInv, id);
			}
		}
		DSTR << "matKIni with boundary condition" << std::endl;
		DSTR << matKIni << std::endl;
		DSTR << "matCIni with boundary condition" << std::endl;
		DSTR << matCIni << std::endl;
		DSTR << "matMInv with boundary condition" << std::endl;
		DSTR << matMInv << std::endl;
		DSTR << "After adding boundary condition" << std::endl;
		DSTR << "det(matKIni) = " << matKIni.det() << std::endl;
		DSTR << "det(matCIni) = " << matCIni.det() << std::endl;
		DSTR << "det(matMIni) = " << matMIni.det() << std::endl;
		DSTR << "det(matMInv) = " << matMInv.det() << std::endl;
		return true;
	}
	return false;
}

bool PHFemVibration::AddBoundaryCondition(const VVector< Vec3i > bcs){ 
	int NVer = NVertices();
	if(NVer != bcs.size()) return false;
	for(int i = 0; i < (int)bcs.size(); i++){
		for(int j = 0; j < 3; j++){
			if(bcs[i][j] == 1){
				const int id = i + j * NVer;
				AddBoundaryCondition(matKIni, id);
				AddBoundaryCondition(matCIni, id);
				AddBoundaryCondition(matMIni, id);
				AddBoundaryCondition(matMInv, id);
			}
		}
	}
	return true;
}

bool PHFemVibration::AddVertexForce(int vtxId, Vec3d fW){
	if(0 <= vtxId && vtxId <= NVertices() -1){
		Vec3d fL = GetPHFemMesh()->GetPHSolid()->GetPose() * fW;
		fl[vtxId] += fL.x;
		fl[vtxId + NVertices()] += fL.y;
		fl[vtxId + NVertices() * 2] += fL.z;
		return true;
	}
	return false;
}

bool PHFemVibration::AddVertexForce(VVector< Vec3d > fWs){
	if(NVertices() != fWs.size()) return false;
	for(int i = 0; i < (int)fWs.size(); i++){
		Vec3d fL = GetPHFemMesh()->GetPHSolid()->GetPose().Inv() * fWs[i];
		fl[i] += fL.x;
		fl[i + NVertices()] += fL.y;
		fl[i + NVertices() * 2] += fL.z;
	}
	return true;
}

//* scilabデバック
/////////////////////////////////////////////////////////////////////////////////////////
//void PHFemVibration::ScilabDeterminant(VMatrixRd& _M, VMatrixRd& _K, VMatrixRd& _C){
//
//}

void PHFemVibration::ScilabEigenValueAnalysis(VMatrixRd& _M, VMatrixRd& _K){
	DSTR << "////////////////////////////////////////////" << std::endl;
	DSTR << "ScilabEigenValueAnalysis Start." << std::endl;
	DSTR << "det(M) springhead2 : " << _M.det() << std::endl;
	DSTR << "det(K) springhead2 : " << _K.det() << std::endl;
	VVectord evalue;
	VMatrixRd evector;
	SubSpace(_M, _K, 5, 1e-5, evalue, evector);
	DSTR << "eigenvalue springhead2" << std::endl;
	DSTR << evalue << std::endl;
	DSTR << "eigen vibration value springhead2" << std::endl;
	for(int i = 0; i < evalue.size(); i++){
		evalue[i] = sqrt(evalue[i])/2/M_PI;
	}
	DSTR << evalue << std::endl;
	DSTR << "eigenvector springhead2" << std::endl;
	DSTR << evector << std::endl;

	if(!IsScilabStarted){
		DSTR << "Scilab has not started" << std::endl;
		return;
	}
	ScilabSetMatrix("M", _M);
	ScilabJob("detM = det(M);");
	std::cout << "det(M) scilab : ";
	ScilabJob("disp(detM);");	
	ScilabSetMatrix("K", _K);
	ScilabJob("detK = det(K);");
	std::cout << "det(K) scilab : ";
	ScilabJob("disp(detK);");

	ScilabJob("[P D] = spec(inv(M) * K);");
	VMatrixRd D;
	ScilabGetMatrix(D, "D");
	DSTR << "eigenvalue scilab" << std::endl;
	DSTR << D << std::endl;
	VMatrixRd P;
	ScilabGetMatrix(P, "P");
	DSTR << "eigenvector scilab" << std::endl;
	DSTR << P << std::endl;
	DSTR << "ScilabEigenValueAnalysis End." << std::endl;
	DSTR << "////////////////////////////////////////////" << std::endl;
}
}
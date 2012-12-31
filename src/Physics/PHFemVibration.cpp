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
	integration_mode = PHFemVibrationDesc::MODE_NEWMARK_BETA;
	integration_mode = PHFemVibrationDesc::MODE_EXPLICIT_EULER;
	integration_mode = PHFemVibrationDesc::MODE_MODAL_ANALYSIS;
} 

void PHFemVibration::Init(){
	// Scilab
	if (!ScilabStart()){
		std::cout << "Error : ScilabStart \n";
		return;
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
	matKIni.resize(NDof, NDof);
	matKIni.clear(0.0);
	matMIni.resize(NDof, NDof);
	matMIni.clear(0.0);
	matMInv.resize(NDof, NDof);
	matMInv.clear(0.0);
	matCIni.resize(NDof, NDof);
	matCIni.clear(0.0);
	for(int i = 0; i < NTets; i++){
		// 要素行列の計算
		/// tetが持つ頂点順
		/// 要素剛性行列 u = (u0, u1, ..., u3, v0, v1, ..., v3, w0, w1, ..., w3)として計算
		Vec3d pos[4];
		for(int j = 0; j < 4; j++){
			pos[j] = mesh->vertices[mesh->tets[i].vertexIDs[j]].pos;
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
		matB[3][2] = 1.0;
		matB[3][5] = 1.0;
		matB[4][7] = 1.0;
		matB[4][10] = 1.0;
		matB[5][3] = 1.0;
		matB[5][9] = 1.0;

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
				//DSTR << "ID1:ID2 " << id << " : " << id2 << std::endl;
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

		DSTR << "matCk1" << std::endl;	DSTR << matCk1 << std::endl;
		DSTR << "matCk1Inv" << std::endl;	DSTR << matCk1Inv << std::endl;
		DSTR << "matCkInv" << std::endl;	DSTR << matCkInv << std::endl;
		DSTR << "matB" << std::endl;		DSTR << matB << std::endl;
		DSTR << "matD" << std::endl;		DSTR << matD << std::endl;
		DSTR << "matBtDB" << std::endl;	DSTR << matBtDB << std::endl;
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
	xlocalInit.resize(NDof);
	xlocalInit.clear(0.0);
	xlocal.resize(NDof);
	xlocal.clear(0.0);
	vlocal.resize(NDof);
	vlocal.clear(0.0);
	flocal.resize(NDof);
	flocal.clear(0.0);
	/// FemVertexから値を取ってくる
	for(int i = 0; i < NVer; i++){
		for(int j = 0; j < 3; j++){
			// u = (x1, ..., xn-1, y1, ..., yn-1, z1, ..., zn-1)の順
			xlocalInit[i + NVer * j] = mesh->vertices[i].pos[j];
			xlocal[i + NVer * j] = mesh->vertices[i].pos[j];
		}
	}
	//DSTR << "initial xlocal" << std::endl;	DSTR << xlocalInit << std::endl;
	//DSTR << "initial vlocal" << std::endl;	DSTR << vlocal << std::endl;
	DSTR << "Initializing Completed." << std::endl;

	// テストコード

	vdt = 0.001;
	AddBoundaryCondition(0, Vec3d(1, 1, 1));
	//AddBoundaryCondition(5, Vec3d(1, 1, 1));
	//AddVertexForce(1, Vec3d(1000.0, 0.0, 0.0));
	//mesh->AddLocalDisplacement(1, Vec3d(0.1, 0.0, 0.0));
	matCIni.clear(0.0);
} 

//#define DEBUG
void PHFemVibration::Step(){
	//DSTR << "Step" << std::endl;
#ifdef DEBUG
	DSTR << "///////////////////////////////////////////////" << std::endl;
#endif
	int NVer = NVertices();
	/// FemVertexから値を取ってくる
	for(int i = 0; i < NVer; i++){
		for(int j = 0; j < 3; j++){
			// u = (x1, ..., xn-1, y1, ..., yn-1, z1, ..., zn-1)の順
			xlocal[i + NVer * j] = GetPHFemMesh()->vertices[i].pos[j];
		}
	}

#ifdef DEBUG
	DSTR << "vlocal last" << std::endl;	DSTR << vlocal << std::endl;
	DSTR << "xlocal last" << std::endl;	DSTR << xlocal  << std::endl;
#endif

	///積分
	switch(integration_mode){
		case PHFemVibrationDesc::MODE_EXPLICIT_EULER:
			ExplicitEuler();
			break;
		case PHFemVibrationDesc::MODE_IMPLICIT_EULER:
			ImplicitEuler();
			break;
		case PHFemVibrationDesc::MODE_NEWMARK_BETA:
			NewmarkBeta();
			break;
		case PHFemVibrationDesc::MODE_MODAL_ANALYSIS:
			ModalAnalysis(1);
			break;
		default:
			break;
	}
	flocal.clear(0.0);

	/// 計算結果をFemVertexに戻す
	for(int i = 0; i < NVer; i++){
		for(int j = 0; j < 3; j++){
			// u = (x1, ..., xn-1, y1, ..., yn-1, z1, ..., zn-1)の順
			GetPHFemMesh()->vertices[i].pos[j] = xlocal[i + NVer * j];
		}
	}
#ifdef DEBUG
	DSTR << "vlocal updated" << std::endl;	DSTR << vlocal << std::endl;
	DSTR << "xlocal updated" << std::endl;	DSTR << xlocal << std::endl;
#endif
}

//* 積分関数
/////////////////////////////////////////////////////////////////////////////////////////
void PHFemVibration::ExplicitEuler(){
	int NDof = NVertices() * 3;
	VVectord xlocalDisp;
	xlocalDisp.resize(NDof);
	xlocalDisp.clear(0.0);
	VVectord tmp;
	tmp.resize(NDof);
	tmp.clear(0.0);
	xlocalDisp = xlocal - xlocalInit;
	tmp = -1 * matKIni * xlocalDisp - matCIni * vlocal + flocal;
	vlocal += matMInv * tmp * vdt;
	xlocal += vlocal * vdt;
}

void PHFemVibration::ImplicitEuler(){
	// 外力をまだ組み込んでない
	int NDof = GetPHFemMesh()->vertices.size() * 3;
	VMatrixRd E;	// 単位行列
	E.resize(NDof, NDof);
	E.clear(0.0);
	for(int i = 0; i < NDof; i++){
		E[i][i] = 1.0;
	}
	VMatrixRd _K;
	_K.resize(NDof, NDof);
	_K.clear(0.0);	
	VMatrixRd _CInv;
	_CInv.resize(NDof, NDof);
	_CInv.clear(0.0);
	VMatrixRd _DInv;
	_DInv.resize(NDof, NDof);
	_DInv.clear(0.0);

	_K = matMInv * matKIni * vdt;
	_CInv = (E + matMInv * matCIni * vdt).inv();
	_DInv = (E + _CInv * _K * vdt).inv();

	xlocal = _DInv * (xlocal + _CInv * (vlocal + _K * xlocalInit) * vdt);
	vlocal = _CInv * (vlocal - _K * (xlocal - xlocalInit));
	//DSTR << "Integrate" << std::endl;
	//DSTR << "_K" << std::endl;
	//DSTR << _K << std::endl;
	//DSTR << "_CInv" << std::endl;
	//DSTR << _CInv << std::endl;
	//DSTR << "_DInv" << std::endl;
	//DSTR << _DInv << std::endl;
}

void PHFemVibration::NewmarkBeta(const double b){
	int NDof = NVertices() * 3;
	static VVectord alocal;
	alocal.resize(NDof);
	alocal.clear(0.0);

	VVectord alocalLast;
	alocalLast.resize(NDof);
	alocalLast.clear(0.0);

	VMatrixRd _MInv;
	_MInv.resize(NDof, NDof);
	_MInv.clear(0.0);
	_MInv = (matMIni + 0.5 * matCIni * vdt + b * matKIni * pow(vdt, 2)).inv();

	VVectord Ct;
	Ct.resize(NDof);
	Ct.clear(0.0);

	VVectord Kt;
	Kt.resize(NDof);
	Kt.clear(0.0);

	VVectord xlocalDisp;
	xlocalDisp.resize(NDof);
	xlocalDisp.clear(0.0);
	xlocalDisp = xlocal - xlocalInit;

	static bool bFirst = true;
	if(bFirst){
		alocal = _MInv * (- 1 * matCIni * vlocal -1 * matKIni * xlocalDisp + flocal);
		bFirst = false;
	}

	Ct = matCIni * (vlocal + 0.5 * alocal * vdt);
	Kt = matKIni * (xlocalDisp + vlocal * vdt + alocal * pow(vdt, 2) * (0.5-b));
	alocalLast = alocal;
	alocal = _MInv * (flocal - Ct - Kt);
	vlocal += 0.5 * (alocalLast + alocal) * vdt;
	xlocal += vlocal * vdt + (0.5 - b)* alocalLast * pow(vdt, 2) + alocal * b * pow(vdt, 2);
	//DSTR << "Ct" << std::endl;		DSTR << Ct << std::endl;
	//DSTR << "Kt" << std::endl;		DSTR << Kt << std::endl;
	//DSTR << "alocal" << std::endl;	DSTR << alocal << std::endl;
	//DSTR << "_MInv" << std::endl;	DSTR << _MInv << std::endl;
}

void PHFemVibration::ModalAnalysis(int nmode){
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

#if 1
	static bool bFirst = true;
	if(bFirst){
		evalue.resize(nmode);
		evalue.clear(0.0);
		evector.resize(matMIni.height(), nmode);
		evector.clear(0.0);
		SubSpace(matMIni, matKIni, 23, 1e-10, evalue, evector);
		DSTR << "eigenvalue" << std::endl;
		DSTR << evalue << std::endl;
		DSTR << "eigenvector" << std::endl;
		DSTR << evector << std::endl;

		// Scilab
		std::cout << "Scilab" << std::endl;
		VMatrixRd MinvK;
		MinvK.assign(matMIni.inv() * matKIni);
		ScilabSetMatrix("MinvK", MinvK);
		ScilabJob("[P D] = spec(MinvK);");
		DSTR << "eigenvector scilab" << std::endl;
		VMatrixRd P;
		ScilabGetMatrix(P, "P");
		DSTR << P << std::endl;
		DSTR << "eigenvalue scilab" << std::endl;
		VMatrixRd D;
		ScilabGetMatrix(D, "D");
		DSTR << D << std::endl;
		ScilabEnd();

		// 初期変位、速度をモード座標系にする
		q0.resize(nmode);
		q0.clear(0.0);
		q0 = evector.trans() * matMIni * (xlocal - xlocalInit);
		qv0.resize(nmode);
		qv0.clear(0.0);
		qv0 = evector.trans() * matMIni * vlocal / vdt;
		fC.resize(nmode, 2);
		fC.clear(0.0);
		fS.resize(nmode, 2);
		fS.clear(0.0);
		w.resize(evalue.size());
		for(int i = 0; i < w.size(); i++){
			w[i] = sqrt(evalue[i]);	// 固有角振動数
		}
		bFirst = false;
	}

	// 積分
	VVectord q;		// 更新後のモード振動ベクトル(m)
	q.resize(nmode);
	q.clear(0.0);
	VVectord qv;	// 更新後のモード振動速度ベクトル(m)
	qv.resize(nmode);
	qv.clear(0.0);
	VVectord fM;	// モード外力(m)
	fM.resize(nmode);
	fM.clear(0.0);
	fM =  evector.trans() * flocal;

	for(int i = 0; i < nmode; i++){
		double wt = w[i] * time;
		double ftemp = fM[i] * cos(wt);
		fC.item(i, 0) += 0.5 * (ftemp + fC.item(i, 1)) * vdt;
		fC.item(i, 1) = ftemp;
		ftemp = fM[i] * sin(wt);
		fS.item(i, 0) += 0.5 * (ftemp + fS.item(i, 1)) * vdt;
		fS.item(i, 1) = ftemp;
		q[i] = q0[i] * cos(wt) + qv0[i] * sin(wt) / w[i] + (fC.item(i, 0) * sin(wt) - fS.item(i, 0) * cos(wt))/ w[i];
	}
	//DSTR << fM << std::endl;
	//DSTR << fC << std::endl;
	//DSTR << fS << std::endl;
	//DSTR << time << std::endl;

	xlocal += evector * q;

	time += vdt;

#else
	/// テストコード
	VMatrixRd k;
	k.resize(4, 4);
	k.clear(0.0);
	k.item(0, 0) = 200;		k.item(0, 1) = -100;
	k.item(1, 0) = -100;	k.item(1, 1) = 100;
	k.item(2, 2) = 20;
	k.item(3, 3) = 20;

	VMatrixRd m;
	m.resize(4, 4);
	m.clear(0.0);
	m.item(0, 0) = 3;	m.item(0, 2) = 2;
	m.item(1, 1) = 3;	m.item(1, 3) = 2;
	m.item(2, 0) = 2;	m.item(2, 2) = 4;
	m.item(3, 1) = 2;	m.item(3, 3) = 4;
	DSTR << "det(k)" << std::endl;
	DSTR << k.det() << std::endl;
	DSTR << "det(m)" << std::endl;
	DSTR << m.det() << std::endl;

	SubSpace(m,k, 3, 1e-10, evalue, evector);
	DSTR << "eigen vibration value" << std::endl;
	for(int i = 0; i < evalue.size(); i++){
		DSTR << sqrt(evalue[i])/2/M_PI << std::endl;
	}
	DSTR << "eigenvalue" << std::endl;
	DSTR << evalue << std::endl;
	DSTR << "eigenvector" << std::endl;
	DSTR << evector << std::endl;
	DSTR << evector.trans() * m * evector << std::endl;
	DSTR << evector.trans() * k * evector << std::endl;

	std::cout << "Scilab" << std::endl;
	ScilabSetMatrix("MinvK", m.inv() * k);
	ScilabJob("[P D] = spec(MinvK);");
	DSTR << "eigenvector scilab" << std::endl;
	VMatrixRd P;
	ScilabGetMatrix(P, "P");
	DSTR << P << std::endl;
	DSTR << "eigenvalue scilab" << std::endl;
	VMatrixRd D;
	ScilabGetMatrix(D, "D");
	DSTR << D << std::endl;
	ScilabEnd();
#endif
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
	evalue.resize(nmode);
	evalue.clear(0.0);
	// 固有ベクトル
	evector.resize(size, nmode);
	evector.clear(0.0);
	// 初期値ベクトル
	VMatrixRd y;
	y.resize(size, nmode);
	y.clear(0.0);
	VMatrixRd ylast;
	ylast.resize(size, nmode);
	ylast.clear(0.0);
	VVectord yini;
	yini.resize(size);
	yini.clear(1.0);
	yini.unitize();
	for(int i = 0; i < nmode; i++){
		y.col(i) = yini;
		ylast.col(i) = yini;
	}	

	/// _M, _Kをコレスキー分解
	// _AInvの計算はコレスキー分解値を使ってfor文で計算したほうが速いはず。
	// 今は速さを気にせず逆行列を計算してる。
	VMatrixRd _Mc;
	_Mc.resize(size, size);
	_Mc.clear(0.0);
	cholesky(_M, _Mc);	
	VMatrixRd _Kc;
	_Kc.resize(size, size);
	_Kc.clear(0.0);
	cholesky(_K, _Kc);
	VMatrixRd _AInv;			
	_AInv.resize(size, size);
	_AInv.clear(0.0);
	_AInv = _Mc.trans() * (_Kc.inv()).trans() * _Kc.inv() * _Mc;

	/// 反復計算
	for(int k = 0; k < nmode; k++){
		VVectord z;
		z.resize(size);
		z.clear(0.0);
		int cnt = 0;
		while(1){
			// zの計算
			z = _AInv * y.col(k);	 
			// グラム・シュミットの直交化
			VVectord c;
			c.resize(size);
			c.clear(0.0);
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
		//DSTR << "matKIni with boundary condition" << std::endl;
		//DSTR << matKIni << std::endl;
		//DSTR << "matCIni with boundary condition" << std::endl;
		//DSTR << matCIni << std::endl;
		//DSTR << "matMInv with boundary condition" << std::endl;
		//DSTR << matMInv << std::endl;
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
		flocal[vtxId] += fL.x;
		flocal[vtxId + NVertices()] += fL.y;
		flocal[vtxId + NVertices() * 2] += fL.z;
		return true;
	}
	return false;
}

bool PHFemVibration::AddVertexForce(VVector< Vec3d > fWs){
	if(NVertices() != fWs.size()) return false;
	for(int i = 0; i < (int)fWs.size(); i++){
		Vec3d fL = GetPHFemMesh()->GetPHSolid()->GetPose().Inv() * fWs[i];
		flocal[i] += fL.x;
		flocal[i + NVertices()] += fL.y;
		flocal[i + NVertices() * 2] += fL.z;
	}
	return true;
}


}
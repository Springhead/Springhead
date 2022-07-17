/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHIKEngine.h>
#include <Physics/PHIKActuator.h>
#include <Physics/PHIKEndEffector.h>

#include <chrono>

using namespace std;
namespace Spr{

//static std::ofstream *dlog;

PHIKEngineDesc::PHIKEngineDesc():
	numIter(1),
	maxVel(20), // 20[m/s]
	maxAngVel(Rad(500)), // 500[deg/s]
	maxActVel(Rad(30)),  //  10[deg/s]
	regularizeParam(0.7)
{
	regularizeParam2 = 0.5;
	regularizeMode = 0;
	iterGaussSeidel = 100;
	solverMode = Mode::SVD;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// IKEngine
PHIKEngine::PHIKEngine(const PHIKEngineDesc& desc):
	lastM(0),
	lastN(0),
	iterCutOffAngVel(0.01)
{
	SetDesc(&desc);
}

void PHIKEngine::ApplyExactState(bool reverse) {
	for(size_t i=0; i<actuators.size(); ++i){
		actuators[i]->ApplyExactState(reverse);
	}
	if (!reverse) {
		for(size_t i=0; i<endeffectors.size(); ++i){
			endeffectors[i]->ApplyExactState();
		}
	}
}

void PHIKEngine::Prepare(bool second) {
	// <!!>
	if (!second) {
		// エンドエフェクタの有効・無効に基づいてアクチュエータの有効・無効を切替え
		for(size_t i=0; i<actuators.size(); ++i){
			actuators[i]->Enable(false);
		}
		for(size_t i=0; i<actuators.size(); ++i){
			if (actuators[i]->eef && actuators[i]->eef->IsEnabled()) {
				actuators[i]->Enable(true);
				for(size_t j=0; j<actuators[i]->ascendant.size(); ++j){
					actuators[i]->ascendant[j]->Enable(true);
				}
			}
		}
	}

	// 計算用変数準備の前処理
	for(size_t i=0; i<actuators.size(); ++i){
		if (actuators[i]->IsEnabled()) {
			actuators[i]->BeforeSetupMatrix();
		}
	}

	// 計算用変数の準備
	for(size_t i=0; i<actuators.size(); ++i){
		if (actuators[i]->IsEnabled()) {
			actuators[i]->SetupMatrix();
		}
	}

	// 変化フラグの確認とクリア
	for(size_t i=0; i<actuators.size(); ++i){
		actuators[i]->bActuatorAdded	= false;
		actuators[i]->bNDOFChanged		= false;
	}
	for(size_t i=0; i<endeffectors.size(); ++i){
		endeffectors[i]->bNDOFChanged	= false;
	}


	// <!!> 行列のストライドマップを作る
	if (actuators.size()   !=strideAct.size()) { strideAct.resize(actuators.size());    }
	if (endeffectors.size()!=strideEff.size()) { strideEff.resize(endeffectors.size()); }

	int n=0;
	for (size_t i=0; i<actuators.size(); ++i){
		strideAct[i] = n;
		if (actuators[i]->IsEnabled()) { n += actuators[i]->ndof; }
	}

	int m=0;
	for (size_t i=0; i<endeffectors.size(); ++i){
		strideEff[i] = m;
		if (endeffectors[i]->IsEnabled()) { m += endeffectors[i]->ndof; }
	}
	// std::cout << m << " x " << n << std::endl;

	if (m!=lastM || n!=lastN) {
		J.resize(m, n); J.clear();
		W.resize(n);    W.clear();
		V.resize(m);    V.clear();
		Wp.resize(n);   Wp.clear();
		l.resize(m);    l.clear();
		F.resize(m);    F.clear();
		T.resize(n);    T.clear();

		lastM = m;
		lastN = n;
	}
}

void PHIKEngine::CalcJacobian() {
	// ヤコビアン計算の前処理
	for(size_t i=0; i<actuators.size(); ++i){
		if (actuators[i]->IsEnabled()) {
			actuators[i]->BeforeCalcAllJacobian();
		}
	}

	// ヤコビアンの計算
	for(size_t i=0; i<actuators.size(); ++i){
		if (actuators[i]->IsEnabled()) {
			actuators[i]->CalcAllJacobian();
		}
	}

	// <!!>ヤコビアンのコピー
	typedef std::map< int,PTM::VMatrixRow<double> >::iterator eff_iter;
	for(size_t i=0; i<actuators.size(); ++i){
		if (actuators[i]->IsEnabled()) {
			PHIKActuator* act = actuators[i];
			for (eff_iter it=act->Mj.begin(); it!=act->Mj.end(); ++it) {
				int j = (*it).first;

				if (endeffectors[j]->IsEnabled()) {
					PTM::VMatrixRow<double>& Jpart = (*it).second;
					for (size_t x=0; x<Jpart.width(); ++x) {
						for (size_t y=0; y<Jpart.height(); ++y) {
							size_t X = strideAct[i] + x;
							size_t Y = strideEff[j] + y;
							J.at_element(Y,X) = Jpart[y][x];
						}
					}
				}
			}
		}
	}
	// std::cout << J << std::endl;

	// <!!>標準姿勢に戻ろうとする関節角速度の計算
	for(size_t i=0; i<actuators.size(); ++i){
		if (actuators[i]->IsEnabled()) {
			PHIKActuator* act = actuators[i];

			act->CalcPullbackVelocity();

			for (size_t x=0; x<(size_t)act->ndof; ++x) {
				size_t X = strideAct[i] + x;
				Wp[X] = act->domega_pull[x];
			}
		}
	}
}

void PHIKEngine::IK(bool nopullback) {
#ifdef USE_LAPACK
	auto start = std::chrono::system_clock::now();
	// 計算の準備（α・β・γの事前計算）
	for(size_t i=0; i<actuators.size(); ++i){
		actuators[i]->PrepareSolve();
	}

	// <!!>Vの作成
	for (size_t j=0; j<endeffectors.size(); ++j){
		if (endeffectors[j]->IsEnabled() && (endeffectors[j]->bPosition || endeffectors[j]->bOrientation)) {
			// V
			PHIKEndEffector* eff = endeffectors[j];
			PTM::VVector<double> Vpart; Vpart.resize(eff->ndof);
			Vec3d force;
			eff->GetTempTarget(Vpart);
			force = eff->GetTargetForce();//add here
			for (size_t y=0; y<(size_t)eff->ndof; ++y) {
				size_t Y = strideEff[j] + y;
				V[Y] = Vpart[y];
				F[Y] = force[y];// add here
			}
		}
	}
	// std::cout << "V : " << V << std::endl;

	// <!!>擬似逆解を求める・lapack-SVD版
	vector_type S; S.resize((std::min)(J.size1(), J.size2())); S.clear();

	ublas::matrix<double> U, Vt;
	ublas::diagonal_matrix<double> D, Di, Di_;
	svd(J, U, D, Vt);

	Di.resize(D.size2(), D.size1());
	
	float regularizeValue = 0;
	switch (regularizeMode) {
		case 0:   // Static
			regularizeValue = regularizeParam * regularizeParam;
			break;
		case 1:   // Effector error
		{
			double error = std::min(ublas::norm_2(V), 1.0);
			regularizeValue = regularizeParam * regularizeParam * error;
			break;
		}
		case 2:  // Manipulability measure
		{
			double manipulability = std::sqrt(std::abs(determinant(ublas::prod(J, ublas::trans(J)))));
			if (regularizeParam2 > 0 && manipulability < regularizeParam2) {
				regularizeValue = regularizeParam * regularizeParam * std::pow((1 - manipulability / regularizeParam2), 2.0);
			}
			else {
				regularizeValue = 0;
			}
			break;
		}
		default:  // None
			break;
	}
	for (size_t i = 0; i<(std::min(J.size1(), J.size2())); ++i) {
		// Tikhonov Regularization
		Di.at_element(i, i)  = D(i, i) / (D(i, i)*D(i, i) + regularizeValue);
	}

	// --- 位置
	vector_type      UtV = ublas::prod(ublas::trans(U)  , V    );
	vector_type    DiUtV = ublas::prod(Di               , UtV  );
	W                    = ublas::prod(ublas::trans(Vt) , DiUtV);

	//--- 力 //add here
	T = ublas::prod(ublas::trans(J), F);


	// <!!>Wに標準姿勢復帰速度を加える
	if (!nopullback) {
		vector_type       JWp = ublas::prod(J, Wp);
		vector_type     UtJWp = ublas::prod(ublas::trans(U), JWp);
		vector_type   DiUtJWp = ublas::prod(Di, UtJWp);
		vector_type  VDiUtJWp = ublas::prod(ublas::trans(Vt), DiUtJWp);
		vector_type Wpullback = Wp - VDiUtJWp;
		W = W + Wpullback;
	}

	// <!!>非常に大きくなりすぎた解を切り捨てる
	double limitW = 1e+10;
	for (size_t i=0; i<W.size(); ++i) {
		if (W[i]  >  limitW) { W[i]  =  limitW; }
		if (W[i]  < -limitW) { W[i]  = -limitW; }
	}
	
	for (size_t i = 0; i < T.size(); ++i) { //add here
		if (T[i] > limitW) { T[i] = limitW; }
		if (T[i] < -limitW) { T[i] = -limitW; }
	}

	// <!!>各Actuatorのωに擬似逆解を代入 //here
	for (size_t i=0; i<actuators.size(); ++i) {
		if (actuators[i]->IsEnabled()) {
			PHIKActuator* act = actuators[i];
			for (size_t x=0; x<(size_t)act->ndof; ++x) {
				size_t X = strideAct[i] + x;
				act->omega[x] = W[X];
				act->tau[x] = T[X]; //add here
			}
		}
	}

	// 結果にしたがってActuatorの一時変数を動かす
	for(size_t i=0; i<actuators.size(); ++i){
		if (actuators[i]->IsEnabled()) {
			actuators[i]->MoveTempJoint();
		}
	}
	auto fin = std::chrono::system_clock::now();
	double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(fin - start).count();
	DSTR << "calc:" << elapsed << std::endl;
#else
# pragma message("IK: define USE_LAPACK in SprDefs.h to use this function")
#endif
}
void PHIKEngine::LQIK(bool nopullback) {
#ifdef USE_LAPACK
	auto start = std::chrono::system_clock::now();
	// 計算の準備（α・β・γの事前計算）
	for (size_t i = 0; i<actuators.size(); ++i) {
		actuators[i]->PrepareSolve();
	}

	// <!!>Vの作成//here
	for (size_t j = 0; j<endeffectors.size(); ++j) {
		if (endeffectors[j]->IsEnabled() && (endeffectors[j]->bPosition || endeffectors[j]->bOrientation)) {
			// V
			PHIKEndEffector* eff = endeffectors[j];
			PTM::VVector<double> Vpart; Vpart.resize(eff->ndof);
			eff->GetTempTarget(Vpart);
			for (size_t y = 0; y<(size_t)eff->ndof; ++y) {
				size_t Y = strideEff[j] + y;
				V[Y] = Vpart[y];
			}
		}
	}
	// std::cout << "V : " << V << std::endl;

	// <!!>擬似逆解を求める・lapack-SVD版
	ublas::matrix<double> Q(J.size2(), J.size2());
	ublas::triangular_matrix<double, ublas::upper> L, Li;
	//ublas::matrix<double> L(J.size2(), J.size2());
	diag_matrix_type Wn; Wn.resize(J.size1(), J.size1());

	float regularizeValue = 0;
	switch (regularizeMode) {
	case 0:   // Static
		regularizeValue = regularizeParam * regularizeParam;
		break;
	case 1:   // Effector error
	{
		double error = std::min(ublas::norm_2(V), 1.0);
		regularizeValue = regularizeParam * regularizeParam * error;
		break;
	}
	case 2:  // Manipulability measure
	{
		double manipulability = std::sqrt(std::abs(determinant(ublas::prod(J, ublas::trans(J)))));
		if (regularizeParam2 > 0 && manipulability < regularizeParam2) {
			regularizeValue = regularizeParam * regularizeParam * std::pow((1 - manipulability / regularizeParam2), 2.0);
		}
		else {
			regularizeValue = 0;
		}
		break;
	}
	default:  // None
		break;
	}
	for (size_t i = 0; i<(std::min(J.size1(), J.size2())); ++i) {
		// Tikhonov Regularization
		Wn.at_element(i, i) = regularizeValue;
	}

	//matrix_type JtWe = ublas::prod(ublas::trans(J), We);
	//matrix_type JtWe = ublas::trans(J);
	//matrix_type JtWeJ = ublas::prod(JtWe, J) + Wn;

	matrix_type JJt = ublas::prod(J, ublas::trans(J)) + Wn;
	/*
	ublas::matrix<double> U, Vt;
	ublas::diagonal_matrix<double> D, Di, Di_;
	svd(JJt, U, D, Vt);

	Di.resize(D.size2(), D.size1());

	for (size_t i = 0; i<(std::min(J.size1(), J.size2())); ++i) {
		// Tikhonov Regularization
		Di.at_element(i, i) = D(i, i) / (D(i, i)*D(i, i));
	}

	// --- 位置
	matrix_type      DiUt = ublas::prod(Di, ublas::trans(U));
	matrix_type    VtDiUt = ublas::prod(ublas::trans(Vt), DiUt);
	matrix_type      pinvJ = ublas::prod(ublas::trans(J), VtDiUt);
	W = ublas::prod(pinvJ, V);
	/*/
	// LQ分解
	
	qr(JJt, Q, L);
	
	Li.resize(L.size1(), L.size2());
	for (int i = 0; i < (int) J.size1(); i++) {
		Li.at_element(i, i) = 1 / L(i, i);
	}
	for (int i = 0; i < (int) J.size1(); i++) {
		for (int j = i + 1; j < (int) L.size1(); j++) {
			double s = 0;
			for (int k = i; k < j; k++) s += Li(i, k) * L(k, j);
			Li(i, j) = -s / L(j, j);
		}
	}
	
	matrix_type invJtWeJ = ublas::prod(Li, ublas::trans(Q));

	matrix_type pinvJ = ublas::prod(ublas::trans(J), invJtWeJ);
	
	// --- 位置
	//vector_type      JtWeV = ublas::prod(JtWe, V);
	//W = ublas::prod(invJtWeJ, JtWeV);
	W = ublas::prod(pinvJ, V);
	
	// <!!>Wに標準姿勢復帰速度を加える
	if (!nopullback) {
		
		vector_type       JWp = ublas::prod(J, Wp);
		vector_type     pinvJJWp = ublas::prod(pinvJ, JWp);
		vector_type Wpullback = Wp - pinvJJWp;
		W = W + Wpullback;
		/*/
		vector_type       JWp = ublas::prod(Q, Wp);
		vector_type     pinvJJWp = ublas::prod(ublas::trans(Q), JWp);
		vector_type Wpullback = Wp - pinvJJWp;
		W = W + Wpullback;
		*/
	}

	// <!!>非常に大きくなりすぎた解を切り捨てる
	double limitW = 1e+10;
	for (size_t i = 0; i<W.size(); ++i) {
		if (W[i]  >  limitW) { W[i] = limitW; }
		if (W[i]  < -limitW) { W[i] = -limitW; }
	}

	// <!!>各Actuatorのωに擬似逆解を代入
	for (size_t i = 0; i<actuators.size(); ++i) {
		if (actuators[i]->IsEnabled()) {
			PHIKActuator* act = actuators[i];
			for (size_t x = 0; x<(size_t)act->ndof; ++x) {
				size_t X = strideAct[i] + x;
				act->omega[x] = W[X];
			}
		}
	}

	// 結果にしたがってActuatorの一時変数を動かす
	for (size_t i = 0; i<actuators.size(); ++i) {
		if (actuators[i]->IsEnabled()) {
			actuators[i]->MoveTempJoint();
		}
	}
	auto fin = std::chrono::system_clock::now();
	double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(fin - start).count();
	DSTR << "calc:" << elapsed << std::endl;
#else
# pragma message("IK: define USE_LAPACK in SprDefs.h to use this function")
#endif
}

void PHIKEngine::LagrangeMultiplierIK(bool nopullback) {
#ifdef USE_LAPACK
	/*
	Vl.resize(J.size1() + J.size2()); Vl.clear();
	Jl.resize(J.size1() + J.size2(), J.size2());

	// <!!> Vlの作成
	for (size_t j = 0; j<endeffectors.size(); ++j) {
		if (endeffectors[j]->IsEnabled() && (endeffectors[j]->bPosition || endeffectors[j]->bOrientation)) {
			// V
			PHIKEndEffector* eff = endeffectors[j];
			PTM::VVector<double> Vpart; Vpart.resize(eff->ndof);
			PTM::VVector<double> wpart; wpart.resize(eff->ndof);
			eff->GetTempTarget(Vpart);
			for (size_t y = 0; y<(size_t)eff->ndof; ++y) {
				size_t Y = strideEff[j] + y;
				Vl[Y] = Vpart[y];
				//We.at_element(Y, Y) = wpart[y];
			}
		}
	}
	for (size_t j = 0; j < Wp.size(); j++) {
		Vl[V.size() + j] = Wp[j];
	}

	// Jlの作成
	for (size_t i = 0; i < J.size1(); i++) {
		for (size_t j = 0; j < J.size2(); j++) {
			Jl(i, j) = J(i, j);
		}
	}
	for (size_t i = 0; i < J.size1(); i++) {
		Jl(J.size1() + i, i) = 1;
	}
	*/
	// <!!> Vの作成
	for (size_t j = 0; j<endeffectors.size(); ++j) {
		if (endeffectors[j]->IsEnabled() && (endeffectors[j]->bPosition || endeffectors[j]->bOrientation)) {
			// V
			PHIKEndEffector* eff = endeffectors[j];
			PTM::VVector<double> Vpart; Vpart.resize(eff->ndof);
			PTM::VVector<double> wpart; wpart.resize(eff->ndof);
			eff->GetTempTarget(Vpart, wpart);
			for (size_t y = 0; y<(size_t)eff->ndof; ++y) {
				size_t Y = strideEff[j] + y;
				V[Y] = Vpart[y];
				//We.at_element(Y, Y) = wpart[y];
			}
		}
	}
	// 

	// 正規化項を計算
	ublas::diagonal_matrix<double> Wn; Wn.resize(J.size2(), J.size2());
	float regularizeValue = 0;
	switch (regularizeMode) {
	case 0:   // Static
		regularizeValue = regularizeParam * regularizeParam;
		break;
	case 1:   // Effector error
	{
		double error = std::min(ublas::norm_2(V), 1.0);
		regularizeValue = regularizeParam * regularizeParam * error;
		break;
	}
	case 2:  // Manipulability measure
	{
		double manipulability = std::sqrt(std::abs(determinant(ublas::prod(J, ublas::trans(J)))));
		if (regularizeParam2 > 0 && manipulability < regularizeParam2) {
			regularizeValue = regularizeParam * regularizeParam * std::pow((1 - manipulability / regularizeParam2), 2.0);
		}
		else {
			regularizeValue = 0;
		}
		break;
	}
	default:  // None
		break;
	}
	for (size_t i = 0; i< J.size2(); ++i) {
		// Tikhonov Regularization
		Wn.at_element(i, i) = regularizeValue + 1;
	}
	// Gauss-Seidelで更新量を計算
	matrix_type JstWe = ublas::trans(J);
	matrix_type JstWeJs = ublas::prod(JstWe, J) + Wn;
	//vector_type WeV = ublas::prod(We, Vl);
	vector_type JtWeV = ublas::prod(ublas::trans(J), V) + Wp;
	/*
	ublas::matrix<double> U, Vt;
	ublas::diagonal_matrix<double> D, Di, Di_;
	svd(JstWeJs, U, D, Vt);

	Di.resize(D.size2(), D.size1());

	for (size_t i = 0; i<(std::min(J.size1(), J.size2())); ++i) {
		// Tikhonov Regularization
		Di.at_element(i, i) = D(i, i) / (D(i, i)*D(i, i));
	}

	// --- 位置
	vector_type      UtV = ublas::prod(ublas::trans(U), JtWeV);
	vector_type    DiUtV = ublas::prod(Di, UtV);
	W = ublas::prod(ublas::trans(Vt), DiUtV);
	/*/
	vector_type x;
	x.resize(J.size2());
	x.clear();
	for (int iter = 0; iter < iterGaussSeidel; ++iter) {
		// 動作確認のため、普通に行列表現で
		for (int i = 0; i < (int) W.size(); i++) {
			double d = x[i];
			x[i] = x[i] + ((JtWeV[i] - ublas::inner_prod(ublas::row(JstWeJs, i), x)) / JstWeJs.at_element(i, i));
			if (std::abs(d - x[i]) < 1e-10) {
				//goto finGS;
			}
		}
	}
//finGS:	// 2020-0420 fk: suppress warning (unused label)
	for (int i = 0; i < (int) x.size(); i++) {
		W[i] = x[i];
	}

	// <!!>各Actuatorのωに擬似逆解を代入
	for (size_t i = 0; i<actuators.size(); ++i) {
		if (actuators[i]->IsEnabled()) {
			PHIKActuator* act = actuators[i];
			for (size_t x = 0; x<(size_t)act->ndof; ++x) {
				size_t X = strideAct[i] + x;
				act->omega[x] = W[X];
			}
		}
	}
	// 結果にしたがってActuatorの一時変数を動かす
	for (size_t i = 0; i<actuators.size(); ++i) {
		if (actuators[i]->IsEnabled()) {
			actuators[i]->MoveTempJoint();
		}
	}
	/*
	// <!!>各EndEffectorのラグランジェ乗数の更新
	for (size_t j = 0; j<endeffectors.size(); ++j) {
		if (endeffectors[j]->IsEnabled()) {
			PHIKEndEffector* eff = endeffectors[j];
			PTM::VVector<double> Vpart; Vpart.resize(eff->ndof);
			for (size_t y = 0; y<(size_t)eff->ndof; ++y) {
				size_t Y = strideEff[j] + y;
				Vpart[y] = V[Y];
			}
			eff->UpdateLagrangeMultiplier(Vpart);
		}
	}
	*/
#else
# pragma message("IK: define USE_LAPACK in SprDefs.h to use this function")
#endif
}

void PHIKEngine::Limit() {
	// IK計算結果をリミットの中に収める
	bool anyLimit = false;
	std::vector<int> disabled;
	for (size_t i=0; i<actuators.size(); ++i) {
		if (actuators[i]->IsEnabled()) {
			bool limit = actuators[i]->LimitTempJoint();
			anyLimit = anyLimit || limit;
			if (limit) {
				actuators[i]->Enable(false);
				disabled.push_back((const int)i);
			}
		}
	}

	// リミットにかかった関節があればIK-Disableして再度IK計算をやりなおす
	// <!!> Prepare->FKではなく？ Prepareするとenableが変化してしまうから先にFK? だとしても、enable=falseでもFKやられたはず
	if (anyLimit) {
		FK();
		Prepare(true);
		CalcJacobian();
		IK();
		for (size_t n=0; n<disabled.size(); ++n) {
			actuators[disabled[n]]->Enable(true);
		}
	}
}

void PHIKEngine::FK() {
	// 順運動学
	for(size_t i=0; i<actuators.size(); ++i){
		if (actuators[i]->parent==NULL) {
			actuators[i]->FK();
		}
	}
}

void PHIKEngine::Move() {
	// 関節の動作
	for(size_t i=0; i<actuators.size(); ++i){
		if (actuators[i]->IsEnabled()) {
			actuators[i]->Move();
		}
	}
	if (constraintChangedIntpRate > 0) {
		constraintChangedIntpRate--;
	}
}

void PHIKEngine::SaveFKResult() {
	for (size_t j=0; j<actuators.size(); ++j) {
		actuators[j]->SaveFKResult();
	}
}

void PHIKEngine::Step() {
	if (!bEnabled) return;
	if (actuators.empty() || endeffectors.empty()) return;
	
	// <!!>
	// ApplyExactState();
	
	Prepare();
	FK();

	// <!!>
	// ApplyExactState(true);

	size_t iter;
	for (iter=0; iter<numIter; ++iter) {
		CalcJacobian();
		if (numIter == 1 || iter < numIter - 1) {
			switch (solverMode) {
				case Mode::SVD:
					IK(false);
					break;
				case Mode::QR:
					LQIK(false);
					break;
				case Mode::LM:
					LagrangeMultiplierIK(false);
					break;
			}
		} else {
			switch (solverMode) {
			case Mode::SVD:
				IK(true);
				break;
			case Mode::QR:
				LQIK(true);
				break;
			case Mode::LM:
				LagrangeMultiplierIK(true);
				break;
			}
		}
		Limit();
		FK();
		SaveFKResult();

		double err = 0;
		for (size_t i=0; i<actuators.size(); ++i) {
			if (actuators[i]->IsEnabled()) {
				PHIKActuator* act = actuators[i];
				for (size_t x=0; x<(size_t)act->ndof; ++x) {
					err += ((act->omega[x])*(act->omega[x]));
				}
			}
		}

		if (sqrt(err) < iterCutOffAngVel) {
			if (numIter > 10) { std::cout << "IK Iter : " << iter << std::endl; }
			break;
		}
	}
	/*/
	CalcJacobian();
	matrix_type A;
	A.resize(J.size2(), J.size2());
	A.clear();
	A = ublas::prod(ublas::trans(J), J) + 0.001 * ublas::identity_matrix<double>(J.size2());
	vector_type x;
	x.resize(J.size2());
	x.clear();
	// <!!>Vの作成
	for (size_t j = 0; j<endeffectors.size(); ++j) {
		if (endeffectors[j]->IsEnabled() && (endeffectors[j]->bPosition || endeffectors[j]->bOrientation)) {
			// V
			PHIKEndEffector* eff = endeffectors[j];
			PTM::VVector<double> Vpart; Vpart.resize(eff->ndof);
			eff->GetTempTarget(Vpart);
			for (size_t y = 0; y<(size_t)eff->ndof; ++y) {
				size_t Y = strideEff[j] + y;
				V[Y] = Vpart[y];
			}
		}
	}
	vector_type b;
	b.resize(J.size2());
	b = ublas::prod(ublas::trans(J), V);
	std::cout << x.size() << " " << V.size() << endl;
	for (iter = 0; iter < numIter; ++iter) {
		for (int i = 0; i < x.size(); i++) {
			x[i] = x[i] + ((b[i] - ublas::inner_prod(ublas::row(A, i), x)) / A.at_element(i,i));
		}
	}
	for (int i = 0; i < x.size(); i++) {
		W[i] = x[i];
	}
	// <!!>非常に大きくなりすぎた解を切り捨てる
	double limitW = 1e+10;
	for (size_t i = 0; i<W.size(); ++i) {
		if (W[i]  >  limitW) { W[i] = limitW; }
		if (W[i]  < -limitW) { W[i] = -limitW; }
	}

	// <!!>各Actuatorのωに擬似逆解を代入
	for (size_t i = 0; i<actuators.size(); ++i) {
		if (actuators[i]->IsEnabled()) {
			PHIKActuator* act = actuators[i];
			for (size_t x = 0; x<(size_t)act->ndof; ++x) {
				size_t X = strideAct[i] + x;
				act->omega[x] = W[X];
			}
		}
	}

	// 結果にしたがってActuatorの一時変数を動かす
	for (size_t i = 0; i<actuators.size(); ++i) {
		if (actuators[i]->IsEnabled()) {
			actuators[i]->MoveTempJoint();
		}
	}
	FK();
	SaveFKResult();
	*/
	
	Move();
}

void PHIKEngine::Clear(){
	for(size_t i=0; i<actuators.size(); ++i){
		delete actuators[i];
	}
	actuators.clear();

	for(size_t i=0; i<endeffectors.size(); ++i){
		delete endeffectors[i];
	}
	endeffectors.clear();
}

PHIKActuator* PHIKEngine::CreateIKActuator(const IfInfo* ii, const PHIKActuatorDesc& desc){
	PHIKActuator* ikactuator = NULL;

	if (ii == PHIKBallActuatorIf::GetIfInfoStatic()) {
		ikactuator = DBG_NEW PHIKBallActuator();
		DCAST(PHIKBallActuatorIf,ikactuator)->SetDesc(&desc);

	} else if (ii == PHIKHingeActuatorIf::GetIfInfoStatic()) {
		ikactuator = DBG_NEW PHIKHingeActuator();
		DCAST(PHIKHingeActuatorIf,ikactuator)->SetDesc(&desc);

	} else if (ii == PHIKSpringActuatorIf::GetIfInfoStatic()) {
		ikactuator = DBG_NEW PHIKSpringActuator();
		DCAST(PHIKSpringActuatorIf, ikactuator)->SetDesc(&desc);

	}
	return ikactuator;
}

PHIKEndEffector* PHIKEngine::CreateIKEndEffector(const PHIKEndEffectorDesc& desc){
	PHIKEndEffector* ikendeffector = NULL;

	ikendeffector = DBG_NEW PHIKEndEffector();
	DCAST(PHIKEndEffectorIf,ikendeffector)->SetDesc(&desc);

	return ikendeffector;
}

bool PHIKEngine::DelChildObject(ObjectIf* o){
	typedef std::vector< UTRef<PHIKActuator>    > ActsU;
	typedef std::vector< UTRef<PHIKEndEffector> > EffsU;
	typedef std::vector< PHIKActuator*          > Acts;
	typedef std::vector< PHIKEndEffector*       > Effs;

	// --- --- --- --- --- --- ---
	// アクチュエータの場合
	PHIKActuator* ia = o->Cast();
	if(ia){
		ActsU::iterator it = std::find(actuators.begin(), actuators.end(), ia);
		if (it != actuators.end()) {
			// actuatorsから削除
			actuators.erase(it);

			// 現存する全てのactuatorのascendant, descendant, children, parent, eefから削除
			for (int i=0; i<(int)actuators.size(); ++i) {
				PHIKActuator* act = actuators[i];
				{
					Acts::iterator it_ = std::find(act->ascendant.begin(), act->ascendant.end(), ia);
					if (it_ != act->ascendant.end()) { act->ascendant.erase(it_); }
				}
				{
					Acts::iterator it_ = std::find(act->descendant.begin(), act->descendant.end(), ia);
					if (it_ != act->descendant.end()) { act->descendant.erase(it_); }
				}
				{
					Acts::iterator it_ = std::find(act->children.begin(), act->children.end(), ia);
					if (it_ != act->children.end()) { act->children.erase(it_); }
				}
				if (act->parent == ia) { act->parent = NULL; }
			}

			return true;
		}
		return false;
	}

	// --- --- --- --- --- --- ---
	// エンドエフェクタの場合
	PHIKEndEffector* ie = o->Cast();
	if(ie){
		std::vector< UTRef<PHIKEndEffector> >::iterator it = std::find(endeffectors.begin(), endeffectors.end(), ie);
		if (it != endeffectors.end()) {
			// endeffectorsから削除
			endeffectors.erase(it);

			// 現存する全てのactuatorのeefから削除
			for (int i=0; i<(int)actuators.size(); ++i) {
				if (actuators[i]->eef == ie) { actuators[i]->eef = NULL; }
			}

			return true;
		}
		return false;
	}

	return false;
}

bool PHIKEngine::AddChildObject(ObjectIf* o){
	// --- --- --- --- --- --- ---
	// アクチュエータの場合
	PHIKBallActuator* bj = o->Cast();
	if(bj){
		if (std::find(actuators.begin(), actuators.end(), bj)==actuators.end()) {
			actuators.push_back(bj);
			bj->number = int(actuators.size()-1);
			return true;
		}
		return false;
	}

	PHIKHingeActuator* hj = o->Cast();
	if(hj){
		if (std::find(actuators.begin(), actuators.end(), hj)==actuators.end()) {
			actuators.push_back(hj);
			hj->number = int(actuators.size()-1);
			return true;
		}
		return false;
	}

	PHIKSpringActuator* sj = o->Cast();
	if (sj) {
		if (std::find(actuators.begin(), actuators.end(), sj) == actuators.end()) {
			actuators.push_back(sj);
			sj->number = int(actuators.size() - 1);
			return true;
		}
		return false;
	}

	// --- --- --- --- --- --- ---
	// エンドエフェクタの場合
	PHIKEndEffector* ef = o->Cast();
	if(ef){
		if (std::find(endeffectors.begin(), endeffectors.end(), ef)==endeffectors.end()) {
			endeffectors.push_back(ef);
			ef->number = int(endeffectors.size()-1);
			return true;
		}
		return false;
	}

	return false;
}

}

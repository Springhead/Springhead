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

using namespace std;
namespace Spr{

//static std::ofstream *dlog;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// IKEngine
PHIKEngine::PHIKEngine():numIter(25), bEnabled(false), bTrajectory(false), bTest(false), pullbackRate(0.1), linearDist(0.1), lastM(0), lastN(0) {}

void PHIKEngine::Prepare() {
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
							int X = strideAct[i] + x;
							int Y = strideEff[j] + y;
							J.at_element(Y,X) = Jpart[y][x];
						}
					}
				}
			}
		}
	}
	// std::cout << J << std::endl;

	// <!!>引き戻し速度の計算
	for(size_t i=0; i<actuators.size(); ++i){
		if (actuators[i]->IsEnabled()) {
			PHIKActuator* act = actuators[i];

			act->CalcPullbackVelocity(pullbackRate);

			for (size_t x=0; x<act->ndof; ++x) {
				int X = strideAct[i] + x;
				Wp[X] = act->domega_pull[x];
			}
		}
	}
}

void PHIKEngine::IK() {
	// 計算の準備（α・β・γの事前計算）
	for(size_t i=0; i<actuators.size(); ++i){
		actuators[i]->PrepareSolve();
	}

	// <!!>Vの作成
	for (size_t j=0; j<endeffectors.size(); ++j){
		if (endeffectors[j]->IsEnabled()) {
			PHIKEndEffector* eff = endeffectors[j];
			PTM::VVector<double> Vpart; Vpart.resize(eff->ndof);
			eff->GetTempTarget(Vpart);
			for (size_t y=0; y<eff->ndof; ++y) {
				int Y = strideEff[j] + y;
				V[Y] = Vpart[y];
			}
		}
	}
	// std::cout << "V : " << V << std::endl;

	// <!!>擬似逆解を求める・lapack版
	/*
	bool bOverwrite = true;
	ublas::vector<double> S;
	int rank = least_squares(J, V, W, S, 0.05);
	*/

	bool bOverwrite = true;

	// <!!>擬似逆解を求める・lapack-SVD版
	vector_type S; S.resize((std::min)(J.size1(), J.size2())); S.clear();

	// lapack::gesdd('A', J, S, U, Vt);
	ublas::matrix<double> U, Vt;
	ublas::diagonal_matrix<double> D, Di;
	svd(J, U, D, Vt);

	Di.resize(D.size2(), D.size1());
	for (size_t i=0; i<(std::min(J.size1(),J.size2())); ++i) {
		if (D(i,i) > 1e-5) {
			Di.at_element(i, i) = 1/D(i,i);
		} else {
			Di.at_element(i, i) = 0;
		}
	}

	vector_type   UtV  = ublas::prod(ublas::trans(U)  , V    );
	vector_type DiUtV  = ublas::prod(Di               , UtV  );
	W                  = ublas::prod(ublas::trans(Vt) , DiUtV);



	// <!!>Wに引き戻し速度を加える
	vector_type      JWp = ublas::prod(J                ,      Wp);
	vector_type    UtJWp = ublas::prod(ublas::trans(U)  ,     JWp);
	vector_type  DiUtJWp = ublas::prod(Di               ,   UtJWp);
	vector_type VDiUtJWp = ublas::prod(ublas::trans(Vt) , DiUtJWp);
	W = W + Wp - VDiUtJWp;

	/*
	matrix_type DiUt = ublas::prod(Di, ublas::trans(U));
	matrix_type Js   = ublas::prod(ublas::trans(Vt), DiUt);
	matrix_type JsJ  = ublas::prod(Js, J);
	matrix_type I    = ublas::identity_matrix<double>(JsJ.size1(), JsJ.size2());
	vector_type ns   = ublas::prod(I-JsJ, Wp);

	matrix_type JJsJ = ublas::prod(J, JsJ);
	std::cout << "Check : " << (J - JJsJ) << std::endl;
	*/





	for (size_t i=0; i<W.size(); ++i) {
		if (W[i] >  1.0e+10) { W[i] = 0; }
		if (W[i] < -1.0e+10) { W[i] = 0; }
	}

	// 繰り返し計算の実行
	int iter = (int)((numIter > 0) ? numIter : 200);
	for(int n=0; n<iter; n++){
		double dErr = 0;
		for(size_t i=0; i<actuators.size(); ++i){
			if (actuators[i]->IsEnabled()) {
				actuators[i]->ProceedSolve();
				PTM::VVector<double> diff = actuators[i]->omega_prev - actuators[i]->omega;
				dErr += (diff.norm() * diff.norm());
			}
		}
		if ((((int)numIter) < 0) && (sqrt(dErr) < 1e-8)) { break; }
	}

	// <!!>計算結果を擬似逆解で上書き
	if (bOverwrite) {
		for (size_t i=0; i<actuators.size(); ++i) {
			if (actuators[i]->IsEnabled()) {
				PHIKActuator* act = actuators[i];
				for (size_t x=0; x<act->ndof; ++x) {
					int X = strideAct[i] + x;
					act->omega[x] = W[X];
				}
			}
		}
	}

	// 結果をActuatorの一時変数に保存
	for(size_t i=0; i<actuators.size(); ++i){
		if (actuators[i]->IsEnabled()) {
			actuators[i]->MoveTempJoint();
		}
	}
}

void PHIKEngine::FK() {
	// 順運動学
	for(size_t i=0; i<actuators.size(); ++i){
		if (actuators[i]->IsEnabled() && actuators[i]->parent==NULL) {
			actuators[i]->FK();
		}
	}
}

void PHIKEngine::Pullback() {
	/*
	// <!!>Wに引き戻し速度を加える
	vector_type      JWp = ublas::prod(J                ,      Wp);
	vector_type    UtJWp = ublas::prod(ublas::trans(U)  ,     JWp);
	vector_type  DiUtJWp = ublas::prod(Di               ,   UtJWp);
	vector_type VDiUtJWp = ublas::prod(ublas::trans(Vt) , DiUtJWp);
	W = Wp - VDiUtJWp;
	for (size_t i=0; i<W.size(); ++i) {
		if (W[i] >  1.0e+10) { W[i] = 0; }
		if (W[i] < -1.0e+10) { W[i] = 0; }
	}

	// <!!>計算結果を上書き
	for (size_t i=0; i<actuators.size(); ++i) {
		if (actuators[i]->IsEnabled()) {
			PHIKActuator* act = actuators[i];
			for (size_t x=0; x<act->ndof; ++x) {
				int X = strideAct[i] + x;
				act->omega[x] = W[X];
			}
		}
	}

	// 結果をActuatorの一時変数に保存
	for(size_t i=0; i<actuators.size(); ++i){
		if (actuators[i]->IsEnabled()) {
			actuators[i]->MoveTempJoint();
		}
	}
	*/
}

void PHIKEngine::Move() {
	// 関節の動作
	for(size_t i=0; i<actuators.size(); ++i){
		if (actuators[i]->IsEnabled()) {
			actuators[i]->Move();
		}
	}
}

void PHIKEngine::Step() {
	if (!bEnabled) return;
	if (actuators.empty() || endeffectors.empty()) return;

	// std::cout << "1" << std::endl;

	Prepare();
	// std::cout << "2" << std::endl;

	// 開始地点を標準姿勢側に引き戻し　→　FK
	/*
	for(size_t i=0; i<actuators.size(); ++i){
		if (actuators[i]->IsEnabled()) {
			actuators[i]->PullbackTempJoint(pullbackRate);
		}
	}
	*/
	FK();
	// std::cout << "3" << std::endl;

	for (size_t j=0; j<actuators.size(); ++j) {
		if (actuators[j]->IsEnabled()) {
			actuators[j]->SaveFKResult();
		}
	}

	// IK繰り返し数決定
	/*
	PHIKEndEffector* eef = NULL;
	double maxdev = 0;
	for (size_t i=0; i<endeffectors.size(); ++i) {
		if (endeffectors[i]->IsEnabled()) {
			double dev = endeffectors[i]->PosDeviation();
			if (dev > maxdev) {
				maxdev = dev;
				eef    = endeffectors[i];
			}
		}
	}
	int iter = min((int)(maxdev/linearDist), 50);
	std::cout << "max=" << eef->GetName() << ", iter=" << iter << std::endl;
	*/

	CalcJacobian();
	// std::cout << "4" << std::endl;
	IK();
	// std::cout << "5" << std::endl;


	// <!!>Pullback地点候補そのいち
	/*
	bool bPullback = true;
	for (size_t i=0; i<endeffectors.size(); ++i) {
		PTM::VVector<double> tt; endeffectors[i]->GetTempTarget(tt);
		Vec3d temp = Vec3d(tt[0], tt[1], tt[2]);
		Vec3d goal; endeffectors[i]->GetTargetPosition();
		if ((temp - goal).norm() > 0.1) { bPullback = false; }
	}

	if (bPullback) {
		Pullback();
	}
	*/



	FK();
	// std::cout << "6" << std::endl;

	for (size_t j=0; j<actuators.size(); ++j) {
		if (actuators[j]->IsEnabled()) {
			actuators[j]->SaveFKResult();
		}
	}
	// std::cout << "7" << std::endl;

	Move();
	// std::cout << "8" << std::endl;


	// <!!>Pullback地点候補そのに
	bool bPullback = true;
	for (size_t i=0; i<endeffectors.size(); ++i) {
		PTM::VVector<double> tt; endeffectors[i]->GetTempTarget(tt);
		Vec3d temp = Vec3d(tt[0], tt[1], tt[2]);
		Vec3d goal; endeffectors[i]->GetTargetPosition();
		if ((temp - goal).norm() > 0.1) { bPullback = false; }
	}

	if (bPullback) {
		Pullback();
	}

	// std::cout << "9" << std::endl;
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

	}
	return ikactuator;
}

PHIKEndEffector* PHIKEngine::CreateIKEndEffector(const PHIKEndEffectorDesc& desc){
	PHIKEndEffector* ikendeffector = NULL;

	ikendeffector = DBG_NEW PHIKEndEffector();
	DCAST(PHIKEndEffectorIf,ikendeffector)->SetDesc(&desc);

	return ikendeffector;
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

	// --- --- --- --- --- --- ---
	// 制御点の場合
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

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
PHIKEngine::PHIKEngine():
	numIter(1),
	bEnabled(false),
	bTest(false),
	maxVel(20), // 20[m/s]
	maxAngVel(Rad(500)), // 500[deg/s]
	maxActVel(Rad(30)),  //  10[deg/s]
	regularizeParam(0.7),
	lastM(0),
	lastN(0),
	iterCutOffAngVel(0.01)
	{}

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
	if (second) {
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

void PHIKEngine::IK() {
	// 計算の準備（α・β・γの事前計算）
	for(size_t i=0; i<actuators.size(); ++i){
		actuators[i]->PrepareSolve();
	}

	// <!!>Vの作成
	for (size_t j=0; j<endeffectors.size(); ++j){
		if (endeffectors[j]->IsEnabled()) {
			// V
			PHIKEndEffector* eff = endeffectors[j];
			PTM::VVector<double> Vpart; Vpart.resize(eff->ndof);
			eff->GetTempTarget(Vpart);
			for (size_t y=0; y<(size_t)eff->ndof; ++y) {
				size_t Y = strideEff[j] + y;
				V[Y] = Vpart[y];
			}
		}
	}
	// std::cout << "V : " << V << std::endl;

	// <!!>擬似逆解を求める・lapack-SVD版
	vector_type S; S.resize((std::min)(J.size1(), J.size2())); S.clear();

	ublas::matrix<double> U, Vt;
	ublas::diagonal_matrix<double> D, Di;
	svd(J, U, D, Vt);

	Di.resize(D.size2(), D.size1());
	for (size_t i=0; i<(std::min(J.size1(),J.size2())); ++i) {
		// Tikhonov Regularization
		Di.at_element(i,i) = D(i,i) / (D(i,i)*D(i,i) + regularizeParam*regularizeParam);
	}

	// --- 位置
	vector_type   UtV  = ublas::prod(ublas::trans(U)  , V    );
	vector_type DiUtV  = ublas::prod(Di               , UtV  );
	W                  = ublas::prod(ublas::trans(Vt) , DiUtV);

	// <!!>Wに標準姿勢復帰速度を加える
	vector_type      JWp = ublas::prod(J                ,      Wp);
	vector_type    UtJWp = ublas::prod(ublas::trans(U)  ,     JWp);
	vector_type  DiUtJWp = ublas::prod(Di               ,   UtJWp);
	vector_type VDiUtJWp = ublas::prod(ublas::trans(Vt) , DiUtJWp);
	W = W + Wp - VDiUtJWp;

	// <!!>非常に大きくなりすぎた解を切り捨てる
	double limitW = 1e+10;
	for (size_t i=0; i<W.size(); ++i) {
		if (W[i]  >  limitW) { W[i]  =  limitW; }
		if (W[i]  < -limitW) { W[i]  = -limitW; }
	}

	// <!!>各Actuatorのωに擬似逆解を代入
	for (size_t i=0; i<actuators.size(); ++i) {
		if (actuators[i]->IsEnabled()) {
			PHIKActuator* act = actuators[i];
			for (size_t x=0; x<(size_t)act->ndof; ++x) {
				size_t X = strideAct[i] + x;
				act->omega[x] = W[X];
			}
		}
	}

	// 結果にしたがってActuatorの一時変数を動かす
	for(size_t i=0; i<actuators.size(); ++i){
		if (actuators[i]->IsEnabled()) {
			actuators[i]->MoveTempJoint();
		}
	}
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
	// ApplyExactState(/* reverse = */ true);

	size_t iter;
	for (iter=0; iter<numIter; ++iter) {
		CalcJacobian();
		IK();
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

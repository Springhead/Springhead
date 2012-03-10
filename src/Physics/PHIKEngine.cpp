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
PHIKEngine::PHIKEngine():numIter(25), bEnabled(false), bTrajectory(false), bTest(false) {}

void PHIKEngine::Step(){
	if(!bEnabled) return;
	if(actuators.empty() || endeffectors.empty()) return;

	// 計算用変数準備の前処理
	for(size_t i=0; i<actuators.size(); ++i){
		actuators[i]->BeforeSetupMatrix();
	}

	// 計算用変数の準備
	for(size_t i=0; i<actuators.size(); ++i){
		actuators[i]->SetupMatrix();
	}

	// 変化フラグのクリア
	for(size_t i=0; i<actuators.size(); ++i){
		actuators[i]->bActuatorAdded	= false;
		actuators[i]->bNDOFChanged		= false;
	}
	for(size_t i=0; i<endeffectors.size(); ++i){
		endeffectors[i]->bNDOFChanged	= false;
	}

	for (int nStatic=0; nStatic<1; ++nStatic) {

		// ヤコビアン計算の前処理
		for(size_t i=0; i<actuators.size(); ++i){
			actuators[i]->BeforeCalcAllJacobian();
		}

		// ヤコビアンの計算
		for(size_t i=0; i<actuators.size(); ++i){
			actuators[i]->CalcAllJacobian();
		}

		// 計算の準備（α・β・γの事前計算）
		for(size_t i=0; i<actuators.size(); ++i){
			actuators[i]->PrepareSolve();
		}

		// 繰り返し計算の実行
		int iter = ((numIter > 0) ? numIter : 200);
		for(int n=0; n<iter; n++){
			double dErr = 0;
			for(size_t i=0; i<actuators.size(); ++i){
				actuators[i]->ProceedSolve();
				PTM::VVector<double> diff = actuators[i]->omega_prev - actuators[i]->omega;
				dErr += (diff.norm() * diff.norm());
			}
			if ((((int)numIter) < 0) && (sqrt(dErr) < 1e-8)) { break; }
		}

		// 関節の動作
		for(size_t i=0; i<actuators.size(); ++i){
			actuators[i]->Move();
		}
	}
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

PHIKEndEffector* PHIKEngine::CreateIKEndEffector(const IfInfo* ii, const PHIKEndEffectorDesc& desc){
	PHIKEndEffector* ikendeffector = NULL;

	if (ii == PHIKEndEffectorIf::GetIfInfoStatic()) {
		ikendeffector = DBG_NEW PHIKEndEffector();
		DCAST(PHIKEndEffectorIf,ikendeffector)->SetDesc(&desc);

	}
	return ikendeffector;
}

bool PHIKEngine::AddChildObject(ObjectIf* o){
	// --- --- --- --- --- --- ---
	// アクチュエータの場合
	PHIKBallActuator* bj = o->Cast();
	if(bj){
		actuators.push_back(bj);
		bj->number = (actuators.size()-1);
		return true;
	}

	PHIKHingeActuator* hj = o->Cast();
	if(hj){
		actuators.push_back(hj);
		hj->number = (actuators.size()-1);
		return true;
	}

	// --- --- --- --- --- --- ---
	// 制御点の場合
	PHIKEndEffector* ef = o->Cast();
	if(ef){
		endeffectors.push_back(ef);
		ef->number = (endeffectors.size()-1);
		return true;
	}

	return false;
}

}

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHJointMotor.h>
#include <Physics/PHJoint.h>
#include <Physics/PHBallJoint.h>
#include <Physics/PHSpring.h>
#include <Physics/PHConstraintEngine.h>

#include <iomanip>

using namespace PTM;
using namespace std;
namespace Spr{;

// バネ・ダンパ係数を0と見なす閾値
const double epsilon = 1e-10;
//const double inf     = 1e+10;
const double inf     = 1e+32;	//	2022.01.13 strong spring was needed. (hasevr)

double JointFunctions::resistCalc(double d, double k_1, double k_2, double k_3, double k_4){
	return exp(k_1 * (d - k_2)) - exp(k_3 * (k_4 - d));
}

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----

// PH1DJointNonLinearMotorのFuncDatabase

// <!!> マクローリン展開での近似にして係数を渡す？より広範囲だとテイラー
// <!!>データ点と傾きを与えて、スプライン近似？ 

Vec2d JointFunctions::ResistanceTorque(PH1DJointIf* jo, void* param){
	double k_1 = ((double*)param)[0];
	double k_2 = ((double*)param)[1];
	double k_3 = ((double*)param)[2];
	double k_4 = ((double*)param)[3];
	double delta = jo->GetPosition() + jo->GetVelocity() * jo->GetScene()->GetTimeStep();
	if (DCAST(PH1DJointLimitIf, jo->GetLimit())){
		Vec2d range;
		jo->GetLimit()->GetRange(range);
		if (delta < range[0]){
			delta = range[0];
		}
		else if (delta > range[1]){
			delta = range[1];
		}
	}
	double k = k_1 * exp(k_1 * (delta - k_2)) + k_3 * exp(k_3 * (k_4 - delta));
	//DSTR << k_1 << " " << k_2 << " " << k_3 << " " << k_4 << " " << delta << std::endl;
	double t = (k == 0 ? 0 : delta - (exp(k_1 * (delta - k_2)) - exp(k_3 * (k_4 - delta))) / k);
	//DSTR << k << " " << t << std::endl;
	return Vec2d(abs(k), t);
}

Vec2d JointFunctions::PD(PH1DJointIf* jo, void* param){
	return Vec2d(((double*)param)[0], ((double*)param)[1]);
}

Vec2d (*PH1DJointFunc[])(PH1DJointIf*, void*) = {
	JointFunctions::PD,
	JointFunctions::ResistanceTorque,
};

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----

// PHBallJointNonLinearMotorのFuncDatabase

Vec2d JointFunctions::ResistanceTorque(int a, PHBallJointIf* jo, void* param){
	double k_1 = ((double*)param)[0];
	double k_2 = ((double*)param)[1];
	double k_3 = ((double*)param)[2];
	double k_4 = ((double*)param)[3];
	Vec3d dq;
	jo->GetPosition().ToEuler(dq);
	dq = Vec3d(dq.z, dq.x, dq.y);
	dq += jo->GetVelocity() * jo->GetScene()->GetTimeStep();
	if (a < 0 || a >= 3) return Vec2d();
	double delta = dq[a];
	if (DCAST(PHBallJointIndependentLimitIf, jo->GetLimit())){
		Vec2d range;
		DCAST(PHBallJointIndependentLimitIf, jo->GetLimit())->GetLimitRangeN(a, range);
		if (delta < range[0]){
			delta = range[0];
		}
		else if (delta > range[1]){
			delta = range[1];
		}
	}
	double dt = jo->GetScene()->GetTimeStep();
	double torque = resistCalc(delta, k_1, k_2, k_3, k_4);
	if (abs(torque) > (jo->GetMaxForce() / 2)) { 
		DSTR << "over:" << torque << std::endl; 
	}
	double k = k_1 * exp(k_1 * (delta - k_2)) + k_3 * exp(k_3 * (k_4 - delta));
	double t = (k == 0 ? 0 : delta - (exp(k_1 * (delta - k_2)) - exp(k_3 * (k_4 - delta))) / k);
	return Vec2d(abs(k), t);
}
Vec2d JointFunctions::PD(int a, PHBallJointIf* jo, void* param){
	return Vec2d(((double*)param)[0], ((double*)param)[1]);
}
Vec2d(*PHBallJointFunc[])(int a, PHBallJointIf*, void*) = {
	JointFunctions::PD,
	JointFunctions::ResistanceTorque,
};

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 

// PHNDJointMotor
template<int NDOF>
void PHNDJointMotor<NDOF>::SetupAxisIndex(){
	PHNDJointMotorParam<NDOF> p; GetParams(p);
	axes.Clear();
	for(int n = 0; n < joint->movableAxes.size(); ++n) {
		if (p.spring[n] >= epsilon || p.damper[n] >= epsilon || std::abs(p.offsetForce[n]) >= epsilon){
			axes.Enable(n);
			joint->targetAxes.Enable(joint->movableAxes[n]);
		}
	}
}	

template<int NDOF>
void PHNDJointMotor<NDOF>::Setup(){
	PHNDJointMotorParam<NDOF> p; GetParams(p);
	PHSceneIf* scene = joint->GetScene();
	double dt = scene->GetTimeStep();
	
	for(int n = 0; n < joint->movableAxes.size(); ++n){
		int j = joint->movableAxes[n];
		double fmax = std::min(
			(j < 3 ? scene->GetMaxForce() : scene->GetMaxMoment()),
			joint->GetMaxForce());
		fMaxDt[n]  =  (fmax * dt);
		fMinDt[n]  = -(fmax * dt);
	}

	for(int n = 0; n < axes.size(); ++n) {
		int j = axes[n];

		b [j] = joint->b [joint->movableAxes[j]];
		dv[j] = joint->dv[joint->movableAxes[j]];

		f [j] *= axes.IsContinued(j) ? joint->engine->shrinkRate : 0;
	}

	if (p.spring.norm() < epsilon && p.damper.norm() < epsilon) {
		// オフセット力のみ有効の場合は拘束力初期値に設定するだけでよい
		for(int n = 0; n < joint->movableAxes.size(); ++n){
			f [n] = p.offsetForce[n] * dt;
			dA[n] = inf;
			db[n] = inf;
		}
	}
	else {
		VecNd sd = p.secondDamper;
		bool bHasSecondDamper = true;
		for (int i=0; i<NDOF; ++i) {
			if (sd[i] > FLT_MAX*0.1) {
				bHasSecondDamper = false;
			}
		}
		if (!bHasSecondDamper) {
			// 第二ダンパを無視（バネダンパ：弾性変形）
			CompBiasElastic();
		} else {
			// 第二ダンパ有効（三要素モデル）
			if (p.yieldStress > (FLT_MAX * 0.1)) {
				// 降伏応力無効（塑性変形）
				CompBiasPlastic();
			} else {
				// 降伏応力有効（弾塑性変形）
				CheckYielded();
				if (p.bYielded) {
					CompBiasPlastic();
				} else {
					CompBiasElastic();
				}
			}
		}
	}

	for(int n = 0; n < axes.size(); ++n){
		int j = axes[n];
		A   [n] = joint->A[joint->movableAxes[j]];
		if(A[n] + dA[n] < inf)
			 Ainv[n] = 1.0 / (A[n] + dA[n]);
		else Ainv[n] = 0.0;
	}

	SetParams(p);

}

template<int NDOF>
bool PHNDJointMotor<NDOF>::Iterate(){
	bool updated = false;
	SpatialMatrix Jdot[2];

	//Jdot[0] =(joint->J[0] - joint->preJ[0]) /** timeStepInv*/;
	//Jdot[1] = (joint->J[1] - joint->preJ[1]) /** timeStepInv*/;
	//cout << "Jdot[0] = " << joint->Jdot[0] << endl;
	//cout << "Jdot[1] = " << joint->Jdot[1] << endl;
	//cout << "preJdot[0] = " << joint->preJdot[0] << endl;
	//cout << "preJdot[1] = " << joint->preJdot[1] << endl;
	//cout << "Jdotdot[0] = " << joint->Jdotdot[0] << endl;
	//cout << "Jdotdot[1] = " << joint->Jdotdot[1] << endl;
	//Jdot[0] = Jdot[0]* timeStepInv;
	//Jdot[1] = Jdot[1]* timeStepInv;
	//cout << "Jdot[0] = " << Jdot[0] << endl;
	//cout << "Jdot[1] = " << Jdot[1] << endl;
	//cout << "joint->J[0] = " << joint->J[0] << endl;
	//cout << "joint->preJ[0] = " << joint->preJ[0] << endl;
	//cout << "joint->J[1] = " << joint->J[1] << endl;
	//cout << "joint->preJ[1] = " << joint->preJ[1] << endl;
	for (int n=0; n<axes.size(); ++n) {
		int i = axes[n];
		int j = joint->movableAxes[i];
		if (joint->solid[1]->treeNode != NULL) {
			joint->dv[j] = joint->J[0].row(j) * joint->solid[0]->dv
				+ joint->J[1].row(j) * (joint->solid[1]->dv - joint->solid[1]->treeNode->c);
			//if (joint->GetScene()->GetCount() == 0) {
			//	joint->dv[j] = joint->J[0].row(j) * joint->solid[0]->dv
			//		+ joint->J[1].row(j) * joint->solid[1]->dv;
			//} else {
			//	joint->dv[j] = joint->J[0].row(j) * joint->solid[0]->dv
			//		+ joint->J[1].row(j) * joint->solid[1]->dv + Jdot[1].row(j) * joint->solid[1]->v;
			//}
			
			//cout << j << "J[1] * c    " << joint->J[1].row(j) * (-joint->solid[1]->treeNode->c) << endl;
			//cout << j << "Jdot[1] * v " << joint->Jdot[1].row(j) * joint->solid[1]->v << endl;
			//cout << j << "Jdot[0] * v " << joint->solid[0]->v << endl;
			//cout << "joint->J[1].row(j) " << joint->J[1].row(j) << "c " << -joint->solid[1]->treeNode->c << endl;
			//cout << "Jdot[1].row(j) " << joint->Jdot[1].row(j) << "v " << joint->solid[1]->v << endl;
/*			Posed posePlug;
			joint->GetPlugPose(posePlug);
			Matrix3d matPosePlug;
			posePlug.Ori().ToMatrix(matPosePlug);
			Matrix3d matXjrel;
			joint->Xjrel.q.Conjugated().ToMatrix(matXjrel);
			cout << "Matrix3d::Cross(joint->vjrel.w()) * matXjrel " << Matrix3d::Cross(joint->vjrel.w()) * matXjrel << endl;
			if (joint->GetScene()->GetCount() != 0) {
				cout << "(Matrix3d::Cross(joint->vjrel.w()) * matXjrel).inv() " << (Matrix3d::Cross(joint->vjrel.w()) * matXjrel).inv() << endl;
				cout << "(Matrix3d::Cross(joint->vjrel.w()) * matXjrel).inv() * matPosePlug * joint->solid[1]->v " << (Matrix3d::Cross(joint->vjrel.w()) * matXjrel).inv() * matPosePlug * joint->solid[1]->v.w() << endl;
		}*/	

		}
		else {
			joint->dv[j] = joint->J[0].row(j) * joint->solid[0]->dv
				+ joint->J[1].row(j) * joint->solid[1]->dv;
		}
		dv  [i] = joint->dv[j];
		res [i] = b[i] + db[i] + dA[i]*f[i] + dv[i];
		fnew[i] = f[i] - joint->engine->accelSOR * Ainv[i] * res[i];
	
		fnew[i] = min(max(fMinDt[i], fnew[i]), fMaxDt[i]);

		df[i] = fnew[i] - f[i];
		f [i] = fnew[i];

		if(std::abs(df[i]) > joint->engine->dfEps){
			updated = true;
			CompResponse(df[i], i);
		}
	}

	return updated;
}

template<int NDOF>
void PHNDJointMotor<NDOF>::CompResponse(double df, int i){
	joint->CompResponse(df, joint->movableAxes[i]);
}

/// 弾性変形用のCompBias
template<int NDOF>
void PHNDJointMotor<NDOF>::CompBiasElastic() {
	PHNDJointMotorParam<NDOF> p; GetParams(p);

	double dt = joint->GetScene()->GetTimeStep();
	VecNd propV = GetPropV();
	
	for(int n = 0; n < axes.size(); ++n) {
		int j = axes[n];
		
		double K   = p.spring[j];
		double D   = p.damper[j];
		if(K < epsilon && D < epsilon){
			dA[j] = inf;
			db[j] = inf;
		}
		else{
			double tmp = 1.0 / (D + K*dt);
			dA[j] = tmp * (1.0/dt);
			db[j] = tmp * (-K*propV[j] - p.offsetForce[j])- p.targetVelocity[j] ; 
		}
	}

	SetParams(p);
}

/// 塑性変形用のCompBias
template<int NDOF>
void PHNDJointMotor<NDOF>::CompBiasPlastic() {
	PHNDJointMotorParam<NDOF> p; GetParams(p);

	double dt = joint->GetScene()->GetTimeStep();

	for(int n = 0; n < axes.size(); ++n) {
		int j = axes[n];
		
		// こっちだけhardnessRate掛けてるけどいいの？
		double K   = p.spring      [j] * p.hardnessRate;
		double D   = p.damper      [j] * p.hardnessRate;
		double D2  = p.secondDamper[j] * p.hardnessRate;
		double tmp = D+D2+K*dt;

		int i = joint->movableAxes[j];
		newXs[i] = ((D+D2)/tmp)*p.xs[i] + (D2*dt/tmp) * joint->vjrel[i];

		dA[j] = tmp/(D2*(K*dt + D)) * (1.0/dt);
		db[j] = K/(K*dt + D)*(p.xs[i]);
	}

	// 弾塑性変形：TargetPositionを変更して残留変位を残す
	if (p.yieldStress < (FLT_MAX * 0.1)) {
		if (joint->vjrel.w().norm() < 0.01) {
			PH1DJoint* jnt1D = joint->Cast();
			if(jnt1D)
				jnt1D->SetTargetPosition(jnt1D->GetPosition());
			PHBallJoint* ball = joint->Cast();
			if(ball)
				ball->SetTargetPosition(ball->GetPosition());
		}
	}

	p.xs = newXs;

	SetParams(p);
}

/// 降伏したかどうか
template<int NDOF>
void PHNDJointMotor<NDOF>::CheckYielded() {
	PHNDJointMotorParam<NDOF> p; GetParams(p);

	for(int n = 0; n < axes.size(); n++){
		int j = axes[n];
		p.fAvg[j] = (0.8 * p.fAvg[j]) + (0.2 * f[j] * joint->GetScene()->GetTimeStepInv());
	}

	if(p.bYielded) {
		if (p.fAvg.norm() < p.yieldStress) {
			if (NDOF==3) {
				Vec3d angle = newXs.w();
				Quaterniond qForct;
				qForct.FromEuler(Vec3d(angle.y, angle.z, angle.x));
				DCAST(PHBallJoint,joint)->SetTargetPosition( qForct.Inv() * joint->Xjrel.q );
			}   // NDOF=1や，NDOF=6(PHSpring）の場合は未実装 <!!>
			p.bYielded = false;
		}
	} else {
		if (p.fAvg.norm() > p.yieldStress) {
			p.bYielded = true;
		}
	}

	SetParams(p);
}

// テンプレートの明示的実体化（PHNDJointMotorのメンバ関数をcppで定義できるようにするための措置）
template class PHNDJointMotor<1>;
template class PHNDJointMotor<3>;
template class PHNDJointMotor<6>;

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PH1DJointMotor

/// propVを計算する
PTM::TVector<1,double> PH1DJointMotor::GetPropV() {
	PH1DJoint* j = joint->Cast();
	PTM::TVector<1,double> propV;
	propV[0] = -1.0 * j->GetDeviation();
	return propV;
}

/// パラメータを取得する
void PH1DJointMotor::GetParams(PHNDJointMotorParam<1>& p) {
	PH1DJoint* j = joint->Cast();
	p.fAvg[0]           = j->fAvg[0];
	p.xs                = j->xs;
	p.bYielded          = j->bYielded;
	p.spring[0]         = j->spring;
	p.damper[0]         = j->damper;
	p.secondDamper[0]   = j->secondDamper;
	p.targetVelocity[0] = j->targetVelocity;
	p.offsetForce[0]    = j->offsetForce;
	p.yieldStress       = j->yieldStress;
	p.hardnessRate      = j->hardnessRate;
}

/// パラメータを反映する
void PH1DJointMotor::SetParams(PHNDJointMotorParam<1>& p) {
	PH1DJoint* j = joint->Cast();
	j->fAvg[0]  = p.fAvg[0];
	j->xs       = p.xs;
	j->bYielded = p.bYielded;
	// 上記以外の変数は特に反映する必要はない．
}

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PH1DJointNonLinearMotor
void PH1DJointNonLinearMotor::SetSpring(FunctionMode m, void* param){
	if (!(m < 0 || m > sizeof(PH1DJointFunc) / sizeof(PH1DJointFunc[0]))) {
		springMode = m;
		this->springParam = param;
	}
}
void PH1DJointNonLinearMotor::SetDamper(FunctionMode m, void* param) {
	if (!(m < 0 || m > sizeof(PH1DJointFunc) / sizeof(PH1DJointFunc[0]))) {
		damperMode = m;
		this->damperParam = param;
	}
}
void PH1DJointNonLinearMotor::SetSpringDamper(FunctionMode smode, FunctionMode dmode, void* sparam, void* dparam){
	SetSpring(smode, sparam);
	SetDamper(dmode, dparam);
}

/// propVを計算する
PTM::TVector<1, double> PH1DJointNonLinearMotor::GetPropV() {
	PH1DJoint* j = joint->Cast();
	PTM::TVector<1, double> propV;
	/*if (m_func == NULL)
	propV[0] = -1.0 * j->GetDeviation();
	else{*/
		propV[0] = -1.0 * (j->GetPosition() - targetPos);
	//}
	
	return propV;
}

/// パラメータを取得する
void PH1DJointNonLinearMotor::GetParams(PHNDJointMotorParam<1>& p) {
	PH1DJoint* j = joint->Cast();
	p.fAvg[0] = j->fAvg[0];
	p.xs = j->xs;
	p.bYielded = j->bYielded;
	p.secondDamper[0] = j->secondDamper;
	p.offsetForce[0] = offset;
	p.yieldStress = j->yieldStress;
	p.hardnessRate = j->hardnessRate;
	//関数存在するならそれに合わせてspringとdamper, 各targetを変更
	Vec2d sp = PH1DJointFunc[springMode](joint->Cast(), springParam);
	p.spring[0] = sp[0];
	targetPos = sp[1];
	Vec2d da = PH1DJointFunc[damperMode](joint->Cast(), damperParam);
	p.damper[0] = da[0];
	p.targetVelocity[0] = da[1];
}

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
//PHHuman1DJointResistance

//現在の関節抵抗値
double PHHuman1DJointResistance::GetCurrentResistance() {
	double* spring = ((double*)springParam);
	PH1DJointIf* jo = joint->Cast();
	return JointFunctions::resistCalc(jo->GetPosition(), spring[0], spring[1], spring[2], spring[3]);
}

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHBallJointMotor

/// propVを計算する
PTM::TVector<3,double> PHBallJointMotor::GetPropV() {
	PHBallJoint* j = joint->Cast();
	Quaterniond pQ = j->targetPosition * joint->Xjrel.q.Inv();
	return((PTM::TVector<3,double>)(pQ.RotationHalf()));
}

/// パラメータを取得する
void PHBallJointMotor::GetParams(PHNDJointMotorParam<3>& p) {
	PHBallJoint* j = joint->Cast();
	p.fAvg           = j->fAvg.w();
	p.xs             = j->xs;
	p.bYielded       = j->bYielded;
	p.spring         = PTM::TVector<3,double>(Vec3d(1,1,1) * j->spring);
	p.damper         = PTM::TVector<3,double>(Vec3d(1,1,1) * j->damper);
	p.secondDamper   = PTM::TVector<3,double>(j->secondDamper);
	p.targetVelocity = PTM::TVector<3,double>(j->targetVelocity);
	p.offsetForce    = PTM::TVector<3,double>(j->offsetForce);
	p.yieldStress    = j->yieldStress;
	p.hardnessRate   = j->hardnessRate;
}

/// パラメータを反映する
void PHBallJointMotor::SetParams(PHNDJointMotorParam<3>& p) {
	PHBallJoint* j = joint->Cast();
	j->fAvg.w() = p.fAvg;
	j->xs       = p.xs;
	j->bYielded = p.bYielded;
	// 上記以外の変数は特に反映する必要はない．
}

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHBallJointNonLinearMotor

void PHBallJointNonLinearMotor::SetFuncFromDatabaseN(int n, int i, int j, void* sparam, void* dparam){
	if (n >= 0 && n < 3){
		if (!(i < 0 || i > sizeof(PHBallJointFunc) / sizeof(PHBallJointFunc[0]))) {
			springFunc[n] = i;
			this->springParam[n] = sparam;
		}
		if (!(j < 0 || j > sizeof(PHBallJointFunc) / sizeof(PHBallJointFunc[0]))) {
			damperFunc[n] = j;
			this->damperParam[n] = dparam;
		}
	}
}

void PHBallJointNonLinearMotor::SetFuncFromDatabase(Vec3i i, Vec3i j, void* sparam[], void* dparam[]){
	for (int n = 0; n < 3; n++) {
		SetFuncFromDatabaseN(n, i[n], j[n], sparam[n], dparam[n]);
	}
}

/// propVを計算する
PTM::TVector<3, double> PHBallJointNonLinearMotor::GetPropV() {
	PHBallJoint* j = joint->Cast();
	Quaterniond qtar;
	qtar.FromEuler(Vec3d(targetPos.y, targetPos.z, targetPos.x)); //順番あってる？
	Quaterniond pQ = qtar * joint->Xjrel.q.Inv();
	return((PTM::TVector<3, double>)(pQ.RotationHalf()));
}

/// パラメータを取得する
void PHBallJointNonLinearMotor::GetParams(PHNDJointMotorParam<3>& p) {
	PHBallJoint* j = joint->Cast();
	p.fAvg = j->fAvg.w();
	p.xs = j->xs;
	p.bYielded = j->bYielded;
	PTM::TVector<3, double> sp;
	PTM::TVector<3, double> da;
	PTM::TVector<3, double> tarVel;
	for (int i = 0; i < 3; i++){
		Vec2d temp_sp = PHBallJointFunc[springFunc[i]](i, joint->Cast(), springParam[i]);
		sp[i] = temp_sp[0];
		targetPos[i] = temp_sp[1];
		Vec2d temp_da = PHBallJointFunc[damperFunc[i]](i, joint->Cast(), damperParam[i]);
		da[i] = temp_da[0];
		tarVel[i] = temp_da[1];
	}
	p.spring = sp;
	p.damper = da;
	p.secondDamper = PTM::TVector<3, double>(j->secondDamper);
	p.targetVelocity = tarVel;
	p.offsetForce = PTM::TVector<3, double>(offset);
	p.yieldStress = j->yieldStress;
	p.hardnessRate = j->hardnessRate;
}

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----
// PHHumanBallJointResistance

Vec3d PHHumanBallJointResistance::GetCurrentResistance() {
	Vec3d resistance;
	double* spring;
	PHBallJointIf* jo = joint->Cast();
	Vec3d delta;
	jo->GetPosition().ToEuler(delta);
	delta = Vec3d(delta.z, delta.x, delta.y);
	for (int i = 0; i < 3; i++) {
		spring = (double*)(springParam[i]);
		resistance[i] = JointFunctions::resistCalc(delta[i], spring[0], spring[1], spring[2], spring[3]);
	}
	return resistance;
}

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----
// PHSpringMotor

/// propVを計算する
PTM::TVector<6,double> PHSpringMotor::GetPropV() {
	Vec6d propV;
	Quaterniond diff = DCAST(PHSpring,joint)->targetOrientation * joint->Xjrel.q.Inv();
	propV.SUBVEC(0,3) = DCAST(PHSpring, joint)->targetPosition - joint->Xjrel.r;
	propV.SUBVEC(3,3) = diff.RotationHalf();
	return propV;
}

/// パラメータを取得する
void PHSpringMotor::GetParams(PHNDJointMotorParam<6>& p) {
	PHSpring* j = joint->Cast();
	p.fAvg.SUBVEC(0,3)                        = j->fAvg.v();
	p.fAvg.SUBVEC(3,3)                        = j->fAvg.w();
	p.xs                                      = j->xs;
	p.bYielded                                = j->bYielded;
	for(int i=0;i<3;++i){ p.spring[i]         = j->spring[i]; }
	for(int i=0;i<3;++i){ p.spring[i+3]       = j->springOri; }
	for(int i=0;i<3;++i){ p.damper[i]         = j->damper[i]; }
	for(int i=0;i<3;++i){ p.damper[i+3]       = j->damperOri; }
	for(int i=0;i<3;++i){ p.secondDamper[i]   = j->secondDamper[i]; }
	for(int i=0;i<3;++i){ p.secondDamper[i+3] = j->secondDamperOri; }
	for(int i=0;i<6;++i){ p.targetVelocity[i] = j->targetVelocity[i]; } 
	for(int i=0;i<6;++i){ p.offsetForce[i]    = j->offsetForce[i]; } 
	p.yieldStress    = j->yieldStress;
	p.hardnessRate   = j->hardnessRate;
}

/// パラメータを反映する
void PHSpringMotor::SetParams(PHNDJointMotorParam<6>& p) {
	PHSpring* j = joint->Cast();
	for (int i=0; i<6; ++i) { j->fAvg[i]=p.fAvg[i]; }
	j->xs       = p.xs;
	j->bYielded = p.bYielded;
	// 上記以外の変数は特に反映する必要はない．
}

}

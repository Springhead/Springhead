/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Physics.h"
#pragma hdrstop

using namespace PTM;
using namespace std;
namespace Spr{;

PHSpringDesc::PHSpringDesc(){
	springOri = damperOri = 0.0;
	fMax = NULL;
}

//----------------------------------------------------------------------------
// PHSpring
PHSpring::PHSpring(const PHSpringDesc& desc){
	SetDesc(&desc);
	yieldFlag = false;
}

//void PHSpring::SetDesc(const void* desc){
//	PHConstraint::SetDesc(desc);
//	const PHSpringDesc& descSpring = *(const PHSpringDesc*)desc;
//	spring = descSpring.spring;
//	damper = descSpring.damper;
//	springOri = descSpring.springOri;
//	damperOri = descSpring.damperOri;
//}


void PHSpring::SetConstrainedIndex(bool* con){
	for(int i=0; i<3; ++i) con[i] = (damper[i] != 0.0 || spring[i] != 0.0);
	con[3] = con[4] = con[5] = (damperOri != 0.0 || springOri != 0.0);
}
void PHSpring::SetConstrainedIndexCorrection(bool* con){
	con[0] = con[1] = con[2] = con[3] = con[4] = con[5] = false;
}

void PHSpring::ElasticDeformation(){
	//rjrel
	double dtinv = 1.0 / GetScene()->GetTimeStep();
	double tmp;
	for(int i = 0; i < 3; i++){
		if (!constr[i]) continue;
		tmp = 1.0 / (damper[i] + spring[i] * GetScene()->GetTimeStep());
		dA[i] = tmp * dtinv;
		db[i] = spring[i] * Xjrel.r[i] * tmp;
	}

	// 姿勢に対するバネ
	if(springOri != 0.0 || damperOri != 0.0){
		Quaterniond diff = Xjrel.q; // * targetPosition.Inv();
		Vec3d prop = diff.RotationHalf();
		double tmpInv = damperOri +springOri * GetScene()->GetTimeStep();
		if (tmpInv > 1e-30){
			tmp = 1.0/tmpInv;
			dA.w() = Vec3d(tmp * dtinv, tmp * dtinv, tmp * dtinv);
			db.w() = springOri * (prop) * tmp;
		}else{
			DSTR<<"PHSpring :: tmpInv is small"<<std::endl;
		}
	}
}

void PHSpring::PlasticDeformation(){
	//3要素モデル
	double dt		= GetScene()->GetTimeStep();
	double dtinv	= GetScene()->GetTimeStepInv();
	Vec3d D		= damper * hardnessRate;
	Vec3d D2	= secondDamper * hardnessRate;
	Vec3d K		= spring * hardnessRate;

	//rjrel
	Vec3d tmp = D+D2+K*dt;
	SpatialVector ws = vjrel;	//バネとダンパの並列部の速さ
	for(int i = 0; i < 3; i++){
		xs[1].v()[i] = ((D[i]+D2[i])/tmp[i])*xs[0].v()[i]+(D2[i]*dt/tmp[i])*ws.v()[i];	//バネとダンパの並列部の距離の更新
		dA[i]		 = tmp[i]/(D2[i]*(K[i]*dt+D[i])) * dtinv;
		db[i]		 = K[i]/(K[i]*dt+D[i])*(xs[0].v()[i]) ;
	}

	// 姿勢に対するバネ
	if(springOri != 0.0 || damperOri != 0.0){
		Quaterniond diff = Xjrel.q; // * targetPosition.Inv();
		Vec3d prop = diff.RotationHalf();
		double tmpInv = damperOri +springOri * GetScene()->GetTimeStep();
		if (tmpInv > 1e-30){
			double tmpO = 1.0/tmpInv;
			dA.w() = Vec3d(tmpO * dtinv, tmpO * dtinv, tmpO * dtinv);
			db.w() = springOri * (prop) * tmpO;
		}
	}

	
	//ELASTIC_PLASTICモードの場合,PLASTIC状態の終了時に残留変位を保存する位置にTargetPositionを変更
	if(type==PHJointDesc::ELASTIC_PLASTIC){
		if(ws.w().norm()<0.01){
			yieldFlag = false;
			//SetTargetPosition(Xjrel.q);
		}
	}
	xs[0]=xs[1];	//バネとダンパの並列部の距離のステップを進める

}
void PHSpring::CompBias(){

	//fの平均値を計算
	double fNorm = 0;
	for(int i=0; i<5 ;i++){
		if(i==4){
			fs[4] = motorf;
		}else{ 
			fs[i] = fs[i+1];
		}
		
		fNorm+=fs[i].norm()/5;
	}
	if(fNorm > yieldStress){
		yieldFlag = true;
	}
	switch(type){
	case PHJointDesc::ELASTIC:	//PHDeformationType::Elastic 0　初期値
		ElasticDeformation();
		break;
	case PHJointDesc::PLASTIC:	//PHDeformationType::Plastic 1
		PlasticDeformation();
		break;
	case PHJointDesc::ELASTIC_PLASTIC: //PHDeformationType::ELASTIC_PLASTIC 2	
		if(yieldFlag){
			PlasticDeformation();	//塑性変形
		}else {
			ElasticDeformation();	//弾性変形
		}
		break;
	default:
		ElasticDeformation();
		break;
	}

}

void PHSpring::IterateLCP(){
	if(!bEnabled || !bFeasible || bArticulated)
		return;
	FPCK_FINITE(f.v());

	double fMaxDt  = fMax * GetScene()->GetTimeStep();
	SpatialVector fnew, df;
	for(int j = 0; j < 6; j++){
		if(!constr[j])continue;
		
		fnew[j] = f[j] - engine->accelSOR * Ainv[j] * (dA[j] * f[j] + b[j] + db[j] 
				+ J[0].row(j) * solid[0]->dv + J[1].row(j) * solid[1]->dv);

		if (!FPCK_FINITE(fnew[0])){
			FPCK_FINITE(b[0]);
			DSTR << "s0:" << (solid[0]->dv) << std::endl;
			DSTR << "s1:" << (solid[1]->dv)  << std::endl;
		}
	}



	if(fnew.v().norm() > fMaxDt){
		fnew.v() = fnew.v() * fMaxDt/fnew.v().norm();
		//DSTR<<GetName()<<":"<<fnew<<std::endl;
	}
	if(fnew.w().norm() > fMaxDt){
		fnew.w() = fnew.w() * fMaxDt/fnew.w().norm();
		//DSTR<<GetName()<<":"<<fnew<<std::endl;
	}

	for(int j = 0; j < 6; j++){
		Projection(fnew[j], j);
		df[j] = fnew[j] - f[j];
		CompResponse(df[j], j);
		f[j] = fnew[j];
	}
}

}

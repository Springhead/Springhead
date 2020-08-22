/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <HumanInterface/HISpidarCalc.h>
#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

namespace Spr{;

void HISpidarCalcBase::Init(int dof, const VVector<float>& minF, const VVector<float>& maxF){
	size_t wire = minF.size();
	matA.resize(wire, dof);
	lengthDiff.resize(wire);
	lengthDiffAve.resize(wire);
	lengthDiffAve.clear();
	
	matATA.resize(dof,dof);
	postureDiff.resize(dof);
	
	measuredLength.resize(wire);
	calculatedLength.resize(wire);
	postureDiff.resize(dof);
	quadpr.Init(minF, maxF);
}

HISpidarCalc3Dof::HISpidarCalc3Dof(){
	//Setting of default value
	sigma=0.0005f;
	nRepeat=1;
	smooth=0.001f;
	lambda_t=1.0f;
}

HISpidarCalc3Dof::~HISpidarCalc3Dof(){	
}

void HISpidarCalc3Dof::Init(int dof, const VVector<float>& minF, const VVector<float>& maxF){
	wireDirection.resize(minF.size());
	HISpidarCalcBase::Init(dof, minF, maxF);
}

void HISpidarCalc3Dof::SetForce(const Vec3f& TF){
	int i,j;
	
	int wire = (int)wireDirection.size();
	for(i=0;i<wire;i++){
		quadpr.matQ[i][i] = wireDirection[i]*wireDirection[i]+smooth;
		for(j=i+1;j<wire;j++){
			quadpr.matQ[i][j] = quadpr.matQ[j][i] = wireDirection[i]*wireDirection[j];
		}
		quadpr.vecC[i] = wireDirection[i] * TF;
	}
	quadpr.Solve();

	trnForce.clear();
	for (i=0; i<wire; i++)
		trnForce += quadpr.vecX[i] * wireDirection[i];
}

void HISpidarCalc3Dof::Update(){
	int i,j,k;

	MeasureWire();
	MakeWireVec();
	int wire = (int)matA.height();
	for(int step=0; step< nRepeat; ++ step){
		for(i=0;i<wire;i++) {
			matA[i][0]=wireDirection[i].X();
			matA[i][1]=wireDirection[i].Y();
			matA[i][2]=wireDirection[i].Z();
			lengthDiff[i]=calculatedLength[i]-measuredLength[i];
		}

		for(i=0;i<3;i++){
			matATA[i][i]=0.0f;
			postureDiff[i]=0.0f;
			for(k=0;k<wire;k++) matATA[i][i]+=matA[k][i]*matA[k][i];
			for(j=i+1;j<3;j++){
				matATA[i][j]=0.0f;
				for(k=0;k<wire;k++) matATA[i][j]+=matA[k][i]*matA[k][j];
				matATA[j][i]=matATA[i][j];
			}
		}

		for(i=0;i<3;i++) {
			for(k=0;k<wire;k++) postureDiff[i]+=matA[k][i]*lengthDiff[k];
			matATA[i][i]+=sigma;
		}


		matATA.cholesky(postureDiff);
		for(i=0;i<3;i++) postureDiff[i]=postureDiff[i];
		pos.X() += postureDiff[0];
		pos.Y() += postureDiff[1];
		pos.Z() += postureDiff[2];


		UpdatePos();
		MakeWireVec();
	}
}



HISpidarCalc6Dof::HISpidarCalc6Dof(){	
	//Setting of default value
	lambda_r=0.3f;
	angleMax = M_PI / 2;	//	SPIDARのグリップは90度以上回転しないと仮定
}

HISpidarCalc6Dof::~HISpidarCalc6Dof(){	
}

void HISpidarCalc6Dof::Init(int dof, const VVector<float>& minF, const VVector<float>& maxF){
	wireMoment.resize(minF.size());
	HISpidarCalc3Dof::Init(dof, minF, maxF);
}

void HISpidarCalc6Dof::Update(){
	int i,j,k;
	MeasureWire();	//get length from encorder
	MakeWireVec();

	int nWire = (int)matA.height();
	for(int step=0; step < nRepeat; ++step){
		for(i=0;i<nWire;i++) {
			matA[i][0]=wireDirection[i].X();
			matA[i][1]=wireDirection[i].Y();
			matA[i][2]=wireDirection[i].Z();
			matA[i][3]=wireMoment[i].X();
			matA[i][4]=wireMoment[i].Y();
			matA[i][5]=wireMoment[i].Z();
			lengthDiff[i] = calculatedLength[i]-measuredLength[i] - lengthDiffAve[i];
		}
		for(i=0;i<6;i++){
			matATA[i][i]=0.0f;
			postureDiff[i]=0.0f;
			for(k=0;k<nWire;k++) matATA[i][i]+=matA[k][i]*matA[k][i];
			for(j=i+1;j<6;j++){
				matATA[i][j]=0.0f;
				for(k=0;k<nWire;k++) matATA[i][j]+=matA[k][i]*matA[k][j];
				matATA[j][i]=matATA[i][j];
			}
		}
		for(i=0;i<6;i++) {
			for(k=0;k<nWire;k++) postureDiff[i]+=matA[k][i]*lengthDiff[k];
			matATA[i][i]+=sigma;
		}

		matATA.cholesky(postureDiff);

		pos.X() +=postureDiff[0];
		pos.Y() +=postureDiff[1];
		pos.Z() +=postureDiff[2];
	
		Vec3f delta;
		delta.X() = postureDiff[3];
		delta.Y() = postureDiff[4];
		delta.Z() = postureDiff[5];

		
		float a = delta.norm();
		if (a > 1e-10) ori = Quaternionf::Rot(a, delta/a) * ori;
		if (angleMax) {
			if (ori.Theta() > angleMax) {
				ori = Quaternionf::Rot(angleMax, ori.Axis());
			}else if (ori.Theta() < -angleMax) {
				ori = Quaternionf::Rot(-angleMax, ori.Axis());
			}
		}

		UpdatePos();
		MakeWireVec();
	}
	float alpha = 0.00001f;
	lengthDiff = calculatedLength-measuredLength-lengthDiffAve;
	lengthDiffAve += lengthDiff * alpha;
//	DSTR << lengthDiffAve << std::endl;
}

void HISpidarCalc6Dof::SetForce(const Vec3f& trnF ,const Vec3f& rotF){
	int nWire = (int)wireDirection.size();

	int i,j;

	for(i=0;i<nWire;i++){
		quadpr.matQ[i][i] = lambda_t*(wireDirection[i]*wireDirection[i])+lambda_r*(wireMoment[i]*wireMoment[i])+smooth;
		for(j=i+1;j<nWire;j++){
			quadpr.matQ[i][j] = lambda_t*(wireDirection[i]*wireDirection[j])+lambda_r*(wireMoment[i]*wireMoment[j]);
			quadpr.matQ[j][i] = quadpr.matQ[i][j];
		}
		quadpr.vecC[i] = lambda_t*(wireDirection[i] * trnF)+lambda_r*(wireMoment[i] * rotF);
	}
	quadpr.Solve();

	trnForce.clear();
	rotForce.clear();
	for (i=0; i<nWire; i++) {
		trnForce += Tension()[i] * wireDirection[i];
		rotForce += Tension()[i] * wireMoment[i];
	}
}


}	//	namespace Spr;

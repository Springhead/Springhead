#include "HumanInterface.h"
#ifdef _WIN32
 #pragma hdrstop
#endif
//#include "HISpidarCalc3Dof.h"

namespace Spr{;

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

void HISpidarCalc3Dof::SetForce(const Vec3f& TF ,const Vec3f& RF)
{
	SetForce(TF);
}

void HISpidarCalc3Dof::SetForce(const Vec3f& TF)
{
	int i,j;
	
	int wire = wireDirection.size();
	for(i=0;i<wire;i++){
		quadpr.matQ[i][i] = wireDirection[i]*wireDirection[i]+smooth;
		for(j=i+1;j<wire;j++){
			quadpr.matQ[i][j] = quadpr.matQ[j][i] = wireDirection[i]*wireDirection[j];
		}
		quadpr.vecC[i] = wireDirection[i] * TF;
	}
	quadpr.Solve();

	trnForce.clear();
	for (i=0; i<wire; i++) trnForce += quadpr.vecX[i] * wireDirection[i];
}

void HISpidarCalc3Dof::Update()
{
	int i,j,k;

	MeasureWire();
	MakeWireVec();

	int wire = matA.height();
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

}

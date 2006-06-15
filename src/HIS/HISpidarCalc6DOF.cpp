#include "HIS.h"
#ifdef _WIN32
 #pragma hdrstop
#endif

//#include "HISpidarCalc6Dof.h"

namespace Spr{;

HISpidarCalc6Dof::HISpidarCalc6Dof()
{	
	//Setting of default value
	lambda_r=0.3f;
}
HISpidarCalc6Dof::~HISpidarCalc6Dof()
{	
}
void HISpidarCalc6Dof::Init(int dof, const VVector<float>& minF, const VVector<float>& maxF){
	wireMoment.resize(minF.size());
	HISpidarCalc3Dof::Init(dof, minF, maxF);
}




void HISpidarCalc6Dof::Update()
{
	int i,j,k;
	MeasureWire();	//get length from encorder
	int nWire = matA.height();

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
		UpdatePos();
		MakeWireVec();
	}
	float alpha = 0.00001f;
	lengthDiff = calculatedLength-measuredLength-lengthDiffAve;
	lengthDiffAve += lengthDiff * alpha;
//	DSTR << lengthDiffAve << std::endl;
}

void HISpidarCalc6Dof::SetForce(const Vec3f& trnF ,const Vec3f& rotF){
	int nWire = wireDirection.size();

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

void HISpidarCalc6Dof::SetForce(const Vec3f& trnF)
{
	SetForce(trnF,Vec3f());
}


}	//	namespace Spr

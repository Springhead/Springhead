#include "HIS.h"
#ifdef _WIN32
 #pragma hdrstop
#endif

namespace Spr{;

void HISpidarCalcBase::Init(int dof, const VVector<float>& minF, const VVector<float>& maxF){
	int wire = minF.size();
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


}	//	namespace Spr;

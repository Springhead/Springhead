/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Physics.h"
#include <Math.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace PTM;
using namespace std;
namespace Spr{;

PH3ElementDesc::PH3ElementDesc(){
	springOri = damperOri = 0.0;
	secondDamper		  = Vec3d(0.0, 0.0, 0.0);
	yieldStress			  = 0.0;			// 降伏応力
	hardnessRate		  = 1.0;		// 降伏応力以下の場合に二個目のダンパ係数に掛ける比率
}
//----------------------------------------------------------------------------
// PH3Element
//コンストラクタ
PH3Element::PH3Element(const PH3ElementDesc& desc){
	SetDesc(&desc);
}

void PH3Element::SetDesc(const void* desc){
	PHConstraint::SetDesc(desc);
	const PH3ElementDesc& desc3Element = *(const PH3ElementDesc*)desc;
	spring = desc3Element.spring;
	damper = desc3Element.damper;
	secondDamper = desc3Element.secondDamper;
	springOri = desc3Element.springOri;
	damperOri = desc3Element.damperOri;
	yieldStress	= desc3Element.yieldStress;			// 降伏応力
	hardnessRate = desc3Element.hardnessRate;		// 降伏応力以下の場合に二個目のダンパ係数に掛ける比率
}

void PH3Element::SetConstrainedIndex(bool* con){
	for(int i=0; i<3; ++i) con[i] = (damper[i] != 0.0 || spring[i] != 0.0);
	con[3] = con[4] = con[5] = (damperOri != 0.0 || springOri != 0.0);
}
void PH3Element::SetConstrainedIndexCorrection(bool* con){
	con[0] = con[1] = con[2] = con[3] = con[4] = con[5] = false;
}

void PH3Element::CompBias(){
	
	double dtinv = 1.0 / GetScene()->GetTimeStep(), tmp,tmp2;
	Vec3d D1 = damper;
	Vec3d K = spring;
	Vec3d D2 = secondDamper;
	double h = GetScene()->GetTimeStep();

	//降伏応力以下ではジョイントを固くする
	if(f.v().norm()>=yieldStress ){
		K = spring;
		D1 = damper;
		D2=secondDamper;
	}else{
		K = spring;
		D1 = damper;
		D2= secondDamper*hardnessRate;
	}

	//3要素モデル	
	ws=vjrel;
	for(int i = 0; i < 3; i++){
		if (!constr[i]) continue;
		//ばねの長さを更新
		tmp = D2[i]-D1[i]+K[i]*h;
		tmp2=D2[i]*K[i]*h*(2*D1[i]-D2[i])*(D2[i]-D1[i])-tmp*D1[i]*D2[i];

		xs[1][i] = ((D2[i]-D1[i])/tmp)*xs[0][i] + (D2[i]*h/tmp)*ws[i];		
		dA[i] = -tmp*(D2[i]-D1[i])/tmp2* dtinv;
		db[i] = K[i]*(2*D1[i]-D2[i])*(D2[i]-D1[i])/tmp2*xs[0][i];
	}
	xs[0]=xs[1];


	////3要素モデルとフォークトモデルの切り替え方式	
	//ws=vjrel;
	//if(f.v().y<-30){
	//	for(int i = 0; i < 3; i++){
	//		if (!constr[i]) continue;
	//		//ばねの長さを更新
	//		tmp = D2[i]-D1[i]+K[i]*h;
	//		tmp2=D2[i]*K[i]*h*(2*D1[i]-D2[i])*(D2[i]-D1[i])-tmp*D1[i]*D2[i];

	//		xs[1][i] = ((D2[i]-D1[i])/tmp)*xs[0][i] + (D2[i]*h/tmp)*ws[i];		
	//		dA[i] = -tmp*(D2[i]-D1[i])/tmp2* dtinv;
	//		db[i] = K[i]*(2*D1[i]-D2[i])*(D2[i]-D1[i])/tmp2*xs[0][i];
	//	}
	//xd[0].v()=Xjrel.r-xss[0].v()-xs[0].v();
	//xs[0]=xs[1];
	//}else{
	//	for(int i = 0; i < 3; i++){
	//		if (!constr[i]) continue;
	//		tmp = 1.0 / (damper[i] + spring[i] * GetScene()->GetTimeStep());
	//		dA[i] = tmp * dtinv;
	//		db[i] = spring[i] *( Xjrel.r[i] -xd[0].v()[i])* tmp;
	//	}
	//	xss[0].v()=Xjrel.r;
	//}

	//if(secondDamper[0]!=0.0||secondDamper[1]!=0.0||secondDamper[2]!=0.0){
	//	//3要素モデル
	//	Vec3d D2 = secondDamper;
	//	ws=vjrel;
	//	for(int i = 0; i < 3; i++){
	//		if (!constr[i]) continue;
	//		//ばねの長さを更新
	//		tmp = D2[i]-D1[i]+K[i]*h;
	//		xs[1][i] = ((D2[i]-D1[i])/tmp)*xs[0][i] + (D2[i]*h/(D2[i]-D1[i]))*ws[i];		
	//		dA[i] = (D2[i]-D1[i])*(D2[i]-D1[i])/(D1[i]*D2[i]*tmp) * dtinv;
	//		db[i] = K[i]*(D2[i]-D1[i])*(D2[i]-D1[i])/(D2[i]*tmp*tmp) * xs[0][i];
	//	}
	//	xs[0]=xs[1];
	//}else	{
	//	////マクスウェルモデル
	//	//ws=vjrel;
	//	//for(int i = 0; i < 3; i++){
	//	//	if (!constr[i]) continue;
	//	//	//ばねの長さを更新
	//	//	xs[1][i] = D1[i]/(D1[i]+K[i]*h)*(xs[0][i]+ws[i]*h);
	//	//	tmp = (D1[i]+K[i]*h)/(D1[i]*K[i]*h);
	//	//	dA[i] = tmp * dtinv;
	//	//	db[i] = xs[0][i]/h;
	//	//}
	//	//xs[0]=xs[1];
	//}
	
	

	// 姿勢に対するバネ
	if(springOri != 0.0 || damperOri != 0.0){
		Quaterniond diff =  Xjrel.q; // * origin.Inv();
		Vec3d prop = diff.RotationHalf();
		double tmpInv = damperOri + springOri * GetScene()->GetTimeStep();
		if (tmpInv > 1e-30){
			tmp = 1.0/tmpInv;
			dA.w() = Vec3d(tmp * dtinv, tmp * dtinv, tmp * dtinv);
			db.w() = springOri * (prop) * tmp;
		}
	}

}

}

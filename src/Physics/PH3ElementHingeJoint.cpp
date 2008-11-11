/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 * 
 *　SwingDirの制限は入っていません。必要な人が頑張って実装してください(by toki 2007.12.05)
 *
 */
#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace PTM;
using namespace std;

namespace Spr{;
//----------------------------------------------------------------------------
// PH3ElementHingeJointDesc

PH3ElementHingeJointDesc::PH3ElementHingeJointDesc(){
	
	PHHingeJointDesc();
	secondDamper		  = 0.0;

}

//----------------------------------------------------------------------------
// PH3ElementHingeJoint
// コンストラクタ
PH3ElementHingeJoint::PH3ElementHingeJoint(const PH3ElementHingeJointDesc& desc){
	PHHingeJoint::PHHingeJoint();
}

void PH3ElementHingeJoint::CompBias(){
//	DSTR << "spring " << spring << " goal " << origin*180/M_PI << endl;
	double dtinv = 1.0 / GetScene()->GetTimeStep();
	if (engine->numIterCorrection==0){	//	Correction を速度LCPで行う場合
		//	次のステップでの位置の誤差の予測値が0になるような速度を設定
		//	dv * dt = x + v*dt
		db.v() = Xjrel.r * dtinv + vjrel.v();
		//	角度の誤差を0にするような回転角度を求める。
		Quaterniond qarc;
		qarc.RotationArc(Xjrel.q * Vec3d(0,0,1), Vec3d(0,0,1)); // 軸を一致させるような回転
		db.w() = -(qarc.Theta() * dtinv) * qarc.Axis() + vjrel.w();
		db *= engine->velCorrectionRate;
	}

	if(mode == PHJointDesc::MODE_VELOCITY){
		db.w().z = -desiredVelocity;
	}else if(spring != 0.0 || damper != 0.0){
		if (onUpper&& GetVelocity()>0){
		}else if (onLower && GetVelocity()<0){
		}else{
			double diff;
			diff = GetPosition() - origin;
			double springLim = 0;
			double damper_ = damper;
			double diffLim = 0;
			if (onUpper){
				diffLim = GetPosition() - upper;
				springLim = 10000;
				damper_   = 100;
			} else if (onLower){
				diffLim = GetPosition() - lower;
				springLim = 10000;
				damper_   = 100;
			}

			// 不連続なトルク変化を避けるため (ゼンマイのようにいくらでも巻けるように削除)。 07/07/26
			//// ↑むしろこのコードがあることで不連続なトルク変化が避けられているのでは？と思い復活． 08/10/07 mitake
			while(diff >  M_PI) diff -= 2 * M_PI;
			while(diff < -M_PI) diff += 2 * M_PI;
			
			//3要素モデルの設定
			double dtinv = 1.0 / GetScene()->GetTimeStep(), tmp, tmpA, tmpB;
			double D1 = damper_;
			double D2 = secondDamper;
			double K = spring;
			double h = GetScene()->GetTimeStep();
			
			ws=vjrel;	//バネのダンパの並列部の速さ
			tmp = D2-D1+K*h;
			xs[1] = ((D2-D1)/tmp)*xs[0] + (D2*h/(D2-D1))*ws;	//バネとダンパの並列部の距離の更新	
			tmpA =(D2-D1)*(D2-D1)/(D1*D2*tmp) ;
			tmpB =K*(D2-D1)*(D2-D1)/(D2*tmp*tmp)*(xs[0].w().z);
			dA.w().z= tmpA* dtinv;
			db.w().z = tmpB * ((spring * diff + springLim*diffLim)- (damper_ * desiredVelocity) - offsetForce );	
			xs[0]=xs[1];	//バネとダンパの並列部の距離のステップを進める
			}
	}
}
}
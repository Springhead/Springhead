/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHPENALTYENGINE_H
#define PHPENALTYENGINE_H

#include <Physics/PHContactDetector.h>

namespace Spr{;

class PHShapePairForPenalty : public PHShapePair{
public:
	///@name	抗力の計算
	//@{
	float area;							///<	交差部の面積
	Vec3f reflexSpringForce;			///<	ばねによる抗力
	Vec3f reflexDamperForce;			///<	ダンパーによる抗力
	Vec3f reflexSpringTorque;			///<	ばねによる抗トルク(commonPoint系)
	Vec3f reflexDamperTorque;			///<	ダンパーによる抗トルク(commonPoint系)
	Vec3f reflexForcePoint;				///<	抗力の作用点(commonPoint系)
	//@}

	///@name	摩擦の計算
	//@{
	Vec3f transFrictionBase[2];			///<	並進静止摩擦用バネの端点(ローカルフレーム系)
	float rotSpring;					///<	回転ばねの伸び(ラジアン)
	Vec3f dynaFric;						///<	動摩擦力
	Vec3f dynaFricMom;					///<	動摩擦力のモーメント(commonPoint系)
	Vec3f frictionForce;				///<	摩擦力
	Vec3f frictionTorque;				///<	摩擦トルク(摩擦力の作用点 ＝ (reflexForcePoint+commonPoint)系)
	enum FrictionState{ STATIC, DYNAMIC };
	FrictionState frictionState;		///<	摩擦の状態
	//@}

	void Clear();
	
	///@name	絶対座標系での読み出し
	//@{
	///	バネによる抗力の作用点
	Vec3f GetReflexForcePoint(){ return reflexForcePoint + commonPoint;}
	///	バネによる抗力
	Vec3f GetReflexSpringForce(){ return reflexSpringForce;}
	///	ダンパによる抗力
	Vec3f GetReflexDamperForce(){ return reflexDamperForce;}
	///	抗力
	Vec3f GetReflexForce(){ return reflexSpringForce + reflexDamperForce;}

	///	摩擦力
	Vec3f GetFrictionForce(){ return frictionForce; }
	///	摩擦トルク
	Vec3f GetFrictionTorque(){ return frictionTorque + (GetReflexForcePoint()^frictionForce); }
	//@}
};

class PHPenaltyEngine;
class PHSolidPairForPenalty : public PHSolidPair{
public:
	Vec3f cocog;					///<	2剛体の重心の中点(絶対系)
	Vec3f reflexForce;				///<	抗力
	Vec3f reflexTorque;				///<	抗力によるトルク(cocog系)
	Vec3f frictionForce;			///<	摩擦力
	Vec3f frictionTorque;			///<	摩擦力によるトルク(cocog系)
	float convertedMass;			///<	剛体の組の換算質量
	float area;						///<	接触面積
	Quaternionf lastOri[2];			///<	前回の剛体の向き(絶対系)

	virtual PHShapePair* CreateShapePair(){ return DBG_NEW PHShapePairForPenalty(); }
	virtual void OnDetect(PHShapePair* sp, unsigned ct, double dt);	///< 交差が検知されたときの処理

	PHShapePairForPenalty* GetShapePair(int i, int j){ return (PHShapePairForPenalty*)(PHShapePair*)shapePairs.item(i,j); }
	void Setup(unsigned int ct, double dt);
	void GenerateForce();

	///@name	絶対系での読み出し
	//@{
	///	抗力
	Vec3f GetReflexForce(){ return reflexForce; }
	///	抗力のトルク
	Vec3f GetReflexTorque(){ return reflexTorque + (cocog^reflexForce); }
	///	摩擦力
	Vec3f GetFrictionForce(){ return frictionForce; }
	///	摩擦力のトルク
	Vec3f GetFrictionTorque(){ return frictionTorque + (cocog^frictionForce); }
	//@}
	
	///	力の最大値を制約する．
	void LimitForces(){
		bool b = false;
		b |= LimitForce(reflexForce);
		b |= LimitForce(frictionForce);
		b |= LimitForce(reflexTorque);
		b |= LimitForce(frictionTorque);
		if (b){
			reflexForce = frictionForce = reflexTorque = frictionTorque = Vec3f();
		}
	}
protected:
	bool LimitForce(Vec3f& f){
		float sq = f.square();
		const float MF = 200000;
		if (!(sq < MF*MF)){
			return true;
		}
		return false;
	}
	void CalcReflexForce(PHShapePairForPenalty* sp, CDContactAnalysis* analyzer);
	void CalcTriangleReflexForce(PHShapePairForPenalty* cp, Vec3f p0, Vec3f p1, Vec3f p2, Vec3f v0, Vec3f v1, Vec3f v2, bool bFront);
	void CalcFriction(PHShapePairForPenalty* cp);
};

class PHPenaltyEngine : public PHContactDetector{
public:
	SPR_OBJECTDEF1(PHPenaltyEngine, PHEngine);

public:
	PHSolidPairForPenalty* GetSolidPair(int i, int j){ return (PHSolidPairForPenalty*)(PHSolidPair*)solidPairs.item(i,j); }
	
	// PHEngineの仮想関数
	virtual int  GetPriority() const {return SGBP_PENALTYENGINE;}
	virtual void Step();

	// PHContactDetectorの仮想関数
	virtual PHSolidPair* CreateSolidPair(){ return DBG_NEW PHSolidPairForPenalty(); }

};

}	//	namespace Spr
#endif

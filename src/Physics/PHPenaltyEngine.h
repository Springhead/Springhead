/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHPENALTYENGINE_H
#define PHPENALTYENGINE_H

#include <Physics/PHContactDetector.h>

namespace Spr{;

class PHShapePairForPenalty : public CDShapePair{
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

	///@name摩擦の計算
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

class PHSolidInfoForPenalty : public PHSolidInfo{
public:
	///@name	UpdateChacheで更新する変数
	//@{
	/**	最後に接触した時刻 = キャッシュパラメータを最後に更新した時刻．
		接触時に，キャッシュを更新するので，count が現在の時刻と等しければ
		衝突が起きたことを意味する．	*/
	unsigned count;
	//	フレーム(剛体)単位のパラメータ
	Vec3f cog, vel, angVel;			///<	重心，速度，角速度
	Vec3f pos, lastPos;				///<	位置，最後の位置
	Quaternionf ori, lastOri;		///<	向き，前回の向き
	//@}
	void UpdateCache(int c);		///<	キャッシュ変数を剛体などから取ってくる．
	PHSolidInfoForPenalty(PHSolid* solid):PHSolidInfo(solid){}
};

class PHPenaltyEngine;
class PHSolidPairForPenalty : public PHSolidPair<PHSolidInfoForPenalty, PHShapePairForPenalty, PHPenaltyEngine>{
public:
	typedef PHSolidPair<PHSolidInfoForPenalty, PHShapePairForPenalty, PHPenaltyEngine> base_type;
	typedef base_type::shapepair_type shapepair_type;
	typedef base_type::engine_type engine_type;

	Vec3f cocog;					///<	2剛体の重心の中点(絶対系)
	Vec3f reflexForce;				///<	抗力
	Vec3f reflexTorque;				///<	抗力によるトルク(cocog系)
	Vec3f frictionForce;			///<	摩擦力
	Vec3f frictionTorque;			///<	摩擦力によるトルク(cocog系)
	float convertedMass;			///<	剛体の組の換算質量
	float area;						///<	接触面積

	virtual void OnDetect(shapepair_type* sp, engine_type* engine, unsigned ct, double dt);	///< 交差が検知されたときの処理

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
	void CalcTriangleReflexForce(PHShapePairForPenalty* cp, Vec3f p0, Vec3f p1, Vec3f p2, Vec3f cog, Vec3f vel, Vec3f angVel);
	void CalcFriction(PHShapePairForPenalty* cp);
};

class PHPenaltyEngine : public PHContactDetectorImp<PHSolidInfoForPenalty, PHShapePairForPenalty, PHSolidPairForPenalty, PHPenaltyEngine>{
	OBJECT_DEF_NOIF(PHPenaltyEngine);
public:
	int GetPriority() const {return SGBP_PENALTYENGINE;}
	virtual void Step();
};

}	//	namespace Spr
#endif

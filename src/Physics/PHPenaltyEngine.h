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

class PHShapePairForPenalty : public CDShapePair{
public:
	///@name	çRóÕÇÃåvéZ
	//@{
	float area;							///<	åç∑ïîÇÃñ êœ
	Vec3f reflexSpringForce;			///<	ÇŒÇÀÇ…ÇÊÇÈçRóÕ
	Vec3f reflexDamperForce;			///<	É_ÉìÉpÅ[Ç…ÇÊÇÈçRóÕ
	Vec3f reflexSpringTorque;			///<	ÇŒÇÀÇ…ÇÊÇÈçRÉgÉãÉN(commonPointån)
	Vec3f reflexDamperTorque;			///<	É_ÉìÉpÅ[Ç…ÇÊÇÈçRÉgÉãÉN(commonPointån)
	Vec3f reflexForcePoint;				///<	çRóÕÇÃçÏópì_(commonPointån)
	//@}

	///@name	ñÄéCÇÃåvéZ
	//@{
	Vec3f transFrictionBase[2];			///<	ï¿êiê√é~ñÄéCópÉoÉlÇÃí[ì_(ÉçÅ[ÉJÉãÉtÉåÅ[ÉÄån)
	float rotSpring;					///<	âÒì]ÇŒÇÀÇÃêLÇ—(ÉâÉWÉAÉì)
	Vec3f dynaFric;						///<	ìÆñÄéCóÕ
	Vec3f dynaFricMom;					///<	ìÆñÄéCóÕÇÃÉÇÅ[ÉÅÉìÉg(commonPointån)
	Vec3f frictionForce;				///<	ñÄéCóÕ
	Vec3f frictionTorque;				///<	ñÄéCÉgÉãÉN(ñÄéCóÕÇÃçÏópì_ ÅÅ (reflexForcePoint+commonPoint)ån)
	enum FrictionState{ STATIC, DYNAMIC };
	FrictionState frictionState;		///<	ñÄéCÇÃèÛë‘
	//@}

	void Clear();
	
	///@name	ê‚ëŒç¿ïWånÇ≈ÇÃì«Ç›èoÇµ
	//@{
	///	ÉoÉlÇ…ÇÊÇÈçRóÕÇÃçÏópì_
	Vec3f GetReflexForcePoint(){ return reflexForcePoint + commonPoint;}
	///	ÉoÉlÇ…ÇÊÇÈçRóÕ
	Vec3f GetReflexSpringForce(){ return reflexSpringForce;}
	///	É_ÉìÉpÇ…ÇÊÇÈçRóÕ
	Vec3f GetReflexDamperForce(){ return reflexDamperForce;}
	///	çRóÕ
	Vec3f GetReflexForce(){ return reflexSpringForce + reflexDamperForce;}

	///	ñÄéCóÕ
	Vec3f GetFrictionForce(){ return frictionForce; }
	///	ñÄéCÉgÉãÉN
	Vec3f GetFrictionTorque(){ return frictionTorque + (GetReflexForcePoint()^frictionForce); }
	//@}
};

class PHPenaltyEngine;
class PHSolidPairForPenalty : public PHSolidPair<PHShapePairForPenalty, PHPenaltyEngine>, public Object{
public:
	typedef PHSolidPair<PHShapePairForPenalty, PHPenaltyEngine> base_type;
	typedef base_type::shapepair_type shapepair_type;
	typedef base_type::engine_type engine_type;

	Vec3f cocog;					///<	2çÑëÃÇÃèdêSÇÃíÜì_(ê‚ëŒån)
	Vec3f reflexForce;				///<	çRóÕ
	Vec3f reflexTorque;				///<	çRóÕÇ…ÇÊÇÈÉgÉãÉN(cocogån)
	Vec3f frictionForce;			///<	ñÄéCóÕ
	Vec3f frictionTorque;			///<	ñÄéCóÕÇ…ÇÊÇÈÉgÉãÉN(cocogån)
	float convertedMass;			///<	çÑëÃÇÃëgÇÃä∑éZéøó 
	float area;						///<	ê⁄êGñ êœ

	virtual void OnDetect(shapepair_type* sp, engine_type* engine, unsigned ct, double dt);	///< åç∑Ç™åüímÇ≥ÇÍÇΩÇ∆Ç´ÇÃèàóù

	void Setup(unsigned int ct, double dt);
	void GenerateForce();

	///@name	ê‚ëŒånÇ≈ÇÃì«Ç›èoÇµ
	//@{
	///	çRóÕ
	Vec3f GetReflexForce(){ return reflexForce; }
	///	çRóÕÇÃÉgÉãÉN
	Vec3f GetReflexTorque(){ return reflexTorque + (cocog^reflexForce); }
	///	ñÄéCóÕ
	Vec3f GetFrictionForce(){ return frictionForce; }
	///	ñÄéCóÕÇÃÉgÉãÉN
	Vec3f GetFrictionTorque(){ return frictionTorque + (cocog^frictionForce); }
	//@}
	
	///	óÕÇÃç≈ëÂílÇêßñÒÇ∑ÇÈÅD
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

class PHPenaltyEngine : public PHContactDetector<PHShapePairForPenalty, PHSolidPairForPenalty, PHPenaltyEngine>{
	SPR_OBJECTDEF_NOIF1(PHPenaltyEngine, PHEngine);
public:
	int GetPriority() const {return SGBP_PENALTYENGINE;}
	virtual void Step();
};
}	//	namespace Spr
#endif

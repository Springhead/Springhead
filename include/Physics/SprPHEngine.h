/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_PHENGINEIF_H
#define SPR_PHENGINEIF_H

namespace Spr{;
/** \addtogroup gpPhysics */
//@{

///	エンジンの基本クラス
struct PHEngineIf : public SceneObjectIf{
public:
	IF_DEF(PHEngine);
};

struct PHConstraintEngineIf : PHEngineIf{
public:
	IF_DEF(PHConstraintEngine);

	/** @brief 接触点の集合を返す
	*/
	virtual PHConstraintsIf* GetContactPoints() = 0;

	/** @brief velCorrectionRateを書き換える
		@param value 書き換え後の値
	*/
	virtual void SetVelCorrectionRate(double value) = 0;

	/** @brief posCorrectionRateを書き換える
		@param value 書き換え後の値
	*/
	virtual void SetPosCorrectionRate(double value) = 0;

};

struct PHGravityEngineIf : PHEngineIf{
public:
	IF_DEF(PHGravityEngine);
};

struct PHPenaltyEngineIf : PHEngineIf{
public:
	IF_DEF(PHPenaltyEngine);
};
//@}
}
#endif

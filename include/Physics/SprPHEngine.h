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
	SPR_IFDEF(PHEngine);
};

struct PHConstraintEngineIf : PHEngineIf{
public:
	SPR_IFDEF(PHConstraintEngine);

	/** @brief 接触点の集合を返す
	*/
	PHConstraintsIf* GetContactPoints();

	/** @brief velCorrectionRateを書き換える
		@param value 書き換え後の値
	*/
	void SetVelCorrectionRate(double value);

	/** @brief posCorrectionRateを書き換える
		@param value 書き換え後の値
	*/
	void SetPosCorrectionRate(double value);

	/** @brief contactCorrectionRateを書き換える
		@param value 書き換え後の値
	*/
	void SetContactCorrectionRate(double value);
};

struct PHGravityEngineIf : PHEngineIf{
public:
	SPR_IFDEF(PHGravityEngine);
};

struct PHPenaltyEngineIf : PHEngineIf{
public:
	SPR_IFDEF(PHPenaltyEngine);

};

struct PHIKEngineIf : PHEngineIf{
public:
	SPR_IFDEF(PHIKEngine);
};
//@}
}
#endif

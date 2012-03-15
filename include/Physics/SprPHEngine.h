/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_PHENGINEIF_H
#define SPR_PHENGINEIF_H

#include <Foundation/SprObject.h>

namespace Spr{;

struct PHConstraintsIf;

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

	/** @brief velCorrectionRateを取得する
		@return value velCorrectionRateの値
	*/
	double GetVelCorrectionRate();

	/** @brief posCorrectionRateを書き換える
		@param value 書き換え後の値
	*/
	void SetPosCorrectionRate(double value);

	/** @brief posCorrectionRateを取得する
		@return value posCorrectionRateの値
	*/
	double GetPosCorrectionRate();

	/** @brief contactCorrectionRateを書き換える
		@param value 書き換え後の値
	*/
	void SetContactCorrectionRate(double value);

	/** @brief contactCorrectionRateを取得する
		@return value contactCorrectionRateの値
	*/
	double GetContactCorrectionRate();

	/** @brief bSaveConstraintsを書き換える
		@return value SaveState, LoadStateに， constraints を含めるかどうか．本来不要だが，f, Fが変化する．
	*/
	void	SetBSaveConstraints(bool value);

	/** @brief 剛体の速度だけ更新するフラグを設定する
	*/
	void SetUpdateAllSolidState(bool flag);

	/** @brief 面接触を使うフラグを設定する
	*/
	void SetUseContactSurface(bool flag);

	/** @brief shrinkRateの値を設定する
	*/
	void SetShrinkRate(double data);
	
	/** @brief shrinkRateを取得する
		@return value shrinkCorrectionRateの値
	*/
	double GetShrinkRate();

	/** @brief shrinkRateCorrectionの値を設定する
	*/
	void SetShrinkRateCorrection(double data);

	/** @brief shrinkRateCorrectionを取得する
		@return value shrinkRateCorrectionの値
	*/
	double GetShrinkRateCorrection();

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
	/** @brief 有効か無効かを取得する
	*/
	bool IsEnabled();

	/** @brief 有効・無効を切り替える
	*/
	void Enable(bool bEnabled);

	/** @brief 繰り返し回数をセットする
	*/
	void SetNumIter(int numIter);

	SPR_IFDEF(PHIKEngine);
};

struct PHFemEngineIf : PHEngineIf{
public:
	SPR_IFDEF(PHFemEngine);
};

struct PHHapticEngineDesc{
	enum HapticEngineMode{
		SINGLE_THREAD,
		MULTI_THREAD,
		LOCAL_DYNAMICS,
	};
	PHHapticEngineDesc();
};
struct PHHapticEngineIf : public PHHapticEngineDesc, PHEngineIf{
public:
	SPR_IFDEF(PHHapticEngine);
	void EnableHapticEngine(bool b);
	void SetHapticEngineMode(HapticEngineMode mode);
	void StepPhysicsSimulation();
};
//@}
}
#endif

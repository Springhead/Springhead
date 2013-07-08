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
struct PHSceneIf;

/** \addtogroup gpPhysics */
//@{
///	エンジンの基本クラス
struct PHEngineIf : public SceneObjectIf{
public:
	SPR_IFDEF(PHEngine);
	///	実行順序を決めるプライオリティ値．小さいほど早い
	int GetPriority() const;
	///	時間を dt 進める
	void Step();
	///
	PHSceneIf* GetScene();
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
	void SetBSaveConstraints(bool value);

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

	/** @brief 接触判定エンジンの有効・無効を設定する．ここでの無効化はPHScene::SetContactModeより優先する．
	*/
	void EnableContactDetection(bool enable);
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
	/** @brief IK機能が有効か無効か
	*/
	void Enable(bool bEnabled);
	bool IsEnabled();

	/** @brief IKエンドエフェクタの移動速度限界（1stepあたりの移動量限界を決める）
	*/
	void SetMaxVelocity(double maxVel);
	double GetMaxVelocity();

	/** @brief IKエンドエフェクタの回転角速度限界（1stepあたりの回転量限界を決める）
	*/
	void SetMaxAngularVelocity(double maxAV);
	double GetMaxAngularVelocity();

	/** @brief ヤコビアンの擬似逆の正則化パラメータ
	*/
	void SetRegularizeParam(double epsilon);
	double GetRegularizeParam();

	/** @brief 軌道制御（＝速度の明示的な指定）の有効・無効
	*/
	void EnableTrajectoryControl(bool enable);
	bool IsTrajectoryControlEnabled();

	// <!!> Gauss-SeidelソルバをやめたらObsoleteになる予感
	/** @brief 繰り返し回数をセットする
	*/
	void SetNumIter(int numIter);

	SPR_IFDEF(PHIKEngine);
};

struct PHFemEngineIf : PHEngineIf{
public:
	void SetTimeStep(double dt);
	double GetTimeStep();
	SPR_IFDEF(PHFemEngine);
};

struct PHHapticEngineDesc{
	enum HapticEngineMode{
		SINGLE_THREAD = 0,
		MULTI_THREAD,
		LOCAL_DYNAMICS,
	};
	PHHapticEngineDesc();
};
struct PHHapticEngineIf : public PHHapticEngineDesc, PHEngineIf{
public:
	SPR_IFDEF(PHHapticEngine);
	/** @breif HapticEngineを有効化する
	*/
	void EnableHapticEngine(bool b);

	/** @breif HapticEngineのモードを切り替える
		@param mode HapticEngineMode
	*/
	void SetHapticEngineMode(HapticEngineMode mode);

	/** @brief シミュレーションをすすめる。HapticEngineを有効化した場合には
		この関数を使ってシミュレーションをすすめる
	*/
	void StepPhysicsSimulation();

	/** @brief シミュレーションを実行する直前かどうかを返す
	*/
	// bool IsBeforeStepPhysicsSimulation();

	/** @brief シミュレーションを実行した直後かどうかを返す
	*/
	// bool IsAfterStepPhysicsSimulation();

	/** @breif シーングラフの状態(ObjectStatesIf)を開放する。
				動的にオブジェクトを追加する時には直前に呼ぶ必要がある。
	*/
	void ReleaseState();

	///	コールバック関数の型
	typedef void (SPR_CDECL *Callback)(void* arg);

	/** @brief シミュレーションを実行する直前に実行されるコールバックを登録する
	*/
	bool SetCallbackBeforeStep(Callback f, void* arg);

	/** @brief シミュレーションを実行した直後に実行されるコールバックを登録する
	*/
	bool SetCallbackAfterStep(Callback f, void* arg);

};
//@}
}
#endif

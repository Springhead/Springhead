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
#include <Physics\SprPHOpObj.h>

namespace Spr{;

struct PHConstraintsIf;
struct PHSceneIf;

/** \addtogroup gpPhysics */
//@{
struct PHEngineIf : public SceneObjectIf{
public:
	SPR_IFDEF(PHEngine);
	
	///	実行順序を決めるプライオリティ値．小さいほど早い
	int GetPriority() const;
	
	///	時間を dt 進める
	void Step();
	
	/// 有効化/無効化
	void Enable(bool on = true);

	/// 
	bool IsEnabled();

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

	/** レポートを有効化/無効化 */
	void EnableReport(bool on = true);

	/** @brief 接触領域を表示するための情報を更新するかどうか。FWScene::EnableRenderContact()が呼び出す。
	*/
	void EnableRenderContact(bool enable);
};

struct PHConstraintEngineDesc{
	int		method;						///< LCPの解法
	int		numIter;					///< 速度更新LCPの反復回数
	int		numIterCorrection;			///< 誤差修正LCPの反復回数
	int		numIterContactCorrection;	///< 接触点の誤差修正LCPの反復回数
	double	velCorrectionRate;			///< 速度のLCPで関節拘束の誤差を修正する場合の誤差修正比率
	double	posCorrectionRate;			///< 位置のLCPで，関節拘束の誤差を修正する場合の誤差修正比率
	double  contactCorrectionRate;		///< 接触の侵入解消のための，速度のLCPでの補正比率．
	double	shrinkRate;					///< LCP初期値を前回の解に対して縮小させる比率
	double	shrinkRateCorrection;
	double	freezeThreshold;			///< 剛体がフリーズする閾値
	double	accelSOR;					///< SOR法の加速係数
	double  dfEps;
	bool	bSaveConstraints;			///< SaveState, LoadStateに， constraints を含めるかどうか．本来不要だが，f, Fが変化する．
	bool	bUpdateAllState;			///< 剛体の速度，位置の全ての状態を更新する．
	bool	bUseContactSurface;			///< 面接触を使う
	bool    bReport;                    ///< 計算時間などのレポートをデバッグ出力およびファイルへ出力する

	PHConstraintEngineDesc(){
		numIter					 = 15;
		numIterCorrection		 = 0;
		numIterContactCorrection = 0;
		velCorrectionRate		 = 0.3;
		posCorrectionRate		 = 0.3;
		contactCorrectionRate	 = 0.1;
		shrinkRate				 = 0.7;
		shrinkRateCorrection	 = 0.7;
		freezeThreshold			 = 0.0;
		accelSOR				 = 1.0;
		dfEps                    = 0.0;
		bSaveConstraints         = false;
		bUpdateAllState	         = true;
		bUseContactSurface       = false;
		bReport                  = false;
	}
};

struct PHGravityEngineIf : PHEngineIf{
public:
	SPR_IFDEF(PHGravityEngine);
};

struct PHPenaltyEngineIf : PHEngineIf{
public:
	SPR_IFDEF(PHPenaltyEngine);

};

struct PHIKEngineDesc{
	size_t	numIter;
	double  maxVel, maxAngVel, maxActVel;
	double  regularizeParam;

	PHIKEngineDesc();
};

struct PHIKEngineIf : PHEngineIf{
public:
	/** @brief IKエンドエフェクタの移動速度限界（1stepあたりの移動量限界を決める）
	*/
	void SetMaxVelocity(double maxVel);
	double GetMaxVelocity();

	/** @brief IKエンドエフェクタの回転角速度限界（1stepあたりの回転量限界を決める）
	*/
	void SetMaxAngularVelocity(double maxAV);
	double GetMaxAngularVelocity();

	/** @brief IKアクチュエータ回転角速度限界
	*/
	void SetMaxActuatorVelocity(double maxAV);
	double GetMaxActuatorVelocity();

	/** @brief ヤコビアンの擬似逆の正則化パラメータ
	*/
	void SetRegularizeParam(double epsilon);
	double GetRegularizeParam();

	/** @brief IK計算結果の関節角速度のノルムがこの値以下になったらIterationを打ち切る
	*/
	void SetIterCutOffAngVel(double epsilon);
	double GetIterCutOffAngVel();

	/** @brief 一時変数の関節角度・剛体姿勢を現実のものに合わせる
	*/
	void ApplyExactState(bool reverse=false);

	/** @brief (IK->FK)の繰り返し回数
	*/
	void SetNumIter(int numIter);
	int GetNumIter();

	SPR_IFDEF(PHIKEngine);
};

struct PHFemEngineIf : PHEngineIf{
public:
	void SetTimeStep(double dt);
	double GetTimeStep();
	SPR_IFDEF(PHFemEngine);
};
struct PHOpEngineIf : PHEngineIf{
	SPR_IFDEF(PHOpEngine);
public:
	void SetTimeStep(double dt);
	double GetTimeStep();
	void SetGravity(bool gflag);
	void InitialHapticRenderer(int objId, HISdkIf* hisdk);
	//ObjectIf* GetHapticController();
	PHOpObjDesc* GetOpObj(int i);
	void StepWithBlend();
	int AddOpObj();
	bool TrySetHapticEnable(bool enable, HISdkIf* hisdk);
	bool IsHapticEnabled();
	void SetHapticSolveEnable(bool enable);
	bool IsHapticSolve();
	void SetProxyCorrectionEnable(bool enable);
	bool IsProxyCorrection();
	void SetUpdateNormal(bool flag);
	bool IsUpdateNormal(int obji);

	PHOpObjIf* GetOpObjIf(int obji);
	int GetOpObjNum();

	ObjectIf* GetOpAnimator();
	ObjectIf* GetOpHapticController();
	ObjectIf* GetOpHapticRenderer();
	void SetIterationCount(int count);
	int GetIterationCount();
	void SetAnimationFlag(bool flag);
	bool GetAnimationFlag();
	void SetDrawPtclR(float r);
	float GetDrawPtclR();
	void InitialNoMeshHapticRenderer(HISdkIf* hisdk);
	//void SetCrossPlatformCoord(bool InverX, bool InverY, bool InverZ);
};

struct PHHapticEngineDesc{
	enum HapticEngineMode{
		SINGLE_THREAD = 0,
		MULTI_THREAD,
		LOCAL_DYNAMICS,
	};
	PHHapticEngineDesc();
};

struct PHHapticEngineIf : public PHEngineIf{
public:
	SPR_IFDEF(PHHapticEngine);
	
	/** @breif HapticEngineのモードを切り替える
		@param mode HapticEngineMode
	*/
	void SetHapticEngineMode(PHHapticEngineDesc::HapticEngineMode mode);

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

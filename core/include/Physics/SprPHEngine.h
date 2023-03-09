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
#include <Physics/SprPHOpObj.h>

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

	/** @brief set regularization parameter
	    @param reg regularization parameter to be set 
	*/
	void SetRegularization(double reg);

	/** @brief returns regularization parameter
	*/
	double GetRegularization();

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
	double  regularization;
	bool	bSaveConstraints;			///< SaveState, LoadStateに， constraints を含めるかどうか．本来不要だが，f, Fが変化する．
	bool	bUpdateAllState;			///< 剛体の速度，位置の全ての状態を更新する．
	bool	bUseContactSurface;			///< 面接触を使う
	bool    bReport;                    ///< 計算時間などのレポートをデバッグ出力およびファイルへ出力する

	PHConstraintEngineDesc();
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
	double  maxVel;
	double  maxAngVel;
	double  maxActVel;
	double  regularizeParam;
	double  regularizeParam2;
	int     regularizeMode;
	int     iterGaussSeidel;
	enum Mode {
		SVD,
		QR,
		LM,
	};
	int solverMode;

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

	void SetIntpRate();
	int GetIntpRate();

	/** @brief 一時変数の関節角度・剛体姿勢を現実のものに合わせる
	*/
	void ApplyExactState(bool reverse=false);

	/** @brief (IK->FK)の繰り返し回数
	*/
	void SetNumIter(int numIter);
	int GetNumIter();

	/** @brief FKの実行
	*/
	void FK();

	SPR_IFDEF(PHIKEngine);
};

struct PHBallJointIf;
struct PHSolidIf;
struct SpatialVector;
struct PHBallJointNodeIf;
struct PHRootNodeIf;
struct PHTrackingEngineIf : PHEngineIf{
public:
	SPR_IFDEF(PHTrackingEngine);

	/** @brief 追従させる際の目標となる入力角度を設定する
	*/
	void SetTrackingInputs(PHRootNodeIf* root, Vec3d rootPosition, std::vector<Quaterniond> inputs);

	/** @brief Rootの目標位置を設定する
	*/
	void SetTrackingRootPosition(PHRootNodeIf* root, Vec3d input);

	/** @brief 追従のペアを設定するノードを追加
	*/
	void AddTrackingNode(PHRootNodeIf* root, PHBallJointIf* reactJoint, PHBallJointIf* calcJoint, PHSolidIf* reactRootSolid, PHSolidIf* calcRootSolid, bool isRoot);

	/** @brief 加速度・角加速度を取得する
	*/
	SpatialVector GetTipAcceleration(PHRootNodeIf* root, int i);

	/** @brief 追従の力とトルクを計算する
	*/
	void TrackWithForce(PHRootNodeIf* root);

	/** @brief Nodeごとの追従の力とトルクを計算し、追加する
	*/
	void AddTrackingForce(PHBallJointNodeIf* calcNode, PHBallJointNodeIf* reactNode, double timeStep, Vec3d targetAngularAcceleration, SpatialVector parentTargetAcceleration, SpatialVector& targetAcceleration, Vec3d& force, Vec3d& torque);
};

struct PHFemMeshIf;
struct PHFemMeshNewIf;
/**
	For historical reason, there are two kinds of FEM mesh, FemMesh and FemMeshNew.
	Now FemMeshNew is mainly used and FemMesh will be removed in the future.	*/
struct PHFemEngineIf : PHEngineIf{
public:
	SPR_IFDEF(PHFemEngine);
	void SetTimeStep(double dt);				///<	FEM engine has it's time step independently.
	double GetTimeStep();						///<	FEM engine has it's time step independently.
	void SetVibrationTransfer(bool bEnable);	///< Enable vibration tranfer between fem meshes for haptic vibration
	void SetThermalTransfer(bool bEnable);		///< Enable thermal transfer between feme meshes for thermal simulation

	int NMeshNew();
	PHFemMeshNewIf* GetMeshNew(int i);

	bool AddMeshPair(PHFemMeshNewIf* m0, PHFemMeshNewIf* m1);
	bool RemoveMeshPair(PHFemMeshNewIf* m0, PHFemMeshNewIf* m1);
	void ThermalTransfer();
	void setheatTransferRatio(double setheatTransferRatio);

	//	Old contact treatment codes for vibration simulation. Will be replaced by new functions.
	void FEMSolidMatchRefresh();				//Refreshing the solid->FEMindex values
	void InitContacts();						 //Used to match the FEM objects with their solids counterparts
	void ClearContactVectors();

	//	For old mesh
	int NMesh();
	PHFemMeshIf* GetMesh(int i);
};
struct PHOpEngineIf : PHEngineIf{
	SPR_IFDEF(PHOpEngine);
public:
	void SetTimeStep(double dt);
	double GetTimeStep();
	void SetGravity(bool gflag);
	void InitialHapticRenderer(int objId);
	//ObjectIf* GetHapticController();
	PHOpObjDesc* GetOpObj(int i);
	void StepWithBlend();
	int AddOpObj();
	void SetHapticSolveEnable(bool enable);
	bool IsHapticSolve();
	void SetProxyCorrectionEnable(bool enable);
	bool IsProxyCorrection();
	void SetUpdateNormal(bool flag);
	bool IsUpdateNormal(int obji);
	void SetUseHaptic(bool hapticUsage);
	bool GetUseHaptic();

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
	void InitialNoMeshHapticRenderer();
	//void SetCrossPlatformCoord(bool InverX, bool InverY, bool InverZ);
};
//@}
}
#endif

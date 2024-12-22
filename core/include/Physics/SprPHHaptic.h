/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */


#ifndef SPR_PHHAPTIC_H
#define SPR_PHHAPTIC_H

#include <Collision/SprCDDetector.h>
#include <Physics/SprPHSolid.h>
#include <Physics/SprPHEngine.h>

namespace Spr {
/** \addtogroup gpPhysics */
//@{

struct PHHapticPointerDesc {
	enum HapticRenderMode {
		PENALTY,				///<	ペナルティ法でProxyの行き先を求める
		CONSTRAINT,				///<	中間表現を拘束条件だと思って、Proxyの行き先を求める
		DYNAMIC_PROXY,			///<	摩擦計算のときだけ、Proxyに質量と動力学をもたせたCONSTRAINT
	};
	///	haptic renderingのモード
	HapticRenderMode renderMode;
	PHHapticPointerDesc();
};

struct PHHapticEngineDesc {
	enum HapticStepMode {
		SINGLE_THREAD = 0,
		MULTI_THREAD,
		LOCAL_DYNAMICS,
		LOCAL_DYNAMICS6DOF,
	};
	PHHapticEngineDesc();
};
struct PHHapticPointerIf : public PHSolidIf { // , public PHHapticPointerDesc
	SPR_IFDEF(PHHapticPointer);
	void	SetHapticRenderMode(PHHapticPointerDesc::HapticRenderMode m);
	PHHapticPointerDesc::HapticRenderMode GetHapticRenderMode();

	void	EnableRotation(bool b);
	bool	IsRotation();
	void	EnableForce(bool b);
	bool	IsForce();
	void	EnableFriction(bool b);
	bool	IsFriction();
	void	EnableTimeVaryFriction(bool b);
	bool	IsTimeVaryFriction();
	void	EnableVibration(bool b);
	bool	IsVibration();

	void	SetFrictionSpring(float s);				///<	摩擦で動的Proxyを使う場合のバネ係数
	float	GetFrictionSpring();					///<	摩擦で動的Proxyを使う場合のバネ係数
	void	SetFrictionDamper(float s);				///<	摩擦で動的Proxyを使う場合のダンパ係数
	float	GetFrictionDamper();					///<	摩擦で動的Proxyを使う場合のダンパ係数
	void	SetReflexSpring(float s);				///<	提示力計算に使うバネ係数
	float	GetReflexSpring();						///<	提示力計算に使うバネ係数
	void	SetReflexDamper(float d);				///<	提示力計算に使うダンパ係数
	float	GetReflexDamper();						///<	提示力計算に使うダンパ係数
	void	SetRotationReflexSpring(float s);		///<	提示力計算に使う回転バネ係数
	float	GetRotationReflexSpring();				///<	提示力計算に使う回転バネ係数
	void	SetRotationReflexDamper(float d);		///<	提示力計算に使う回転ダンパ係数
	float	GetRotationReflexDamper();				///<	提示力計算に使う回転ダンパ係数
	void	SetLocalRange(float r);					///<	中間表現を作る距離
	float	GetLocalRange();						///<	中間表現を作る距離
	void	SetPosScale(double scale);				///<	インタフェースの位置の倍率
	double	GetPosScale();							///<	インタフェースの位置の倍率
	void	SetRotationalWeight(double w);			///<	Proxyの回転による破綻を減らすための、倍率
	double	GetRotationalWeight();					///<	Proxyの回転による破綻を減らすための、倍率
	void	SetDefaultPose(Posed p);				///<	インタフェースに対するポインタの向き
	Posed	GetDefaultPose();						///<	インタフェースに対するポインタの向き

	int     NNeighborSolids();						///<	ポインタ近傍の剛体数
	int		GetNeighborSolidId(int i);				///<	ポインタ近傍の剛体のIDを取得
	PHSolidIf*   GetNeighborSolid(int i);			///<	ポインタ近傍の剛体を取得	
	float   GetContactForce(int i);					///<	近傍物体iからの接触力
	SpatialVector GetHapticForce();					///<	力覚インタフェースに出力する力
	SpatialVector GetProxyVelocity();				///<	質量ありプロキシの速度
	Posed GetProxyPose();
	Posed GetLastProxyPose();

	void SetProxyVelocity(SpatialVector spv);

	void	AddHapticForce(const SpatialVector& f);
	void	ClearHapticForce();
	void	UpdateHumanInterface(const Posed& pose, const SpatialVector& vel);
													///<	HumanInterfaceの位置の変化をHapticPointerに伝える。普通はFWSceeneが呼び出すので、呼び出し不要。
};
struct PHSolidForHapticIf : public ObjectIf {
	SPR_IFDEF(PHSolidForHaptic);
	PHSolidIf* GetLocalSolid();
	PHSolidIf* GetSceneSolid();
	void AddForce(Vec3d f);
	void AddForce(Vec3d f, Vec3d r);
};

struct PHShapePairForHapticIf : public CDShapePairIf {
	SPR_IFDEF(PHShapePairForHaptic);
	int NIrs();
	int NIrsNormal();
	Vec3d GetIrForce(int i);
	double GetMu();
	PHFrameIf* GetFrame(int i);
	void UpdateCache();
};
struct PHSolidPairForHapticIf : public PHSolidPairIf {
	SPR_IFDEF(PHSolidPairForHaptic);
	enum FrictionState {
		FREE,
		STATIC,
		DYNAMIC,
	};
	PHShapePairForHapticIf* GetShapePair(int i, int j);
	FrictionState GetFrictionState();
	unsigned GetContactCount();
	unsigned GetFrictionCount();
	Vec3d GetForce();
	Vec3d GetTorque();
};

struct PHHapticStepBaseIf: public ObjectIf {
	SPR_IFDEF(PHHapticStepBase);
	double GetHapticTimeStep();
	void SetHapticTimeStep(double dt);
	int NHapticPointers();
	int NHapticSolids();
	PHHapticPointerIf* GetHapticPointer(int i);
	PHSolidForHapticIf* GetHapticSolid(int i);

	int NPointersInHaptic();
	int NSolidsInHaptic();
	PHHapticPointerIf* GetPointerInHaptic(int i);
	PHSolidForHapticIf* GetSolidInHaptic(int i);
	///	剛体と力覚ポインタのペアを取得する（i:剛体、j:力覚ポインタ）iには力覚ポインタも含まれる。
	PHSolidPairForHapticIf* GetSolidPairInHaptic(int i, int j);
	void ReleaseState(PHSceneIf* scene);
};
struct PHHapticStepSingleIf :public PHHapticStepBaseIf {
	SPR_IFDEF(PHHapticStepSingle);
};
struct PHHapticStepMultiIf :public PHHapticStepBaseIf {
	SPR_IFDEF(PHHapticStepMulti);
	int GetHapticCount();
	int GetLoopCount();
	bool GetSyncRequired();
	bool GetPhysicsRequired();
};

struct PHHapticEngineIf : public PHEngineIf {
public:
	SPR_IFDEF(PHHapticEngine);

	/** @breif HapticEngineのモードを切り替える
	@param mode HapticStepMode
	*/
	void SetHapticStepMode(PHHapticEngineDesc::HapticStepMode mode);
	PHHapticEngineDesc::HapticStepMode GetHapticStepMode();
	PHHapticStepBaseIf* GetHapticStep();

	///	Physics側の剛体数
	int NSolids();
	///	Physics側の力覚ポインタ数
	int NPointers();
	///	Physics側の力覚ポインタの取得
	PHHapticPointerIf* GetPointer(int i);
	///	Physics側のSolidPairForHapticの取得。iが剛体、jがHapticPointerの番号
	PHSolidPairForHapticIf* GetSolidPair(int i, int j);

	///	Haptics側の剛体数
	int NSolidsInHaptic();
	///	Haptics側の力覚ポインタ数
	int NPointersInHaptic();
	///	Haptics側の力覚ポインタの取得
	PHHapticPointerIf* GetPointerInHaptic(int i);
	///	Haptics側のSolidPairForHapticの取得。iが剛体、jがHapticPointerの番号
	PHSolidPairForHapticIf* GetSolidPairInHaptic(int i, int j);

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
	//@}

	//	Function for debug
	void HapticRendering(PHHapticStepBaseIf* hs);
};

}
#endif

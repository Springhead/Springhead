/*
 *  Copyright (c) 2003-2011, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PH_HAPTIC_STEP_BASE_H
#define PH_HAPTIC_STEP_BASE_H

#include <Physics/PHContactDetector.h>

namespace Spr{;

class PHHapticEngine;
class PHHapticPointer;
class PHSolidPairForHaptic;
class PHShapePairForHaptic;

struct PHIntermediateRepresentation :public UTRefCount{
	int solidID;				///< 剛体のID(PHSolidsForHapitcとPHSolidPairForHaptic)と侵入しているか
	PHSolidPairForHaptic* solidPair;	///< PHSolidPairForHapticへのポインタ
	PHShapePairForHaptic* shapePair;	///< PHSolidPairForHapticへのポインタ
	double depth;				///< 侵入量
	Vec3d normal;				///< 面法線
	Vec3d r;					///< 力覚ポインタの重心からの接触点までのベクトル
	Vec3d pointerPointW;		///< 力覚ポンタの侵入点(world)
	Vec3d contactPointW;		///< 中間表現面にvertexを投影した位置(world)
	Vec3d pointerPointVel;		///< 力覚ポインタ側の接触点の速度(world)
	Vec3d contactPointVel;		///< 剛体側の接触点の速度(world)
	Vec3d force;				///< 力覚ポインタからの力
	Posed interpolation_pose;	///< 形状の補間姿勢

	PHIntermediateRepresentation(){
		solidID = -1;
		solidPair = NULL;
		shapePair = NULL;
	}
	PHIntermediateRepresentation(PHSolidPairForHaptic* so, int soId, PHShapePairForHaptic* sh)
		:solidID(soId), solidPair(so), shapePair(sh), depth(0.0){	
	}
};
typedef PHIntermediateRepresentation PHIr;
typedef std::vector< PHIr* > PHIrs;

//----------------------------------------------------------------------------
// PHSolidForHaptic
//Haptic側からPhysics側へ渡す情報
class PHSolidForHapticSt{
public:
	Vec3d force;			// 力覚レンダリングによって加わる全ての力
	Vec3d torque;;			// 力覚レンダリングによって加わる全てのトルク
};
// Physics側からHaptic側へ渡す情報
class PHSolidForHapticSt2{
public:
	PHSolid* sceneSolid;	// PHSceneが持つ剛体
	
	bool bPointer;			// 力覚ポインタであるかどうか
	int  doSim;				// 近傍であるかどうか 0:近傍でない，1:はじめて近傍，2:継続して近傍

	SpatialVector b;		///< 予測シミュレーションで求めたモビリティ（重力等の定数項）
	SpatialVector curb;		///< 通常シミュレーションででた定数項
	SpatialVector lastb;	///< 前回の予測シミュレーションで求めた定数項
	SpatialVector bimpact;
};

class PHSolidForHaptic : public PHSolidForHapticSt, public PHSolidForHapticSt2, public Object{  
private:
	PHSolidForHaptic(const PHSolidForHaptic&) { assert(0); }
	void operator = (const PHSolidForHaptic&) { assert(0); }
public:
	SPR_OBJECTDEF_ABST(PHSolidForHaptic);
	PHSolid localSolid;		// sceneSolidのクローン

	// 衝突判定用の一時変数
	int NLocalFirst;		// はじめて近傍になる力覚ポインタの数（衝突判定で利用）
	int NLocal;				// 近傍な力覚ポインタの数（衝突判定で利用）
	PHSolidForHaptic();
	PHSolidIf* GetLocalSolid() { return (PHSolidIf*)GetLocalSolidImp(); }
	PHSolidIf* GetSceneSolid() { return (PHSolidIf*)GetSceneSolidImp(); }
	PHSolid* GetLocalSolidImp() { return &localSolid; }
	PHSolid* GetSceneSolidImp() { return sceneSolid; }
	void AddForce(Vec3d f);
	void AddForce(Vec3d f, Vec3d r);
	void CopyFromPhysics(PHSolidForHaptic* src);
	void CopyFromHaptics(PHSolidForHaptic* src);
};
class PHSolidsForHaptic : public std::vector< UTRef< PHSolidForHaptic > >{};

//----------------------------------------------------------------------------
// PHShapePairForHaptic
class PHSolidPairForHaptic;
class PHShapePairForHaptic : public PHShapePair{
public:	
	SPR_OBJECTDEF(PHShapePairForHaptic);
	//	variables for haptic rendering in haptic thread.
	// 0:solid、1:pointer
	// Vec3d normalは剛体から力覚ポインタへの法線ベクトル
	Posed lastShapePoseW[2];	///< 前回の形状姿勢
	Vec3d lastClosestPoint[2];	///< 前回の近傍点(ローカル座標)
	Vec3d lastNormal;			///< 前回の近傍物体の提示面の法線
	std::vector< Vec3d > intersectionVertices; ///< 接触体積の頂点(ローカル座標)
	std::vector< UTRef< PHIr > > irs;	///<	中間表現、後半に摩擦の拘束が追加される
	int nIrsNormal;						///<	法線の中間表現の数、以降が摩擦

	//	variables updated by UpdateCache()
	float springK;				///< バネ係数
	float damperD;				///< ダンパ係数
	float mu;					///< 動摩擦係数
	float mu0;					///< 最大静止摩擦係数	
	float timeVaryFrictionA;	///< 時変摩擦定数A
	float timeVaryFrictionB;	///< 時変摩擦定数B
	float timeVaryFrictionC;	///< 時変摩擦定数C
	float frictionViscosity;	///< 粘性摩擦のための係数	f_t = frictionViscocity * vel * f_N
	float stribeckVelocity;		///< ストライベク効果の速度の影響の強さ	: 動摩擦 =　mu + (mu - stribeckmu) * (exp(-v / stribeckVelocity) - 1.0)
	float stribeckmu;			///< 速度∞のときの摩擦係数
	float muCur;				///< 計算された時変摩擦係数

	//LuGreモデルに必要なもの
	bool hasContact;				///< 前回の更新時に2つのShapeが接触していたかどうか
	double bristlesSpringK;			///< LuGreモデルにおける剛毛のバネ係数
	double bristlesDamperD;			///< LuGreモデルにおける剛毛のダンパ係数
	double bristlesViscosityV;		///< LuGreモデルにおける剛毛にはたらく粘性抵抗の係数
	Vec2d avgBristlesDeflection;	///< 剛毛の平均変位(接触面上の座標)
	Vec2d avgBristlesDeflectionVel;	///< 剛毛の平均変位(接触面上の座標)を微分したもの　平均変位の変化の速さを表す
	Posed contactSurfacePose;		///< World座標系から見た接触面上の座標系の相対位置姿勢
	Vec3d pointerPos;				///< ハプティックポインタの位置(World座標)
	double pointerDepth;			///< ハプティックポインタの最も沈み込んでいる部分の深さ(侵入量)
	Vec3d proxyPos;					///< プロキシの位置(World座標)
	Vec3d objectVel;				///< 接触している相手の物体の速度(World座標)
	Vec2d relativeVelOnSurface;		///< プロキシと接触している物体の相対速度(接触面上の座標)
	Vec2d avgStickingTime;			///< 平均固着継続時間
	Vec2d frictionForce;			///< 摩擦力 (接触絵面上の座標)
	double LuGreParameterA;			///< LuGreモデルの関数g(T)を計算するときに使用されるパラメータA
	double LuGreParameterB;			///< LuGreモデルの関数g(T)を計算するときに使用されるパラメータB
	double LuGreParameterC;			///< LuGreモデルの関数g(T)を計算するときに使用されるパラメータC
	Vec2d LuGreFunctionG;			///< LuGreモデルにおける関数g(T) = A + B log(C * avgStickingTime + 1)の値


	PHShapePairForHaptic();
	void CopyFromPhysics(const PHShapePairForHaptic* src);
	void CopyFromHaptics(const PHShapePairForHaptic* src);
	void Init(PHSolidPair* sp, PHFrame* fr0, PHFrame* fr1);
	void UpdateCache();
	/// 接触判定．近傍点対を常時更新
	virtual bool Detect(unsigned ct, const Posed& pose0, const Posed& pose1);
	/// 接触時の判定
	int OnDetect(unsigned ct, const Vec3d& center0);
	bool CompIntermediateRepresentation(Posed curShapePoseW[2], double t, bool bInterpolatePose, bool bMultiPoints);
	int NIrs() { return (int)irs.size();  }
	int NIrsNormal() { return nIrsNormal;  }
	Vec3d GetIrForce(int i) { return irs[i]->force;  }
	double GetMu() { return muCur;  }
};

//----------------------------------------------------------------------------

struct PHSolidPairForHapticVarsBase{
	Vec3d force;			///< 力覚ポインタがこの剛体に加える力
	Vec3d torque;			///< 力覚ポインタがこの剛体に加えるトルク

	Posed interpolationPose;	///< 剛体の補間姿勢
	Posed lastInterpolationPose;
	Posed initialRelativePose;	///< 接触開始時の相対位置姿勢
	Posed relativePose;			///< 接触中の相対位置姿勢

	unsigned contactCount;
	unsigned fricCount;			///< 静止摩擦/動摩擦の継続Hapticステップ数, 時変摩擦と固有振動用の時間計測

	Vec3d contactVibrationVel;
	Vec3d lastStaticFrictionForce;
	PHSolidPairForHapticIf::FrictionState  frictionState;

	int solidID[2];
	PHSolidPairForHapticVarsBase() :contactCount(0), fricCount(0),
		frictionState(PHSolidPairForHapticIf::FREE)
	{
		solidID[0] = solidID[1] = -1;
	}
};
struct PHSolidPairForHapticVarsLocalDynamics {
	int inLocal;	// 0:NONE, 1:in local first, 2:in local
	TMatrixRow<6, 3, double> A;		// LocalDynamicsで使うアクセレランス
	SpatialMatrix A6D;				// LocalDynamics6Dで使うアクセレランス
	PHSolidPairForHapticVarsLocalDynamics() :
		inLocal(0)
	{
		A.clear();
		A6D.clear();
	}
};
struct PHSolidPairForHapticVars : public PHSolidPairForHapticVarsBase, public PHSolidPairForHapticVarsLocalDynamics {
};


class PHSolidPairForHaptic : public PHSolidPairForHapticVars, public PHSolidPair/*< PHShapePairForHaptic, PHHapticEngine >*/{
private:
	//	copy or assign are not permitted.
	PHSolidPairForHaptic(const PHSolidPairForHaptic& s) { assert(0);  }
	void operator = (const PHSolidPairForHaptic& s){ assert(0); }
public:
	SPR_OBJECTDEF(PHSolidPairForHaptic);

	PHSolidPairForHaptic();

	virtual PHShapePairForHaptic* CreateShapePair(){ return DBG_NEW PHShapePairForHaptic(); }
	PHShapePairForHapticIf*       GetShapePair(int i, int j){ return (PHShapePairForHapticIf*)&*shapePairs.item(i,j); }
	const PHShapePairForHapticIf* GetShapePair(int i, int j) const { return (const PHShapePairForHapticIf*)&*shapePairs.item(i,j); }
	PHSolidPairForHapticIf::FrictionState GetFrictionState() { return frictionState; }
	unsigned GetContactCount() { return contactCount;  }
	unsigned GetFrictionCount() { return fricCount; }

	Vec3d GetForce(){ return force; }
	Vec3d GetTorque(){ return torque; }

	/// 交差が検知された後の処理
	virtual void  OnDetect(PHShapePair* sp, unsigned ct, double dt);	///< 交差が検知されたときの処理

	void CopyFromPhysics(const PHSolidPairForHaptic* phys);
	void CopyFromHaptics(const PHSolidPairForHaptic* haptics);
	void CopyForDisplay(const PHSolidPairForHaptic* phys);
};


//----------------------------------------------------------------------------
// PHHapticStepBase
class PHHapticStepBase : public SceneObject{
public:
	SPR_OBJECTDEF_ABST(PHHapticStepBase);
	PHHapticEngine* engine;
	PHHapticStepBase(){}
	///	物理シミュレーションのdt
	double GetPhysicsTimeStep();
	///	力覚レンダリングのdt
	double GetHapticTimeStep();
	void SetHapticTimeStep(double dt);
	///	
	virtual void Step1(){};
	///	
	virtual void Step2(){};
	virtual void StepHapticLoop() = 0;
	virtual void StepHapticSync() = 0;
	virtual void UpdateHapticPointer() = 0;

	int NHapticPointers();
	int NHapticSolids();
	PHHapticPointerIf* GetHapticPointer(int i) { return (PHHapticPointerIf*) GetHapticPointerImp(i); }
	PHSolidForHapticIf* GetHapticSolid(int i) { return (PHSolidForHapticIf*) GetHapticSolidImp(i); }
	PHHapticPointer* GetHapticPointerImp(int i);
	PHSolidForHaptic* GetHapticSolidImp(int i);

	virtual int NPointersInHaptic()=0;
	virtual int NSolidsInHaptic()=0;
	PHHapticPointerIf* GetPointerInHaptic(int i) { return (PHHapticPointerIf*)GetPointerInHapticImp(i); }
	virtual PHHapticPointer* GetPointerInHapticImp(int i) = 0;
	PHSolidForHapticIf* GetSolidInHaptic(int i) { return (PHSolidForHapticIf*)GetSolidInHapticImp(i); }
	virtual PHSolidForHaptic* GetSolidInHapticImp(int i) = 0;
	///	剛体と力覚ポインタのペアを取得する（i:剛体、j:力覚ポインタ）iには力覚ポインタも含まれる。
	PHSolidPairForHapticIf* GetSolidPairInHaptic(int i, int j) { return (PHSolidPairForHapticIf*)GetSolidPairInHapticImp(i, j); }
	virtual PHSolidPairForHaptic* GetSolidPairInHapticImp(int i, int j) = 0;
	virtual void ReleaseState(PHSceneIf* scene) {}


	///< デバック用シミュレーション実行
	virtual void StepPhysicsSimulation();
	/// シミュレーションを実行する直前に実行されるコールバックを登録する
	virtual bool SetCallbackBeforeStep(PHHapticEngineIf::Callback f, void* arg);
	/// シミュレーションを実行した直後に実行されるコールバックを登録する
	virtual bool SetCallbackAfterStep(PHHapticEngineIf::Callback f, void* arg);
	///	物理ステップの中の何度目のHapticStepかを返す
	virtual int GetLoopCount() = 0;
	///	中間表現を補間する場合 true
	virtual bool IsInterporate() = 0;
};

}
#endif

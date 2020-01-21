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
//#include <Physics/PHHapticEngine.h>
//#include <Physics/PHHapticRender.h>

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

class PHSolidForHaptic : public PHSolidForHapticSt, public PHSolidForHapticSt2, public UTRefCount{  
public:
	PHSolid localSolid;		// sceneSolidのクローン

	// 衝突判定用の一時変数
	int NLocalFirst;		// はじめて近傍になる力覚ポインタの数（衝突判定で利用）
	int NLocal;				// 近傍な力覚ポインタの数（衝突判定で利用）
	PHSolidForHaptic();
	PHSolid* GetLocalSolid(){ return &localSolid; }
	void AddForce(Vec3d f);
	void AddForce(Vec3d f, Vec3d r);
};
class PHSolidsForHaptic : public std::vector< UTRef< PHSolidForHaptic > >{};

//----------------------------------------------------------------------------
// PHShapePairForHaptic
class PHSolidPairForHaptic;
class PHShapePairForHaptic : public PHShapePair{
public:	
	SPR_OBJECTDEF(PHShapePairForHaptic);
	// 0:solid、1:pointer
	// Vec3d normalは剛体から力覚ポインタへの法線ベクトル
	Posed lastShapePoseW[2];	///< 前回の形状姿勢
	Vec3d lastClosestPoint[2];	///< 前回の近傍点(ローカル座標)
	Vec3d lastNormal;			///< 前回の近傍物体の提示面の法線
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

	std::vector<float> mus;					///< 動摩擦係数
	std::vector<float> mu0s;					///< 最大静止摩擦係数	
	std::vector<float> timeVaryFrictionAs;	///< 時変摩擦定数A
	std::vector<float> timeVaryFrictionBs;	///< 時変摩擦定数B
	std::vector<float> timeVaryFrictionDs;	///< 時変摩擦定数D
	std::vector<float> timeVaryFrictionCs;	///< 時変摩擦定数C
	std::vector<float> muCurs;				///< 計算された時変摩擦係数
	std::vector<float> stribeckVelocitys;
	std::vector<float> stribeckmus;
	std::vector<double> c;
	std::vector<float> bristleK;			/// 剛毛モデルもばね係数


	std::vector< Vec3d > intersectionVertices; ///< 接触体積の頂点(ローカル座標)
	std::vector< UTRef< PHIr > > irs;	///<	中間表現、後半に摩擦の拘束が追加される
	int nIrsNormal;						///<	法線の中間表現の数、以降が摩擦

	PHShapePairForHaptic();
	void Init(PHSolidPair* sp, PHFrame* fr0, PHFrame* fr1);
	void UpdateCache();
	/// 接触判定．近傍点対を常時更新
	virtual bool Detect(unsigned ct, const Posed& pose0, const Posed& pose1);
	/// 接触時の判定
	int OnDetect(unsigned ct, const Vec3d& center0);
	bool AnalyzeContactRegion();
	bool CompIntermediateRepresentation(Posed curShapePoseW[2], double t, bool bInterpolatePose, bool bMultiPoints);
	int NIrs() { return (int)irs.size();  }
	int NIrsNormal() { return nIrsNormal;  }
	Vec3d GetIrForce(int i) { return irs[i]->force;  }
	double GetMu() { return muCur;  }

	// GMS用
	double GetMus(int id) { return muCurs[id]; }
};

//----------------------------------------------------------------------------

struct PHSolidPairForHapticSt{
	Vec3d force;			///< 力覚ポインタがこの剛体に加える力
	Vec3d torque;			///< 力覚ポインタがこの剛体に加えるトルク

	Posed interpolationPose;	///< 剛体の補間姿勢
	Posed lastInterpolationPose;
	Posed initialRelativePose;	///< 接触開始時の相対位置姿勢
	Posed relativePose;			///< 接触中の相対位置姿勢

	unsigned contactCount;
	unsigned fricCount;			///< 静止摩擦/動摩擦の継続Hapticステップ数, 時変摩擦と固有振動用の時間計測
	unsigned slipCount;      ///stribeck効果用

							 //GMS用
	std::vector<unsigned> fricCounts;
	std::vector<unsigned> contactCounts;
	std::vector<PHSolidPairForHapticIf::FrictionState>  frictionStates;
	std::vector<Vec3d> z;
	std::vector<Vec3d> lastz;
	Vec3d lasttangent;
	std::vector<double> laststaticflag;

	Vec3d contactVibrationVel;
	Vec3d lastStaticFrictionForce;
	PHSolidPairForHapticIf::FrictionState  frictionState;
};

class PHSolidPairForHaptic : public PHSolidPairForHapticSt, public PHSolidPair/*< PHShapePairForHaptic, PHHapticEngine >*/{
public:
	SPR_OBJECTDEF(PHSolidPairForHaptic);
	int solidID[2];
	
	int inLocal;	// 0:NONE, 1:in local first, 2:in local
	TMatrixRow<6, 3, double> A;		// LocalDynamicsで使うアクセレランス
	//TMatrixRow<6, 6, double> A6D;  // LocalDynamics6Dで使うアクセレランス
	SpatialMatrix A6D;
	
	PHSolidPairForHaptic();
	PHSolidPairForHaptic(const PHSolidPairForHaptic& s);

	virtual PHShapePairForHaptic* CreateShapePair(){ return DBG_NEW PHShapePairForHaptic(); }
	PHShapePairForHapticIf*       GetShapePair(int i, int j){ return (PHShapePairForHapticIf*)&*shapePairs.item(i,j); }
	const PHShapePairForHapticIf* GetShapePair(int i, int j) const { return (const PHShapePairForHapticIf*)&*shapePairs.item(i,j); }
	PHSolidPairForHapticIf::FrictionState GetFrictionState() { return frictionState; }
	unsigned GetContactCount() { return contactCount;  }
	unsigned GetFrictionCount() { return fricCount; }

	//GMS用
	void InitFrictionState(int n) {
		frictionStates.clear();
		for (int i = 0; i < n; i++) {
			frictionStates.push_back(PHSolidPairForHapticIf::FREE);
		}
	}
	void InitFrictionCount(int n) {
		fricCounts.clear();
		for (int i = 0; i < n; i++) {
			fricCounts.push_back(0);
		}
	}
	void InitContactCount(int n) {
		contactCounts.clear();
		for (int i = 0; i < n; i++) {
			contactCounts.push_back(0);
		}
	}
	void InitZ(int n) {
		z.clear();
		for (int i = 0; i < n; i++) {
			z.push_back(Vec3d(0.0f,0.0f,0.0f));
		}
	}
	void InitlastStaticFlag(int n) {
		laststaticflag.clear();
		for (int i = 0; i < n; i++) {
			laststaticflag.push_back(0);
		}
	}
	Vec3d GetZ(int i) { return z[i]; }


	Vec3d GetForce(){ return force; }
	Vec3d GetTorque(){ return torque; }

	/// 交差が検知された後の処理
	virtual void  OnDetect(PHShapePair* sp, unsigned ct, double dt);	///< 交差が検知されたときの処理
};


//----------------------------------------------------------------------------
// PHHapticStepBase
class PHHapticStepBase : public SceneObject{
public:
	SPR_OBJECTDEF_ABST_NOIF(PHHapticStepBase);
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
	PHHapticPointer*       GetHapticPointer(int i);
	PHSolidForHaptic*      GetHapticSolid(int i);

	virtual int NPointersInHaptic()=0;
	virtual int NSolidsInHaptic()=0;
	virtual PHHapticPointer* GetPointerInHaptic(int i)=0;
	virtual PHSolidForHaptic* GetSolidInHaptic(int i)=0;
	///	剛体と力覚ポインタのペアを取得する（i:剛体、j:力覚ポインタ）iには力覚ポインタも含まれる。
	virtual PHSolidPairForHaptic* GetSolidPairInHaptic(int i, int j)=0;
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

/*
 *  Copyright (c) 2003-2011, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PH_HAPTICENGINE_H
#define PH_HAPTICENGINE_H

//#include <Springhead.h>
#include <HumanInterface/SprHIBase.h>
#include <Physics/PHContactDetector.h>
#include <Physics/PHHapticPointer.h>
#include <Physics/PHHapticRender.h>

using namespace PTM;
namespace Spr{;

//----------------------------------------------------------------------------
// PHSolidForHaptic
//Haptic側からPhysics側へ渡す情報
struct PHSolidForHapticSt{
public:
	Vec3d force;			// 力覚レンダリングによって加わる全ての力
	Vec3d torque;;			// 力覚レンダリングによって加わる全てのトルク
};
// Physics側からHaptic側へ渡す情報
struct PHSolidForHapticSt2{
public:
	PHSolid* sceneSolid;	// PHSceneが持つ剛体
	
	bool bPointer;			// 力覚ポインタであるかどうか
	int doSim;				// 近傍であるかどうか 0:近傍でない，1:はじめて近傍，2:継続して近傍

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
class PHShapePairForHaptic : public CDShapePair{
public:	
	// 0:solid、1:pointer
	// Vec3d normalは剛体から力覚ポインタへの法線ベクトル
	Posed lastShapePoseW[2];	///< 前回の形状姿勢
	Vec3d lastClosestPoint[2];	///< 前回の近傍点(ローカル座標)
	Vec3d lastNormal;			///< 前回の近傍物体の提示面の法線
	std::vector< Vec3d > intersectionVertices; ///< 接触体積の頂点(ローカル座標)
	std::vector< UTRef< PHIr > > irs;

	PHShapePairForHaptic();
	/// 接触判定．近傍点対を常時更新
	bool Detect(unsigned ct, const Posed& pose0, const Posed& pose1);
	/// 接触時の判定
	int OnDetect(unsigned ct, const Vec3d& center0);
	bool AnalyzeContactRegion();
	bool CompIntermediateRepresentation(Posed curShapePoseW[2], double t, bool bInterpolatePose, bool bMultiPoints);
};

//----------------------------------------------------------------------------

struct PHSolidPairForHapticSt{
	Vec3d force;			///< 力覚ポインタがこの剛体に加える力
	Vec3d torque;			///< 力覚ポインタがこの剛体に加えるトルク

	Posed interpolationPose;	///< 剛体の補間姿勢
	Posed lastInterpolationPose;
	Posed initialRelativePose;	///< 接触開始時の相対位置姿勢
	Posed relativePose;			///< 接触中の相対位置姿勢

	int contactCount;
	Vec3d vibrationVel;
	enum FrictionState{
		FREE,
		FIRST,
		STATIC,
		DYNAMIC,
	} frictionState;
};

class PHSolidPairForHaptic : public PHSolidPairForHapticSt, public PHSolidPair< PHShapePairForHaptic, PHHapticEngine >, public Object{
public:
	int solidID[2];
	
	int inLocal;	// 0:NONE, 1:in local first, 2:in local
	TMatrixRow<6, 3, double> A;		// LocalDynamicsで使うアクセレランス
	TMatrixRow<6, 6, double> A6D;  // LocalDynamics6Dで使うアクセレランス
	
	PHSolidPairForHaptic();
	PHSolidPairForHaptic(const PHSolidPairForHaptic& s);
	/// 交差が検知された後の処理
	virtual bool Detect(engine_type* engine, unsigned int ct, double dt);
	virtual void OnDetect(shapepair_type* sp, engine_type* engine, unsigned ct, double dt);	///< 交差が検知されたときの処理
	virtual PHIrs CompIntermediateRepresentation(PHSolid* curSolid[2], double t, bool bInterpolatePose);
	virtual bool CompFrictionIntermediateRepresentation(PHShapePairForHaptic* sp);
	virtual bool CompFrictionIntermediateRepresentation2(PHShapePairForHaptic* sp);
};
class PHSolidPairsForHaptic : public UTCombination< UTRef<PHSolidPairForHaptic> >{};

//----------------------------------------------------------------------------
// PHHapticEngineImp
class PHHapticRender;
class PHHapticLoopImp;
class PHHapticEngineImp : public SceneObject{
	SPR_OBJECTDEF_NOIF(PHHapticEngineImp);
public:
	PHHapticEngine* engine;
	PHHapticLoopImp* hapticLoop;
	UTRef< ObjectStatesIf > states;
	PHHapticEngineImp(){}
	virtual void Step1(){};
	virtual void Step2(){};
	virtual void StepHapticLoop(){};

	double GetPhysicsTimeStep();
	double GetHapticTimeStep();

	int NHapticPointers();
	int NHapticSolids();
	PHHapticPointer* GetHapticPointer(int i);
	PHSolidForHaptic* GetHapticSolid(int i);
	///< 剛体と力覚ポインタのペアを取得する（i:剛体、j:力覚ポインタ）
	// iには力覚ポインタも含まれる。
	PHSolidPairForHaptic* GetSolidPairForHaptic(int i, int j);
	PHHapticPointers* GetHapticPointers();
	PHSolidsForHaptic* GetHapticSolids();
	PHSolidPairsForHaptic* GetSolidPairsForHaptic();
	PHHapticRender* GetHapticRender();
	PHHapticLoopImp* GetHapticLoop();

	///< デバック用シミュレーション実行
	virtual void StepPhysicsSimulation();

	/// シミュレーションを実行する直前に実行されるコールバックを登録する
	virtual bool SetCallbackBeforeStep(PHHapticEngineIf::Callback f, void* arg);
	/// シミュレーションを実行した直後に実行されるコールバックを登録する
	virtual bool SetCallbackAfterStep(PHHapticEngineIf::Callback f, void* arg);
};

//----------------------------------------------------------------------------
// PHHapticEngine
class PHHapticEngine : public PHHapticEngineDesc, public PHContactDetector< PHShapePairForHaptic, PHSolidPairForHaptic, PHHapticEngine >{
public:
	SPR_OBJECTDEF1(PHHapticEngine, PHEngine);
	ACCESS_DESC(PHHapticEngine);
	UTRef< PHHapticEngineImp > engineImp;
	std::vector< UTRef<PHHapticEngineImp> > engineImps;
	UTRef< PHHapticRender > hapticRender;
	PHHapticPointers hapticPointers;
	PHSolidsForHaptic hapticSolids;
	typedef std::vector< HIBaseIf* > HIBaseIfs;
	HIBaseIfs humanInterfaces;
	///描画アクセスで落ちる場合があるかもで追加 2012.12.11 susa
	UTCombination< UTRef< PHSolidPairForHaptic > > solidPairsTemp;	///< hapticsの情報をグラフィクスで表示するためのキャッシュ

	struct Edge{ Vec3f min, max; };
	std::vector< Edge > edges;

	bool bHapticEngine;
	bool bPhysicStep;
	HapticEngineMode engineMode;
	PHHapticEngine();


	//-------------------------------------------------------------------
	// APIの実装
	///< 力覚提示計算のON/OFF
	void EnableHapticEngine(bool b){ bHapticEngine = b; }
	///< エンジンモードの選択
	void SetHapticEngineMode(HapticEngineMode mode);
	///< 力覚ポインタの数を返す
	int NHapticPointers(){ return (int)hapticPointers.size(); }
	///< 力覚ポインタへのポインタを返す
	PHHapticPointer* GetHapticPointer(int i){ return hapticPointers[i]; }
	///< state保存のために確保した領域を開放する
	void ReleaseState();

	//-------------------------------------------------------------------
	// PHHapticEngineの実装
	///< シミュレーションループの更新（PHScene::Integrate()からコール）
	virtual void Step(){ if(bHapticEngine && bPhysicStep) engineImp->Step1(); }
	virtual void Step2(){ if(bHapticEngine && bPhysicStep) engineImp->Step2(); }
	///< 力覚ループの更新	
	virtual void StepHapticLoop(){ if(bHapticEngine) engineImp->StepHapticLoop(); }

	///< 力覚レンダリング用の衝突判定開始
	virtual void StartDetection();
	///< BBoxの向きを更新
	void UpdateEdgeList();
	///< ある剛体の近傍の剛体をAABBでみつける（rangeはBBoxをさらにrange分だけ広げる
	void Detect(PHHapticPointer* pointer);
	int GetPriority() const { return SGBP_HAPTICENGINE1; }
	///< 剛体の追加
	bool AddChildObject(ObjectIf* o);
	///< 剛体の削除
	bool DelChildObject(ObjectIf* o);
	///< ShapePairの更新
	void UpdateShapePairs(PHSolid* solid);
	///< 接触モードの変更
	virtual void SetContactMode();
	//< エンジンモードの取得
	int GetHapticEngineMode();
	///< ローカル側の力覚ポインタをとってくる
	PHHapticPointers* GetLocalHapticPointers();

	///< デバック用シミュレーション実行
	///（PHScene::Stepの変わりに呼ぶ）
	virtual void StepPhysicsSimulation(){ engineImp->StepPhysicsSimulation(); }

	/// シミュレーションを実行する直前に実行されるコールバックを登録する
	virtual bool SetCallbackBeforeStep(PHHapticEngineIf::Callback f, void* arg) { return engineImp->SetCallbackBeforeStep(f, arg); }
	/// シミュレーションを実行した直後に実行されるコールバックを登録する
	virtual bool SetCallbackAfterStep(PHHapticEngineIf::Callback f, void* arg)  { return engineImp->SetCallbackAfterStep(f, arg); }
};

// PHSceneからStep()を2回呼ぶための擬似クラス
class PHHapticEngineCallStep2 : public PHEngine{
public:
	SPR_OBJECTDEF_NOIF(PHHapticEngineCallStep2);
	UTRef< PHHapticEngine > engine;
	int GetPriority() const { return SGBP_HAPTICENGINE2; }
	virtual void Step(){ engine->Step2(); }
};

}	//	namespace Spr
#endif
/*
 *  Copyright (c) 2003-2011, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PH_HAPTICENGINE_H
#define PH_HAPTICENGINE_H

#include <Springhead.h>
#include <Physics/PHContactDetector.h>
#include <Physics/PHHapticPointer.h>

using namespace PTM;
namespace Spr{;

//----------------------------------------------------------------------------
// PHSolidForHaptic
struct PHSolidForHapticSt{
	PHSolid localSolid;		// sceneSolidのクローン
};

class PHSolidForHaptic : public PHSolidForHapticSt, public UTRefCount{  
public:
	PHSolid* sceneSolid;	// PHSceneが持つ剛体
	
	bool bPointer;			// 力覚ポインタであるかどうか
	int doSim;				// 近傍であるかどうか 0:近傍でない，1:はじめて近傍，2:継続して近傍

	SpatialVector b;		///< 予測シミュレーションで求めたモビリティ（重力等の定数項）
	SpatialVector curb;		///< 通常シミュレーションででた定数項
	SpatialVector lastb;	///< 前回の予測シミュレーションで求めた定数項

	// 衝突判定用の一時変数
	int NLocalFirst;		// はじめて近傍になる力覚ポインタの数（衝突判定で利用）
	int NLocal;				// 近傍な力覚ポインタの数（衝突判定で利用）
	PHSolidForHaptic();
	PHSolidForHaptic(const PHSolidForHaptic& s);
};
class PHSolidsForHaptic : public std::vector< UTRef< PHSolidForHaptic > >{};

//----------------------------------------------------------------------------
// PHShapePairForHaptic
class PHSolidPairForHaptic;
class PHShapePairForHaptic : public CDShapePair{
public:	
	//PHSolidPairForHaptic* solidPair;
	// 0:solid、1:pointer
	// Vec3d normalは剛体から力覚ポインタへの法線ベクトル
	Posed lastShapePoseW[2];	///< 前回の形状姿勢
	Vec3d lastClosestPoint[2];	///< 前回の近傍点(ローカル座標)
	Vec3d lastNormal;			///< 前回の近傍物体の提示面の法線
	//double sectionDepth;
	//double lastSectionDepth;
	std::vector< Vec3d > intersectionVertices; ///< 接触体積の頂点(ローカル座標)
	//std::vector< Vec3d > pointerSection;	///< ポインタの接触頂点(ローカル座標)
	//std::vector< Vec3d > solidSection;		///< 剛体の接触頂点(ローカル座標)

	PHShapePairForHaptic();
	PHShapePairForHaptic(const PHShapePairForHaptic& s);
	/// 接触判定．近傍点対を常時更新
	bool Detect(unsigned ct, const Posed& pose0, const Posed& pose1);
	/// 接触時の判定
	int OnDetect(unsigned ct, const Vec3d& center0);
	bool AnalyzeContactRegion();
	bool CompIntermediateRepresentation(PHIrs &irs, Posed curShapePoseW[2], double t, bool bInterpolatePose, bool bPoints);
};

//----------------------------------------------------------------------------
// PHSolidPairForHaptic
//struct ImpulsePoint{
//	Vec3d contactPointW;
//	Vec3d impulse;
//};

struct PHSolidPairForHapticSt{
	Vec3d test_force;				///< 予測シミュレーションで使うテスト力
	Vec3d test_torque;				///< 予測シミュレーションで使うテストトルク
	//Vec3d impulse;					///< 物理プロセスが1ステップ終わるまでに力覚ポインタが加えた力積
	//std::vector< ImpulsePoint > impulsePoints;

	Posed interpolationPose;	///< 剛体の補間姿勢
	Posed lastInterpolationPose;
	Posed initialRelativePose;	///< 接触開始時の相対位置姿勢
	Posed relativePose;			///< 接触中の相対位置姿勢
	enum FrictionState{
		FREE,
		STATIC,
		DYNAMIC,
	} frictionState;
};

class PHSolidPairForHaptic : public PHSolidPairForHapticSt, public PHSolidPair< PHShapePairForHaptic, PHHapticEngine >, public Object{
public:
	//typedef PHSolidPair<PHShapePairForHaptic, PHHapticEngine> base_type;
	//typedef base_type::shapepair_type shapepair_type;
	//typedef base_type::engine_type engine_type;
	int solidID[2];
	
	int inLocal;	// 0:NONE, 1:in local first, 2:in local
	struct Accelerance{
		Vec3d force;					// LocalDynamicsで使う力
		TMatrixRow<6, 3, double> A;		// LocalDynamicsで使うアクセレランス
		std::vector< Vec3d > f;			// LocalDynamics6Dで使う力
		TMatrixRow<6, 6, double> Minv;  // LocalDynamics6Dで使うアクセレランス
	} accelerance;
	
	PHSolidPairForHaptic();
	PHSolidPairForHaptic(const PHSolidPairForHaptic& s);
	/// 交差が検知された後の処理
	virtual bool Detect(engine_type* engine, unsigned int ct, double dt);
	virtual void OnDetect(shapepair_type* sp, engine_type* engine, unsigned ct, double dt);	///< 交差が検知されたときの処理
	virtual PHIrs CompIntermediateRepresentation(PHSolid* curSolid[2], double t, bool bInterpolatePose, bool bPoints);
};
class PHSolidPairsForHaptic : public UTCombination< UTRef<PHSolidPairForHaptic> >{};

//----------------------------------------------------------------------------
// PHHapticEngineImp
class PHHapticEngineImp : public SceneObject{
	SPR_OBJECTDEF_NOIF(PHHapticEngineImp);
public:
	PHHapticEngine* engine;
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
	PHSolidPairForHaptic* GetSolidPairForHaptic(int i, int j);
	PHHapticPointers* GetHapticPointers();
	PHSolidsForHaptic* GetHapticSolids();
	PHSolidPairsForHaptic* GetSolidPairsForHaptic();

	///< デバック用シミュレーション実行
	virtual void StepSimulation(){};
};

//----------------------------------------------------------------------------
// PHHapticEngine


struct PHHapticEngineDesc{
	bool bHaptic;
	PHHapticEngineDesc();
};
class PHHapticEngine : public PHHapticEngineDesc, public PHContactDetector< PHShapePairForHaptic, PHSolidPairForHaptic, PHHapticEngine >{
public:
	SPR_OBJECTDEF1(PHHapticEngine, PHEngine);
	ACCESS_DESC(PHHapticEngine);
	UTRef< PHHapticEngineImp > engineImp;
	std::vector< UTRef<PHHapticEngineImp> > engineImps;
	PHHapticPointers hapticPointers;
	PHSolidsForHaptic hapticSolids;
	typedef std::vector< HIBaseIf* > HIBaseIfs;
	HIBaseIfs humanInterfaces;

	struct Edge{ Vec3f min, max; };
	std::vector< Edge > edges;

	enum EngineType{
		NONE,
		MULTI_THREAD,
		SINGLE_THREAD,
	} engineType;


	PHHapticEngine();

	///< シミュレーションループの更新（PHScene::Integrate()からコール）
	virtual void Step(){ if(bHaptic) engineImp->Step1(); }
	virtual void Step2(){ if(bHaptic) engineImp->Step2(); }
	///< 力覚ループの更新	
	virtual void StepHapticLoop(){ if(bHaptic) engineImp->StepHapticLoop(); }

	///< 力覚提示計算のON/OFF
	void EnableHaptic(bool b){ bHaptic = b; }
	///< レンダリングモードの選択
	void SetHapticEngineType(EngineType e);
	///< 力覚ポインタの数を返す
	int NHapticPointers(){ return (int)hapticPointers.size(); }
	///< 力覚ポインタへのポインタを返す
	PHHapticPointer* GetHapticPointer(int i){ return hapticPointers[i]; }

	///< 力覚ポインタの状態の更新
	virtual void UpdateHapticPointer();
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
	void UpdateShapePairs(PHSolid* solid);

	///< デバック用シミュレーション実行
	///（PHScene::Stepの変わりに呼ぶ）
	virtual void StepSimulation(){ engineImp->StepSimulation(); }

};

// PHSceneからStep()を2回呼ぶための擬似クラス
class PHHapticPseudEngine : public PHEngine{
public:
	SPR_OBJECTDEF_NOIF(PHHapticPseudEngine);
	UTRef< PHHapticEngine > engine;
	int GetPriority() const { return SGBP_HAPTICENGINE2; }
	virtual void Step(){ engine->Step2(); }
};


}	//	namespace Spr
#endif

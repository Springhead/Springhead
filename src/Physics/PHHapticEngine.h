/*
 *  Copyright (c) 2003-2011, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHHAPTICENGINE_H
#define PHHAPTICENGINE_H

#include <Springhead.h>
#include <Physics/PHContactDetector.h>

using namespace PTM;
namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticPointer
class PHHapticPointer : public PHSolid{
	SPR_OBJECTDEF(PHHapticPointer);
protected:
	int solidID;
	float localRange;
public:
	std::vector<int> neighborSolidIDs;
	HIBaseIf* humanInterface;
	PHHapticPointer(){ localRange = 1.0; }
	void	SetID(int id){ solidID = id; }
	int		GetID(){ return solidID; }
	void	SetLocalRange(float r){ localRange = r; } 
	float	GetLocalRange(){ return localRange; }
};
class PHHapticPointers : public std::vector< UTRef< PHHapticPointer > >{};

//----------------------------------------------------------------------------
// PHSolidForHaptic
class PHSolidForHaptic : public UTRefCount{  
	//SPR_OBJECTDEF(PHSolidForHaptic);
public:
	PHSolid* solid;
	bool bPointer;
	int NLocalFirst;
	int NLocal;
	int doSim;
	SpatialVector b;				///< 予測シミュレーションで求めたモビリティ（重力等の定数項）
	SpatialVector curb;				///< 通常シミュレーションででた定数項
	SpatialVector lastb;			///< 前回の予測シミュレーションで求めた定数項
	PHSolidForHaptic(){
		solid = NULL;		bPointer = false;
		NLocalFirst = 0;	NLocal = 0;	doSim = 0;
	}
};
class PHSolidsForHaptic : public std::vector< UTRef< PHSolidForHaptic > >{};


//----------------------------------------------------------------------------
// PHShapePairForHaptic
class PHSolidPairForHaptic;
class PHShapePairForHaptic : public CDShapePair{
public:	
	//PHSolidPairForHaptic* solidPair;

	Posed lastShapePoseW[2];
	Vec3d lastClosestPoint[2];	///< 前回の近傍物体の接触点(ローカル座標)
	Vec3d lastNormal;			///< 前回の近傍物体の提示面の法線
	double sectionDepth;
	double lastSectionDepth;
	std::vector< Vec3d > intersectionVertices; ///< 接触体積の頂点(ローカル座標)
	std::vector< Vec3d > pointerSection;	///< ポインタの接触頂点(ローカル座標)
	std::vector< Vec3d > solidSection;		///< 剛体の接触頂点(ローカル座標)

	Vec3d test_force;				///< 予測シミュレーションで使うテスト力
	Vec3d test_torque;				///< 予測シミュレーションで使うテストトルク
	Vec3d impulse;					///< 物理プロセスが1ステップ終わるまでに力覚ポインタが加えた力積


	PHShapePairForHaptic(){}
	bool Detect(unsigned ct, const Posed& pose0, const Posed& pose1);
	/// 接触判定．近傍点対を常時更新
	bool DetectClosestPoints(unsigned ct, const Posed& pose0, const Posed& pose1);
	int OnDetectClosestPoints(unsigned ct, const Vec3d& center0);

};

//----------------------------------------------------------------------------
// PHSolidPairForHaptic
class PHHapticEngineImp;
class PHSolidPairForHaptic : public PHSolidPair< PHShapePairForHaptic, PHHapticEngine >, public Object{
public:
	typedef PHSolidPair<PHShapePairForHaptic, PHHapticEngine> base_type;
	typedef base_type::shapepair_type shapepair_type;
	typedef base_type::engine_type engine_type;

	int inLocal;	// 0:NONE, 1:in local first, 2:in local
	struct Accelerance{
		Vec3d force;					// LocalDynamicsで使う力
		TMatrixRow<6, 3, double> A;		// LocalDynamicsで使うアクセレランス
		std::vector< Vec3d > f;			// LocalDynamics6Dで使う力
		TMatrixRow<6, 6, double> Minv;  // LocalDynamics6Dで使うアクセレランス
	} accelerance;
	
	/// 交差が検知された後の処理
	virtual bool Detect(engine_type* engine, unsigned int ct, double dt);
	virtual void OnDetect(shapepair_type* sp, engine_type* engine, unsigned ct, double dt);	///< 交差が検知されたときの処理
	/// 
	
	//void Setup(unsigned int ct, double dt);
	//void GenerateForce();
protected:
};


//----------------------------------------------------------------------------
// PHHapticRenderImp
class PHHapticRenderImp : public SceneObject{
	SPR_OBJECTDEF_NOIF(PHHapticRenderImp);
public:
	PHHapticEngine* engine;
	PHHapticRenderImp(){}
	virtual void Step(){};
	virtual void StepHapticLoop(){};

	double GetPhysicsTimeStep();
	double GetHapticTimeStep();
	int NHapticPointers();
	PHHapticPointer** GetHapticPointers();
	int NHapticSolids();
	PHSolidsForHaptic** GetHapticSolids();
	PHSolidPairForHaptic* GetSolidPairForHaptic(int i, int j);

	///< デバック用シミュレーション実行
	virtual void StepSimulation(){};
};

//----------------------------------------------------------------------------
// PHHapticEngine


struct PHHapticEngineDesc{
	bool bHaptic;
	double hdt;
	PHHapticEngineDesc();
};
class PHHapticEngine : public PHHapticEngineDesc, public PHContactDetector< PHShapePairForHaptic, PHSolidPairForHaptic, PHHapticEngine >{
public:
	SPR_OBJECTDEF1(PHHapticEngine, PHEngine);
	ACCESS_DESC(PHHapticEngine);
	PHHapticRenderImp* renderImp;
	std::vector< UTRef<PHHapticRenderImp> > renderImps;
	PHHapticPointers hapticPointers;
	PHSolidsForHaptic hapticSolids;
	typedef std::vector< HIBaseIf* > HIBaseIfs;
	HIBaseIfs humanInterfaces;

	struct Edge{ Vec3f min, max; };
	std::vector< Edge > edges;

	enum RenderMode{
		NONE,
		IMPULSE,
	} renderMode;

	PHHapticEngine();

	///< 力覚提示計算のON/OFF
	void EnableHaptic(bool b){ bHaptic = b; }
	///< レンダリングモードの選択
	void SetRenderMode(RenderMode r);
	///< シミュレーションループの更新（PHScene::Integrate()からコール）
	virtual void Step(){ if(bHaptic) renderImp->Step(); }
	///< 力覚ループの更新	
	virtual void StepHapticLoop(){ if(bHaptic) renderImp->StepHapticLoop(); }
	///< 力覚ループの刻み
	void SetHapticTimeStep(double dt = 0.001){ hdt = dt; }

	///< 力覚ポインタの状態の更新
	virtual void UpdateHapticPointer();
	///< 力覚レンダリング用の衝突判定開始
	virtual void StartDetection();
	///< BBoxの向きを更新
	void UpdateEdgeList();
	///< ある剛体の近傍の剛体をAABBでみつける（rangeはBBoxをさらにrange分だけ広げる
	void Detect(PHHapticPointer* q);
	int GetPriority() const {return SGBP_HAPTICENGINE;}
	///< 剛体の追加
	bool AddChildObject(ObjectIf* o);
	///< 剛体の削除
	bool DelChildObject(ObjectIf* o);

	///< デバック用シミュレーション実行
	///（PHScene::Stepの変わりに呼ぶ）
	virtual void StepSimulation(){ renderImp->StepSimulation(); }



};

}	//	namespace Spr
#endif

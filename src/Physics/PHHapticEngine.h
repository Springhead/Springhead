/*
 *  Copyright (c) 2003-2011, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHHAPTICENGINE_H
#define PHHAPTICENGINE_H

#include <Physics/PHContactDetector.h>

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
	PHSolidPairForHaptic* solidPair;
	Posed lastShapePoseW[2];
	Vec3d lastClosestPoint[2];	///< 前回の近傍物体の接触点(ローカル座標)
	Vec3d lastNormal;			///< 前回の近傍物体の提示面の法線
	double sectionDepth;
	double lastSectionDepth;
	std::vector< Vec3d > intersectionVertices; ///< 接触体積の頂点(ローカル座標)
	std::vector< Vec3d > pointerSection;	///< ポインタの接触頂点(ローカル座標)
	std::vector< Vec3d > solidSection;		///< 剛体の接触頂点(ローカル座標)

	PHShapePairForHaptic(){}
	bool Detect(unsigned ct, const Posed& pose0, const Posed& pose1);
	/// 接触判定．近傍点対を常時更新
	bool DetectClosestPoints(unsigned ct, const Posed& pose0, const Posed& pose1);
	int OnDetectClosestPoints(unsigned ct, const Vec3d& center0);

};

//----------------------------------------------------------------------------
// PHSolidPairForHaptic
class PHHapticEngineImp;
class PHSolidPairForHaptic : public PHSolidPair< PHShapePairForHaptic, PHHapticEngineImp >, public Object{
public:
	typedef PHSolidPair<PHShapePairForHaptic, PHHapticEngineImp> base_type;
	typedef base_type::shapepair_type shapepair_type;
	typedef base_type::engine_type engine_type;

	int inLocal;	// 0:NONE, 1:in local first, 2:in local
	
	/// 交差が検知された後の処理
	virtual bool Detect(engine_type* engine, unsigned int ct, double dt);
	virtual void OnDetect(shapepair_type* sp, engine_type* engine, unsigned ct, double dt);	///< 交差が検知されたときの処理
	/// 
	
	//void Setup(unsigned int ct, double dt);
	//void GenerateForce();
protected:
};

//----------------------------------------------------------------------------
// PHHapticEngineImp
class PHHapticEngineImp : public PHContactDetector< PHShapePairForHaptic, PHSolidPairForHaptic, PHHapticEngineImp >{
public:

	PHHapticPointers hapticPointers;
	PHSolidsForHaptic hapticSolids;
	struct Edge{ Vec3f min, max; };
	std::vector< Edge > edges;

	///< シミュレーションループの更新（PHScene::Integrate()からコール）
	virtual void Step();
	///< 力覚ポインタの状態の更新
	virtual void UpdateHapticPointer();
	///< 力覚レンダリング用の衝突判定開始
	virtual void StartDetection();
	///< BBoxの向きを更新
	virtual void UpdateEdgeList();
	///< ある剛体の近傍の剛体をAABBでみつける（rangeはBBoxをさらにrange分だけ広げる
	virtual void Detect(PHHapticPointer* q);
	int GetPriority() const {return SGBP_HAPTICENGINE;}
	///< 剛体の追加
	virtual bool AddChildObject(ObjectIf* o);
	///< 剛体の削除
	virtual bool DelChildObject(ObjectIf* o);


	/// HapticLoopの更新
	virtual void StepHapticLoop(){};
};

//----------------------------------------------------------------------------
// PHHapticEngine
class PHHapticRenderImp : public UTRefCount{
public:
	PHHapticEngineImp* engineImp;
	PHHapticRenderImp(){}
	virtual void Step(){};
	virtual void StepHapticLoop(){};
};

//----------------------------------------------------------------------------
// PHHapticEngine
class PHHapticEngine : public PHContactDetector< PHShapePairForHaptic, PHSolidPairForHaptic, PHHapticEngine >{
	SPR_OBJECTDEF1(PHHapticEngine, PHEngine);
public:
	bool bHaptic;
	PHHapticEngineImp* engineImp;
	PHHapticRenderImp* renderImp;
	enum RenderMode{
		NONE,
		IMPULSE,
	} renderMode;

	PHHapticEngine(){
		bHaptic = false;
		engineImp = DBG_NEW PHHapticEngineImp();
		renderImp = DBG_NEW PHHapticRenderImp();
	}
	virtual void EnableHaptic(bool b){ bHaptic = b; }
	virtual void SetScene(SceneIf* s){ 
		PHEngine::SetScene(s);
		engineImp->SetScene(s);
	}
	void SetRenderMode(RenderMode r);
	virtual void Step(){ if(bHaptic) renderImp->Step(); }
	virtual bool AddChildObject(ObjectIf* o){ return engineImp->AddChildObject(o); }
	virtual bool DelChildObject(ObjectIf* o){ return engineImp->DelChildObject(o); }
	virtual void UpdateShapePairs(PHSolid* solid){ engineImp->UpdateShapePairs(solid); }

	virtual void StepHapticLoop(){ if(bHaptic) renderImp->StepHapticLoop(); }

};

}	//	namespace Spr
#endif

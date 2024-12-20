/*
 *  Copyright (c) 2003-2011, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PH_HAPTICENGINE_H
#define PH_HAPTICENGINE_H

#include <Physics/PHContactDetector.h>
#include <Physics/PHHapticPointer.h>
#include <Physics/PHHapticStepBase.h>

using namespace PTM;
namespace Spr{;
/// ガウスザイデル法を使いAx+b>0を解く
template <class AD, class XD, class BD>
void GaussSeidel(MatrixImp<AD>& a, VectorImp<XD>& x, const VectorImp<BD>& b) {
	int nIter = 15;					// 反復回数の上限
	double error = 0.0;
	double errorRange = 10e-8;		// 許容誤差
	int n = (int)a.height();		// 連立方程式の数(行列aの行数)
	std::vector< double > lastx;
	for (int i = 0; i < n; i++) {
		lastx.push_back(x[i]);
		x[i] = 0;
	}

	for (int k = 0; k < nIter; k++) {
		for (int i = 0; i < n; i++) {
			double term1 = 0.0;
			double term2 = 0.0;
			for (int j = 0; j < i; j++) {
				term1 += a[i][j] * x[j];
			}
			for (int j = i + 1; j < n; j++) {
				term2 += a[i][j] * lastx[j];
			}
			// xの更新(繰り返し計算の式を使用)
			x[i] = (-b[i] - term1 - term2) / a[i][i];
			if (x[i] < 0) x[i] = 0.0;
		}

		// (lastx - x)の2乗の総和と誤差範囲を比較
		error = 0.0;
		for (int i = 0; i < n; i++) {
			error += pow(x[i] - lastx[i], 2);
			//DSTR << "iterete" << i << "," << x[i] << std::endl;
		}
		if (error < errorRange) {
			//DSTR << "Finish the iteration in admissible error. " << std::endl;
			//DSTR << k << std::endl;
			return;
		}

		// 繰り返し計算のために更新後のxをlastxに保存
		for (int i = 0; i < n; i++) lastx[i] = x[i];
	}
	//nIterで計算が終わらなかったので打ち切り
	//static int iterError = 0;
	//iterError += 1;
	//DSTR << iterError << "Could not converge in iteration steps. Error = " << error << std::endl;
	//CSVOUT << error << std::endl;
}

/// ガウスザイデル法を使いAx+b>0を解く
template <class AD, class XD, class BD>
void GaussSeidelMinMax(VectorImp<XD>& x, const VectorImp<XD>& xmin, const VectorImp<XD>& xmax, const MatrixImp<AD>& a, const VectorImp<BD>& b) {
	int nIter = 15;					// 反復回数の上限
	double error = 0.0;
	double errorRange = 10e-8;		// 許容誤差
	int n = (int)a.height();		// 連立方程式の数(行列aの行数)
	std::vector< double > lastx;
	for (int i = 0; i < n; i++) {
		lastx.push_back(x[i]);
		x[i] = 0;
	}

	for (int k = 0; k < nIter; k++) {
		for (int i = 0; i < n; i++) {
			double term1 = 0.0;
			double term2 = 0.0;
			for (int j = 0; j < i; j++) {
				term1 += a[i][j] * x[j];
			}
			for (int j = i + 1; j < n; j++) {
				term2 += a[i][j] * lastx[j];
			}
			// xの更新(繰り返し計算の式を使用)
			x[i] = (-b[i] - term1 - term2) / a[i][i];
			if (x[i] < xmin[i]) {
				x[i] = xmin[i];
			}
			if (x[i] > xmax[i]) {
				x[i] = xmax[i];
			}
		}

		// (lastx - x)の2乗の総和と誤差範囲を比較
		error = 0.0;
		for (int i = 0; i < n; i++) {
			error += pow(x[i] - lastx[i], 2);
			//DSTR << "iterete" << i << "," << x[i] << std::endl;
		}
		if (error < errorRange) {
			//DSTR << "Finish the iteration in admissible error. " << std::endl;
			//DSTR << k << std::endl;
			return;
		}

		// 繰り返し計算のために更新後のxをlastxに保存
		for (int i = 0; i < n; i++) lastx[i] = x[i];
	}
	//nIterで計算が終わらなかったので打ち切り
	//static int iterError = 0;
	//iterError += 1;
	//DSTR << iterError << "Could not converge in iteration steps. Error = " << error << std::endl;
	//CSVOUT << error << std::endl;
	return;
}

//----------------------------------------------------------------------------
/** PHHapticEngine,	This engine is initially disabled. Enable() muse be called prior to use.
	Time stepping of haptic engine is controlled by PHHapticStepXXX
	In multi thread versions (other than PHHapticStepSingle), 
	PHHapticStepXXX has copy of haptic pointers and solid pairs. 
	They can be got by GetHapticPointerInHaptc() and GetSolidPairInHaptic()
*/
class PHHapticEngine : public PHHapticEngineDesc, public PHContactDetector{
public:
	SPR_OBJECTDEF1(PHHapticEngine, PHEngine);
	ACCESS_DESC(PHHapticEngine);
	UTRef< PHHapticStepBase >               hapticStep;			///<	Manager timer stepping for phyics and haptic rendering.
	PHHapticPointers                        hapticPointers;		///<	Haptic pointers correspond to haptic interface.
	PHSolidsForHaptic                       hapticSolids;		///<	Partial copy of solids in PHScene for haptic rendering

	///描画アクセスで落ちる場合があるかもで追加 2012.12.11 susa
	PHSolidPairs solidPairsTemp;	///< hapticsの情報をグラフィクスで表示するためのキャッシュ

	struct Edge{ Vec3f min, max; };
	std::vector< Edge > edges;
	// PHSceneからStep()を2回呼ぶためのクラス
	class PHHapticEngineCallStep2 : public PHEngine {
	public:
		UTRef< PHHapticEngine > engine;
		int GetPriority() const { return SGBP_HAPTICENGINE2; }
		virtual void Step() { engine->Step2(); }
	};

protected:
	HapticStepMode hapticStepMode;
public:
	bool bPhysicStep;
	PHHapticEngine();
	//-------------------------------------------------------------------
	// APIの実装
	///<	Set engine mode: Single, Multi or Local Dynamics.
	void SetHapticStepMode(HapticStepMode mode);
	///<	Time stepping mode for haptic rendering and physics.
	PHHapticEngineDesc::HapticStepMode GetHapticStepMode();
	///<	Time stepping
	PHHapticStepBaseIf* GetHapticStep() { return hapticStep->Cast(); }
	/// 力覚ポインタの数を返す
	int NPointers() { return (int)hapticPointers.size(); }
	/// hapticSolidsの数を返す
	int NSolids() { return (int)hapticSolids.size(); }
	/// Get haptic pointer in haptic engine (physics thread)
	PHHapticPointerIf* GetPointer(int i){ return hapticPointers[i]->Cast(); }
	///	Get solid pair in haptic engine (physics thread);
	PHSolidPairForHapticIf* GetSolidPair(int i, int j) { return (PHSolidPairForHapticIf*)&*solidPairs.item(i, j); }

	//--------------------------------------
	//	Functions to get objects in haptic thead = PHHapticStepXXX.
	/// 力覚ポインタの数を返す。
	int NPointersInHaptic();
	/// returns haptic pointer in haptic thread.
	PHHapticPointerIf* GetPointerInHaptic(int i);
	/// hapticSolidsの数を返す。
	int NSolidsInHaptic();
	/// returns solid pair in haptic thead.
	PHSolidPairForHapticIf* GetSolidPairInHaptic(int i, int j);
	/// state保存のために確保した領域を開放する
	void ReleaseState();
	///	cache of solid pairs in haptic thead. Used for displaying.
	PHSolidPairForHapticIf* GetSolidPairTemp(int i, int j) { return (PHSolidPairForHapticIf*)&*solidPairsTemp.item(i, j); }

	/// Implementaion for base class (PHContactDetector)
	PHSolidPair* CreateSolidPair() { return DBG_NEW PHSolidPairForHaptic(); }

	// PHHapticEngineの実装
	///< Update simulation loop (called from PHScene::Integrate()) シミュレーションループの更新（PHScene::Integrate()からコール）
	virtual void Step();
	virtual void Step2();
	///< Time stepping for haptic rendering. 力覚ループの更新	
	virtual void StepHapticLoop();
	///< Syncronize physics and haptics
	virtual void StepHapticSync();

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
	void UpdateShapePairs(PHBody* body);

	///<	接触判定の有効化・無効化
	void EnableContact(PHSolidIf* lhs, PHSolidIf* rhs, bool bEnable);
	void EnableContact(PHSolidIf** group, size_t length, bool bEnable);
	void EnableContact(PHSolidIf* solid, bool bEnable);
	void EnableContact(bool bEnable);

	///< デバック用シミュレーション実行
	///（PHScene::Stepの変わりに呼ぶ）
	virtual void StepPhysicsSimulation();
	/// シミュレーションを実行する直前に実行されるコールバックを登録する
	virtual bool SetCallbackBeforeStep(PHHapticEngineIf::Callback f, void* arg);
	/// シミュレーションを実行した直後に実行されるコールバックを登録する
	virtual bool SetCallbackAfterStep(PHHapticEngineIf::Callback f, void* arg);

protected:
	///<	TO avoid to get force from other object to the haptic pointer, contact mode for PHScene must be set NONE.
	virtual void DisablePointerContactDetectionInPhysics();


public:
	// Implementation for haptic rendering. The definisions are in PHHapticEngineRender.cpp
	///	start point of haptic rendering
	virtual void HapticRendering(PHHapticStepBaseIf* hs);
	///	Compute all constraints.
	void CompIntermediateRepresentationForDynamicProxy(PHHapticStepBase* hs, PHIrs& irsNormal, PHIrs& irsFric, PHHapticPointer* pointer);

	///	Genreate constraints for surface normal
	bool CompIntermediateRepresentationShapeLevel(PHSolid* solid0, PHHapticPointer* pointer,
		PHSolidPairForHaptic* so, PHShapePairForHaptic* sh, Posed curShapePoseW[2], double t, bool bInterpolatePose, bool bPoints);
	///	Generate constrants for static friction
	bool CompFrictionIntermediateRepresentation(PHHapticStepBase* hs, PHHapticPointer* pointer, PHSolidPairForHaptic* sp, PHShapePairForHaptic* sh);

	///	PENALTY based haptic rendering
	void PenaltyBasedRendering(PHHapticStepBase* hs, PHHapticPointer* pointer);
	///	CONSTRAINT and DYNAMIC_PROXY based Haptic Rendering
	void DynamicProxyRendering(PHHapticStepBase* hs, PHHapticPointer* pointer);

	///	Add vibration to collision and state transition of friction (static to dynamic) events
	void VibrationRendering(PHHapticStepBase* hs, PHHapticPointer* pointer);
	///	Compute proxy's position which satisfy constrants of all intermediate representations.
	void SolveProxyPose(Vec3d& dr, Vec3d& dtheta, Vec3d& allDepth, PHHapticPointer* pointer, const PHIrs& irs);
};

}	//	namespace Spr
#endif

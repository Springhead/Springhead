/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// <memo>
// humanに関する変数や複数bodyについて書いてある部分もありますが、基本的に未実装
// なので、使いたい人が適宜能力を拡張してください(by Toki 2008.01.05)
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

//////////////////////////////////////////////////////
#ifndef CRTRYSTANDINGUPCONTROLLER_H
#define CRTRYSTANDINGUPCONTROLLER_H
//////////////////////////////////////////////////////

#include <Springhead.h>
#include <Foundation/Object.h>

#include "IfStubCreature.h"
#include "CRController.h"
#include "CRFLAnimalGene.h"
#include "CRFLAnimalQL.h"

//////////////////////////////////////////////////////

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 倒れたら立とうと努力するコントローラ
*/
class CRTryStandingUpController : public CRController,
								  public CRTryStandingUpControllerIfInit, 
								  public CRTryStandingUpControllerDesc {


private:
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// 変数宣言

	///////////////////////////////////////////////////////////////////
	//																 //
	// <<<CRControllerから継承されてこの中ですでに使える変数>>>		 //
	//																 //
	// CRCreatureIf*	creature;   //< 制御対象のクリーチャー群	 //
	// PHSceneIf*		phScene;    //< 所属するシーン				 //
	// bool				isEnabled;  //< 有効かどうか				 //
	//																 //
	///////////////////////////////////////////////////////////////////

	// このクラスで使える変数
	std::vector<PHSolidIf*>							foot;					//< 足を構成する剛体を格納しておく配列
	std::vector<CRBodyIf*>							body;					//< 制御対象のボディを扱う配列
	UTRef<CRFLAnimalGene>							animalGeneIf;			//< 遺伝子操作をするためのインタフェース
	UTRef<CRFLAnimalQL>								animalQLIf;				//< QLearningをするためのインタフェース
	std::vector< std::vector<CRFLAnimalGeneData> >	animalGenes;			//< ある時の遺伝子std::vector<CRFLAnimalGeneData>が集まった遺伝子系列std::vector<********>

	unsigned long			totalStep;				//< シミュレーション開始時からのステップ数
	Vec3d					centerOfMass;			//< ボディ全体の重心
	
	std::vector<Vec3d>		footPositions;			//< 足の位置を格納した配列
	// human foot positions	
	Vec3d					rightFootPos;			//< 右足の位置
	Vec3d					leftFootPos;			//< 左足の位置	
	// animal foot positions	
	Vec3d					rightFrontFootPos;		//< 右前足の位置
	Vec3d					rightRearFootPos;		//< 右後足の位置
	Vec3d					leftFrontFootPos;		//< 左前足の位置
	Vec3d					leftRearFootPos;		//< 左後足の位置

	std::vector<Vec3d>		footForces;				//< 足にかかる力を格納した配列
	// human foot forces
	Vec3d					rightFootForce;			//< 右足の力
	Vec3d					leftFootForce;			//< 左足の力	
	// animal foot forces
	Vec3d					rightFrontFootForce;	//< 右前足の力
	Vec3d					rightRearFootForce;		//< 右後足の力
	Vec3d					leftFrontFootForce;		//< 左前足の力
	Vec3d					leftRearFootForce;		//< 左後足の力
	
	//大域カウンター
	int						qLearningStep;			//< GA+QLeaningの1試行内でのQLearningのステップ数

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// 関数宣言

	std::vector<PHSolidIf*>		SetFootSolid(CRBodyIf* body);			//< 足の剛体を指定する。
	Vec3d						GetFootPos(PHSolidIf* footSolid);		//< 足の座標を返す
	Vec3d						CalcFootForce(PHSolidIf* footSolid);	//< 足にかかっている力を計算する
	Vec3d						CalcFootTorque(PHSolidIf* footSolid);	//< 足にかかっているトルクを計算する
	void						UpdateBodyState();						//< クリーチャー達のボディの情報（重心, 足の位置, 足にかかる力）を更新する
	void						MakeLatestGene();						//< 一番新しい遺伝子を一つ作成する
	void						CalcQL();								//< Q学習する
	void						CalcGA();								//< GAを計算する

public:
	//----------------------------------------------------------
	// DCASTをできるようにするための仕組みが入っている。
	// これを書かないとLNK2019 Errorになる。
	OBJECTDEF(CRTryStandingUpController, CRController);

	//----------------------------------------------------------
	// DESCの情報をこちら側にロードするための仕組みが入っている。
	// これを書かないとLNK2019 Errorになる。
	// 定義するクラスのコンストラクタと合わせて書く必要がある。
	// また、引数のある特殊なコンストラクタも必要である。
	ACCESS_DESC(CRTryStandingUpController);

	//----------------------------------------------------------
	// コンストラクタ
	CRTryStandingUpController(){
	}
	CRTryStandingUpController(const CRTryStandingUpControllerDesc&		desc, CRCreatureIf*		c = NULL) 
		: CRTryStandingUpControllerDesc(desc) 
		, CRController((const CRControllerDesc&)	desc, c)
	{
		Init();
	}
	
	//----------------------------------------------------------
	//関数のオーバーライド等
	////////////////////////////////////////////////////////////////////////
	//																	  //
	// <<CRControllerで実装されている関数>>								  //
	//																	  //
	// virtual void		SetEnable(bool enable); //< 有効/無効を切り替える //
	// virtual bool		IsEnabled();			//< 有効/無効の状態を返す //
	//																	  //
	////////////////////////////////////////////////////////////////////////

	//インタフェースの実装
	virtual void		Init();							//< 初期化を行う
	virtual void		Step();							//< 制御のシミュレーションをする

};

}
//@}

#endif//CRTRYSTANDINGUPCONTROLLER_H
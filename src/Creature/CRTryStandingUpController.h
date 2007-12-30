/*
 *  Copyright (c) 2003-2007, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
//////////////////////////////////////////////////////

#ifndef CRTRYSTANDINGUPCONTROLLER_H
#define CRTRYSTANDINGUPCONTROLLER_H

//////////////////////////////////////////////////////

#include <Springhead.h>
#include <Foundation/Object.h>

#include "IfStubCreature.h"
#include "CRController.h"
#include "CRFLAnimalGene.h"

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
	// <<<CRControllerから継承されてこの中ですでに使える変数>>>>	 //
	//																 //
	// CRCreatureIf*	creature;   //< 制御対象のクリーチャー群	 //
	// PHSceneIf*		phScene;    //< 所属するシーン				 //
	// bool				isEnabled;  //< 有効かどうか				 //
	//																 //
	///////////////////////////////////////////////////////////////////

	// このクラスとその派生クラス（今はまだない）で使える変数
	std::vector<PHSolidIf*>							foot;					//< 足を構成する剛体を格納しておく配列
	std::vector<CRBodyIf*>							body;					//< 制御対象のボディを扱う配列
	CRFLAnimalGene*									animalGeneIf;			//< 遺伝子操作をするためのインタフェース
	std::vector< std::vector<CRFLAnimalGeneData> >	animalGenes;			//< 動物の遺伝子を扱う配列

	unsigned long		totalStep;				//< シミュレーション開始時からのステップ数
	Vec3d				centerOfMass;			//< ボディ全体の重心
	
	// human foot positions	
	Vec3d				rightFootPos;			//< 右足の位置
	Vec3d				leftFootPos;			//< 左足の位置
	
	// animal foot positions	
	Vec3d				rightFrontFootPos;		//< 右前足の位置
	Vec3d				rightRearFootPos;		//< 右後足の位置
	Vec3d				leftFrontFootPos;		//< 左前足の位置
	Vec3d				leftRearFootPos;		//< 左後足の位置

	// human foot forces
	Vec3d				rightFootForce;			//< 右足の力
	Vec3d				leftFootForce;			//< 左足の力
	
	// animal foot forces
	Vec3d				rightFrontFootForce;	//< 右前足の力
	Vec3d				rightRearFootForce;		//< 右後足の力
	Vec3d				leftFrontFootForce;		//< 左前足の力
	Vec3d				leftRearFootForce;		//< 左後足の力
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// 関数宣言

	std::vector<PHSolidIf*>		SetFootSolid(CRBodyIf* body);								//< 足の剛体を指定する。
	Vec3d						GetFootPos(PHSolidIf* footSolid);							//< 足の座標を返す
	Vec3d						CalcFootForce(PHSolidIf* footSolid);						//< 足にかかっている力を計算する
	void						TransitionPoseModel(std::vector<CRFLAnimalGeneData> gene);	//< 遺伝子に格納されている目標角度情報からボディを実際に動かす
	void						UpdateBodyState();											//< クリーチャー達のボディの情報（重心, 足の位置, 足にかかる力）を更新する

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
	
	virtual void		Init();							//< 初期化を行う
	virtual void		Step();							//< 制御のシミュレーションをする
	void				QLearning();					//< Q学習する

};

}
//@}

#endif//CRTRYSTANDINGUPCONTROLLER_H
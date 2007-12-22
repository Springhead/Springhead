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

//////////////////////////////////////////////////////

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 倒れたら立とうと努力するコントローラ
*/
class CRTryStandingUpController : public CRController, public CRTryStandingUpControllerIfInit, public CRTryStandingUpControllerDesc {

private:
	///////////////////////////////////////////////////////////////
	//															 //
	// <<<CRControllerから継承されてこの中ですでに使える変数>>>> //
	//															 //
	// CRCreatureIf* creature;   //< 制御対象のクリーチャー		 //
	// PHSceneIf*	 phScene;    //< 所属するシーン				 //
	// bool			 isEnabled;  //< 有効かどうか				 //
	//															 //
	///////////////////////////////////////////////////////////////

	// このクラスとその派生クラス（今はまだない）で使える変数
	unsigned long totalStep;	//< シミュレーション開始時からのステップ数
	Vec3d centerOfMass;			//< 重心
	Vec3d rightFrontFootPos;	//< 右前足の位置
	Vec3d rightRearFootPos;		//< 右後足の位置
	Vec3d leftFrontFootPos;		//< 左前足の位置
	Vec3d leftRearFootPos;		//< 左後足の位置

	/** @brief CreatureのBodyを得る
	*/
	CRBodyIf* GetBody(CRCreatureIf* creature);
	
	/** @brief 足の剛体を指定する。
	*/
	std::vector<PHSolidIf*> SetFootSolid(CRBodyIf* body);

	/** @brief 足の座標を返す
	*/
	Vec3d GetFootPos(PHSolidIf* footSolid);

	/** @brief 遺伝子に格納されている目標角度情報からボディを実際に動かす
	*/
	void TransitionPoseModel(CRBodyIf* crBody);



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
	CRTryStandingUpController(const CRTryStandingUpControllerDesc& desc, CRCreatureIf* c=NULL) 
		: CRTryStandingUpControllerDesc(desc) 
		, CRController((const CRControllerDesc&)desc, c)
	{
	}
	
	//----------------------------------------------------------
	//関数のオーバーライド等
	////////////////////////////////////////////////////////////////////
	//																  //
	// <<CRControllerで実装されている関数>>							  //
	//																  //
	// virtual void SetEnable(bool enable); //< 有効/無効を切り替える //
	// virtual bool IsEnabled();			//< 有効/無効の状態を返す //
	//																  //
	////////////////////////////////////////////////////////////////////
	
	/** @brief 初期化を行う
	*/
	virtual void Init();

	/** @brief 制御のシミュレーションをする
	*/
	virtual void Step();	

};
}
//@}

#endif//CRTRYSTANDINGUPCONTROLLER_H
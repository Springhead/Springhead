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
	unsigned long totalStep;	//< シミュレーション開始時からのステップ数
	Vec3d centerOfMass;			//< 重心
	Vec3d rightFrontFootPos;	//< 右前足の位置
	Vec3d rightRearFootPos;		//< 右後足の位置
	Vec3d leftFrontFootPos;		//< 左前足の位置
	Vec3d leftRearFootPos;		//< 左後足の位置

	CRBodyIf* standUpBody;


	/** @brief ボディ全体の重心の位置座標を出す
	*/
	//Vec3d GetCenterOfMass(CRBodyIf* crBody);

	/** @brief 足の座標を返す
	*/
	Vec3d GetFootPos(PHSolidIf* footSolid);

	/** @brief 遺伝子に格納されている目標角度情報からボディを実際に動かす
	*/
	void TranisitionPoseModel(CRBodyIf* crBody);

public:
	//----------------------------------------------------------
	// DCASTをできるようにするための仕組みが入っている。
	// これを書かないとLNK2019 Errorになる。
	OBJECTDEF(CRTryStandingUpController, CRController);
	//----------------------------------------------------------

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
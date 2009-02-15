/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRCONTROLLER_H
#define CRCONTROLLER_H

#include <Springhead.h>

#include <Foundation/Object.h>

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief コントローラ
*/
class CRController : public SceneObject, public CRControllerDesc {
protected:
	/// 制御対象のクリーチャ
	CRCreatureIf* creature;

	/// 所属するシーン
	PHSceneIf* phScene;

	/// 有効かどうか
	bool isEnabled;

public:
	SPR_OBJECTDEF(CRController);
	ACCESS_DESC(CRController);

	CRController(){}
	CRController(const CRControllerDesc& desc, CRCreatureIf* c=NULL) : CRControllerDesc(desc) {
		if(c!=NULL && c->GetScene()){
			SetScene(c->GetScene());
			phScene = c->GetScene()->Cast();
		}
		creature = c;
	}

	/** @brief 初期化を行う
	*/
	virtual void Init();

	/** @brief 運動制御の処理を実行する
	*/
	virtual void Step();

	/** @breif 有効・無効を切り替える
	*/
	virtual void SetEnable(bool enable);

	/** @brief 有効・無効かを返す
	*/
	virtual bool IsEnabled();
};
}
//@}

#endif//CRCONTROLLER_H

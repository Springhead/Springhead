/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRATTENTIONCONTROLLER_H
#define CRATTENTIONCONTROLLER_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"

#include "CRController.h"

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 注意コントローラ
*/
class CRAttentionController : public CRController, public CRAttentionControllerIfInit, public CRAttentionControllerDesc {
private:
	/// 内部シーン
	CRInternalSceneIf*  internalScene;

	/// 下位のコントローラ：視線コントローラ
	CRGazeControllerIf* gazeCtrl;

public:
	OBJECTDEF(CRAttentionController, CRController);
	ACCESS_DESC(CRAttentionController);

	CRAttentionController(){
	}
	CRAttentionController(const CRAttentionControllerDesc& desc, CRCreatureIf* c=NULL)
		: CRAttentionControllerDesc(desc)
		, CRController((const CRControllerDesc&)desc, c)
	{
	}

	/** @brief 初期化を行う
	*/
	virtual void Init();

	/** @brief 注意制御を実行する
	*/
	virtual void Step();
};
}
//@}

#endif//CRATTENTIONCONTROLLER_H

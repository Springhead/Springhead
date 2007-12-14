/*
 *  Copyright (c) 2003-2007, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRTRYTOSTANDUPCONTROLLER_H
#define CRTRYTOSTANDUPCONTROLLER_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"

#include "CRController.h"

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 倒れたら立とうと努力するコントローラ
*/
class CRTryToStandUpController : public CRController, public CRTryToStandUpControllerIfInit, public CRTryToStandUpControllerDesc {
	
public:
	// DCASTをできるようにするための仕組みが入っている。
	// これを書かないとLNK2019 Errorになる。
	OBJECTDEF(CRTryToStandUpController, CRController);

	// DESCの情報をこちら側にロードするための仕組みが入っている。
	// これを書かないとLNK2019 Errorになる。
	// 定義するクラスのコンストラクタと合わせて書く必要がある。
	// また、引数のある特殊なコンストラクタも必要である。
	ACCESS_DESC(CRTryToStandUpController);


	// コンストラクタ
	CRTryToStandUpController(){
	}
	CRTryToStandUpController(const CRTryToStandUpControllerDesc& desc, CRCreatureIf* c=NULL) 
		: CRTryToStandUpControllerDesc(desc) 
		, CRController((const CRControllerDesc&)desc, c)
	{
	}

};
}
//@}

#endif//CRTRYTOSTANDUPCONTROLLER_H
/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
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

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 注意コントローラ
*/
class CRAttentionController : public SceneObject, public CRAttentionControllerIfInit, public CRAttentionControllerDesc {
private:

public:
	OBJECTDEF(CRAttentionController, SceneObject);
	ACCESS_DESC(CRAttentionController);

	CRAttentionController(){
	}
	CRAttentionController(const CRAttentionControllerDesc& desc, SceneIf* s=NULL) : CRAttentionControllerDesc(desc) {
		if(s){SetScene(s);}
	}

	/** @brief 注意制御を実行する
	*/
	virtual void Step();
};
}
//@}

#endif//CRATTENTIONCONTROLLER_H

/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRAttentionControllerIf_H
#define SPR_CRAttentionControllerIf_H

#include <SprFoundation.h>
#include <SprPhysics.h>

namespace Spr{;

//@{

struct CRAttentionListIf;
struct CRGazeControllerIf;

// ------------------------------------------------------------------------------
/// 注意コントローラのデスクリプタ
struct CRAttentionControllerDesc{
	/// 注意対象のリスト
	CRAttentionListIf*  attentionList;

	/// 視線（視覚的注意）コントローラ
	CRGazeControllerIf* gazeController;

	CRAttentionControllerDesc(){
		attentionList  = NULL;
		gazeController = NULL;
	}
};

/// 注意コントローラのインタフェース
struct CRAttentionControllerIf : SceneObjectIf{
	IF_DEF(CRAttentionController);

	/** @brief 注意の制御を実行する
	*/
	virtual void Step()= 0;
};

//@}

}

#endif//SPR_CRAttentionControllerIf_H

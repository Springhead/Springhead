/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRSensorIf_H
#define SPR_CRSensorIf_H

#include <SprFoundation.h>
#include <SprPhysics.h>

namespace Spr{;

//@{

struct CRAttentionListIf;
struct PHSolidIf;

// ------------------------------------------------------------------------------
/// 視覚センサのデスクリプタ
struct CROpticalSensorDesc{
	/// 入力対象の注意リスト
	CRAttentionListIf* attentionList;

	/// 目のSolid
	PHSolidIf *soLEye, *soREye;

	CROpticalSensorDesc(){
		attentionList = NULL;
	}
};

/// 視覚センサのインタフェース
struct CROpticalSensorIf : SceneObjectIf{
	IF_DEF(CROpticalSensor);

	/** @brief 感覚入力の処理を実行する
	*/
	virtual void Step()= 0;
};

//@}

}

#endif//SPR_CRReachingMovementIf_H

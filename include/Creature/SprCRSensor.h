/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRSensorIf_H
#define SPR_CRSensorIf_H

#include <SprFoundation.h>

namespace Spr{;

//@{

struct CRCreatureIf;

struct CRSensorIf;
struct CROpticalSensorIf;

// ------------------------------------------------------------------------------
/// センサのインタフェース
struct CRSensorIf : SceneObjectIf{
	IF_DEF(CRSensor);

	/** @brief 初期化を行う
	*/
	virtual void Init()= 0;

	/** @brief 感覚入力の処理を実行する
	*/
	virtual void Step()= 0;
};

/// センサのデスクリプタ
struct CRSensorDesc{
	DESC_DEF_FOR_OBJECT(CRSensor);

	CRSensorDesc(){
	}
};

// ------------------------------------------------------------------------------
/// 視覚センサのインタフェース
struct CROpticalSensorIf : CRSensorIf{
	IF_DEF(CROpticalSensor);

	/** @brief 視野内外判定を行う
	*/
	virtual bool IsVisible(PHSolidIf* solid)= 0;

	/** @brief 中心視野内外判定を行う
	*/
	virtual bool IsInCenter(PHSolidIf* solid)= 0;

	/** @brief 視野内外判定を行う
	*/
	virtual bool IsVisible(Vec3f pos)= 0;

	/** @brief 中心視野内外判定を行う
	*/
	virtual bool IsInCenter(Vec3f pos)= 0;

	/** @brief 自分自身の剛体かどうかを判定する
	*/
	virtual bool IsSelfSolid(PHSolidIf* solid)= 0;
};

/// 視覚センサのデスクリプタ
struct CROpticalSensorDesc : CRSensorDesc{
	DESC_DEF_FOR_OBJECT(CROpticalSensor);

	CROpticalSensorDesc(){
	}
};

//@}

}

#endif//SPR_CRSensorIf_H

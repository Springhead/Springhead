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
	SPR_IFDEF(CRSensor);

	/** @brief 初期化を行う
	*/
	void Init();

	/** @brief 感覚入力の処理を実行する
	*/
	void Step();
};

/// センサのデスクリプタ
struct CRSensorDesc{
	SPR_DESCDEF(CRSensor);

	CRSensorDesc(){
	}
};

// ------------------------------------------------------------------------------
/// 視覚センサのインタフェース
struct CROpticalSensorIf : CRSensorIf{
	SPR_IFDEF(CROpticalSensor);

	/** @brief 視野内外判定を行う
	*/
	bool IsVisible(PHSolidIf* solid);

	/** @brief 中心視野内外判定を行う
	*/
	bool IsInCenter(PHSolidIf* solid);

	/** @brief 視野内外判定を行う
	*/
	bool IsVisible(Vec3f pos);

	/** @brief 中心視野内外判定を行う
	*/
	bool IsInCenter(Vec3f pos);

	/** @brief 自分自身の剛体かどうかを判定する
	*/
	bool IsSelfSolid(PHSolidIf* solid);
};

/// 視覚センサのデスクリプタ
struct CROpticalSensorDesc : CRSensorDesc{
	SPR_DESCDEF(CROpticalSensor);

	CROpticalSensorDesc(){
	}
};

//@}

}

#endif//SPR_CRSensorIf_H

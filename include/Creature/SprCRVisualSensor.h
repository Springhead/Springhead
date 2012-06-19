/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CR_VISUALSENSOR_IF_H
#define SPR_CR_VISUALSENSOR_IF_H

#include <Creature/SprCREngine.h>

namespace Spr{;

struct PHSolidIf;

struct CRVisualSensorIf : CREngineIf{
	SPR_IFDEF(CRVisualSensor);

	/** @brief 視野内外判定を行う
	*/
	bool IsVisible(PHSolidIf* solid);
	bool IsVisible(Vec3f pos);

	/** @brief 中心視野内外判定を行う
	*/
	bool IsInCenter(PHSolidIf* solid);
	bool IsInCenter(Vec3f pos);

	/** @brief 自分自身の剛体かどうかを判定する
	*/
	bool IsSelfSolid(PHSolidIf* solid);
};

/// 視覚センサのデスクリプタ
struct CRVisualSensorDesc : CREngineDesc{
	SPR_DESCDEF(CRVisualSensor);

	CRVisualSensorDesc(){
	}
};


}

#endif // SPR_CR_VISUALSENSOR_IF_H

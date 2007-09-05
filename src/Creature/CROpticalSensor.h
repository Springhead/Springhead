/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CROPTICALSENSOR_H
#define CROPTICALSENSOR_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"

#include <vector>
#include <algorithm>

#include "CRSensor.h"

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 視覚センサ
*/
class CROpticalSensor : public CRSensor, public CROpticalSensorIfInit, public CROpticalSensorDesc {
private:
	/// 利用する剛体
	PHSolidIf *soLEye, *soREye;

	/// 書き込み先の内部シーン
	CRInternalSceneIf* internalScene;

	/** @brief 視野内かどうか
	*/
	bool IsVisible(PHSolidIf* solid);

public:
	OBJECTDEF(CROpticalSensor, CRSensor);
	ACCESS_DESC(CROpticalSensor);

	CROpticalSensor(){}
	CROpticalSensor(const CROpticalSensorDesc& desc, CRCreatureIf* c=NULL) 
		: CROpticalSensorDesc(desc) 
		, CRSensor((const CRSensorDesc&)desc, c)
	{
	}

	/** @brief 初期化処理
	*/
	virtual void Init();

	/** @brief １ステップ分の処理
	*/
	virtual void Step();

};
}
//@}

#endif//CROPTICALSENSOR_H

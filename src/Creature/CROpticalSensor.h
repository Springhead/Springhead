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

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 注意物体リスト
*/
class CROpticalSensor : public SceneObject, public CROpticalSensorIfInit, public CROpticalSensorDesc {
private:
	PHSceneIf* phScene;

	/** @brief 視野内かどうか
	*/
	bool IsVisible(PHSolidIf* solid);

	/** @brief 初期化処理
	*/
	void Init();

public:
	OBJECTDEF(CROpticalSensor, SceneObject);
	ACCESS_DESC(CROpticalSensor);

	CROpticalSensor(){ Init(); }
	CROpticalSensor(const CROpticalSensorDesc& desc, SceneIf* s=NULL) : CROpticalSensorDesc(desc) {
		if(s){SetScene(s);}
		Init();
	}

	/** @brief 感覚入力の処理を実行する
	*/
	virtual void Step();
};
}
//@}

#endif//CROPTICALSENSOR_H

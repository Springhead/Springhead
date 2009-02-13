/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRTOUCHSENSOR_H
#define CRTOUCHSENSOR_H

#include <Springhead.h>

#include <Foundation/Object.h>

#include <vector>

#include "CRSensor.h"

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 感覚処理
*/
class CRTouchSensor : public CRSensor, public CRTouchSensorDesc {
	std::vector<CRContactInfo> contactList;

public:
	SPR_OBJECTDEF(CRTouchSensor);
	ACCESS_DESC(CRTouchSensor);

	CRTouchSensor(){}
	CRTouchSensor(const CRTouchSensorDesc& desc, CRCreatureIf* c=NULL) 
		: CRTouchSensorDesc(desc) 
		, CRSensor((const CRSensorDesc&)desc, c)
	{
	}

	/** @brief 初期化を行う
	*/
	virtual void Init();

	/** @brief 感覚入力の処理を実行する
	*/
	virtual void Step();

	/** @brief 現在の接触の個数を返す
	*/
	virtual int NContacts(){ return contactList.size(); }

	/** @brief 接触情報を返す
	*/
	virtual CRContactInfo GetContact(int n){ return contactList[n]; }
};
}
//@}

#endif//CRSENSOR_H

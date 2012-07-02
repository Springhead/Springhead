/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRTOUCHSENSOR_H
#define CRTOUCHSENSOR_H

#include <Creature/CREngine.h>
#include <Creature/SprCRTouchSensor.h>

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 感覚処理
*/
class CRTouchSensor : public CREngine, public CRTouchSensorDesc {
	std::vector<CRContactInfo> contactList;

public:
	SPR_OBJECTDEF(CRTouchSensor);
	ACCESS_DESC(CRTouchSensor);

	CRTouchSensor(){}
	CRTouchSensor(const CRTouchSensorDesc& desc) 
		: CRTouchSensorDesc(desc) 
	{
	}

	/** @brief 実行順序を決めるプライオリティ値．小さいほど早い
	*/
	virtual int GetPriority() const { return CREngineDesc::CREP_SENSOR; }

	/** @brief 感覚入力の処理を実行する
	*/
	virtual void Step();

	/** @brief 現在の接触の個数を返す
	*/
	virtual int NContacts(){ return (int)contactList.size(); }

	/** @brief 接触情報を返す
	*/
	virtual CRContactInfo GetContact(int n){ return contactList[n]; }
};
}
//@}

#endif//CRSENSOR_H

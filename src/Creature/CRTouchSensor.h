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
	struct Contact {
		double		dimension;
		Vec3d		normal;
		double		pressure;
		Vec3d		pos;
		PHSolidIf*	soMe;
		PHSolidIf*	soOther;
	};

	std::vector<Contact> contactList;

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

	/** @brief 接触している剛体（自分の）を返す
	*/
	virtual PHSolidIf* GetContactSolidMe(int n){ return contactList[n].soMe; }

	/** @brief 接触している剛体（相手の）を返す
	*/
	virtual PHSolidIf* GetContactSolidOther(int n){ return contactList[n].soOther; }

	/** @brief 接触圧力の大きさを返す
	*/
	virtual double GetContactPressure(int n){ return contactList[n].pressure; }

	/** @brief 接触地点（グローバル座標）を返す
	*/
	virtual Vec3d GetContactPos(int n){ return contactList[n].pos; }

	/** @brief 接触している剛体（自分の）を返す
	*/
	virtual PHSolidIf* GetContactMySolid(int n){ return contactList[n].soMe; }

	/** @brief 接触している剛体（相手の）を返す
	*/
	virtual PHSolidIf* GetContactOthersSolid(int n){ return contactList[n].soOther; }
};
}
//@}

#endif//CRSENSOR_H

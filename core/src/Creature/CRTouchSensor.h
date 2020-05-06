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
	// 接触情報バッファ
	std::vector<CRContactInfo> contactList[3];

	// 接触情報バッファの用途
	// write : Stepで書込み中
	// read  : NContacts/GetContactでアクセスされる対象
	// keep  : Updateされた時にreadにするためのバッファ
	int write, read, keep, empty;

public:
	SPR_OBJECTDEF(CRTouchSensor);
	ACCESS_DESC(CRTouchSensor);

	CRTouchSensor(){}
	CRTouchSensor(const CRTouchSensorDesc& desc) 
		: CRTouchSensorDesc(desc) 
	{
		write=0; read=1; keep=-1, empty=2;
	}

	/** @brief 実行順序を決めるプライオリティ値．小さいほど早い
	*/
	virtual int GetPriority() const { return CREngineDesc::CREP_SENSOR; }

	/** @brief 感覚入力の処理を実行する
	*/
	virtual void Step();

	/** @brief 現在の接触の個数を返す
	*/
	virtual int NContacts(){ return (int)contactList[read].size(); }

	/** @brief 接触情報を返す
	*/
	virtual CRContactInfo GetContact(int n){ return contactList[read][n]; }

	/** @brief 接触情報をアップデートする
	*/
	void Update() { if (keep >= 0) { empty=read; read=keep; keep=-1; } }
};
}
//@}

#endif//CRSENSOR_H

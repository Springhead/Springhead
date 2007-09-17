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
#include <set>

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

	/// 自己の剛体のセット（自己に属する剛体かどうかを判定するのに使う）
	std::set<PHSolidIf*> selfSolids;

	/// 視野内外判定の基盤となる関数
	bool  IsInside(PHSolidIf* solid, double rangeIn, double rangeOut, double rangeVert);

	/// 方向ベクトルを水平・垂直角度に変換
	Vec2d Vec3ToAngle(Vec3d v);

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

	/** @brief 視野内外判定を行う
	*/
	virtual bool IsVisible(PHSolidIf* solid);

	/** @brief 中心視野内外判定を行う
	*/
	virtual bool IsInCenter(PHSolidIf* solid);
};
}
//@}

#endif//CROPTICALSENSOR_H

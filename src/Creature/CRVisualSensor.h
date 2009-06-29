/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRVISUALSENSOR_H
#define CRVISUALSENSOR_H

#include <Springhead.h>

#include <Foundation/Object.h>

#include <vector>
#include <algorithm>
#include <set>

#include "CREngine.h"

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 視覚センサ
*/
class CRVisualSensor : public CREngine, public CRVisualSensorDesc {
private:
	/// 利用する剛体
	PHSolidIf *soLEye, *soREye;

	/// 書き込み先の内部シーン
	// CRSceneIf* internalScene;

	/// 自己の剛体のセット（自己に属する剛体かどうかを判定するのに使う）
	std::set<PHSolidIf*> selfSolids;

	/// 視野内外判定の基盤となる関数
	bool  IsInside(Vec3f pos, double rangeIn, double rangeOut, double rangeVert);

	/// 方向ベクトルを水平・垂直角度に変換
	Vec2d Vec3ToAngle(Vec3d v);

public:
	SPR_OBJECTDEF(CRVisualSensor);
	ACCESS_DESC(CRVisualSensor);

	CRVisualSensor(){}
	CRVisualSensor(const CRVisualSensorDesc& desc) 
		: CRVisualSensorDesc(desc) 
	{
	}

	/** @brief 実行順序を決めるプライオリティ値．小さいほど早い
	*/
	virtual int GetPriority() const { return CREngineDesc::CREP_SENSOR; }

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

	/** @brief 視野内外判定を行う
	*/
	virtual bool IsVisible(Vec3f pos);

	/** @brief 中心視野内外判定を行う
	*/
	virtual bool IsInCenter(Vec3f pos);

	/** @brief 自分自身の剛体かどうかを判定する
	*/
	virtual bool IsSelfSolid(PHSolidIf* solid);
};
}
//@}

#endif//CRVISUALSENSOR_H

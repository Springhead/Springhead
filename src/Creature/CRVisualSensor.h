/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRVISUALSENSOR_H
#define CRVISUALSENSOR_H

#include <Creature/CREngine.h>
#include <Creature/SprCRVisualSensor.h>
#include <set>

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 視覚センサ
*/
class CRVisualSensor : public CREngine, public CRVisualSensorDesc {
private:
	// 視覚情報バッファ
	std::vector<CRVisualInfo> visibleList[3];

	// 視覚情報バッファの用途
	// write : Stepで書込み中
	// read  : NContacts/GetContactでアクセスされる対象
	// keep  : Updateされた時にreadにするためのバッファ
	int write, read, keep, empty;

	/// 視覚センサを貼り付ける剛体
	PHSolidIf *soVisualSensor;

public:
	SPR_OBJECTDEF(CRVisualSensor);
	ACCESS_DESC(CRVisualSensor);

	CRVisualSensor(){}
	CRVisualSensor(const CRVisualSensorDesc& desc) 
		: CRVisualSensorDesc(desc) 
	{
		write=0; read=1; keep=-1, empty=2;
	}

	/** @brief 実行順序を決めるプライオリティ値．小さいほど早い
	*/
	virtual int GetPriority() const { return CREngineDesc::CREP_SENSOR; }

	/** @brief １ステップ分の処理
	*/
	virtual void Step();

	/** @brief 現在の視覚情報の個数を返す
	*/
	int NVisibles(){ return (int)visibleList[read].size(); }

	/** @brief 視覚情報を返す
	*/
	virtual CRVisualInfo GetVisible(int n){ return visibleList[read][n]; }

	/** @brief 視覚情報をアップデートする
	*/
	void Update() { if (keep >= 0) { empty=read; read=keep; keep=-1; } }

	/** @brief 視野の大きさを設定する
	*/
	void SetRange(Vec2d range) { this->range = range; }

	/** @brief 中心視野の大きさを設定する
	*/
	void SetCenterRange(Vec2d range) { this->centerRange = range; }

	/** @brief 視覚センサを対象剛体に貼り付ける位置・姿勢を指定する
	*/
	void SetPose(Posed pose) { this->pose = pose; }

	/** @brief 視覚センサを貼り付ける対象の剛体をセットする
	*/
	void SetSolid(PHSolidIf* solid) { soVisualSensor = solid; }

	/** @brief 視野の大きさを取得する
	*/
	Vec2d GetRange() { return range; }

	/** @brief 中心視野の大きさを取得する
	*/
	Vec2d GetCenterRange() { return centerRange; }

	/** @brief 視覚センサを対象剛体に貼り付ける位置・姿勢を取得する
	*/
	Posed GetPose() { return pose; }

	/** @brief 視覚センサを貼り付ける対象の剛体を取得する
	*/
	PHSolidIf* GetSolid() { return soVisualSensor; }
};
}
//@}

#endif//CRVISUALSENSOR_H

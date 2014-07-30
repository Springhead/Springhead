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

struct CRVisualInfo {
	Vec3d		posWorld;
	Vec3d		posLocal;
	Vec3d		velWorld;
	Vec3d		velLocal;
	double		angle;
	PHSolidIf*	solid;
	PHSolidIf*	solidSensor;
	Posed		sensorPose;
	bool		bMyBody;
	bool		bCenter;
};

struct CRVisualSensorIf : CREngineIf{
	SPR_IFDEF(CRVisualSensor);

	/** @brief 現在の視覚情報の個数を返す
	*/
	int NVisibles();

	/** @brief 視覚情報を返す
	*/
	CRVisualInfo GetVisible(int n);

	/** @brief 視覚情報をアップデートする
	*/
	void Update();

	/** @brief 視野の大きさを設定する
	*/
	void SetRange(Vec2d range);

	/** @brief 中心視野の大きさを設定する
	*/
	void SetCenterRange(Vec2d range);

	/** @brief 視覚センサを対象剛体に貼り付ける位置・姿勢を指定する
	*/
	void SetPose(Posed pose);

	/** @brief 視覚センサを貼り付ける対象の剛体をセットする
	*/
	void SetSolid(PHSolidIf* solid);

	/** @brief 視野の大きさを取得する
	*/
	Vec2d GetRange();

	/** @brief 中心視野の大きさを取得する
	*/
	Vec2d GetCenterRange();

	/** @brief 視覚センサを対象剛体に貼り付ける位置・姿勢を取得する
	*/
	Posed GetPose();

	/** @brief 視覚センサを貼り付ける対象の剛体を取得する
	*/
	PHSolidIf* GetSolid();
};

/// 視覚センサのデスクリプタ
struct CRVisualSensorDesc : CREngineDesc{
	SPR_DESCDEF(CRVisualSensor);

	Vec2d range;       ///< 視野の大きさ： 水平角度，垂直角度
	Vec2d centerRange; ///< 中心視野の大きさ： 水平角度，垂直角度
	Posed pose;        ///< 視覚センサを対象剛体に貼り付ける位置・姿勢

	CRVisualSensorDesc(){
		range = Vec2d(Rad(90), Rad(90));
		centerRange = Vec2d(Rad(10), Rad(10));
		pose  = Posed();
	}
};


}

#endif // SPR_CR_VISUALSENSOR_IF_H

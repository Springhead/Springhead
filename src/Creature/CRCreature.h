/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRCREATURE_H
#define CRCREATURE_H

#include <Springhead.h>

#include <Foundation/Object.h>

#include <vector>

//@{
namespace Spr{;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief クリーチャの実装
*/
class CRCreature : public SceneObject, public CRCreatureDesc {

protected:
	/** @brief 感覚系
	*/
	std::vector<CRSensorIf*> sensors;

	/** @brief 運動コントローラ
	*/
	std::vector<CRControllerIf*> controllers;

	/** @brief 内部シーン（記憶の類）
	*/
	CRInternalSceneIf* internalScene;

	/** @brief ボディ（複数持てる）
	*/
	std::vector< UTRef<CRBodyIf> > body;

public:
	SPR_OBJECTDEF(CRCreature);
	ACCESS_DESC(CRCreature);

	CRCreature(){
		CRRegisterTypeDescs();
	}
	CRCreature(const CRCreatureDesc& desc, SceneIf* s=NULL) : CRCreatureDesc(desc) {
		CRRegisterTypeDescs();
		internalScene = NULL;
		if(s){SetScene(s);}
	}

	/** @brief 初期化を実行する
	*/
	virtual void Init();

	/** @brief 感覚→情報処理→運動 の１ステップを実行する
	*/
	virtual void Step();

	/** @brief 内部シーンのボトムアップ注意をリセットする
	*/
	virtual void ClearInternalScene();

	/** @brief センサーからの入力を行う
	*/
	virtual void SensorStep();

	/** @brief 内部シーンの処理を行う
	*/
	virtual void InternalSceneStep();

	/** @brief 制御を行う
	*/
	virtual void ControllerStep();

	/** @brief ボディをつくる
	*/
	virtual CRBodyIf* CreateBody(const IfInfo* ii, const CRBodyDesc& desc);

	/** @brief ボディを取得する
	*/
	virtual CRBodyIf* GetBody(int i);

	/** @brief ボディの数を取得する
	*/
	virtual int NBodies();

	/** @brief 感覚系を追加する
	*/
	virtual CRSensorIf* CreateSensor(const IfInfo* ii, const CRSensorDesc& desc);

	/** @brief 感覚系を取得する
	*/
	virtual CRSensorIf* GetSensor(int i);

	/** @brief 感覚系の数を取得する
	*/
	virtual int NSensors();

	/** @brief 運動コントローラを追加する
	*/
	virtual CRControllerIf* CreateController(const IfInfo* ii, const CRControllerDesc& desc);

	/** @brief 運動コントローラを取得する
	*/
	virtual CRControllerIf* GetController(int i);

	/** @brief 運動コントローラの数を取得する
	*/
	virtual int NControllers();

	/** @brief 内部シーンをつくる
	*/
	virtual CRInternalSceneIf* CreateInternalScene(const CRInternalSceneDesc& desc);

	/** @brief 内部シーンを取得する
	*/
	virtual CRInternalSceneIf* GetInternalScene();
};
}
//@}

#endif//CRCREATURE_H

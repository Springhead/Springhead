/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRCreatureIf_H
#define SPR_CRCreatureIf_H

#include <SprFoundation.h>

namespace Spr{;

//@{

struct CRBodyIf;
struct CRBodyDesc;
struct CRSensorIf;
struct CRSensorDesc;
struct CRControllerIf;
struct CRControllerDesc;
struct CRInternalSceneIf;
struct CRInternalSceneDesc;

// ------------------------------------------------------------------------------
/// 型情報登録

void SPR_CDECL CRRegisterTypeDescs();

// ------------------------------------------------------------------------------
/// クリーチャのインタフェース
struct CRCreatureIf : SceneObjectIf{
	IF_DEF(CRCreature);

	/** @brief 初期化を実行する
	*/
	virtual void Init()= 0;

	/** @brief 感覚→情報処理→運動 の１ステップを実行する
	*/
	virtual void Step()= 0;

	/** @brief 内部シーンのボトムアップ注意をリセットする
	*/
	virtual void ClearInternalScene()= 0;

	/** @brief センサーからの入力を行う
	*/
	virtual void SensorStep()= 0;

	/** @brief 内部シーンの処理を行う
	*/
	virtual void InternalSceneStep()= 0;

	/** @brief 制御を行う
	*/
	virtual void ControllerStep()= 0;

	/** @brief ボディをつくる
	*/
	virtual CRBodyIf* CreateBody(const IfInfo* ii, const CRBodyDesc& desc)= 0;
	template <class T> CRBodyIf* CreateBody(const T& desc){
		return CreateBody(T::GetIfInfo(), desc);
	}

	/** @brief ボディを取得する
	*/
	virtual CRBodyIf* GetBody(int i)= 0;

	/** @brief ボディの数を取得する
	*/
	virtual int NBodies()= 0;

	/** @brief 感覚系を追加する（依存するセンサーは先にCreateしておく必要がある．要対策）
	*/
	virtual CRSensorIf* CreateSensor(const IfInfo* ii, const CRSensorDesc& desc)= 0;
	template <class T> CRSensorIf* CreateSensor(const T& desc){
		return CreateSensor(T::GetIfInfo(), desc);
	}

	/** @brief 感覚系を取得する
	*/
	virtual CRSensorIf* GetSensor(int i)= 0;

	/** @brief 感覚系の数を取得する
	*/
	virtual int NSensors()= 0;

	/** @brief 運動コントローラを追加する（依存するコントローラは先にCreateしておく必要がある．要対策）
	*/
	virtual CRControllerIf* CreateController(const IfInfo* ii, const CRControllerDesc& desc)= 0;
	template <class T> CRControllerIf* CreateController(const T& desc){
		return CreateController(T::GetIfInfo(), desc);
	}

	/** @brief 運動コントローラを取得する
	*/
	virtual CRControllerIf* GetController(int i)= 0;

	/** @brief 運動コントローラの数を取得する
	*/
	virtual int NControllers()= 0;

	/** @brief 内部シーンをつくる
	*/
	virtual CRInternalSceneIf* CreateInternalScene(const CRInternalSceneDesc& desc)= 0;

	/** @brief 内部シーンを取得する
	*/
	virtual CRInternalSceneIf* GetInternalScene()= 0;
};

/// クリーチャのデスクリプタ
struct CRCreatureDesc{
	DESC_DEF_FOR_OBJECT(CRCreature);

	CRCreatureDesc(){
	}
};
//@}

}

#endif//SPR_CRCreatureIf_H

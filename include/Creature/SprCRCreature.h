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
	SPR_IFDEF(CRCreature);

	/** @brief 初期化を実行する
	*/
	void Init();

	/** @brief 感覚→情報処理→運動 の１ステップを実行する
	*/
	void Step();

	/** @brief 内部シーンのボトムアップ注意をリセットする
	*/
	void ClearInternalScene();

	/** @brief センサーからの入力を行う
	*/
	void SensorStep();

	/** @brief 内部シーンの処理を行う
	*/
	void InternalSceneStep();

	/** @brief 制御を行う
	*/
	void ControllerStep();

	/** @brief ボディをつくる
	*/
	CRBodyIf* CreateBody(const IfInfo* ii, const CRBodyDesc& desc);
	template <class T> CRBodyIf* CreateBody(const T& desc){
		return CreateBody(T::GetIfInfo(), desc);
	}

	/** @brief ボディを取得する
	*/
	CRBodyIf* GetBody(int i);

	/** @brief ボディの数を取得する
	*/
	int NBodies();

	/** @brief 感覚系を追加する（依存するセンサーは先にCreateしておく必要がある．要対策）
	*/
	CRSensorIf* CreateSensor(const IfInfo* ii, const CRSensorDesc& desc);
	template <class T> CRSensorIf* CreateSensor(const T& desc){
		return CreateSensor(T::GetIfInfo(), desc);
	}

	/** @brief 感覚系を取得する
	*/
	CRSensorIf* GetSensor(int i);

	/** @brief 感覚系の数を取得する
	*/
	int NSensors();

	/** @brief 運動コントローラを追加する（依存するコントローラは先にCreateしておく必要がある．要対策）
	*/
	CRControllerIf* CreateController(const IfInfo* ii, const CRControllerDesc& desc);
	template <class T> CRControllerIf* CreateController(const T& desc){
		return CreateController(T::GetIfInfo(), desc);
	}

	/** @brief 運動コントローラを取得する
	*/
	CRControllerIf* GetController(int i);

	/** @brief 運動コントローラの数を取得する
	*/
	int NControllers();

	/** @brief 内部シーンをつくる
	*/
	CRInternalSceneIf* CreateInternalScene(const CRInternalSceneDesc& desc);

	/** @brief 内部シーンを取得する
	*/
	CRInternalSceneIf* GetInternalScene();
};

/// クリーチャのデスクリプタ
struct CRCreatureDesc{
	SPR_DESCDEF(CRCreature);

	CRCreatureDesc(){
	}
};
//@}

}

#endif//SPR_CRCreatureIf_H

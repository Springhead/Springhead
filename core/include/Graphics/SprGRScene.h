/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprGRScene.h
 *	@brief シーングラフ
*/

/**	\addtogroup	gpGraphics	*/
//@{

#ifndef SPR_GRScene_H
#define SPR_GRScene_H

#include <Foundation/SprScene.h>
#include <Graphics/SprGRFrame.h>
#include <Graphics/SprGRRender.h>

namespace Spr{;

///	@brief GRScene のDesc．中身無し．
struct GRSceneDesc{
};

struct GRSdkIf;
/**	@brief	グラフィックスシーングラフ */
struct GRSceneIf: public SceneIf{
	SPR_IFDEF(GRScene);

	///	シーンのレンダリング
	void Render(GRRenderIf* r);

	///	ワールドフレームの取得
	GRFrameIf* GetWorld();

	///	カメラの取得
	GRCameraIf* GetCamera();

	/// カメラの作成・設定
	void SetCamera(const GRCameraDesc& desc = GRCameraDesc());

	///	アニメーションコントローラの取得
	GRAnimationControllerIf* GetAnimationController();
	
	/** @brief ビジュアルの作成
		@param desc ビジュアルのデスクリプタ
		@param parent 親フレーム
		フレーム、メッシュ、マテリアル、あるいはライトを作成して親フレームparentの下に追加する。
		parentがNULLの場合はワールドフレームが親となる。
	 */
	GRVisualIf*  CreateVisual(const IfInfo* info, const GRVisualDesc& desc, GRFrameIf* parent = NULL);
	template <class T> GRVisualIf* CreateVisual(const T& desc, GRFrameIf* parent = NULL){
		return CreateVisual(T::GetIfInfo(), desc, parent);
	}
	

	/** @brief このSceneをもつSDKを返す
		@return SDKのインタフェース
	*/	
	GRSdkIf* GetSdk();
};


//@}
}
#endif

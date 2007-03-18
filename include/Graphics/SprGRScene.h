/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
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

#include <Graphics/SprGRFrame.h>
#include <Graphics/SprGRRender.h>

namespace Spr{;

///	@brief GRScene のDesc．中身無し．
struct GRSceneDesc{
};

struct GRSdkIf;
/**	@brief	グラフィックスシーングラフ */
struct GRSceneIf: public SceneIf{
	IF_DEF(GRScene);

	///	シーンのレンダリング
	virtual void Render(GRRenderIf* r)=0;

	///	ワールドフレームの取得
	virtual GRFrameIf* GetWorld()=0;

	///	カメラの取得
	virtual GRCameraIf* GetCamera()=0;

	/// カメラの作成・設定
	virtual void SetCamera(const GRCameraDesc& desc)=0;

	/** @brief ビジュアルの作成
		@param desc ビジュアルのデスクリプタ
		@param parent 親フレーム
		フレーム、メッシュ、マテリアル、あるいはライトを作成して親フレームparentの下に追加する。
		parentがNULLの場合はワールドフレームが親となる。
	 */
	virtual GRVisualIf*  CreateVisual(const GRVisualDesc& desc, GRFrameIf* parent = NULL)=0;

	/** @brief このSceneをもつSDKを返す
		@return SDKのインタフェース
	*/	
	virtual GRSdkIf* GetSdk()=0;
};


//@}
}
#endif

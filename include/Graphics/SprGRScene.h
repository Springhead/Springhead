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
	///	フレームの作成
	virtual GRFrameIf* CreateFrame(const GRFrameDesc& desc)=0;
	///	シーンのレンダリング
	virtual void Render(GRRenderIf* r)=0;
	///	World
	virtual GRFrameIf* GetWorld()=0;
	///	Camera
	virtual GRCameraIf* GetCamera()=0;
	/** @brief このSceneをもつSDKを返す
		@return SDKのインタフェース
	*/	
	virtual GRSdkIf* GetSdk()=0;
};


//@}
}
#endif

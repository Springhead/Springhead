/**
 *	@file SprGRScene.h
 *	@brief シーングラフ
*/

/**	\addtogroup	gpGraphics	*/
//@{

#ifndef SPR_GRScene_H
#define SPR_GRScene_H

#include <Springhead.h>
#include <Graphics/SprGRFrame.h>

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
	///	
	virtual void Render(GRRenderIf* r)=0;
	/** @brief このSceneをもつSDKを返す
		@return SDKのインタフェース
	*/	
	virtual GRSdkIf* GetSdk()=0;
};


//@}
}
#endif

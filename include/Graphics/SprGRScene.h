/**	\addtogroup	gpGraphics	*/
//@{

#ifndef SPR_GRScene_H
#define SPR_GRScene_H

#include <Springhead.h>

namespace Spr{;

///	@brief GRScene のDesc．中身無し．
struct GRSceneDesc{
};

struct GRSdkIf;
/**	@brief	グラフィックスシーングラフ */
struct GRSceneIf: public SceneIf{
	IF_DEF(GRScene);
	/** @brief このSceneをもつSDKを返す
		@return SDKのインタフェース
	 */
	virtual GRSdkIf* GetSdk()=0;
	
};


//@}
}
#endif

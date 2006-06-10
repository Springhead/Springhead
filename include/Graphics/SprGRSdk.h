/**	\addtogroup	gpGraphics	*/
//@{

	/**
 *	@file SprGRSdk.h
 *	@brief グラフィックスSDKの基本クラス
 */
#ifndef SPR_GRSDK_H
#define SPR_GRSDK_H

#include <Springhead.h>

namespace Spr{;

struct GRDeviceGLIf;
struct GRDebugRenderIf;
struct GRSceneIf;

///	@brief GRSdkDesc GRSDKの設定．DirectXを使うかOpenGLを使うかの選択などがここに入る予定．
struct GRSdkDesc{
};

/**	@brief	グラフィックスSDKの基本クラス　 */
struct GRSdkIf: public ObjectIf{
	IF_DEF(GRSdk);
	virtual GRDebugRenderIf* CreateDebugRender()=0;
	virtual GRDeviceGLIf* CreateDeviceGL(int window)=0;
	virtual GRSceneIf* CreateScene()=0;
	virtual size_t NChildObject() const = 0;
	virtual ObjectIf* GetChildObject(size_t i) = 0;
	virtual GRSceneIf* GetScene(size_t i)=0;
	virtual size_t NScene()=0;
};
///	グラフィックスSDK
GRSdkIf* SPR_CDECL CreateGRSdk();
//@}
}
#endif

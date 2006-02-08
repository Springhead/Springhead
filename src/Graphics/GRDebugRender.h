/**
  @file GRDebugRender.h
  @brief デバッグ情報レンダラー（剛体、面）　　
*/
#ifndef GRDEBUGRENDER_H
#define GRDEBUGRENDER_H

#include <SprGraphics.h>

namespace Spr{;

/**	@class	GRDebugRender
    @brief	デバッグ情報レンダラーの実装　 */
class GRDebugRender:public GRRender, public GRDebugRenderIf{
	OBJECTDEF(GRDebugRender);
	BASEIMP_GRRENDER(GRRender);
	/**	Viewportと射影行列を設定
		@param	screen		ウィンドウサイズ */
	void Reshape(Vec2f screen);
	/** 剛体をレンダリングする
	    @param	so　　　	剛体 */
	void DrawSolid(PHSolidIf* so);
	/** 面をレンダリングをする
		@param	face　　　 面  
		@param	base　　　 凸形状の頂点群 */
	void DrawFace(CDFaceIf* face, Vec3f * base);
	/** デバッグ用情報レポート */
	void Print(std::ostream& os) const{ GRRender::Print(os); }
};

}
#endif

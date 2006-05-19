/**
 *	@file GRDebugRender.h
 *	@brief デバッグ情報レンダラー（剛体、面）　　
*/
#ifndef GRDEBUGRENDER_H
#define GRDEBUGRENDER_H

#include <SprGraphics.h>

namespace Spr{;

/**	@class	GRDebugRender
    @brief	デバッグ情報レンダラーの実装　 */
class GRDebugRender:public InheritGRRender<GRDebugRenderIf, GRRender>{
	OBJECTDEF(GRDebugRender);
protected:
	int matSampleCount;
	std::vector<GRMaterialDesc> matSample;		/// レンダラーで用意してある材質(24種類)
public:
	/**  コンストラクタ  */
	GRDebugRender();
	/**	 Viewportと射影行列を設定
		 @param	screen		ウィンドウサイズ  */
	void Reshape(Vec2f screen);
	/**  シーン内の全てのオブジェクトをレンダリングする
	     @param  scene		シーン  */
	void DrawScene(PHSceneIf* scene);
	/**  剛体をレンダリングする
	     @param	so　　　	剛体  */
	void DrawSolid(PHSolidIf* so);
	/**  面をレンダリングをする
		 @param	face　　　	面  
		 @param	base　　　	凸形状の頂点群  */
	void DrawFace(CDFaceIf* face, Vec3f * base);
	/**  指定したマテリアルを割り当てる
	     @param mat			マテリアルサンプル  */
	void SetMaterialSample(MaterialSample matname);
};

}
#endif

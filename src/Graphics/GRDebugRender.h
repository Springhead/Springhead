/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file GRDebugRender.h
 *	@brief デバッグ情報レンダラー（剛体、面）　　
*/
#ifndef GRDEBUGRENDER_H
#define GRDEBUGRENDER_H

#include "GRRender.h"

namespace Spr{;

/**	@class	GRDebugRender
    @brief	デバッグ情報レンダラーの実装　 */
class GRDebugRender:public GRRender, public GRDebugRenderIfInit{
	OBJECTDEF(GRDebugRender, GRRender);
protected:
	int matSampleCount;
	std::vector<GRMaterialDesc> matSample;		/// レンダラーで用意してある材質(24種類)
public:
	/**  コンストラクタ  */
	GRDebugRender();
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
	void SetMaterialSample(GRDebugRenderIf::TMaterialSample matname);
};

}
#endif

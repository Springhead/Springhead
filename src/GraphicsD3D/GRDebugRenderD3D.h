/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file GRDebugRenderD3D.h
 *	@brief DirectXのデバッグ情報レンダラー（剛体、面）　　
*/
#ifndef GRDEBUGRENDERD3D_H
#define GRDEBUGRENDERD3D_H

#include "GraphicsD3D.h"

namespace Spr{;

/**	@class	GRDebugRenderD3D
    @brief	DirectXのデバッグ情報レンダラーの実装　 */
class GRDebugRenderD3D:public GRDebugRender, public GRDebugRenderD3DIfInit{
	SPR_OBJECTDEF(GRDebugRenderD3D, GRDebugRender);
public:
	/**  シーン内の全てのオブジェクトをレンダリングする
	     @param  scene		シーン  */
	void DrawScene(PHSceneIf* scene);
	/**  剛体をレンダリングする
	     @param	so　　　	剛体  */
	void DrawSolid(PHSolidIf* so);
};

}
#endif

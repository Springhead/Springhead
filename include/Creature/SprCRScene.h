/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRSCENEIF_H
#define SPR_CRSCENEIF_H

#include <Foundation/SprScene.h>

namespace Spr{;

//@{

// ------------------------------------------------------------------------------

/// クリーチャ・シーンのインターフェース
struct CRSceneIf : SceneIf{
	SPR_IFDEF(CRScene);
};

/// クリーチャ・シーンのデスクリプタ
struct CRSceneDesc{
	SPR_DESCDEF(CRScene);

	CRSceneDesc(){
	}
};

//@}

}

#endif // SPR_CRSCENEIF_H

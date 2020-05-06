/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprGRSphere.h
 *	@brief 球
*/

/**	\addtogroup	gpGraphics	*/
//@{

#ifndef SPR_GRSphere_H
#define SPR_GRSphere_H

#include <Graphics/SprGRFrame.h>

namespace Spr{;


///	@brief	グラフィックスで使う表示用のSphere
struct GRSphereIf: public GRVisualIf{
	SPR_IFDEF(GRSphere);
};
///	@brief 表示用のSphere(GRSphere)のデスクリプタ．
struct GRSphereDesc: public GRVisualDesc {
	SPR_DESCDEF(GRSphere);
	float radius;			///< 半径
	int   slices;			///< 経線方向の分割数
	int   stacks;			///< 緯線方向の分割数	
	GRSphereDesc() {
		radius = 1.0;
		slices = 16;
		stacks = 16;
	}
};


//@}
}
#endif

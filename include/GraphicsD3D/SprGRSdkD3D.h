/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/**
 *	@file SprGRSdkD3D.h
 *	@brief DirectXのグラフィックスSDKの基本クラス
 */
#ifndef SPR_GRSDKD3D_H
#define SPR_GRSDKD3D_H

namespace Spr{;

//@{

/**	@brief	DirectXのレンダラー基本クラス */
struct GRDeviceD3DIf: public GRDeviceIf{
	IF_DEF(GRDeviceD3D);
};

struct GRSdkD3DDesc{
};

/**	@brief	DirectXのグラフィックスSDKの基本クラス　 */
struct GRSdkD3DIf: public SdkIf{
	IF_DEF(GRSdkD3D);
	virtual GRDeviceD3DIf* CreateDeviceD3D()=0;
	static GRSdkD3DIf* SPR_CDECL CreateSdk();
};

//@}
}
#endif

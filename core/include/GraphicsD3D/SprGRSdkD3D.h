/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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

#include <d3d9.h>

namespace Spr{;

struct GRAnimationMeshIf;
struct GRAnimationMeshDesc;

//@{

typedef bool (SPR_CDECL *GRDeviceD3DConfirmDeviceFunc)(D3DCAPS9* pCaps, DWORD dwBehavior);
typedef void (SPR_CDECL *GRDeviceD3DListenerFunc)();

/**	@brief	DirectXのレンダラー基本クラス */
struct GRDeviceD3DIf: public GRDeviceIf{
	SPR_IFDEF(GRDeviceD3D);
	virtual void SetConfirmDeviceFunc(GRDeviceD3DConfirmDeviceFunc func)=0;
	virtual void AddLostDeviceListener(GRDeviceD3DListenerFunc func)=0;
	virtual void AddResetDeviceListener(GRDeviceD3DListenerFunc func)=0;
	virtual void ToggleFullScreen()=0;
	virtual bool IsFullScreen()=0;
};

/**	@brief	DirectXのデバッグ情報レンダラーの基本クラス */
struct GRDebugRenderD3DIf:public GRRenderIf{
	SPR_IFDEF(GRDebugRenderD3D);
};

struct GRSdkD3DDesc{
};

/**	@brief	DirectXのグラフィックスSDKの基本クラス　 */
struct GRSdkD3DIf: public SdkIf{
	SPR_IFDEF(GRSdkD3D);
	virtual GRDeviceD3DIf* CreateDeviceD3D()=0;
	virtual GRDebugRenderD3DIf* CreateDebugRenderD3D()=0;
	virtual GRAnimationMeshIf* CreateAnimationMesh(const GRAnimationMeshDesc& desc)=0;
	static GRSdkD3DIf* SPR_CDECL CreateSdk();
};

//@}
}
#endif

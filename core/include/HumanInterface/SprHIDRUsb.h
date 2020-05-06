/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef SPR_HI_DRUSB_H
#define SPR_HI_DRUSB_H

#include <HumanInterface/SprHIBase.h>

namespace Spr{;

/** \addtogroup gpPhysics */
//@{

/**
	CyverseのUSBデバイス
 **/

///	
struct DRUsb20SimpleIf : public HIRealDeviceIf{
	SPR_IFDEF(DRUsb20Simple);
};
struct DRUsb20SimpleDesc{
	SPR_DESCDEF(DRUsb20Simple);

	int channel;		///< 使用チャンネル．-1なら任意

	DRUsb20SimpleDesc(): channel(10){}
};


///
struct DRUsb20Sh4If : public HIRealDeviceIf{
	SPR_IFDEF(DRUsb20Sh4);
};
struct DRUsb20Sh4Desc: public DRUsb20SimpleDesc{
	SPR_DESCDEF(DRUsb20Sh4);
	
	DRUsb20Sh4Desc(){ channel = 0; }
};
struct DRCyUsb20Sh4If : public DRUsb20Sh4If{
	SPR_IFDEF(DRCyUsb20Sh4);
};
struct DRCyUsb20Sh4Desc:public DRUsb20Sh4Desc{
	SPR_DESCDEF(DRCyUsb20Sh4);
};


//@}

}	//	namespace Spr
#endif

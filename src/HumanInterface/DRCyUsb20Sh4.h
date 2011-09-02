/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
// DRUsb20Sh4.h: DRUsb20Sh4 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef DR_CyUSB20SH4_H
#define DR_CyUSB20SH4_H

#include <HumanInterface/DRUsb20Sh4.h>

namespace Spr {;

/**
	USB2.0 SH アンプ のドライバ
 **/
class SPR_DLL DRCyUsb20Sh4 : public DRUsb20Sh4{
public:
	SPR_OBJECTDEF(DRCyUsb20Sh4);

	DRCyUsb20Sh4(const DRCyUsb20Sh4Desc& d = DRCyUsb20Sh4Desc());
	
protected:
	///	
	virtual void*		UsbOpen(int id);
	void Reset();
	void UsbSend(unsigned char* outBuffer);
	void UsbRecv(unsigned char* inBuffer);
	unsigned UsbVidPid(void* h);
};

} //namespace Spr

#endif

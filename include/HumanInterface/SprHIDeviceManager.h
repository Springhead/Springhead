/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprDVDevicePool.h
 *	@brief デバイスを登録しておくコンテナ．
*/

#ifndef SPR_DVVirtualDevicePool_H
#define SPR_DVVirtualDevicePool_H
#include <SprFoundation.h>

namespace Spr{;

/**	\addtogroup	gpHumanInterface	*/
//@{

struct HIDeviceManagerIf: public ObjectIf{
	IF_DEF(HIDeviceManager);
	virtual void Init()=0;
	virtual void Clear()=0;
	///	Rent virtual device.
	virtual HIVirtualDeviceIf* Rent(const char* type, const char* name=NULL)=0;
	///	Return virutal device.
	virtual bool Return(HIVirtualDeviceIf* dev)=0;
	
	///	Register real device.
	virtual void RegisterRealDevice(HIRealDeviceIf* dev)=0;	
	///	Get real device.
	virtual HIRealDeviceIf* FindRealDevice(const char* name=NULL)=0;
};

//@}
}

#endif

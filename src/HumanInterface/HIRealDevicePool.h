/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef HIREALDEVICEPOOL_H
#define HIREALDEVICEPOOL_H

#include "HIRealDevice.h"

namespace Spr {

/**	実デバイスのプール. 
	ユーザはISAカードやPCカードなどの実デバイスをここに登録する．
	デバイスは初期化時に，仮想デバイス(DVVirtualDevices)に登録される．	*/
class SPR_DLL HIRealDevicePool: public UTRefCount, public std::vector< UTRef<HIRealDevice> >{
public:
	HIRealDevicePool();
	~HIRealDevicePool();
	///	登録
	void Register(HIRealDevice* dev);
	///	初期化
	void Init(HISdkIf* sdk);
	///	ダンプ出力
	void Print(std::ostream& o) const;
	///
	void Clear(){ clear(); }
};
inline std::ostream& operator << (std::ostream& o, const HIRealDevicePool& d){
	d.Print(o);
	return o;
}
}	//	namespace Spr

#endif

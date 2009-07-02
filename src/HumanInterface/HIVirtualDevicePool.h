/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef HIVIRTUALDEVICEPOOL_H
#define HIVIRTUALDEVICEPOOL_H

#include <HumanInterface/HIVirtualDevice.h>
#include <vector>

namespace Spr {

//----------------------------------------------------------------------------
//	HIVirtualDevicePool

/**	仮想デバイスのプール. ISAカードやPCカードなどのデバイスは，
	機能(D/A,カウンタなど)別に分けられ，仮想デバイスとしてこの
	プールに登録される．
	登録されたデバイスは，SPIDARなどのシステムに利用される．	*/

class SPR_DLL HIVirtualDevicePool: public UTRefCount, public std::vector< UTRef<HIVirtualDevice> >{
public:
	///	Rent device.
#ifdef __BORLANDC__
	template <class CT> bool Rent(CT*& t);
#else
	template <class CT> bool Rent(CT*& t){
		CT* pt = (CT*)Rent(CT::TypeS());
		t = pt;
		return t != NULL;
	}
#endif
	///	Rent device.
	HIVirtualDevice* Rent(const char* type, const char* name=NULL);
	HIVirtualDevice* RentNo(const char* type, int No=0, const char* name=NULL);
	///	return device.
	bool Return(HIVirtualDevice* dev);
	///	Register virutal device.
	void Register(HIVirtualDevice* dev){ push_back(dev); }
	///	ダンプ出力
	void Print(std::ostream& o) const;
	///
	void Clear(){ clear(); }
};
#ifdef __BORLANDC__
template <class CT> bool HIVirtualDevicePool::Rent(CT*& t){
		CT* pt = (CT*)Rent(CT::TypeS());
		t = pt;
		return t != NULL;
}
#endif
inline std::ostream& operator << (std::ostream& o, const HIVirtualDevicePool& d){
	d.Print(o);
	return o;
}

}	//	namespace Spr

#endif

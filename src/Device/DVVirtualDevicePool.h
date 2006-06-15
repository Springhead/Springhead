#ifndef VR_DVDEVICES_H
#define VR_DVDEVICES_H

#include "Device/DVVirtualDevice.h"
#include <vector>

namespace Spr {

//----------------------------------------------------------------------------
//	DVVirtualDevices

/**	仮想デバイスのプール. ISAカードやPCカードなどのデバイスは，
	機能(D/A,カウンタなど)別に分けられ，仮想デバイスとしてこの
	プールに登録される．
	登録されたデバイスは，SPIDARなどのシステムに利用される．	*/

class SPR_DLL DVVirtualDevicePool: std::vector< UTRef<DVVirtualDevice> >{
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
	DVVirtualDevice* Rent(const char* type, const char* name=NULL);
	///	return device.
	bool Return(DVVirtualDevice* dev);
	///	Register virutal device.
	void Register(DVVirtualDevice* dev){ push_back(dev); }
	///	ダンプ出力
	void Print(std::ostream& o) const;
	///
	void Clear(){ clear(); }
};
#ifdef __BORLANDC__
template <class CT> bool DVVirtualDevicePool::Rent(CT*& t){
		CT* pt = (CT*)Rent(CT::TypeS());
		t = pt;
		return t != NULL;
}
#endif
inline std::ostream& operator << (std::ostream& o, const DVVirtualDevicePool& d){
	d.Print(o);
	return o;
}

}	//	namespace Spr

#endif

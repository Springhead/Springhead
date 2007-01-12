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

#ifndef VR_DRREALDEVICEPOOL_H
#define VR_DRREALDEVICEPOOL_H

#include "DRRealDevice.h"

namespace Spr {

/**	実デバイスのプール. 
	ユーザはISAカードやPCカードなどの実デバイスをここに登録する．
	デバイスは初期化時に，仮想デバイス(DVVirtualDevices)に登録される．	*/
class SPR_DLL DRRealDevicePool: public std::vector< UTRef<DRRealDevice> >{
public:
	DRRealDevicePool();
	~DRRealDevicePool();
	///	登録
	void Register(DRRealDevice* dev);
	///	初期化
	void Init(DVVirtualDevicePool& vpool);
	///	ダンプ出力
	void Print(std::ostream& o) const;
	///
	void Clear(){ clear(); }
};
inline std::ostream& operator << (std::ostream& o, const DRRealDevicePool& d){
	d.Print(o);
	return o;
}
}	//	namespace Spr

#endif

#ifndef VR_DVCOUNTERBASE_H
#define VR_DVCOUNTERBASE_H

#include "DVVirtualDevice.h"

namespace Spr {

//----------------------------------------------------------------------------
//	DVCountBase

///	カウンタの1チャンネルを表す．
class SPR_DLL DVCounterBase:public DVVirtualDevice{
public:
	DVCounterBase();
	///	カウンタ値の設定
	virtual void Count(long count)=0;
	///	カウンタ値の読み出し
	virtual long Count()=0;
	///	デバイスの名前
	virtual const char* Name() const=0;
	///	デバイスの種類
	static const char* TypeS() { return "Counter"; }
	///	デバイスの種類
	virtual const char* Type() const{ return TypeS(); }
};

}	//	namespace Spr

#endif

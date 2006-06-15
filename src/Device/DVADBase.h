#ifndef VR_DVADBASE_H
#define VR_DVADBASE_H

#include "DVVirtualDevice.h"

namespace Spr {

//----------------------------------------------------------------------------
//	DVAdBase

///	A/Dの1チャンネルを表す．
class DVAdBase:public DVVirtualDevice{
public:
	///	入力デジタル値の取得
	virtual int Digit()=0;
	///	入力電圧の取得
	virtual float Voltage()=0;
	///	デバイスの名前
	virtual const char* Name() const =0;
	///	デバイスの種類
	static const char* TypeS() { return "A/D"; }
	///	デバイスの種類
	virtual const char* Type() const{ return TypeS(); }
};

}	//	namespace Spr

#endif

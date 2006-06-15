#ifndef VR_DVDABASE_H
#define VR_DVDABASE_H

#include "DVVirtualDevice.h"

namespace Spr {

//----------------------------------------------------------------------------
//	DVDaBase

///	D/Aの1チャンネルを表す．
class SPR_DLL DVDaBase:public DVVirtualDevice{
public:
	DVDaBase();
	///	出力するデジタル値の設定
	virtual void Digit(int d)=0;
	///	出力電圧の設定
	virtual void Voltage(float volt)=0;
	///	デバイスの名前
	virtual const char* Name() const =0;
	///	デバイスの種類
	static const char* TypeS() { return "D/A"; }
	///	デバイスの種類
	virtual const char* Type() const{ return TypeS(); }
};

}	//	namespace Spr

#endif

#ifndef VR_DVPIOBASE_H
#define VR_DVPIOBASE_H

#include <HumanInterface/HIVirtualDevice.h>
namespace Spr {;
//----------------------------------------------------------------------------
//	DVPioBase

///	入出力ポートのための定数の定義などだけを行う．
class SPR_DLL DVPortEnum{
	enum TLevel {LEVEL_LO, LEVEL_HI};
	enum TDir {DIR_IN, DIR_OUT};
};
///	入力ポートの1端子をあらわす．
class SPR_DLL DVInputPortBase:public HIVirtualDevice, public DVPortEnum{
public:
	///	デバイスの名前
	virtual const char* Name() const =0;
	///	入力ポートの電圧レベル(Hi:1, Lo:0) を返す．
	virtual int Get()=0;

	///	デバイスの種類
	static const char* TypeS() { return "input port"; }
	///	デバイスの種類
	virtual const char* Type() const{ return TypeS(); }
};

///	出力ポートの1端子をあらわす．
class SPR_DLL DVOutputPortBase:public HIVirtualDevice, public DVPortEnum{
public:
	///	デバイスの名前
	virtual const char* Name() const =0;
	///	入力ポートの電圧レベル(Hi:1, Lo:0) を返す．
	virtual void Set(int l)=0;

	///	デバイスの種類
	static const char* TypeS() { return "output port"; }
	///	デバイスの種類
	virtual const char* Type() const{ return TypeS(); }
};


///	PIOの1端子を表す．
class SPR_DLL DVPioBase:public HIVirtualDevice, public DVPortEnum{
public:
	///	ポートのロジックレベルの入力。Hi:true Lo:false
	virtual int Get()=0;
	///	ポートのロジックレベルの出力。Hi:true Lo:false
	virtual void Set(int l)=0;
	///	デバイスの名前
	virtual const char* Name() const =0;
	///	デバイスの種類
	static const char* TypeS() { return "PIO"; }
	///	デバイスの種類
	virtual const char* Type() const{ return TypeS(); }
};

}	//	namespace Spr
#endif

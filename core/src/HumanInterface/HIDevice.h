/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef HI_DEVICE_H
#define HI_DEVICE_H

#include <HumanInterface/SprHIDevice.h>
#include <HumanInterface/SprHIKeyMouse.h>
#include <HumanInterface/SprHIJoyStick.h>
#include <Foundation/Object.h>
#include <vector>

namespace Spr {;

class HIVirtualDevice;

/// デバイスの基本クラス
class SPR_DLL HIDevice : public NamedObject{
public:
	SPR_OBJECTDEF_ABST(HIDevice);
};

///	実デバイスの基本クラス．ISAカード1枚などに相当する.
class SPR_DLL HIRealDevice: public HIDevice{
public:
	SPR_OBJECTDEF_ABST(HIRealDevice);
	
	/// 提供する仮想デバイスの配列
	std::vector< UTRef<HIVirtualDevice> >	dvPool;
public:
	///	初期化と仮想デバイスの登録。SDKに登録する際に呼ばれる。
	virtual bool Init(){ return false; }

	/// 仮想デバイスの作成
	virtual HIVirtualDeviceIf*	Rent(const IfInfo* ii, const char* name, int portNo);
	/// 仮想デバイスの返却
	virtual bool Return(HIVirtualDeviceIf* dv);
	///	状態の更新
	virtual void Update(){}
	///	ダンプ出力
	//virtual void Print(std::ostream& o) const;

	virtual bool		AddChildObject(ObjectIf* o);
	virtual ObjectIf*	GetChildObject(size_t i);
	virtual size_t		NChildObject() const;
};

/*
/// ウィンドウに関連付けられるデバイスの基本クラス
class DRWinDeviceBase : public HIRealDevice{
public:
	SPR_OBJECTDEF_NOIF(DRWinDeviceBase);

public:
	/// コールバック関数の登録
	virtual void	RegisterCallback(){}
};
*/

//----------------------------------------------------------------------------
//	HIVirtualDevice

///	仮想デバイスの基本クラス．DA,カウンタなどの1チャンネルに相当する.
class SPR_DLL HIVirtualDevice: public HIDevice{
public:
	SPR_OBJECTDEF_ABST(HIVirtualDevice);
	friend class HIVirtualDevicePool;

	/// 使用中フラグ
	bool used;
	/// ポート番号
	int	portNo;
	/// 実デバイスへの参照
	HIRealDevice*	realDevice;
public:
	HIVirtualDevice(HIRealDevice* dev, int ch = -1);
	virtual ~HIVirtualDevice(){}

	/// デバイス名の設定
	void SetDeviceName();
	/// 使用中フラグ
	bool IsUsed(){ return used; }
	void SetUsed(bool b=true){ used = b; }
	/// ポート番号
	int	GetPortNo() const { return portNo; }
	///	実デバイスへのポインタ
	HIRealDeviceIf* GetRealDevice(){ return realDevice->Cast(); }
	
	///	ダンプ出力
	//virtual void Print(std::ostream& o) const;
	/// デバイスタイプ
	//virtual const char* GetDeviceType() const = 0;
	/// 状態の更新
	virtual void Update(){}
};

///	入力ポートの1端子をあらわす．
class SPR_DLL DVInputPort: public HIVirtualDevice, public DVPortEnum{
public:
	///	入力ポートの電圧レベル(Hi:1, Lo:0) を返す．
	virtual int Get()=0;

	///	デバイスの種類
	//static const char* GetDeviceTypeStatic() { return "input port"; }
	//virtual const char* GetDeviceType() const{ return GetDeviceTypeStatic(); }

	DVInputPort(HIRealDevice* r, int c):HIVirtualDevice(r, c){}
};

///	出力ポートの1端子をあらわす．
class SPR_DLL DVOutputPort: public HIVirtualDevice, public DVPortEnum{
public:
	///	入力ポートの電圧レベル(Hi:1, Lo:0) を返す．
	virtual void Set(int l)=0;

	///	デバイスの種類
	//static const char* GetDeviceTypeStatic() { return "output port"; }
	//virtual const char* GetDeviceType() const{ return GetDeviceTypeStatic(); }

	DVOutputPort(HIRealDevice* r, int c):HIVirtualDevice(r, c){}
};


///	PIOの1端子を表す．
class SPR_DLL DVPio: public HIVirtualDevice, public DVPortEnum{
public:
	SPR_OBJECTDEF_ABST(DVPio);
	///	ポートのロジックレベルの入力。Hi:true Lo:false
	virtual int Get()=0;
	///	ポートのロジックレベルの出力。Hi:true Lo:false
	virtual void Set(int l)=0;
	
	///	デバイスの種類
	//static const char* GetDeviceTypeStatic() { return "PIO"; }
	//virtual const char* GetDeviceType() const{ return GetDeviceTypeStatic(); }

	DVPio(HIRealDevice* r, int c):HIVirtualDevice(r, c){}
};

///	カウンタの1チャンネルを表す．
class SPR_DLL DVCounter: public HIVirtualDevice{
public:
	SPR_OBJECTDEF_ABST(DVCounter);
	
	///	カウンタ値の設定
	virtual void Count(long count)=0;
	///	カウンタ値の読み出し
	virtual long Count()=0;
	///	デバイスの種類
	//static const char* GetDeviceTypeStatic() { return "Counter"; }
	//virtual const char* GetDeviceType() const{ return GetDeviceTypeStatic(); }

	DVCounter(HIRealDevice* r, int c):HIVirtualDevice(r, c){}
};

//----------------------------------------------------------------------------
//	DVAd

///	A/Dの1チャンネルを表す．
class DVAd: public HIVirtualDevice{
public:
	SPR_OBJECTDEF_ABST(DVAd);
	///	入力デジタル値の取得
	virtual int Digit()=0;
	///	入力電圧の取得
	virtual float Voltage()=0;
	
	///	デバイスの種類
	//static const char* GetDeviceTypeStatic() { return "A/D"; }
	//virtual const char* GetDeviceType() const{ return GetDeviceTypeStatic(); }

	DVAd(HIRealDevice* r, int c):HIVirtualDevice(r, c){}
};

//----------------------------------------------------------------------------
//	DVDa

///	D/Aの1チャンネルを表す．
class SPR_DLL DVDa: public HIVirtualDevice{
public:
	SPR_OBJECTDEF_ABST(DVDa);
	
	///	出力するデジタル値の設定
	virtual void Digit(int d)=0;
	///	出力電圧の設定
	virtual void Voltage(float volt)=0;

	///	デバイスの種類
	//static const char* GetDeviceTypeStatic() { return "D/A"; }
	//virtual const char* GetDeviceType() const{ return GetDeviceTypeStatic(); }

	DVDa(HIRealDevice* r, int c):HIVirtualDevice(r, c){}
};

//----------------------------------------------------------------------------
//	DVForce

class SPR_DLL DVForce: public HIVirtualDevice{
public:
	SPR_OBJECTDEF_ABST(DVForce);

	///	自由度の取得
	virtual int GetDOF() = 0;
	///	力の取得
	virtual float GetForce(int ch) = 0;
	///	力の取得
	virtual void GetForce3(Vec3f& f) {
		f[0] = GetForce(0);
		f[1] = GetForce(1);
		f[2] = GetForce(2);
	}
	///	力の取得
	virtual void GetForce6(Vec3f& f, Vec3f& t) {
		f.X() = GetForce(0);
		f.Y() = GetForce(1);
		f.Z() = GetForce(2);
		t.X() = GetForce(3);
		t.Y() = GetForce(4);
		t.Z() = GetForce(5);
	}

	///	デバイスの種類
	//static const char* GetDeviceTypeStatic(){ return "Force"; }
	//virtual const char* GetDeviceType() const{ return GetDeviceTypeStatic(); }

	DVForce(HIRealDevice* r):HIVirtualDevice(r){}
};

template<class T>
class DVCallbackContainer{
public:
	typedef std::vector<T*>	Callbacks;
	Callbacks	callbacks;

	void AddCallback(T* cb){
		if(find(callbacks.begin(), callbacks.end(), cb) == callbacks.end())
			callbacks.push_back(cb);
	}
	void RemoveCallback(T* cb){
		callbacks.erase(find(callbacks.begin(), callbacks.end(), cb));
	}
};

///	DVKeyMouse
class SPR_DLL DVKeyMouse: public DVCallbackContainer<DVKeyMouseCallback>, public HIVirtualDevice{
public:
	SPR_OBJECTDEF_ABST(DVKeyMouse);
public:
	// 登録されているハンドラを呼び出す
	virtual void OnMouse(int button, int state, int x, int y);
	virtual void OnDoubleClick(int button, int x, int y);
	virtual void OnMouseMove(int button, int x, int y, int zdelta);
	virtual void OnKey(int state, int key, int x, int y);

	virtual int GetKeyState(int key){ return 0; }
	virtual void GetMousePosition(int& x, int& y, int& time, int count=0){}

	///	デバイスの種類
	//static const char* GetDeviceTypeStatic() { return "KeyMouse"; }
	//virtual const char* GetDeviceType() const{ return GetDeviceTypeStatic(); }

	DVKeyMouse(HIRealDevice* r, int ch):HIVirtualDevice(r, ch){}
};

//-------------------------------------------------------------------------------------------------
///	DVJoyStick
class SPR_DLL DVJoyStick: public DVCallbackContainer<DVJoyStickCallback>, public HIVirtualDevice{
public:
	SPR_OBJECTDEF_ABST(DVJoyStick);

	int pollInterval;
public:
	// 登録されているハンドラを呼び出す
	void OnUpdate(int buttonMask, int x, int y, int z);

	virtual void SetPollInterval(int ms){}
	virtual int  GetPollInterval(){ return pollInterval; }

	DVJoyStick(HIRealDevice* r, int ch):HIVirtualDevice(r, ch){
		pollInterval = 10;
	}
};

//-------------------------------------------------------------------------------------------------
///	向きを返すデバイス
class SPR_DLL DVOrientation: public HIVirtualDevice{
public:
	///	向きの取得(回転行列であらわす)
	virtual Vec3f GetEuler() = 0;
	///	向きの取得(回転行列であらわす)
	virtual void GetMatrix(Matrix3f& ori) = 0;
	
	///	デバイスの種類
	//static const char* GetDeviceTypeStatic() { return "Orientation"; }
	//virtual const char* GetDeviceType() const{ return GetDeviceTypeStatic(); }

	DVOrientation(HIRealDevice* r):HIVirtualDevice(r){}
};

}	//	namespace Spr

#endif

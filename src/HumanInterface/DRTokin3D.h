/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef DEVICE_TOKIN3D_H
#define DEVICE_TOKIN3D_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <HumanInterface/HIRealDevice.h>
#include "DVOrientationBase.h"
#include "DVPioBase.h"
#include <base/Affine.h>

namespace Spr {
class SPR_DLL DRTokin3D: public HIRealDevice{
public:
	///	仮想デバイス
	class DVOrientation:public DVOrientationBase{
	protected:
		DRTokin3D* realDevice;
	public:
		DVOrientation(DRTokin3D* r):realDevice(r){}
		virtual HIRealDeviceIf* RealDevice() { return realDevice->Cast(); }
		virtual const char* Name() const{ return realDevice->Name(); }
		virtual void GetOrientationMatrix(Matrix3f& ori){
			realDevice->GetMatrix(ori);
		}
		virtual void GetMatrix(Matrix3f& ori){
			realDevice->GetMatrix(ori);
		}
		virtual Vec3f GetEuler(){
			return realDevice->GetEuler();
		}
	};
	class DVInputPort:public DVInputPortBase{
	protected:
		DRTokin3D* realDevice;
		int ch;
		char name[256];
	public:
		DVInputPort(DRTokin3D* r, int c);
		virtual HIRealDeviceIf* RealDevice() { return realDevice->Cast(); }
		virtual const char* Name() const{ return name; }
		virtual int Get(){
			return realDevice->GetPort(ch);
		}
	};
protected:
	void* intf;
	char name[1024];
public:
	///
	DRTokin3D();
	///
	virtual ~DRTokin3D();
	///	初期化
	virtual bool Init();
	///	登録
	virtual void Register(HIVirtualDevicePool& vpool);
	///	名前
	virtual const char* Name() const { return name; }
	///	方向行列
	virtual void GetMatrix(Matrix3f& ori);
	///	オイラー角の取得
	virtual Vec3f GetEuler();
	///	ポート
	virtual int GetPort(int ch);
};

}	//	namespace Spr

#endif

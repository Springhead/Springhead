/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef DEVICE_TOKIN3D_H
#define DEVICE_TOKIN3D_H

#include <Base/Affine.h>
#include <HumanInterface/HIDevice.h>

/**
	NEC/TOKINの3Dモーションセンサを使用するためのクラス
	使用するにはコンパイラが見つけられる場所に
		Tkinput.h, Tkinputdef.h, Tuadapio.h, TKINPUT_I.C
	をおき，
		#define SPR_USE_TOKIN3D
	とマクロ定義してSpringheadをビルドする．
	上記ファイルは著作権の都合上Springheadには含まれない．
 */

// #define SPR_USE_TOKIN3D

namespace Spr {
class SPR_DLL DRTokin3D: public HIRealDevice{
public:
	SPR_OBJECTDEF_NOIF(DRTokin3D);

	///	仮想デバイス
	class DVOri: public DVOrientation{
	public:
		DVOri(DRTokin3D* r):DVOrientation(r){}
		DRTokin3D* GetRealDevice() { return realDevice->Cast(); }

		virtual void GetOrientationMatrix(Matrix3f& ori){ GetRealDevice()->GetMatrix(ori); }
		virtual void GetMatrix(Matrix3f& ori){ GetRealDevice()->GetMatrix(ori); }
		virtual Vec3f GetEuler(){ return GetRealDevice()->GetEuler(); }
	};
	class DVIn: public DVInputPort{
	public:
		DVIn(DRTokin3D* r, int c):DVInputPort(r, c){}
		DRTokin3D* GetRealDevice() { return realDevice->Cast(); }
		
		virtual int Get(){ return GetRealDevice()->GetPort(portNo); }
	};
protected:
	void* intf;
public:
	///
	DRTokin3D();
	///
	virtual ~DRTokin3D();
	///	初期化
	virtual bool Init();
	///	方向行列
	virtual void GetMatrix(Matrix3f& ori);
	///	オイラー角の取得
	virtual Vec3f GetEuler();
	///	ポート
	virtual int GetPort(int ch);
};

}	//	namespace Spr

#endif

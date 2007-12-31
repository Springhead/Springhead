/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprHIBase.h
 *	@brief ヒューマンインタフェースの基本クラス
*/


#ifndef SPR_HIBase_H
#define SPR_HIBase_H
#include <HumanInterface/SprHIDevice.h>

namespace Spr{;

/**	\addtogroup	gpHumanInterface	*/
//@{

struct DRRealDeviceIf;

/**	@brief	ヒューマンインタフェースの基本クラス	*/
struct HIBaseIf: public NamedObjectIf{
	IF_DEF(HIBase);
	///	キャリブレーションの前に行う処理
	virtual bool BeforeCalibration()=0;
	///	キャリブレーション処理
	virtual bool Calibration()=0;
	///	キャリブレーションのあとに行う処理
	virtual bool AfterCalibration()=0;
	///	ヒューマンインタフェースの依存関係を追加する
	virtual void AddDeviceDependency(HIRealDeviceIf* rd)=0;
	///	ヒューマンインタフェースの依存関係をクリアする
	virtual void ClearDeviceDependency()=0;
	///	ヒューマンインタフェースの状態を更新する．
	virtual void Update(float dt)=0;
	///	ヒューマンインタフェースが使用可能ならば true．
	virtual bool IsGood()=0;
	/**	ヒューマンインタフェースの初期化．
		必要なデバイスをsdkから借りてきて，ヒューマンインタフェースを構成する	*/
	virtual bool Init(HISdkIf* sdk, const void* desc)=0;
};

/**	@brief	位置入力インタフェース	*/
struct HIPositionIf: public HIBaseIf{
	IF_DEF(HIPosition);
	virtual Vec3f GetPosition()=0;
};

/**	@brief	角度入力インタフェース	*/
struct HIOrientationIf: public HIBaseIf{
	IF_DEF(HIOrientation);
	virtual Quaternionf GetOrientation()=0;
};

/**	@brief	姿勢(=位置＋角度)入力インタフェース	*/
struct HIPoseIf: public HIPositionIf{
	IF_DEF(HIPose);
	virtual Quaternionf GetOrientation()=0;
	virtual Posef GetPose()=0;
};

//@}
}
#endif

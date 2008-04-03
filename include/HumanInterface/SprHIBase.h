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
	SPR_IFDEF(HIBase);
	///	キャリブレーションの前に行う処理
	bool BeforeCalibration();
	///	キャリブレーション処理
	bool Calibration();
	///	キャリブレーションのあとに行う処理
	bool AfterCalibration();
	///	ヒューマンインタフェースの依存関係を追加する
	void AddDeviceDependency(HIRealDeviceIf* rd);
	///	ヒューマンインタフェースの依存関係をクリアする
	void ClearDeviceDependency();
	///	ヒューマンインタフェースの状態を更新する．
	void Update(float dt);
	///	ヒューマンインタフェースが使用可能ならば true．
	bool IsGood();
	/**	ヒューマンインタフェースの初期化．
		必要なデバイスをsdkから借りてきて，ヒューマンインタフェースを構成する	*/
	bool Init(HISdkIf* sdk, const void* desc);
};

/**	@brief	位置入力インタフェース	*/
struct HIPositionIf: public HIBaseIf{
	SPR_IFDEF(HIPosition);
	Vec3f GetPosition();
};

/**	@brief	角度入力インタフェース	*/
struct HIOrientationIf: public HIBaseIf{
	SPR_IFDEF(HIOrientation);
	Quaternionf GetOrientation();
};

/**	@brief	姿勢(=位置＋角度)入力インタフェース	*/
struct HIPoseIf: public HIPositionIf{
	SPR_IFDEF(HIPose);
	Quaternionf GetOrientation();
	Posef GetPose();
};

//@}
}
#endif

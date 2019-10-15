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
#include <HumanInterface/SprHumanInterfaceDecl.hpp>

namespace Spr{;

/**	\addtogroup	gpHumanInterface	*/
//@{

/**	@brief	ヒューマンインタフェースの基本クラス
 **/
struct HIBaseIf : NamedObjectIf{
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
	bool Init(const void* desc);
};

/**	@brief	位置入力インタフェース	*/
/*struct HIPositionIf : HIBaseIf{
	SPR_VIFDEF(HIPosition);
	Vec3f GetPosition();
};*/

/**	@brief	角度入力インタフェース	*/
/*struct HIOrientationIf : HIBaseIf{
	SPR_VIFDEF(HIOrientation);
	Quaternionf GetOrientation();
};*/

/**	@brief	姿勢(=位置＋角度)入力インタフェース	*/
struct HIPoseIf : HIBaseIf{
	SPR_VIFDEF(HIPose);
	//SPR_OVERRIDEMEMBERFUNCOF(HIBaseIf, HIPositionIf);
	
	Vec3f			GetPosition();
	Quaternionf		GetOrientation();
	Posef			GetPose();
	Affinef			GetAffine();
	Vec3f			GetVelocity();
	Vec3f			GetAngularVelocity();
};

/** @brief	6自由度力覚インタフェースの基本クラス
 **/
struct HIHapticIf : HIPoseIf{
	SPR_VIFDEF(HIHaptic);
	///	デバイスの実際の提示トルクを返す
	Vec3f GetTorque();
	///	デバイスの実際の提示力を返す
	Vec3f GetForce();
	///	デバイスの目標出力とトルク出力を設定する
	void SetForce(const Vec3f& f, const Vec3f& t = Vec3f());
};
struct HIHapticDummyDesc{
	Posed pose;
};
struct HIHapticDummyIf : public HIHapticIf{
	SPR_IFDEF(HIHapticDummy);
	void SetPose(Posed p);
	void SetVelocity(Vec3d v);
	void SetAngularVelocity(Vec3d v);
};

//@}
}
#endif

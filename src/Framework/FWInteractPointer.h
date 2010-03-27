/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef FWINTERACT_POINTER_H
#define FWINTERACT_POINTER_H

#include <Foundation/Object.h>
#include <Framework/SprFWInteractPointer.h>
#include <Physics/PHSolid.h>
#include <Framework/FWInteractInfo.h>

namespace Spr{;

class FWInteractPointer : public SceneObject, public FWInteractPointerDesc{
public:
	SPR_OBJECTDEF(FWInteractPointer);
	PHSolid			hiSolid;
	AllInteractInfo interactInfo;
	bool			bForce;
	bool			bVibration;
	std::vector<bool>	bContact;
	int			grabFlag;	//接触している剛体とバーチャルカップリングで掴む場合のFlag
							// 0:接触している剛体とバーチャルカップリングはしない
							// 1:接触している剛体とバーチャルカップリングをするためにジョイント(K,Dを大きく設定した)を作成する
							// 2:K,Dを大きく設定したジョイントによるバーチャルカップリング中
							// 3:設定したK,Dを使用したジョイントによるバーチャルカップリング中
							// 4:接触している剛体に接続したジョイントをクリアする

	Posed  cameraOrigin;	//カメラに対して基準となる姿勢，カメラの移動により変位が蓄積する
	Posed  cameraPose;		//1ステップ前のカメラ姿勢を保存

	double correctionSpringK;
	double correctionDamperD;

	FWInteractPointer();
	FWInteractPointer(const FWInteractPointerDesc& desc);

	void		Init();
	void		Sync();
	void		SetPointerSolid(PHSolidIf* solid);
	PHSolidIf*	GetPointerSolid();
	void		SetHI(HIBaseIf* hi);
	HIBaseIf*	GetHI();
	void		SetPosScale(double s);
	double		GetPosScale();
	void		SetWorldScale(double s);
	double		GetWorldScale();
	void		SetForceScale(double s);
	double		GetForceScale();
	void		SetTorqueScale(double s);
	double		GetTorqueScale();
	void		SetLocalRange(double r);
	double		GetLocalRange();
	void		SetDefaultPosition(Posed p);
	Posed		GetDefaultPosition();
	void		SetPointersCalibPosition(Posed p);
	Posed		GetPointersCalibPosition();
	void		CalcCorrectionSpringDamper();
	void		EnableForce(bool b = true);
	void		EnableVibration(bool b = true);
	void		Calibration();
	void		SetGrabFlag(int f);
	int			GetGrabFlag();
	void		 SetCameraOri(Quaterniond q);
	Quaterniond  GetCameraOri();
	void		 SetCameraOrigin(Posed p);
	Posed		 GetCameraOrigin();
	void		 SetSpringK(double k);
	double		 GetSpringK();
	void		 SetDamperD(double d);
	double		 GetDamperD();
};

}
#endif
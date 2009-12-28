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
							// 1:接触している剛体とバーチャルカップリングをするためにジョイントを作成する
							// 2:接触している剛体とバーチャルカップリング中
							// 3:接触している剛体に接続したジョイントをクリアする

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
	int		GetGrabFlag();
};

}
#endif
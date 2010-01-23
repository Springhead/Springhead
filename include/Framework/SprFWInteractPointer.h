/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef FWINTERACT_POINTERIF_H
#define FWINTERACT_POINTERIF_H

#include <Foundation/SprObject.h>
#include <Physics/SprPHSolid.h>
#include <HumanInterface/SprHIBase.h>

namespace Spr{;

struct FWInteractPointerDesc{
public:
	PHSolidIf*	pointerSolid;
	HIBaseIf*	humanInterface; 
	double		springK;
	double		damperD;
	double		springOriK;
	double		damperOriD;
	Posed		defaultPosition; //World座標系に対する物理シミュレーションのPointerのPose
	Posed		pointersCalibPosition;
	double		posScale;	//ポインタの移動量はグリップの移動に対してposScale*worldScale倍
	double		worldScale;	//シミュレーション空間は実世界のworldScale倍
	double		forceScale; //力覚インタフェースで出力する力をforceScale倍する．通常は1.0
	double		torqueScale;	////力覚インタフェースで出力するトルクをforceScale倍する．通常は1.0
	double		localRange;
	Quaterniond cameraOri;	//カメラ視点の回転に対して常に移動方向を一定にするためのクォータニオン，毎ステップSetする必要がある．
	FWInteractPointerDesc(){ Init(); }
	void Init(){
		pointerSolid  = NULL;
		humanInterface = NULL;
		springK = 0.0;
		damperD = 0.0;
		springOriK = 0.0;
		damperOriD = 0.0;
		defaultPosition = Posed();
		pointersCalibPosition = Posed();
		posScale = 1.0;
		worldScale = 1.0;
		forceScale = 1.0;
		torqueScale = 1.0;
		localRange = 0.1;
	}
};

struct FWInteractPointerIf : public SceneObjectIf{
public:
	SPR_IFDEF(FWInteractPointer);

	void		SetPointerSolid(PHSolidIf* solid);
	PHSolidIf*	GetPointerSolid();
	void		SetHI(HIBaseIf* hi);
	HIBaseIf*	GetHI();
	void		SetPosScale(double s);
	double		GetPosScale();
	void		SetForceScale(double s);
	double		GetForceScale();
	void		SetTorqueScale(double s);
	double		GetTorqueScale();
	void		SetLocalRange(double r);
	double		GetLocalRange();
	void		SetPointersCalibPosition(Posed p);
	Posed		GetPointersCalibPosition();
	void		SetDefaultPosition(Posed p);
	Posed		GetDefaultPosition();
	void		EnableForce(bool b);
	void		EnableVibration(bool b);
	void		Calibration();
	void		SetGrabFlag(int f);
	int			GetGrabFlag();
	void		 SetCameraOri(Quaterniond q);
	Quaterniond  GetCameraOri();
};

typedef std::vector< UTRef<FWInteractPointerIf> > FWInteractPointers;

}
#endif
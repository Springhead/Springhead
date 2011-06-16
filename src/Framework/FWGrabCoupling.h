/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FWGRABCOUPLING_H
#define FWGRABCOUPLING_H

#include <Framework/FWInteractAdaptee.h>

namespace Spr{;

class FWGrabCoupling : public FWInteractAdaptee{
public:
	std::vector<PHSolidIf*> vcSolid;	
	std::vector<PHJointIf*> vcJoint;
	std::vector<PHJointIf*> grabJoint;
protected:
	volatile int hapticcount;
	volatile bool bSync;
	bool bCalcPhys;
	virtual void CreateHapticLoop(){}
public:
	FWGrabCoupling();

	virtual void UpdatePointer();
	void		 UpdatePointerDirect(); //直接ポインタを更新する（力出力なし）
	virtual void Clear();
	virtual void Init(){
			FWInteractAdaptee::Init();
	};
	virtual void Step();
	void UpdateInterface();
	virtual void CreatePointerSolid();
	void CallBackHapticLoop(){};

	///剛体を掴んでVCする場合
	void GrabSolid();
	void GrabSolid2();
	void AdjustSpring(double n);
	void UpdateGrabPointer();
	PHSolidDesc grabSolidDesc;
	CDShapeIf* pointerShape;

	Posed grabPointerPose;


	///クラッチ機構
	Posed cluchPose[2];			//cluchPose[0]:クラッチ前 cluchPose[1]:クラッチ後
	Posed cluchTrans;
	bool cluchFlag;			//cluchFlag[0] 現在のflag cluchFlag[1] 1ステップ前のflag
	void OnCluch(){ cluchFlag = true;}
	void OffCluch(){ cluchFlag = false;}

	SpatialVector lastOutForce;


};
}
#endif
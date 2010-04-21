/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef FWLDHaptic3D_H 
#define FWLDHaptic3D_H

#include <Framework/FWMultiRateHaptic.h>
#include <Framework/FWInteractScene.h>

class FWLDHaptic3DLoop : public FWHapticLoopBase{
private:
	Vec3d vibV;
	double vibT;
	double proK;
	double proD;
	double proM;
	std::vector<Vec3d> pLastPoint[2];
	std::vector<Vec3d> proxy[2];
	std::vector<Vec3d> proVel[2];
	Vec3d dproxy;
	std::vector<Vec3d> oVibForce;
	int picNum;								//	把持している物体の番号
	Vec3d DisplayProxy[2];					//	デバッグ用
	Vec3d DisplayForce[2];					//	プロット用
	bool bPic;								//	プロット用
	std::vector<bool> contactFlag[2];		//	プロット用
public:
	FWLDHaptic3DLoop();
	virtual void Step();
	virtual void HapticRendering3D();
	virtual void HapticRendering();
	virtual void Proxy();
	virtual void ProxySimulation();
	int* ContactStat(int nPic);
	virtual Vec3d Vibration(FWInteractSolid*, FWInteractPointer*, int);		//	ポインタ-剛体
	virtual void PicVibration(int nSolids);									//	剛体-剛体

	Vec3d* GetProxyPoint();					//	デバッグ用
	Vec3d* GetForce();
	bool GetContactFlag();					//	把持しているかの判定
	Vec3d GetPicCenterPosition();			//	把持している物体の重心位置
//	int GetPicTime();
	virtual void LocalDynamics();
};


class FWLDHaptic3D : public FWMultiRateHaptic{
protected:
	UTRef<ObjectStatesIf> states, states2;
	FWLDHaptic3DLoop ldLoop3D;
public:
	FWLDHaptic3D();
	virtual void Init();
	virtual void Clear();
	void CallBackHapticLoop();
	virtual void SyncHaptic2Physic();
	virtual void SyncPhysic2Haptic();
	virtual void Step();
	virtual void PhysicsStep();
	virtual void UpdatePointer();
	virtual void TestSimulation();
	virtual void BeginKeyboard();
	virtual void EndKeyboard();
	void ReleaseState(PHSceneIf*);
	Vec3d* GetProxyPoint();
	Vec3d* GetForce();
	bool GetContactFlag();
	Vec3d GetPicCenterPosition();
};

#endif
/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRWALKINGCONTROLLER_H
#define CRWALKINGCONTROLLER_H

#include <Foundation/Object.h>
#include "IfStubCreature.h"

#include <Springhead.h>		//	Springheadのインタフェース

#include "CRController.h"
#include ".\crwctimeleft.h"
#include ".\crwcchangearoundcenter.h"
#include ".\crwclandingsite.h"
#include ".\crwcfootforce.h"
#include ".\crwcgeneforce.h"
/////////////////////////////

namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 注意コントローラ
*/
class CRWalkingController : public CRController, public CRWalkingControllerIfInit, public CRWalkingControllerDesc {
private:
	/// 依存する他のオブジェクトの例．必要なものを選んで記述．ただしCRControllerに既にあるものは不要．

	std::vector<PHSolidIf*> UpperBody;
	PHSolidIf* tfHead;
	PHSolidIf* tfChest;
	PHSolidIf* tfHip;
	PHSolidIf* tfFootLeft;
	PHSolidIf* tfFootRight;

	PHSolidIf* hiHead;
	PHSolidIf* hiChest;
	PHSolidIf* hiHip;
	PHSolidIf* hiFootLeft;
	PHSolidIf* hiFootRight;

	PHSolidIf* syncFootLeft;
	PHSolidIf* syncFootRight;
	PHSolidIf* syncHip;

	CRHingeHumanBodyIf* hibody;
	CRTrunkFootHumanBodyIf* tfbody;

	double paramVelocityX, paramVelocityZ;              //X, Z方向の目標歩行速度 
    double paramHalfStrideX, paramHalfStrideZ;          //X, Z方向の目標歩幅 
	double footHeight;                                  //遊脚の最高点 
    double amplitude;                                   //重心の上下動の振幅 

	/////////////大域変数//////
	double T0;            //基本歩行周期
	double Tc;            //基本歩行周期を求めるのに使用する定数
	double TimeStep;
	double et;      //elapsed time 単脚支持期間の経過時間
	double timeleft;      //単脚支持期間の残り時間
	double timeDSleft;    //両脚支持期間の残り時間
	double timehalfcycle; //歩行の両脚支持期間も兼ねた１サイクルの２分の１
	double footsize;      //足のサイズ
	double SingleSupportEndX;   //単脚支持期間の終了時の重心位置X
	double SingleSupportEndZ;   //単脚支持期間の終了時の重心位置Z
	double previousPX;          //1ステップ前の重心位置X
	double previousPZ;          //1ステップ前の重心位置Z
	double DSDX;                //両脚支持期間に重心の進む距離
	double CurrentDirection;    //x,z平面状の現在の体の方向
	double TargetDirection;   //x,z平面状の現在の体の目標方向
	bool LF;       //左脚が遊脚の時 true
	bool RF;      //右脚が遊脚の時 true
	bool DSterm;  //両脚支持期間の時 true
	bool EarthConnection; //現在の支持脚が着地条件を満たしている時 true
	bool LandAble;        //次の着地点へ着地することが可能な場合 true
	bool completefall; //歩行の継続が不可能な場合 true
	Vec3d currentlandingsite;   //現在の着地点
	Vec3d nextlandingsite;      //次の目標着地点
	Vec3d CenterVelocity;       //重心位置の速度
	Vec3d PreviousV;            //1ステップ前の重心位置の速度
	Vec3d PositionOfWholeBody;  //重心位置
	Vec3d PositionOfUpperBody;  //上半身の位置
	Vec3d change;   //バランスに関する制御量 PD制御 
	long totalStep;   //開始からのステップ数

	int katoki;       //ステップがgaitbeginに至るまで脚を動かさない制御のために使用.いずれなくす
	int gaitbegin;   //ステップがgaitbeginに至るまで脚を動かさない制御のために使用.いずれなくす
	int forcestep1;   //デモに使用している変数
	int flag;         //デモ用に使用
	int forcestep2;   //デモに使用している変数
	int flag2;        //デモ用に使用
	int flag3;        //デモ用に使用

	double Xs;
	double Zs;

	double nextSpeed;
	bool stop;
	bool reverse;

	CRWCTimeLeft* tl; 
	CRWCChangeAroundCenter* cac;
	CRWCLandingSite* ls;
	CRWCFootForce* ff;
	CRWCGeneForce* gf;

	Vec3d GetCenterOfBlocks(std::vector<PHSolidIf*> objects);
	Vec3d CalcCenterVelocity(std::vector<PHSolidIf*> objs);
	double TotalMass(std::vector<PHSolidIf*> objects);
	double CalcBasicCycle(void);
	double CalcZParamV(double hz, double restT);
	double CalcZParamH(double v, double restT);
	double CalcLocalX(double xb, double zb, double xt, double zt, double theta);
	double CalcLocalZ(double xb, double zb, double xt, double zt, double theta);
	double CalcLocalVX(double vx, double vz, double theta);
	double CalcLocalVZ(double vx, double vz, double theta);
	double square(double a);
	void VelocityXChange(double vx);
	void VelocityZChange(double vz);
	void HalfStrideXChange(double sx);
	void AmplitudeChange(double amp);
	void FootHeightChange(double fh);
	void WCSetSpeed(double v);
	void WCStop(void);
	void WCReverse(void);
	void CalcCurrentDirection(void);
	void CalcTargetAngle(void);
	void ConstraintForce(void);
	void completeFall(void);
	void fallForce(void);
	bool CalcSustainable(void);
	Vec3d CalcChange(void);
	double CalcTimeLeft(void);
	Vec3d CalcNextLandingSite(void);
	double CalcTimeHalfCycle(void);
	void GenerateCenterForce(void);
	void GenerateFootForce(void);
	void func(void);
	void CreateUpperBody(void);
	void AssignFoot(void);
	void AssignCenterObject(void);
	void AssignHip(void);
	void AssignHead(void);
	void SetTimeParams(void);
	void AssignInitialLandingSite(void);
	void CreateCRWCTimeLeft(void);
	void CreateCRWCChangeAroundCenter(void);
	void CreateCRWCLandingSite(void);
	void CreateCRWCFootForce(void);
	void CreateCRWCGeneForce(void);
	void stand(void);
	void gait(void);

	void SuperimposeHingeBody(void);


public:
	OBJECTDEF(CRWalkingController, CRController);
	ACCESS_DESC(CRWalkingController);

	CRWalkingController(){
	}
	CRWalkingController(const CRWalkingControllerDesc& desc, CRCreatureIf* c=NULL) 
		: CRWalkingControllerDesc(desc) 
		, CRController((const CRControllerDesc&)desc, c)
	{
	}

	/** @brief 初期化を行う
	*/
	virtual void Init();

	/** @brief 再初期化を行う
	*/
	virtual void ReInit();

	/** @brief 制御を実行する
	*/
	virtual void Step();

	/** @brief 歩行の速度を設定する
	*/
	virtual void SetSpeed(double speed);

	/** @brief 転回角度を設定する
	*/
	virtual void SetRotationAngle(double rot);

	/** @brief ワールド座標系の進行角度を設定する
	*/
	virtual void SetRotationWorldCoordinate(double r);

	/** @brief 静止する
	*/
	virtual void Stop();

	/** @brief 後ろ向きに歩く
	*/
	virtual void Reverse();

	/** @brief 位置を設定する（初期状態決定のため）
	*/
	virtual void SetPos(Vec3f pos);

	/** @brief 基本歩行周期を取得
	*/
	virtual double GetBasicCycle() { return et/(et+timeleft); }

	/** @brief 歩行の継続が不可能か？
	*/
	virtual bool IsCompleteFall() { return completefall; }

	/** @brief 左足が遊足か？
	*/
	virtual bool isLeftSwingFoot() { return LF; }

	/** @brief 歩幅
	*/
	virtual double GetHalfStrideX() { return abs(CalcLocalX(currentlandingsite.x, currentlandingsite.z, nextlandingsite.x, currentlandingsite.z, CurrentDirection)/2.0); }
};
}
//@}

#endif//CRWALKINGCONTROLLER_H

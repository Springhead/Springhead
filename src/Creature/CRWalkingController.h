/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRWALKINGCONTROLLER_H
#define CRWALKINGCONTROLLER_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"

#include "CRController.h"
#include ".\crwctimeleft.h"
#include ".\crwcchangearoundcenter.h"
#include ".\crwclandingsite.h"
#include ".\crwcfootforce.h"
#include ".\crwcgeneforce.h"

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 注意コントローラ
*/
class CRWalkingController : public CRController, public CRWalkingControllerIfInit, public CRWalkingControllerDesc {
private:
	/// 依存する他のオブジェクトの例．必要なものを選んで記述．ただしCRControllerに既にあるものは不要．
	
	CRTrunkFootHumanBodyIf* body;

	std::vector<PHSolidIf*> UpperBody;
	PHSolidIf* soHead;
	PHSolidIf* soCenterObject;
	PHSolidIf* soHip;
	PHSolidIf* footleft;
	PHSolidIf* footright;

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

	CRWCTimeLeft* tl;
	CRWCChangeAroundCenter* cac;
	CRWCLandingSite* ls;
	CRWCFootForce* ff;
	CRWCGeneForce* gf;
	/*
	CRInternalSceneIf*  internalScene;
	CRGazeControllerIf* gazeController;
	CROpticalSensorIf*  opticalSensor;
	*/

	/// ほかにPrivateな変数やメソッドが必要ならここに定義を追加．
	void CreateUpperBody();
	void AssignFoot();  
	void AssignCenterObject(); 
	void AssignHip();        
	void AssignHead();  
	void InvalidGravity();
	void SetTimeParams();
	void AssignInitialLandingSite();
	void CreateCRWCTimeLeft();
	void CreateCRWCChangeAroundCenter();
	void CreateCRWCLandingSite();
	void CreateCRWCFootForce();
	void CreateCRWCGeneForce();
	Vec3d GetCenterOfBlocks(std::vector<PHSolidIf*> objs);
	Vec3d CalcCenterVelocity(std::vector<PHSolidIf*> objs);
	double TotalMass(std::vector<PHSolidIf*> objs);
	double CalcBasicCycle();
	double CalcZParamV(double hz, double restT);
	double CalcZParamH(double v, double restT);
	double CalcLocalX(double xb, double zb, double xt, double zt, double theta);
	double CalcLocalZ(double xb, double zb, double xt, double zt, double theta);
	double CalcLocalVX(double vx, double vz, double theta);
	double CalcLocalVZ(double vx, double vz, double theta);
	void CalcCurrentDirection();
	void CalcTargetAngle();
	void VelocityXChange(double vx);
	void VelocityZChange(double vz);
	void HalfStrideXChange(double sx);
	void AmplitudeChange(double amp);
	bool CalcSustainable();
	void ConstraintForce();
	void CompleteFall();
	Vec3d CalcChange();
	double CalcTimeLeft();
	Vec3d CalcNextLandingSite();
	double CalcTimeHalfCycle();
	void GenerateCenterForce();
	void GenerateFootForce();
	void gait();
	void test();

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

	/** @brief 制御を実行する
	*/
	virtual void Step();

	/** @brief 歩行の速度を設定する
	*/
	virtual void SetSpeed(float speed);

	/** @brief 転回角度を設定する
	*/
	virtual void SetRotationAngle(float rot);

	virtual void SetVelocityX(double vx) {VelocityXChange(vx);}
	virtual void SetVelocityZ(double vz) {VelocityZChange(vz);}
	virtual void SetHalfStrideX(double hsx) {HalfStrideXChange(hsx);}
	virtual void SetAmplitude(double amp) {AmplitudeChange(amp);}
};
}
//@}

#endif//CRWALKINGCONTROLLER_H

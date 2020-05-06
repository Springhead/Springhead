/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRFOURLEGSTINYANIMALBODYGEN_H
#define CRFOURLEGSTINYANIMALBODYGEN_H

#include <Creature/CRBodyGenerator/CRBodyGen.h>

//@{
namespace Spr{;

/// 胴体と足のみの哺乳類モデルのディスクリプタ
struct CRFourLegsTinyAnimalBodyGenDesc : CRBodyGenDesc{

	// どの種類の関節で脚を構成するか
	enum CRTinyJointsMode{
		HINGE_MODE = 0,
		BALL_MODE,
	} jointType;

	// 脚のポリゴンのタイプ
	enum ShapeMode{
		MODE_ROUNDCONE = 0,
		MODE_BOX, MODE_CAPSULE, MODE_SPHERE,
	} shapeMode;

	/// 体幹に関するパラメータ
	double bodyHeight, bodyBreadth, bodyThickness;

	Vec2d upperSizes;	//< 脚のRoundConeのサイズ（親側，子側）
	Vec2d lowerSizes;	//< 足のRoundConeのサイズ（親側，子側）
	float upperLength;	//< 脚のRoundConeを構成する2球間の距離
	float lowerLength;	//< 足のRoundConeを構成する2球間の距離

	// 質量に関するパラメータ
	double massFF;		//< 前足の質量
	double massFL;		//< 前脚の質量
	double massRF;		//< 後足の質量
	double massRL;		//< 後脚の質量
	double massBody;	//< 体幹の質量

	/// 各関節のバネダンパ
	double springFront, damperFront;
	double springRear,  damperRear;

	/// 可動域制限（BallJointの場合）
	Vec2d rangeFrontSwing;
	Vec2d rangeFrontTwist;
	Vec2d rangeRearSwing;
	Vec2d rangeRearTwist;
	
	/// 可動域制限（HingeJointの場合）
	Vec2d rangeUpperFront;
	Vec2d rangeLowerFront;
	Vec2d rangeUpperRear;
	Vec2d rangeLowerRear;

	bool rfLeg; //< 右前足を作成するか
	bool lfLeg; //< 左前足を作成するか
	bool rrLeg; //< 右後足を作成するか
	bool lrLeg; //< 左後足を作成するか

	CRFourLegsTinyAnimalBodyGenDesc();
};


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRFourLegsTinyAnimalBodyGen
// ヒンジジョイントを用いた哺乳類モデル・クラスの実装（未実装：中身はTrunkFootHumanBody）
class CRFourLegsTinyAnimalBodyGen : public CRBodyGen, public CRFourLegsTinyAnimalBodyGenDesc {
private:
	PHSolidIf* CreateBody();
	void CreateFrontLegs(LREnum lr, PHSolidIf* sBody);
	void CreateRearLegs( LREnum lr, PHSolidIf* sBody);
	PHSolidIf* InitBody();
	PHSolidIf* InitFrontLeg0(LREnum lr, PHSolidIf* sBody);
	void InitFrontLeg1(LREnum lr, PHSolidIf* sLeg0);
	PHSolidIf* InitRearLeg0(LREnum lr, PHSolidIf* sBody);
	void InitRearLeg1(LREnum lr, PHSolidIf* sLeg0);
	void InitLegs(PHSolidIf* sBody);

	void InitContact();
	// void InitControlMode(PHJointDesc::PHControlMode m = PHJointDesc::MODE_POSITION);	//< ボディの制御モードを設定する．


public:

	CRFourLegsTinyAnimalBodyGen(){}
	CRFourLegsTinyAnimalBodyGen(const CRFourLegsTinyAnimalBodyGenDesc& desc, PHSceneIf* s=NULL) 
		: CRFourLegsTinyAnimalBodyGenDesc(desc) 
		, CRBodyGen((const CRBodyGenDesc&)desc, s)
	{
		PHSolidIf* b	= InitBody();
		InitLegs(b);
		InitContact();
		// InitControlMode();
		Init();
	}

	/** @brief 初期化を実行する
	*/
	virtual void Init();
	virtual Vec2d GetSwingLimit(){return rangeFrontSwing;}
	virtual Vec2d GetTwistLimit(){return rangeFrontTwist;}

};

}
//@}

#endif//CRFourLegsTinyAnimalBODYGEN_H

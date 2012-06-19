/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRFOURLEGSANIMALBODYGEN_H
#define CRFOURLEGSANIMALBODYGEN_H

#include <Creature/CRBodyGenerator/CRBodyGen.h>

//@{
namespace Spr{;

/// 4足動物モデルのデスクリプタ
struct CRFourLegsAnimalBodyGenDesc : CRBodyGenDesc {

	enum CRAnimalSolids{
		// Center part of the solids
		SO_WAIST=0,
		SO_CHEST, SO_TAIL1, SO_TAIL2, SO_TAIL3,
		SO_NECK, SO_HEAD, 

		// -- Left part of the solids
		SO_LEFT_BREASTBONE, SO_LEFT_RADIUS, SO_LEFT_FRONT_CANNON_BONE, SO_LEFT_FRONT_TOE,
		SO_LEFT_FEMUR , SO_LEFT_TIBIA, SO_LEFT_REAR_CANNON_BONE, SO_LEFT_REAR_TOE,
		
		// -- Right part of the solids
		SO_RIGHT_BREASTBONE, SO_RIGHT_RADIUS, SO_RIGHT_FRONT_CANNON_BONE, SO_RIGHT_FRONT_TOE,
		SO_RIGHT_FEMUR , SO_RIGHT_TIBIA, SO_RIGHT_REAR_CANNON_BONE, SO_RIGHT_REAR_TOE,

		// -- The number of the solids
		SO_NSOLIDS
	};

	enum CRAnimalJoints{

		//////////////////////////////////////////////////////////////////
		//																//
		// < ボディの定義の順番 >										//
		// PHBallJoint⇒PHHingeJointにしていると遺伝子を組んだ時に		//
		// うまく一点交叉しなくなってしまうので混合で定義している		//
		//																//
		//////////////////////////////////////////////////////////////////

		// -- Center part of the joints
		JO_WAIST_CHEST=0,
		JO_CHEST_NECK,
		JO_NECK_HEAD,
		JO_WAIST_TAIL, JO_TAIL_12, JO_TAIL_23,

		// -- Left part of the joints
		JO_LEFT_SHOULDER, JO_LEFT_ELBOW, JO_LEFT_FRONT_KNEE, JO_LEFT_FRONT_ANKLE,
		JO_LEFT_HIP, JO_LEFT_STIFLE, JO_LEFT_REAR_KNEE, JO_LEFT_REAR_ANKLE,

		// -- Right part of the joints
		JO_RIGHT_SHOULDER,JO_RIGHT_ELBOW, JO_RIGHT_FRONT_KNEE, JO_RIGHT_FRONT_ANKLE,
		JO_RIGHT_HIP, JO_RIGHT_STIFLE, JO_RIGHT_REAR_KNEE, JO_RIGHT_REAR_ANKLE,

		// -- The number of the all joints (ball + hinge)
		JO_NJOINTS								//(nHingeJoints = nJoints - nBallJoints - 1)
	};

	enum CRAnimalLegs{
		LEG_RIGHT_FRONT=0, LEG_LEFT_FRONT,
		LEG_RIGHT_REAR, LEG_LEFT_REAR
	};

	int joNBallJoints;
	int joNHingeJoints;

	/// サイズに関するパラメータ
	double waistBreadth,	       waistHeight,			  waistThickness;
	double chestBreadth,	       chestHeight,			  chestThickness;
	double tailBreadth,		       tailHeight,		      tailThickness;
	double neckBreadth,		       neckHeight,			  neckThickness;
	double headBreadth,		       headHeight,			  headThickness;
	double breastboneBreadth,      breastboneHeight,	  breastboneThickness;
	double radiusBreadth,	       radiusHeight,		  radiusThickness;
	double frontCannonBoneBreadth, frontCannonBoneHeight, frontCannonBoneThickness;
	double frontToeBreadth,		   frontToeHeight,		  frontToeThickness;
	double femurBreadth,		   femurHeight,			  femurThickness;
	double tibiaBreadth,		   tibiaHeight,			  tibiaThickness;
	double rearCannonBoneBreadth,  rearCannonBoneHeight,  rearCannonBoneThickness;
	double rearToeBreadth,		   rearToeHeight,		  rearToeThickness;

	/// 各BallJointのバネダンパ
	double springWaistChest,   damperWaistChest;	//腰-胸
	double springWaistTail,	   damperWaistTail;		//腰-尾
	double springTail,		   damperTail;			//尾
	double springChestNeck,	   damperChestNeck;     //胸-首
	double springNeckHead,	   damperNeckHead;		//首-頭
	double springShoulder,	   damperShoulder;		//肩
	double springFrontAnkle,   damperFrontAnkle;	//かかと（前足）
	double springHip,		   damperHip;			//尻
	double springRearAnkle,    damperRearAnkle;		//かかと（後足）
	
	// 各HingeJointのバネダンパ
	double springElbow,		   damperElbow;			//肘（前足）
	double springFrontKnee,	   damperFrontKnee;		//膝（前足）
	double springStifle,	   damperStifle;		//肘？（後足）
	double springRearKnee,	   damperRearKnee;		//膝（後足）
	
	/// HingeJoint可動域制限
	Vec2d  rangeElbow;
	Vec2d  rangeFrontKnee;
	Vec2d  rangeStifle;
	Vec2d  rangeRearKnee;
	
	// BallJoint制御目標
	Quaterniond goalWaistChest;
	Quaterniond goalWaistTail;
	Quaterniond goalTail;
	Quaterniond goalChestNeck;
	Quaterniond goalNeckHead;
	Quaterniond goalShoulder;
	Quaterniond goalFrontAnkle;
	Quaterniond goalHip;
	Quaterniond goalRearAnkle;

	// HingeJoint制御目標
	double originElbow;
	double originFrontKnee;
	double originStifle;
	double originRearKnee;

	// BallJoint可動域制限の中心
	Vec3d limitDirWaistChest;
	Vec3d limitDirWaistTail;
	Vec3d limitDirTail;
	Vec3d limitDirChestNeck;
	Vec3d limitDirNeckHead;
	Vec3d limitDirShoulder;
	Vec3d limitDirFrontAnkle;
	Vec3d limitDirHip;
	Vec3d limitDirRearAnkle;

	/// BallJointのswing可動域:
	Vec2d limitSwingWaistChest;
	Vec2d limitSwingWaistTail;
	Vec2d limitSwingTail;
	Vec2d limitSwingChestNeck;
	Vec2d limitSwingNeckHead;
	Vec2d limitSwingShoulder;
	Vec2d limitSwingFrontAnkle;
	Vec2d limitSwingHip;
	Vec2d limitSwingRearAnkle;

	/// BallJointのtwist可動域
	Vec2d limitTwistWaistChest;
	Vec2d limitTwistWaistTail;
	Vec2d limitTwistTail;
	Vec2d limitTwistChestNeck;
	Vec2d limitTwistNeckHead;
	Vec2d limitTwistShoulder;
	Vec2d limitTwistFrontAnkle;
	Vec2d limitTwistHip;
	Vec2d limitTwistRearAnkle;

	// 関節の出せる力の最大値
	double fMaxWaistChest;
	double fMaxChestNeck;
	double fMaxNeckHead;
	double fMaxWaistTail;
	double fMaxTail12;
	double fMaxTail23;
	double fMaxLeftShoulder;
	double fMaxLeftElbow;
	double fMaxLeftFrontKnee;
	double fMaxLeftFrontAnkle;
	double fMaxLeftHip;
	double fMaxLeftStifle;
	double fMaxLeftRearKnee;
	double fMaxLeftRearAnkle;
	double fMaxRightShoulder;
	double fMaxRightElbow;
	double fMaxRightFrontKnee;
	double fMaxRightFrontAnkle;
	double fMaxRightHip;
	double fMaxRightStifle;
	double fMaxRightRearKnee;
	double fMaxRightRearAnkle;

	// 物体の摩擦係数
	float materialMu;

	/// 裏オプション
	bool noLegs;
	bool noHead;

	/// ダイナミカルを入れるかどうか
	bool dynamicalMode;
	/// 全体の体重
	double totalMass;
	/// fMaxを入れるかどうか
	bool flagFMax;
	/// 稼働域制限を入れるかどうか
	bool flagRange;

	CRFourLegsAnimalBodyGenDesc(bool enableRange = false, bool enableFMax = false);
};



// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRFourLegsAnimalBodyGen
// ボールやヒンジのジョイントを用いた哺乳類モデル・クラスの実装
class CRFourLegsAnimalBodyGen : public CRBodyGen, public CRFourLegsAnimalBodyGenDesc {
private:
	
	std::vector<PHSolidIf*> upperBody;		//< 上体を構成する剛体の登録先の配列
	
	void InitBody();						//< 体幹部の作成計画を立てる
	void CreateWaist();						//< 腰を作成する
	void CreateChest();						//< 胸を作成する
	void CreateTail();						//< 尾を作成する

	void InitHead();						//< 頭部の作成計画を立てる
	void CreateNeck();						//< 首を作成する
	void CreateHead();						//< 頭の作成をする

	void InitFrontLegs();					//< 前脚部の作成計画を立てる
	void CreateBreastBone(LREnum lr);		//< 上腕骨を作成する
	void CreateRadius(LREnum lr);			//< 前腕骨を作成する
	void CreateFrontCannonBone(LREnum lr);	//< 中手骨を作成する
	void CreateFrontToeBones(LREnum lr);	//< 指骨を作成する

	void InitRearLegs();					//< 後脚部の作成計画を立てる
	void CreateFemur(LREnum lr);			//< 大腿骨を作成する
	void CreateTibia(LREnum lr);			//< 脛骨を作成する
	void CreateRearCannonBone(LREnum lr);	//< 中足骨を作成する
	void CreateRearToeBones(LREnum lr);		//< 趾骨を作成する
	
	void InitEyes();						//< 感覚器の作成計画を立てる（未実装）
	void CreateEye(LREnum lr);				//< 目を作成する（未実装）

	void InitContact();						//< ボディ全ての剛体同士の接触を切る
	// void InitControlMode(PHJointDesc::PHControlMode m = PHJointDesc::MODE_POSITION);	//< ボディの制御モードを設定する．
	void SetUpperBody();					//< ボディの上体を構成する剛体を登録する
	
public:

	CRFourLegsAnimalBodyGen(){}
	CRFourLegsAnimalBodyGen(const CRFourLegsAnimalBodyGenDesc& desc, PHSceneIf* s=NULL) 
		: CRFourLegsAnimalBodyGenDesc(desc) 
		, CRBodyGen((const CRBodyGenDesc&)desc, s)
	{
		solids.resize(CRFourLegsAnimalBodyGenDesc::SO_NSOLIDS);
		for(unsigned int i = 0; i < solids.size(); i++) solids[i] = NULL;
		joints.resize(CRFourLegsAnimalBodyGenDesc::JO_NJOINTS);
		for(unsigned int i = 0; i < joints.size(); i++) joints[i] = NULL;

		InitBody();
		InitHead();
		InitFrontLegs();
		InitRearLegs();
		InitEyes();
		
		InitContact();
		// InitControlMode();
		Init();
	}

	// インタフェースの実装
	virtual Vec3d	GetUpperCenterOfMass();		//< 上体の剛体の重心を得る
	virtual int		NBallJoints();				//< ボディに含まれているボールジョイントの数を返す
	virtual int		NHingeJoints();				//< ボディに含まれているヒンジジョイントの数を返す
	virtual double	VSolid(int i);				//< i番目の剛体の体積を返す
	virtual double	VSolids();					//< 剛体の体積総和を返す
	virtual double	GetTotalMass();				//< 総質量を返す
	virtual void	SetTotalMass(double value);	//< 総質量を設定する
	virtual double  GetLegLength(int i);		//< 脚の長さを返す[0]:右前，[1]:左前, [2]:右後, [3]:左後
};

}
//@}

#endif//CRFOURLEGSANIMALBODYGEN_H

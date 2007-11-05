/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRBodyIf_H
#define SPR_CRBodyIf_H

#include <SprFoundation.h>
#include <SprPhysics.h>

namespace Spr{;

//@{

struct PHSolidIf;
struct PHJointIf;

struct CRBodyIf;
struct CRHingeHumanBodyIf;

// ------------------------------------------------------------------------------
/// クリーチャのボディモデルのインターフェイス
struct CRBodyIf : SceneObjectIf{
	IF_DEF(CRBody);

	/** @brief 初期化を行う
	*/
	virtual void Init()= 0;

	/** @brief 剛体の数を得る
	*/
	virtual int NSolids()= 0;

	/** @brief i番目の剛体を得る
	*/
	virtual PHSolidIf* GetSolid(int i)= 0;

	/** @brief 関節の数を得る
	*/
	virtual int NJoints()= 0;

	/** @brief i番目の関節を得る
	*/
	virtual PHJointIf* GetJoint(int i)= 0;
};

/// クリーチャのボディモデルのデスクリプタ
struct CRBodyDesc{
	DESC_DEF_FOR_OBJECT(CRBody);

	/// プラグ側を親剛体にするかソケット側を親剛体にするか
	//enum CRHumanJointOrder{
	enum CRCreatureJointOrder{
		SOCKET_PARENT,
		PLUG_PARENT,
	} jointOrder;

	CRBodyDesc(){
	}
};

// ------------------------------------------------------------------------------
/// ヒンジジョイント人体モデルのインターフェイス
struct CRHingeHumanBodyIf : CRBodyIf {
	IF_DEF(CRHingeHumanBody);

	/** @brief 上半身の関節の堅さを変える
		@param stifness 堅さ：デフォルトの堅さに対する倍数で指定
	*/
	virtual void SetUpperBodyStiffness(float stiffness)=0;

	/** @brief 上半身のポーズを保持する
	*/
	virtual void KeepUpperBodyPose()=0;

	/** @brief 上半身のポーズを初期位置に戻す
	*/
	virtual void ResetUpperBodyPose()=0;
};

/// ヒンジジョイント人体モデルのデスクリプタ
struct CRHingeHumanBodyDesc : CRBodyDesc {
	DESC_DEF_FOR_OBJECT(CRHingeHumanBody);

	enum CRHumanSolids{
		// Center
		SO_WAIST=0,
		SO_ABDOMEN, SO_CHEST, SO_NECK, SO_HEAD,

		// Right
		SO_RIGHT_UPPER_ARM, SO_RIGHT_LOWER_ARM, SO_RIGHT_HAND,
		SO_RIGHT_UPPER_LEG, SO_RIGHT_LOWER_LEG, SO_RIGHT_FOOT,
		SO_RIGHT_EYE,

		// Left
		SO_LEFT_UPPER_ARM, SO_LEFT_LOWER_ARM, SO_LEFT_HAND,
		SO_LEFT_UPPER_LEG, SO_LEFT_LOWER_LEG, SO_LEFT_FOOT,
		SO_LEFT_EYE,

		// 関節の中継ぎ用の形状を有しない剛体
		SO_CHEST_NECK_XZ, SO_CHEST_NECK_ZY,
		SO_NECK_HEAD_XZ,

		SO_RIGHT_SHOULDER_ZX, SO_RIGHT_SHOULDER_XY, 
		SO_RIGHT_WRIST_YX, SO_RIGHT_WRIST_XZ,
		SO_RIGHT_WAIST_LEG_ZX, SO_RIGHT_WAIST_LEG_XY, 
		SO_RIGHT_ANKLE_YX, SO_RIGHT_ANKLE_XZ,
		SO_RIGHT_EYE_YX,

		SO_LEFT_SHOULDER_ZX, SO_LEFT_SHOULDER_XY, 
		SO_LEFT_WRIST_YX, SO_LEFT_WRIST_XZ, 
		SO_LEFT_WAIST_LEG_ZX, SO_LEFT_WAIST_LEG_XY, 
		SO_LEFT_ANKLE_YX, SO_LEFT_ANKLE_XZ,
		SO_LEFT_EYE_YX,

		// 剛体の数
		SO_NSOLIDS
	};

	enum CRHumanJoints{
		// -- Center
		JO_WAIST_ABDOMEN=0, JO_ABDOMEN_CHEST,
		JO_CHEST_NECK_X, JO_CHEST_NECK_Z, JO_CHEST_NECK_Y,
		JO_NECK_HEAD_X, JO_NECK_HEAD_Z,

		// -- Right
		JO_RIGHT_SHOULDER_Z, JO_RIGHT_SHOULDER_X, JO_RIGHT_SHOULDER_Y,
		JO_RIGHT_ELBOW,
		JO_RIGHT_WRIST_Y, JO_RIGHT_WRIST_X, JO_RIGHT_WRIST_Z,

		JO_RIGHT_WAIST_LEG_Z, JO_RIGHT_WAIST_LEG_X, JO_RIGHT_WAIST_LEG_Y,
		JO_RIGHT_KNEE,
		JO_RIGHT_ANKLE_Y, JO_RIGHT_ANKLE_X, JO_RIGHT_ANKLE_Z,

		JO_RIGHT_EYE_Y, JO_RIGHT_EYE_X,

		// -- Left
		JO_LEFT_SHOULDER_Z, JO_LEFT_SHOULDER_X, JO_LEFT_SHOULDER_Y,
		JO_LEFT_ELBOW,
		JO_LEFT_WRIST_Y, JO_LEFT_WRIST_X, JO_LEFT_WRIST_Z,

		JO_LEFT_WAIST_LEG_Z, JO_LEFT_WAIST_LEG_X, JO_LEFT_WAIST_LEG_Y,
		JO_LEFT_KNEE,
		JO_LEFT_ANKLE_Y, JO_LEFT_ANKLE_X, JO_LEFT_ANKLE_Z,

		JO_LEFT_EYE_Y, JO_LEFT_EYE_X,

		// 関節の数
		JO_NJOINTS
	};

	/// サイズに関するパラメータ
	double waistHeight, waistBreadth, waistThickness;
	double abdomenHeight, abdomenBreadth, abdomenThickness;
	double chestHeight, chestBreadth, chestThickness;
	double neckLength, neckDiameter;
	double headDiameter;
	double upperArmLength, upperArmDiameter;
	double lowerArmLength, lowerArmDiameter;
	double handLength, handBreadth, handThickness;
	double upperLegLength, upperLegDiameter, interLegDistance;
	double lowerLegLength, lowerLegDiameter;
	double footLength, footBreadth, footThickness, ankleToeDistance;
	double vertexToEyeHeight, occiputToEyeDistance;
	double eyeDiameter, interpupillaryBreadth;

	/// 各関節のバネダンパ
	double springWaistAbdomen, damperWaistAbdomen;
	double springAbdomenChest, damperAbdomenChest;
	double springChestNeckX,   damperChestNeckX;
	double springChestNeckY,   damperChestNeckY;
	double springChestNeckZ,   damperChestNeckZ;
	double springNeckHeadX,    damperNeckHeadX;
	double springNeckHeadZ,    damperNeckHeadZ;
	double springShoulderZ,    damperShoulderZ;
	double springShoulderX,    damperShoulderX;
	double springShoulderY,    damperShoulderY;
	double springElbow,        damperElbow;
	double springWristY,       damperWristY;
	double springWristX,       damperWristX;
	double springWristZ,       damperWristZ;
	double springWaistLegZ,    damperWaistLegZ;
	double springWaistLegX,    damperWaistLegX;
	double springWaistLegY,    damperWaistLegY;
	double springKnee,         damperKnee;
	double springAnkleY,       damperAnkleY;
	double springAnkleX,       damperAnkleX;
	double springAnkleZ,       damperAnkleZ;
	double springEyeY,         damperEyeY;
	double springEyeX,         damperEyeX;

	/// 関節取り付け角度・位置
	Vec3d       posRightUpperArm;
	Quaterniond oriRightUpperArm;
	Quaterniond oriRightLowerArm;
	Quaterniond oriRightHand;

	/// 可動域制限
	Vec2d rangeWaistAbdomen;
	Vec2d rangeAbdomenChest;
	Vec2d rangeChestNeckX;
	Vec2d rangeChestNeckY;
	Vec2d rangeChestNeckZ;
	Vec2d rangeNeckHeadX;
	Vec2d rangeNeckHeadZ;
	Vec2d rangeShoulderZ;
	Vec2d rangeShoulderX;
	Vec2d rangeShoulderY;
	Vec2d rangeElbow;
	Vec2d rangeWristY;
	Vec2d rangeWristX;
	Vec2d rangeWristZ;
	Vec2d rangeWaistLegZ;
	Vec2d rangeWaistLegX;
	Vec2d rangeWaistLegY;
	Vec2d rangeKnee;
	Vec2d rangeAnkleY;
	Vec2d rangeAnkleX;
	Vec2d rangeAnkleZ;
	Vec2d rangeRightEyeY;
	Vec2d rangeEyeX;

	/// 裏オプション
	bool noLegs;

	CRHingeHumanBodyDesc(){
		jointOrder = PLUG_PARENT;

		waistHeight    = 0.2298;
		waistBreadth   = 0.3067;
		waistThickness = 0.2307;

		abdomenHeight    = 1.2253 - 1.0142;
		abdomenBreadth   = 0.2611;
		abdomenThickness = 0.1882;

		chestHeight    = 1.4020 - 1.2253;
		chestBreadth   = 0.2887;
		chestThickness = 0.2118;

		neckLength   = 1.7219 - 1.4564 - 0.1732;
		neckDiameter = 0.1442;

		headDiameter = 0.2387;

		upperArmLength   = 0.3406;
		upperArmDiameter = 0.2808 / 3.1415;

		lowerArmLength   = 0.2766;
		lowerArmDiameter = 0.1677 / 3.1415;

		handLength    = 0.1908;
		handBreadth   = 0.1018;
		handThickness = 0.0275;

		upperLegLength   = 0.4092;
		upperLegDiameter = 0.1682;
		interLegDistance = waistBreadth - upperLegDiameter;

		lowerLegLength   = 0.3946;
		lowerLegDiameter = 0.1109;

		footLength       = 0.2544;
		footBreadth      = 0.0994;
		footThickness    = 0.0619;
		ankleToeDistance = 0.2010;

		vertexToEyeHeight     = 0.1255;
		occiputToEyeDistance  = 0.1688;
		eyeDiameter           = 0.0240;
		interpupillaryBreadth = 0.0619;

		springWaistAbdomen =  10.0;  damperWaistAbdomen =   5.0;
		springAbdomenChest =  10.0;  damperAbdomenChest =   5.0;
		springChestNeckX   = 100.0;  damperChestNeckX   =  50.0;
		springChestNeckY   = 100.0;  damperChestNeckY   =  50.0;
		springChestNeckZ   = 100.0;  damperChestNeckZ   =  50.0;
		springNeckHeadX    =  50.0;  damperNeckHeadX    =  20.0;
		springNeckHeadZ    =  50.0;  damperNeckHeadZ    =  20.0;
		springShoulderZ    =   5.0;  damperShoulderZ    =   2.0;
		springShoulderX    =   5.0;  damperShoulderX    =   2.0;
		springShoulderY    =   5.0;  damperShoulderY    =   2.0;
		springElbow        =   5.0;  damperElbow        =   2.0;
		springWristY       =   5.0;  damperWristY       =   2.0;
		springWristX       =   5.0;  damperWristX       =   2.0;
		springWristZ       =  10.0;  damperWristZ       =   5.0;
		springWaistLegZ    =   5.0;  damperWaistLegZ    =   2.0;
		springWaistLegX    =   5.0;  damperWaistLegX    =   2.0;
		springWaistLegY    =   5.0;  damperWaistLegY    =   2.0;
		springKnee         =   5.0;  damperKnee         =   2.0;
		springAnkleY       =   5.0;  damperAnkleY       =   2.0;
		springAnkleX       =   5.0;  damperAnkleX       =   2.0;
		springAnkleZ       =  10.0;  damperAnkleZ       =   5.0;
		springEyeY         = 500.0;  damperEyeY         =   5.0;
		springEyeX         = 500.0;  damperEyeX         =   5.0;

		posRightUpperArm = Vec3d(0,0,0);

		// Vec2d(lower, upper)  lower>upperのとき可動域制限無効
		rangeWaistAbdomen = Vec2d(Rad(+360) , Rad(-360));
		rangeAbdomenChest = Vec2d(Rad(+360) , Rad(-360));
		rangeChestNeckX   = Vec2d(Rad(+360) , Rad(-360));
		rangeChestNeckY   = Vec2d(Rad(+360) , Rad(-360));
		rangeChestNeckZ   = Vec2d(Rad(+360) , Rad(-360));
		rangeNeckHeadX    = Vec2d(Rad(+360) , Rad(-360));
		rangeNeckHeadZ    = Vec2d(Rad(+360) , Rad(-360));
		rangeShoulderZ    = Vec2d(Rad(+360) , Rad(-360));
		rangeShoulderX    = Vec2d(Rad(+360) , Rad(-360));
		rangeShoulderY    = Vec2d(Rad(+360) , Rad(-360));
		rangeElbow        = Vec2d(Rad(+360) , Rad(-360));
		rangeWristY       = Vec2d(Rad(+360) , Rad(-360));
		rangeWristX       = Vec2d(Rad(+360) , Rad(-360));
		rangeWristZ       = Vec2d(Rad(+360) , Rad(-360));
		rangeWaistLegZ    = Vec2d(Rad(+360) , Rad(-360));
		rangeWaistLegX    = Vec2d(Rad(+360) , Rad(-360));
		rangeWaistLegY    = Vec2d(Rad(+360) , Rad(-360));
		rangeKnee         = Vec2d(Rad(+360) , Rad(-360));
		rangeAnkleY       = Vec2d(Rad(+360) , Rad(-360));
		rangeAnkleX       = Vec2d(Rad(+360) , Rad(-360));
		rangeAnkleZ       = Vec2d(Rad(+360) , Rad(-360));
		//rangeRightEyeY    = Vec2d(Rad( -30) , Rad( +45));
		//rangeEyeX         = Vec2d(Rad( -45) , Rad( +45));
		rangeRightEyeY    = Vec2d(Rad(+360) , Rad(-360));
		rangeEyeX         = Vec2d(Rad(+360) , Rad(-360));

		noLegs = false;
	}
};


// ------------------------------------------------------------------------------
/// ヒンジジョイント動物モデルのインターフェイス
struct CRHingeAnimalBodyIf : CRBodyIf {
	IF_DEF(CRHingeAnimalBody);

};

/// ヒンジジョイント動物モデルのデスクリプタ
struct CRHingeAnimalBodyDesc : CRBodyDesc {
	DESC_DEF_FOR_OBJECT(CRHingeAnimalBody);

	enum CRAnimalSolids{
		// Center
		SO_WAIST=0,
		SO_CHEST, SO_TAIL1, SO_TAIL2, SO_TAIL3,
		
		//Head parts
		SO_NECK, SO_HEAD, 

		// Right
		SO_RIGHT_BREASTBONE, SO_RIGHT_RADIUS, SO_RIGHT_FRONT_CANNON_BONE, SO_RIGHT_FRONT_TOE,
		SO_RIGHT_FEMUR , SO_RIGHT_TIBIA, SO_RIGHT_REAR_CANNON_BONE, SO_RIGHT_REAR_TOE,
		SO_RIGHT_EYE,

		// Left
		SO_LEFT_BREASTBONE, SO_LEFT_RADIUS, SO_LEFT_FRONT_CANNON_BONE, SO_LEFT_FRONT_TOE,
		SO_LEFT_FEMUR , SO_LEFT_TIBIA, SO_LEFT_REAR_CANNON_BONE, SO_LEFT_REAR_TOE,
		SO_LEFT_EYE,

		// 関節の中継ぎ用の形状を有しない剛体
		SO_CHEST_NECK_XY, SO_CHEST_NECK_YZ, 

		SO_RIGHT_SHOULDER_XY, SO_RIGHT_SHOULDER_YZ, 
		SO_RIGHT_FRONT_ANKLE_XY, SO_RIGHT_FRONT_ANKLE_YZ,
		SO_RIGHT_HIP_XY, SO_RIGHT_HIP_YZ, 
		SO_RIGHT_REAR_ANKLE_XY, SO_RIGHT_REAR_ANKLE_YZ,
		SO_RIGHT_EYE_XY,

		SO_LEFT_SHOULDER_XY, SO_LEFT_SHOULDER_YZ,
		SO_LEFT_FRONT_ANKLE_XY, SO_LEFT_FRONT_ANKLE_YZ, 
		SO_LEFT_HIP_XY, SO_LEFT_HIP_YZ, 
		SO_LEFT_REAR_ANKLE_XY, SO_LEFT_REAR_ANKLE_YZ,
		SO_LEFT_EYE_XY,

		// 剛体の数
		SO_NSOLIDS
	};

	enum CRAnimalJoints{
		// -- Center
		JO_WAIST_CHEST=0,
		JO_CHEST_NECK_X, JO_CHEST_NECK_Y, JO_CHEST_NECK_Z,
		JO_NECK_HEAD,
		JO_TAIL_WAIST, JO_TAIL_12, JO_TAIL_23,

		// -- Right
		JO_RIGHT_SHOULDER_Z, JO_RIGHT_SHOULDER_X, JO_RIGHT_SHOULDER_Y,
		JO_RIGHT_ELBOW,JO_RIGHT_FRONT_KNEE,
		JO_RIGHT_FRONT_ANKLE_Y, JO_RIGHT_FRONT_ANKLE_X, JO_RIGHT_FRONT_ANKLE_Z,

		JO_RIGHT_HIP_X, JO_RIGHT_HIP_Y, JO_RIGHT_HIP_Z,
		JO_RIGHT_STIFLE,JO_RIGHT_REAR_KNEE,
		JO_RIGHT_REAR_ANKLE_Y, JO_RIGHT_REAR_ANKLE_X, JO_RIGHT_REAR_ANKLE_Z,

		JO_RIGHT_EYE_Y, JO_RIGHT_EYE_X,

		// -- Left
		JO_LEFT_SHOULDER_Z, JO_LEFT_SHOULDER_X, JO_LEFT_SHOULDER_Y,
		JO_LEFT_ELBOW,JO_LEFT_FRONT_KNEE,
		JO_LEFT_FRONT_ANKLE_Y, JO_LEFT_FRONT_ANKLE_X, JO_LEFT_FRONT_ANKLE_Z,

		JO_LEFT_HIP_X, JO_LEFT_HIP_Y, JO_LEFT_HIP_Z,
		JO_LEFT_STIFLE,JO_LEFT_REAR_KNEE,
		JO_LEFT_REAR_ANKLE_Y, JO_LEFT_REAR_ANKLE_X, JO_LEFT_REAR_ANKLE_Z,

		JO_LEFT_EYE_Y, JO_LEFT_EYE_X,

		// 関節の数
		JO_NJOINTS
	};

	/// サイズに関するパラメータ
	double waistBreadth,	      waistHeight,			 waistThickness;
	double chestBreadth,	      chestHeight,			 chestThickness;
	double tailBreath,		      tailHeight,		     tailThickness;
	double neckBreath,		      neckHeight,			 neckThickness;
//	double headRadius,		      headLength;
	double headBreath,		      headHeight,			 headThickness;
	double breathtboneBreath,     breathtboneHeight,	 breathtboneThickness;
	double radiusBreath,	      radiusHeight,		     radiusThickness;
	double frontCannonBoneBreath, frontCannonBoneHeight, frontCannonBoneThickness;
	double frontToeBreath,		  frontToeHeight,		 frontToeThickness;
	double femurBreath,			  femurHeight,			 femurThickness;
	double tibiaBreath,			  tibiaHeight,			 tibiaThickness;
	double rearCannonBoneBreath,  rearCannonBoneHeight,	 rearCannonBoneThickness;
	double rearToeBreath,		  rearToeHeight,		 rearToeThickness;

	/// 各関節のバネダンパ
	double springWaistChest,   damperWaistChest;	//腰-胸
	double springTailWaist,	   damperTailWaist;		//腰-尾
	double springTail,		   damperTail;			//尾
	double springChestNeckXY,  damperChestNeckXY;	//胸-首
	double springChestNeckYZ,  damperChestNeckYZ;
	double springChestNeck,	   damperChestNeck;
	double springNeckHead,	   damperNeckHead;		//首-頭
	double springShoulder_X,   damperShoulder_X;	//肩
	double springShoulder_Y,   damperShoulder_Y;
	double springShoulder_Z,   damperShoulder_Z;
	double springElbow,		   damperElbow;			//肘（前足）
	double springFrontKnee,	   damperFrontKnee;		//膝（前足）
	double springFrontAnkle_X, damperFrontAnkle_X;	//かかと（前足）
	double springFrontAnkle_Y, damperFrontAnkle_Y;
	double springFrontAnkle_Z, damperFrontAnkle_Z;
	double springHip_X,		   damperHip_X;			//尻
	double springHip_Y,		   damperHip_Y;
	double springHip_Z,		   damperHip_Z;
	double springStifle,	   damperStifle;		//肘？（後足）
	double springRearKnee,	   damperRearKnee;		//膝（後足）
	double springRearAnkle_X,  damperRearAnkle_X;	//かかと（後足）
	double springRearAnkle_Y,  damperRearAnkle_Y;
	double springRearAnkle_Z,  damperRearAnkle_Z;
	
	/// 関節取り付け角度・位置
	//Vec3d       posRightFrontUpperLeg;
	//Quaterniond oriRightFrontUpperLeg;
	
	/// 可動域制限hingejoint
	Vec2d  rangeChestNeckXY;
	Vec2d  rangeChestNeckYZ;
	Vec2d  rangeChestNeck;
	Vec2d  rangeNeckHead;
	Vec2d  rangeShoulder_X;
	Vec2d  rangeShoulder_Y;
	Vec2d  rangeShoulder_Z;
	Vec2d  rangeElbow;
	Vec2d  rangeFrontKnee;
	Vec2d  rangeFrontAnkle_X;
	Vec2d  rangeFrontAnkle_Y;
	Vec2d  rangeFrontAnkle_Z;
	Vec2d  rangeHip_X;
	Vec2d  rangeHip_Y;
	Vec2d  rangeHip_Z;
	Vec2d  rangeStifle;
	Vec2d  rangeRearKnee;
	Vec2d  rangeRearAnkle_X;
	Vec2d  rangeRearAnkle_Y;
	Vec2d  rangeRearAnkle_Z;

	//ball joints取り付け位置・角度
	Quaterniond oriTailWaist;
	Quaterniond oriTail;
	Quaterniond oriNeckHead;

	/// 可動域制限balljoint
	double rangeWaistChest;
	double rangeTailWaist;
	double rangeTail;
//	double rangeNeckHead;

	/// 裏オプション
	bool noLegs;

	CRHingeAnimalBodyDesc(){
		jointOrder = SOCKET_PARENT;

		waistBreadth   = 0.6;
		waistHeight    = 0.8;
		waistThickness = 0.65;

		chestBreadth   = 0.6;
		chestHeight    = 1.5;
		chestThickness = 0.65;

		tailBreath    = 0.1;
		tailHeight    = 0.2;
		tailThickness = 0.1;

		neckBreath	  = 0.5;
		neckHeight	  = 1.0;
		neckThickness = 0.4;

//		headRadius    = 0.3;
//		headLength    = 0.5;

		headBreath    = 0.4;
		headHeight	  = 0.7;
		headThickness = 0.3;

		breathtboneBreath    = 0.2;
		breathtboneHeight    = 0.5;
		breathtboneThickness = 0.2; 

		radiusBreath	= 0.2;
		radiusHeight	= 0.7;
		radiusThickness = 0.2;

		frontCannonBoneBreath	 = 0.15;
		frontCannonBoneHeight	 = 0.6;
		frontCannonBoneThickness = 0.15;

		frontToeBreath	  = 0.25;
		frontToeHeight	  = 0.1;
		frontToeThickness = 0.25;

		femurBreath    = 0.25;
		femurHeight	   = 0.65;
		femurThickness = 0.25;

		tibiaBreath	   = 0.2;
		tibiaHeight	   = 0.5;
		tibiaThickness = 0.2;

		rearCannonBoneBreath    = 0.15;
		rearCannonBoneHeight    = 0.65;
		rearCannonBoneThickness = 0.15;

		rearToeBreath    = 0.25;
		rearToeHeight    = 0.1;
		rearToeThickness = 0.25;

		// spring and damper
		springWaistChest   = 500.0;  damperWaistChest	= 200.0;
		springTailWaist    =  50.0;  damperTailWaist	=  20.0;
		springTail		   =   0.0;  damperTail			=   0.0;
		springChestNeckXY  =  50.0;  damperChestNeckXY	=  20.0;
		springChestNeckYZ  =  50.0;  damperChestNeckYZ	=  20.0;
		springChestNeck    =  50.0;  damperChestNeck	=  20.0;
		springNeckHead	   =  50.0;  damperNeckHead		=  20.0;
		springShoulder_X   =  50.0;  damperShoulder_X	=  20.0;
		springShoulder_Y   =  50.0;  damperShoulder_Y	=  20.0;
		springShoulder_Z   =  50.0;  damperShoulder_Z	=  20.0;
		springElbow		   =  50.0;  damperElbow		=  20.0;
		springFrontKnee	   =  50.0;  damperFrontKnee	=  20.0;
		springFrontAnkle_X =  50.0;  damperFrontAnkle_X	=  20.0;
		springFrontAnkle_Y =  50.0;  damperFrontAnkle_Y	=  20.0;
		springFrontAnkle_Z =  50.0;  damperFrontAnkle_Z	=  20.0;
		springHip_X		   =  50.0;  damperHip_X		=  20.0;
		springHip_Y		   =  50.0;  damperHip_Y		=  20.0;
		springHip_Z		   =  50.0;  damperHip_Z		=  20.0;
		springStifle	   =  50.0;  damperStifle		=  20.0;
		springRearKnee	   =  50.0;  damperRearKnee		=  20.0;
		springRearAnkle_X  =  50.0;  damperRearAnkle_X	=  20.0;
		springRearAnkle_Y  =  50.0;  damperRearAnkle_Y	=  20.0;
		springRearAnkle_Z  =  50.0;  damperRearAnkle_Z	=  20.0;
		
		// origin of ball joints
		oriTailWaist = Quaterniond::Rot(Rad(+60), 'x');
		oriTail		 = Quaterniond::Rot(Rad(-30), 'x');		
//		oriNeckHead	 = Quaterniond::Rot(Rad(+60), 'x');

		// Range of  ball joints
		rangeWaistChest = Rad(+40);
		rangeTailWaist  = Rad(+30);
		rangeTail		= Rad(+20);
//		rangeNeckHead   = Rad(+30);

		// Range of hinge joints (Vec2d(lower, upper)  lower>upperのとき可動域制限無効)
		rangeChestNeckXY  = Vec2d(Rad(-360), Rad(+360));
		rangeChestNeckYZ  = Vec2d(Rad(-360), Rad(+360));
		rangeChestNeck    = Vec2d(Rad(-360), Rad(+360));
		rangeNeckHead	  = Vec2d(Rad(   0), Rad(+180));
		rangeShoulder_X   = Vec2d(Rad(- 90), Rad(+120));
		rangeShoulder_Y   = Vec2d(Rad(- 30), Rad(+ 30));
		rangeShoulder_Z	  = Vec2d(Rad(- 30), Rad(+ 30));
		rangeElbow		  = Vec2d(Rad(- 90), Rad(+ 90));
		rangeFrontKnee	  = Vec2d(Rad(- 90), Rad(   0));
		rangeFrontAnkle_X = Vec2d(Rad(- 30), Rad(+ 30));
		rangeFrontAnkle_Y = Vec2d(Rad(- 30), Rad(+ 30));
		rangeFrontAnkle_Z = Vec2d(Rad(- 20), Rad(+ 20));
		rangeHip_X		  = Vec2d(Rad(+ 30), Rad(+180));
		rangeHip_Y		  = Vec2d(Rad(- 30), Rad(+ 30));
		rangeHip_Z		  = Vec2d(Rad(- 30), Rad(+ 30));
		rangeStifle		  = Vec2d(Rad(- 90), Rad(+ 90));
		rangeRearKnee	  = Vec2d(Rad(- 90), Rad(+ 90));
		rangeRearAnkle_X  = Vec2d(Rad(- 30), Rad(+ 30));
		rangeRearAnkle_Y  = Vec2d(Rad(- 30), Rad(+ 30));
		rangeRearAnkle_Z  = Vec2d(Rad(- 20), Rad(+ 20));


		noLegs = false;
	}
};





// ------------------------------------------------------------------------------
/// おもに胴体と足のみの人体モデルのインターフェイス（歩行制御用）
struct CRTrunkFootHumanBodyIf : CRBodyIf {
	IF_DEF(CRTrunkFootHumanBody);

};

/// 胴体・足人体モデルのデスクリプタ
struct CRTrunkFootHumanBodyDesc : CRBodyDesc {
	DESC_DEF_FOR_OBJECT(CRTrunkFootHumanBody);

	enum CRHumanSolids{
		// 剛体
		SO_WAIST=0, SO_CHEST, SO_HEAD, SO_RIGHT_FOOT, SO_LEFT_FOOT,
		// 剛体の数
		SO_NSOLIDS
	};

	enum CRHumanJoints{
		// 関節
		JO_WAIST_CHEST=0, JO_CHEST_HEAD,
		// 関節の数
		JO_NJOINTS
	};

	/// サイズに関するパラメータ
	double waistHeight, waistBreadth, waistThickness;
	double chestHeight, chestBreadth, chestThickness;
	double neckLength;
	double headDiameter;
	double footLength, footBreadth, footThickness;

	/// 各関節のバネダンパ
	double springWaistChest, damperWaistChest;
	double springChestHead,    damperChestHead;

	/// 可動域制限
	Vec2d rangeWaistChest;
	Vec2d rangeChestHead;

	CRTrunkFootHumanBodyDesc(){
		jointOrder = PLUG_PARENT;

		waistHeight    = 0.2298;
		waistBreadth   = 0.3067;
		waistThickness = 0.2307;

		chestHeight    = 1.4020 - 1.2253 + 1.2253 - 1.0142;
		chestBreadth   = 0.2887;
		chestThickness = 0.2118;

		neckLength   = 1.7219 - 1.4564 - 0.1732;
		headDiameter = 0.2387;

		footLength       = 0.2544;
		footBreadth      = 0.0994;
		footThickness    = 0.0619;

		springWaistChest   = 100.0;  damperWaistChest   =  50.0;
		springChestHead    = 100.0;  damperChestHead    =  50.0;

		// Vec2d(lower, upper)  lower>upperのとき可動域制限無効
		rangeWaistChest   = Vec2d(Rad(0.0) , Rad(0.01));
		rangeChestHead    = Vec2d(Rad(0.0) , Rad(0.01));
	}
};
//@}

// ------------------------------------------------------------------------------
/// おもに胴体と足のみの哺乳類モデルのインターフェイス（制御用）
struct CRTrunkFootAnimalBodyIf : CRBodyIf{
	IF_DEF(CRTrunkFootAnimalBody);
};

/// 胴体と足のみの哺乳類モデルのディスクリプタ
struct CRTrunkFootAnimalBodyDesc : CRBodyDesc{
	DESC_DEF_FOR_OBJECT(CRTrunkFootAnimalBody);

	enum CRAnimalSolids{
		// 剛体
		SO_WAIST=0, SO_CHEST, SO_HEAD, SO_RIGHT_FOOT, SO_LEFT_FOOT,
		// 剛体の数
		SO_NSOLIDS
	};

	enum CRAnimalJoints{
		// 関節
		JO_WAIST_CHEST=0, JO_CHEST_HEAD,
		// 関節の数
		JO_NJOINTS
	};

	/// サイズに関するパラメータ
	double waistHeight, waistBreadth, waistThickness;
	double chestHeight, chestBreadth, chestThickness;
	double neckLength;
	double headDiameter;
	double footLength, footBreadth, footThickness;

	/// 各関節のバネダンパ
	double springWaistChest, damperWaistChest;
	double springChestHead,    damperChestHead;

	/// 可動域制限
	Vec2d rangeWaistChest;
	Vec2d rangeChestHead;

	CRTrunkFootAnimalBodyDesc(){
		
		jointOrder = PLUG_PARENT;

		waistHeight    = 0.2298;
		waistBreadth   = 0.3067;
		waistThickness = 0.2307;

		chestHeight    = 1.4020 - 1.2253 + 1.2253 - 1.0142;
		chestBreadth   = 0.2887;
		chestThickness = 0.2118;

		neckLength   = 1.7219 - 1.4564 - 0.1732;
		headDiameter = 0.2387;

		footLength       = 0.2544;
		footBreadth      = 0.0994;
		footThickness    = 0.0619;

		springWaistChest   = 100.0;  damperWaistChest   =  50.0;
		springChestHead    = 100.0;  damperChestHead    =  50.0;

		// Vec2d(lower, upper)  lower>upperのとき可動域制限無効
		rangeWaistChest   = Vec2d(Rad(0.0) , Rad(0.01));
		rangeChestHead    = Vec2d(Rad(0.0) , Rad(0.01));
	}
};

}

#endif//SPR_CRHingeHumanBodyIf_H

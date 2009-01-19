/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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
struct CRFourLegsAnimalBodyIf;

// ------------------------------------------------------------------------------
/// クリーチャのボディモデルのインターフェイス
struct CRBodyIf : SceneObjectIf{
	SPR_IFDEF(CRBody);

	/** @brief 初期化を行う
	*/
	void Init();

	/** @brief 剛体の数を得る
	*/
	int NSolids();

	/** @brief i番目の剛体を得る
	*/
	PHSolidIf* GetSolid(int i);

	/** @brief 関節の数を得る
	*/
	int NJoints();

	/** @brief i番目の関節を得る
	*/
	PHJointIf* GetJoint(int i);

	/** @brief IKノードの数を得る
	*/
	int NIKNodes();

	/** @brief i番目のIKノードを得る
	*/
	PHIKNodeIf* GetIKNode(int i);

	/** @brief IK制御点の数を得る
	*/
	int NControlPoints();

	/** @brief i番目のIK制御点を得る
	*/
	PHIKControlPointIf* GetControlPoint(int i);

	/** @brief ボディの重心座標を得る
	*/
	Vec3d GetCenterOfMass();

	/** @brief ボディの質量を得る
	*/
	double GetSumOfMass();
};

/// クリーチャのボディモデルのデスクリプタ
struct CRBodyDesc{
	SPR_DESCDEF(CRBody);

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
	SPR_IFDEF(CRHingeHumanBody);

	/** @brief 上半身の関節の堅さを変える
		@param stifness 堅さ：デフォルトの堅さに対する倍数で指定
	*/
	void SetUpperBodyStiffness(float stiffness);

	/** @brief 上半身のポーズを保持する
	*/
	void KeepUpperBodyPose();

	/** @brief 上半身のポーズを初期位置に戻す
	*/
	void ResetUpperBodyPose();
};

/// ヒンジジョイント人体モデルのデスクリプタ
struct CRHingeHumanBodyDesc : CRBodyDesc {
	SPR_DESCDEF(CRHingeHumanBody);

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

	/// 体重
	double bodyMass;

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

	/// 関節バネダンパ係数
	double spring, damper;

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

	CRHingeHumanBodyDesc();
};


// ------------------------------------------------------------------------------
/// ボールジョイント人体モデルのインターフェイス
struct CRBallHumanBodyIf : CRBodyIf {
	SPR_IFDEF(CRBallHumanBody);
};

/// ボールジョイント人体モデルのデスクリプタ
struct CRBallHumanBodyDesc : CRBodyDesc {
	SPR_DESCDEF(CRBallHumanBody);

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

		// 剛体の数
		SO_NSOLIDS
	};

	enum CRHumanJoints{
		// -- Center
		JO_WAIST_ABDOMEN=0, JO_ABDOMEN_CHEST,
		JO_CHEST_NECK, JO_NECK_HEAD,

		// -- Right
		JO_RIGHT_SHOULDER, JO_RIGHT_ELBOW, JO_RIGHT_WRIST,
		JO_RIGHT_WAIST_LEG, JO_RIGHT_KNEE, JO_RIGHT_ANKLE,
		JO_RIGHT_EYE,

		// -- Left
		JO_LEFT_SHOULDER, JO_LEFT_ELBOW, JO_LEFT_WRIST,
		JO_LEFT_WAIST_LEG, JO_LEFT_KNEE, JO_LEFT_ANKLE,
		JO_LEFT_EYE,

		// 関節の数
		JO_NJOINTS
	};

	/// 体重
	double bodyMass;

	/// サイズに関するパラメータ
	double waistLength, waistBreadth;
	double abdomenLength, abdomenBreadth;
	double chestLength, chestBreadth, bideltoidBreadth;
	double neckLength, neckDiameter, neckPosZ, neckPosY;
	double headBreadth, headHeight, bigonialBreadth;
	double upperArmLength, upperArmDiameter;
	double lowerArmLength, lowerArmDiameter;
	double handLength, handBreadth, handThickness;
	double upperLegLength, upperLegDiameter, interLegDistance, legPosY;
	double lowerLegLength, lowerLegDiameter, kneeDiameter, ankleDiameter;
	double calfPosY, calfPosZ, calfDiameter;
	double footLength, footBreadth, footThickness, ankleToeDistance;
	double vertexToEyeHeight, occiputToEyeDistance;
	double eyeDiameter, interpupillaryBreadth;

	/// 各関節のバネダンパ
	double springWaistAbdomen, damperWaistAbdomen;
	double springAbdomenChest, damperAbdomenChest;
	double springChestNeck,    damperChestNeck;
	double springNeckHead,     damperNeckHead;
	double springShoulder,     damperShoulder;
	double springElbow,        damperElbow;
	double springWrist,        damperWrist;
	double springWaistLeg,     damperWaistLeg;
	double springKnee,         damperKnee;
	double springAnkle,        damperAnkle;
	double springEye,          damperEye;

	// 可動域制限など（未実装）

	CRBallHumanBodyDesc();
};


// ------------------------------------------------------------------------------
/// ４足動物モデルのインターフェイス
struct CRFourLegsAnimalBodyIf : CRBodyIf {
	SPR_IFDEF(CRFourLegsAnimalBody);

	/** @brief 初期化を実行する
	*/
	void Init();
	
	/** @brief 状態の重心座標を返す
	*/
	Vec3d GetUpperCenterOfMass();

	/** @brief ボールジョイントの数を返す
	*/
	int NBallJoints();

	/** @brief ヒンジジョイントの数を返す
	*/
	int NHingeJoints();
	/** @brief i番目の剛体の体積を返す
	*/
	double VSolid(int i);
	/** @brief 剛体の体積総和を返す
	*/
	double VSolids();
	/** @brief ボディの総質量を返す
	*/
	double	GetTotalMass();
	/** @brief ボディの総質量を設定する
	*/
	void	SetTotalMass(double value);
	/** @brief ボディの脚の長さを返す
	*/
	double GetLegLength(int i);

};

/// 4足動物モデルのデスクリプタ
struct CRFourLegsAnimalBodyDesc : CRBodyDesc {
	SPR_DESCDEF(CRFourLegsAnimalBody);

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

	CRFourLegsAnimalBodyDesc(bool enableRange = false, bool enableFMax = false);
};

// ------------------------------------------------------------------------------
/// デバッグ用のヘビ状モデルのインターフェイス
struct CRDebugLinkBodyIf : CRBodyIf {
	SPR_IFDEF(CRDebugLinkBody);

	/** @brief ボールジョイントの数を返す
	*/
	int NBallJoints();

	/** @brief ヒンジジョイントの数を返す
	*/
	int NHingeJoints();
	
};

/// デバッグ用のヘビ状モデルのデスクリプタ
struct CRDebugLinkBodyDesc : CRBodyDesc {
	
	SPR_DESCDEF(CRDebugLinkBody);

	enum CDShapeMode{
		MODE_SPHERE=0, MODE_CAPSULE, MODE_BOX
	} shapeMode;

	enum PHJointMode{
		MODE_BALL=0, MODE_HINGE
	} jointMode;

	int soNSolids;
	int joNBallJoints;
	int joNHingeJoints;
	int joNJoints;

	/// サイズに関するパラメータ(radius, length)
	double radius, length;

	/// リンク一つの重さ
	double mass;

	/// 各BallJointのバネダンパ
	double spring, damper;

	/// HingeJoint可動域制限
	Vec2d  range;
	
	// BallJoint制御目標
	Quaterniond goal;

	/// BallJointのswing可動域:
	Vec2d limitSwing;

	/// BallJointのtwist可動域
	Vec2d limitTwist;

	// 関節の出せる力の最大値
	double fMax;

	/// ダイナミカルを入れるかどうか
	bool dynamicalMode;

	CRDebugLinkBodyDesc(bool enableRange = false, bool enableFMax = false);
};



// ------------------------------------------------------------------------------
/// おもに胴体と足のみの人体モデルのインターフェイス（歩行制御用）
struct CRTrunkFootHumanBodyIf : CRBodyIf {
	SPR_IFDEF(CRTrunkFootHumanBody);

};

/// 胴体・足人体モデルのデスクリプタ
struct CRTrunkFootHumanBodyDesc : CRBodyDesc {
	SPR_DESCDEF(CRTrunkFootHumanBody);

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

	CRTrunkFootHumanBodyDesc();

};
//@}

// ------------------------------------------------------------------------------
/// おもに胴体と足のみの哺乳類モデルのインターフェイス（制御用）
struct CRFourLegsTinyAnimalBodyIf : CRBodyIf{
	SPR_IFDEF(CRFourLegsTinyAnimalBody);
};

/// 胴体と足のみの哺乳類モデルのディスクリプタ
struct CRFourLegsTinyAnimalBodyDesc : CRBodyDesc{
	SPR_DESCDEF(CRFourLegsTinyAnimalBody);

	enum CRAnimalSolids{
		// 剛体
		SO_BODY=0, SO_HEAD,SO_RIGHT_FRONT_LEG, SO_LEFT_FRONT_LEG,
		SO_RIGHT_REAR_LEG, SO_LEFT_REAR_LEG,
		// 剛体の数
		SO_NSOLIDS
	};

	enum CRAnimalJoints{
		// 関節
		JO_BODY_HEAD=0,
		JO_BODY_RIGHT_FRONT_LEG, JO_BODY_LEFT_FRONT_LEG,
		JO_BODY_RIGHT_REAR_LEG,  JO_BODY_LEFT_REAR_LEG,
		// 関節の数
		JO_NJOINTS
	};

	/// サイズに関するパラメータ
	double bodyHeight, bodyBreadth, bodyThickness;
	double frontLegsBreadth, frontLegsHeight, frontLegsThickness;
	double rearLegsBreadth, rearLegsHeight, rearLegsThickness;

	/// 各関節のバネダンパ
	double springFront, damperFront;
	double springRear,  damperRear;

	/// 可動域制限
	Vec2d rangeFrontSwing;
	Vec2d rangeFrontTwist;
	Vec2d rangeRearSwing;
	Vec2d rangeRearTwist;

	bool noLegs;
	bool noHead;
	bool onlyOneLeg;
	bool hingeDebug;
	CRFourLegsTinyAnimalBodyDesc();
};

}

#endif//SPR_CRBODY_H

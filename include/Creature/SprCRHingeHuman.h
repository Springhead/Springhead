/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRHingeHumanIf_H
#define SPR_CRHingeHumanIf_H

#include <SprFoundation.h>
#include <SprPhysics.h>

namespace Spr{;

//@{

struct PHSolidIf;
struct PHJointIf;

// ------------------------------------------------------------------------------
/// ヒンジジョイント人体モデルのデスクリプタ
struct CRHingeHumanDesc{
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
		JO_WAIST_ABDOMEN, JO_ABDOMEN_CHEST,
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

	/// プラグ側を親剛体にするかソケット側を親剛体にするか
	enum CRHumanJointOrder{
		SOCKET_PARENT,
		PLUG_PARENT,
	} jointOrder;

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

	Vec3d       posRightUpperArm;
	Quaterniond oriRightUpperArm;
	Quaterniond oriRightLowerArm;
	Quaterniond oriRightHand;

	CRHingeHumanDesc(){
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
	}
};

/// ヒンジジョイント人体モデルのインターフェイス
struct CRHingeHumanIf : SceneObjectIf{
	IF_DEF(CRHingeHuman);

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

//@}

}

#endif//SPR_CRHingeHumanIf_H

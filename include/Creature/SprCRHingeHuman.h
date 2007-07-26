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

// ------------------------------------------------------------------------------
/// ヒンジジョイント人体モデルのデスクリプタ
struct CRHingeHumanDesc{
	enum CRHumanSolids{
		SO_WAIST=0,
		SO_ABDOMEN, SO_CHEST, SO_HEAD,
		SO_RIGHT_SHOULDER_ZY, SO_RIGHT_SHOULDER_YX, SO_RIGHT_UPPER_ARM, SO_RIGHT_LOWER_ARM,
		SO_RIGHT_WRIST_ZY, SO_RIGHT_WRIST_YX, SO_RIGHT_HAND,
		SO_RIGHT_EYE, SO_RIGHT_EYE_YX,
		SO_LEFT_SHOULDER_ZY, SO_LEFT_SHOULDER_YX, SO_LEFT_UPPERARM, SO_LEFT_LOWERARM,
		SO_LEFT_WRIST_ZY, SO_LEFT_WRIST_YX, SO_LEFT_HAND,
		SO_LEFT_EYE, SO_LEFT_EYE_YX,
	};

	enum CRHumanJoints{
		JO_WAIST_ABDOMEN, JO_ABDOMEN_CHEST, JO_NECK,
		JO_RIGHT_SHOULDER_Z, JO_RIGHT_SHOULDER_Y, JO_RIGHT_SHOULDER_X,
		JO_RIGHT_ELBOW,
		JO_RIGHT_WRIST_Z, JO_RIGHT_WRIST_Y, JO_RIGHT_WRIST_X,
		//JO_RIGHT_EYE_Y, JO_RIGHT_EYE_X,
		JO_RIGHT_EYE,
		JO_LEFT_SHOULDER_Z, JO_LEFT_SHOULDER_Y, JO_LEFT_SHOULDER_X,
		JO_LEFT_ELBOW,
		JO_LEFT_WRIST_Z, JO_LEFT_WRIST_Y, JO_LEFT_WRIST_X,
		//JO_LEFT_EYE_Y, JO_LEFT_EYE_X,
		JO_LEFT_EYE,
	};

	double headRadius, headLength;
	double chestWidth, chestHeight, chestThinkness;
	double abdomenWidth, abdomenHeight, abdomenThinkness;
	double waistWidth, waistHeight, waistThinkness;
	double upperarmRadius, upperarmLength;
	double lowerarmRadius, lowerarmLength;
	double handLength, handWidth, handThickness;

	CRHingeHumanDesc(){
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

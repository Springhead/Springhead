﻿/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRBONEIF_H
#define SPR_CRBONEIF_H

#include <Foundation/SprObject.h>

namespace Spr{;

struct PHSolidIf;
struct PHJointIf;
struct PHIKActuatorIf;
struct PHIKEndEffectorIf;

/// クリーチャのボーン（剛体一つ＋親ボーンへの関節一つ）
struct CRBoneIf : SceneObjectIf {
	SPR_IFDEF(CRBone);

	/** @brief ラベル（役割を示す文字列：Handなど）の取得
	 */
	const char* GetLabel() const;

	/** @brief ラベル（役割を示す文字列：Handなど）の設定
	 */
	void SetLabel(const char* str);

	/** @brief PHSolidを取得
	 */
	PHSolidIf* GetPHSolid();

	/** @brief PHSolidを設定
	 */
	void SetPHSolid(PHSolidIf* so);

	/** @brief IKエンドエフェクタを取得
	 */
	PHIKEndEffectorIf* GetIKEndEffector();

	/** @brief IKエンドエフェクタを設定
	 */
	void SetIKEndEffector(PHIKEndEffectorIf* ikEE);

	/** @brief PHJointを取得
	 */
	PHJointIf* GetPHJoint();

	/** @brief PHJointを設定
	 */
	void SetPHJoint(PHJointIf* jo);

	/** @brief IKアクチュエータを取得
	 */
	PHIKActuatorIf* GetIKActuator();

	/** @brief IKアクチュエータを設定
	 */
	void SetIKActuator(PHIKActuatorIf* ikAct);
};

struct CRBoneDesc {
	SPR_DESCDEF(CRBone);
	std::string	label;			///<	ラベル（役割を示す文字列：Handなど）
};

}

#endif//SPR_CRBONEIF_H
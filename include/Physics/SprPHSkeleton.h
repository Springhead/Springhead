/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/** \addtogroup gpPhysics 	*/
//@{

/**
 *	@file SprPHSkeleton.h
 *	@brief スケルトン
*/
#ifndef SPR_PHSKELETONIF_H
#define SPR_PHSKELETONIF_H

namespace Spr{;

struct PHBoneDesc {
	PHBoneDesc() {
	}
};

struct PHBoneIf : public SceneObjectIf{
public:
	SPR_IFDEF(PHBone);

	/** @brief Solidを設定する
	 */
	void SetSolid(PHSolidIf* solid);

	/** @brief Solidを取得する
	 */
	PHSolidIf* GetSolid();

	/** @brief Jointを設定する
	 */
	void SetJoint(PHJointIf* joint);

	/** @brief Jointを取得する
	 */
	PHJointIf* GetJoint();

	/** @brief 親Boneを設定する
	 */
	void SetParent(PHBoneIf* parent);

	/** @brief 親Boneを取得する
	 */
	PHBoneIf* GetParent();

	// ----- ----- ----- ----- -----

	/** @brief ボーンの長さをセットする
	 */
	void SetLength(double length);

	/** @brief ボーンの向きをセットする（子ボーンのPositionも同時にセットする）
	 */
	void SetDirection(Vec3d dir);

	/** @brief ボーンの位置をセットする（親ボーンのDirectionも同時にセットする）
	 */
	void SetPosition(Vec3d pos);

};

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

struct PHSkeletonDesc {
	PHSkeletonDesc() {
	}
};

struct PHSkeletonIf : public SceneObjectIf{
public:
	SPR_IFDEF(PHSkeleton);

	/** @brief Boneの数を取得する
	 */
	int NBones();

	/** @brief Boneを取得する
	 */
	PHBoneIf* GetBone(int i);

	/** @brief Boneを作成する
	 */
	PHBoneIf* CreateBone(PHBoneIf* parent, const PHBoneDesc& desc = PHBoneDesc());
};


}	//	namespace Spr

//@}

#endif

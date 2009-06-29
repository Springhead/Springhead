/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRBODYIF_H
#define SPR_CRBODYIF_H

#include <SprFoundation.h>
#include <SprPhysics.h>

namespace Spr{;

//@{

struct PHSolidIf;
struct PHJointIf;

// ------------------------------------------------------------------------------

/// クリーチャのボディを構成する要素
struct CRBodyPartIf : SceneObjectIf {
	SPR_IFDEF(CRBodyPart);

	/** @brief ラベル（役割を示す文字列：Handなど）の取得
	 */
	const char* GetLabel() const;

	/** @brief ラベル（役割を示す文字列：Handなど）の設定
	 */
	void SetLabel(const char* str);
};
struct CRBodyPartDesc {
	SPR_DESCDEF(CRBodyPart);
	std::string	label;			///<	ラベル（役割を示す文字列：Handなど）
};

/// クリーチャのボディを構成する剛体
struct CRSolidIf : CRBodyPartIf {
	SPR_IFDEF(CRSolid);

	/** @brief PHSolidを取得
	 */
	PHSolidIf* GetPHSolid();

	/** @brief PHSolidを設定
	 */
	void SetPHSolid(PHSolidIf* so);
};
struct CRSolidDesc : CRBodyPartDesc {
	SPR_DESCDEF(CRSolid);
};

/// クリーチャのボディを構成するIK制御点付きの剛体
struct CRIKSolidIf : CRSolidIf {
	SPR_IFDEF(CRIKSolid);

	/** @brief IK制御点の数を取得
	 */
	int NIKControlPoints();

	/** @brief i番目のIK制御点を取得
	 */
	PHIKControlPointIf* GetIKControlPoint(int i);

	/** @brief IK制御点を追加
	 */
	void AddIKControlPoint(PHIKControlPointIf* ikCP);
};
struct CRIKSolidDesc : CRSolidDesc {
	SPR_DESCDEF(CRSolid);
};

/// クリーチャのボディを構成する関節
struct CRJointIf : CRBodyPartIf {
	SPR_IFDEF(CRJoint);

	/** @brief PHJointを取得
	 */
	PHJointIf* GetPHJoint();

	/** @brief PHJointを設定
	 */
	void SetPHJoint(PHJointIf* jo);
};
struct CRJointDesc : CRBodyPartDesc {
	SPR_DESCDEF(CRJoint);
};

/// クリーチャのボディを構成するIKノード付きの関節
struct CRIKJointIf : CRJointIf {
	SPR_IFDEF(CRIKJoint);

	/** @brief IKノードの数を取得
	 */
	int NIKNodes();

	/** @brief i番目のIKノードを取得
	 */
	PHIKNodeIf* GetIKNode(int i);

	/** @brief IKノードを追加
	 */
	void AddIKNode(PHIKNodeIf* ikNd);
};
struct CRIKJointDesc : CRJointDesc {
	SPR_DESCDEF(CRJoint);
};

// ------------------------------------------------------------------------------

/// クリーチャのボディモデルのインターフェイス
struct CRBodyIf : SceneObjectIf{
	SPR_IFDEF(CRBody);

	/** @brief 構成剛体の数を得る
	*/
	int NSolids();

	/** @brief i番目の構成剛体を得る
	*/
	CRSolidIf* GetSolid(int i);

	/** @brief 構成関節の数を得る
	*/
	int NJoints();

	/** @brief i番目の構成関節を得る
	*/
	CRJointIf* GetJoint(int i);

	/** @brief ボディの重心座標を得る
	*/
	Vec3d GetCenterOfMass();

	/** @brief ボディの総質量を得る
	*/
	double GetSumOfMass();
};

/// クリーチャのボディモデルのデスクリプタ
struct CRBodyDesc{
	SPR_DESCDEF(CRBody);

	CRBodyDesc(){
	}
};

}

#endif//SPR_CRBODY_H

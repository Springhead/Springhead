/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRTRUNKFOOTHUMANBODYGEN_H
#define CRTRUNKFOOTHUMANBODYGEN_H

#include <Creature/CRBodyGenerator/CRBodyGen.h>

//@{
namespace Spr{;

/// 胴体・足人体モデルのデスクリプタ
struct CRTrunkFootHumanBodyGenDesc : CRBodyGenDesc {

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

	CRTrunkFootHumanBodyGenDesc();

};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRTrunkFootHumanBodyGen
// ヒンジジョイントを用いた人体モデル・クラスの実装
class CRTrunkFootHumanBodyGen : public CRBodyGen, public CRTrunkFootHumanBodyGenDesc {
private:
	void InitBody();
	void CreateWaist();
	void CreateChest();
	void CreateHead();

	void InitLegs();
	void CreateFoot(LREnum lr);

	void InitContact();

public:

	CRTrunkFootHumanBodyGen(){}
	CRTrunkFootHumanBodyGen(const CRTrunkFootHumanBodyGenDesc& desc, PHSceneIf* s=NULL) 
		: CRTrunkFootHumanBodyGenDesc(desc) 
		, CRBodyGen((const CRBodyGenDesc&)desc, s)
	{
		solids.resize(CRTrunkFootHumanBodyGenDesc::SO_NSOLIDS);
		joints.resize(CRTrunkFootHumanBodyGenDesc::JO_NJOINTS);

		InitBody();
		InitLegs();

		InitContact();
	}

	/** @brief 初期化を実行する
	*/
	virtual void Init();

};

}
//@}

#endif//CRTRUNKFOOTHUMANBODYGEN_H

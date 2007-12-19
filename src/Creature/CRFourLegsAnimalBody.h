/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRFOURLEGSANIMALBODY_H
#define CRFOURLEGSANIMALBODY_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"

#include <vector>

#include "CRBody.h"

//@{
namespace Spr{;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRFourLegsAnimalBody
// ボールやヒンジのジョイントを用いた哺乳類モデル・クラスの実装
class CRFourLegsAnimalBody : public CRBody, public CRFourLegsAnimalBodyIfInit, public CRFourLegsAnimalBodyDesc {
private:

	std::vector<PHSolidIf*> upperBody;	//< 上体を構成する剛体の登録先の配列
	
	/// 体幹部の作成
	void InitBody();
	void CreateWaist();
	void CreateChest();
	void CreateTail();

	/// 頭部の作成
	void InitHead();
	void CreateNeck();
	void CreateHead();

	/// 前脚部の作成
	void InitFrontLegs();
	void CreateBreastBone(LREnum lr);
	void CreateRadius(LREnum lr);
	void CreateFrontCannonBone(LREnum lr);
	void CreateFrontToeBones(LREnum lr);

	/// 後脚部の作成
	void InitRearLegs();
	void CreateFemur(LREnum lr);
	void CreateTibia(LREnum lr);
	void CreateRearCannonBone(LREnum lr);
	void CreateRearToeBones(LREnum lr);
	
	/// 感覚器の作成（未実装）
	void InitEyes();
	void CreateEye(LREnum lr);

	/// ボディ全ての剛体同士の接触を切る
	void InitContact();

	/// ボディの上体を構成する剛体を登録する
	void SetUpperBody();
	
public:
	OBJECTDEF(CRFourLegsAnimalBody, CRBody);
	ACCESS_DESC(CRFourLegsAnimalBody);

	CRFourLegsAnimalBody(){}
	CRFourLegsAnimalBody(const CRFourLegsAnimalBodyDesc& desc, CRCreatureIf* c=NULL) 
		: CRFourLegsAnimalBodyDesc(desc) 
		, CRBody((const CRBodyDesc&)desc, c)
	{
		solids.resize(CRFourLegsAnimalBodyDesc::SO_NSOLIDS);
		joints.resize(CRFourLegsAnimalBodyDesc::JO_NJOINTS);

		InitBody();
		InitHead();
		InitFrontLegs();
		InitRearLegs();
		InitEyes();
		

		InitContact();
	}

	

	/** @brief 初期化を実行する
	*/
	virtual void Init();
	
	/** @brief 上体の剛体の重心を得る
	*/
	Vec3d GetUpperCenterOfMass();

};

}
//@}

#endif//CRFOURLEGSANIMALBODY_H

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

	void SetUpperBody();					//< ボディの上体を構成する剛体を登録する
	
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

	virtual void Init();				//< 初期化を実行する
	
	Vec3d GetUpperCenterOfMass();		//< 上体の剛体の重心を得る

};

}
//@}

#endif//CRFOURLEGSANIMALBODY_H

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRFOURLEGSANIMALBODY_H
#define CRFOURLEGSANIMALBODY_H

#include <Springhead.h>

#include <Foundation/Object.h>

#include <vector>

#include "CRBody.h"

//@{
namespace Spr{;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRFourLegsAnimalBody
// ボールやヒンジのジョイントを用いた哺乳類モデル・クラスの実装
class CRFourLegsAnimalBody : public CRBody, public CRFourLegsAnimalBodyDesc {
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
	SPR_OBJECTDEF(CRFourLegsAnimalBody);
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
		Init();
	}

	// インタフェースの実装
	virtual void	Init();						//< 初期化を実行する
	virtual Vec3d	GetUpperCenterOfMass();		//< 上体の剛体の重心を得る
	virtual int		NSolids();					//< ボディに含まれている剛体の数を返す
	virtual int		NJoints();					//< ボディに含まれている関節の数を返す
	virtual int		NBallJoints();				//< ボディに含まれているボールジョイントの数を返す
	virtual int		NHingeJoints();				//< ボディに含まれているヒンジジョイントの数を返す
	virtual double	VSolid(int i);				//< i番目の剛体の体積を返す
	virtual double	VSolids();					//< 剛体の体積総和を返す
	virtual double	GetTotalMass();				//< 総質量を返す
	virtual void	SetTotalMass(double value);	//< 総質量を設定する
	virtual double  GetLegLength(int i);		//< 脚の長さを返す[0]:右前，[1]:左前, [2]:右後, [3]:左後
};

}
//@}

#endif//CRFOURLEGSANIMALBODY_H

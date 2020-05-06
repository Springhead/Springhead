/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CR_BODYGEN_H
#define CR_BODYGEN_H

#include <Foundation/Object.h>
#include <Physics/SprPHJoint.h>
#include <Physics/SprPHSolid.h>
#include <Creature/SprCRBody.h>
#include <Creature/SprCRBone.h>
#include <Creature/SprCRCreature.h>
#include <vector>

//@{
namespace Spr{;

struct PHSdkIf;
struct PHIKActuatorIf;
struct PHIKEndEffectorIf;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief ボディ構築クラスのベース
*/
struct CRBodyGenDesc{
	CRBodyGenDesc(){
	}
};

class CRBodyGen : public CRBodyGenDesc {
protected:
	enum LREnum{LEFTPART=-1, RIGHTPART=+1};

	// 対象Creature
	CRCreatureIf* crCreature;

	// 作成されるBody
	CRBodyIf* crBody;

	/// ボディを構成する剛体
	std::vector< UTRef<CRBoneIf> > solids;	
	
	/// ボディを構成する関節
	std::vector< UTRef<CRBoneIf> > joints;

	/** @brief 関節を作る
	*/
	CRBoneIf* CreateJoint(CRBoneIf* soChild, CRBoneIf* soParent, const PHHingeJointDesc& desc);
	CRBoneIf* CreateJoint(CRBoneIf* soChild, CRBoneIf* soParent, const PHBallJointDesc& desc);

	/** @brief 剛体を作る
	*/
	CRBoneIf* CreateSolid(const PHSolidDesc& desc);

	/** @brief すべての剛体の初期姿勢を目標角に収束したものとして初期化する	
	*/
	void SetInitPosition(PHSolidIf* parentSolid, PHJointIf* childJoint);

public:
	CRBodyGen(const CRBodyGenDesc& desc = CRBodyGenDesc());

	/** @brief ボディを生成する
	*/
	virtual CRBodyIf* Generate(CRCreatureIf* crCreature);


	// これらの関数はCRBodyに移動予定（mitake）
	#if 0
	/** @brief CDBoxのInertiaを計算する（慣性テンソルの対角要素には値が入って，慣性乗積は0）
		@param CDBoxDesc::boxsize, PHSolidDesc::mass
		@return 慣性テンソル
	*/
	Matrix3d CalcBoxInertia(Vec3d boxsize, double mass);

	/**
		@brief childのポテンシャルエネルギーを計算する
	*/
	double CalcPotential(Posed parentPos, PHSolidIf* parentSolid, PHJointIf* childJoint);

	/** @brief ボディの重心座標を得る
	*/
	virtual Vec3d GetCenterOfMass();
	
	/** @brief ボディの質量を得る
	*/
	virtual double GetSumOfMass();

	/**
		@brief 目標角・目標角速度に収束した場合の力学的エネルギーを返す
	*/
	virtual double GetTargetMechanicalEnergy(PHSolidIf* rootSolid);

	/**
		@brief 目標角・目標角速度に収束した場合の運動エネルギーを返す
	*/
	virtual double GetTargetKineticEnergy();

	/**
		@brief 目標角・目標角速度に収束した場合の位置エネルギーを返す
	*/
	virtual double GetTargetPotentialEnergy(PHSolidIf* rootSolid);

	/**
		@breif 各剛体の目標状態に収束した場合の位置エネルギーを返す
	*/
	 virtual double CalcTargetPotential(Posed parentPos, PHSolidIf* parentSolid, PHJointIf* childJoint);

	/**
		@brief 現在の力学的エネルギーを返す
	*/
	virtual double GetMechanicalEnergy();

	/**
		@brief 現在の運動エネルギーを返す
	*/
	virtual double GetKineticEnergy();

	/**
		@brief 現在の位置エネルギーを返す
	*/
	virtual double GetPotentialEnergy();
	#endif
};
}
//@}

#endif // CR_BODYGEN_H

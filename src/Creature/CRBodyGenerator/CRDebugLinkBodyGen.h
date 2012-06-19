/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRDEBUGLINKBODYGEN_H
#define CRDEBUGLINKBODYGEN_H

#include <Creature/CRBodyGenerator/CRBodyGen.h>

//@{
namespace Spr{;

// ------------------------------------------------------------------------------
/// デバッグ用のヘビ状モデルのデスクリプタ
struct CRDebugLinkBodyGenDesc : CRBodyGenDesc {
	
	/* 
		MODE_SPHEERE 全て球
		MODE_CAPSULE 全てカプセル
		MODE_BOX	 全てボックス
		MODE_MIX	 球とボックスが交互
	*/
	enum CDShapeMode{
		MODE_SPHERE=0, MODE_CAPSULE, MODE_BOX, MODE_MIX,
	} shapeMode;

	enum PHJointMode{
		MODE_BALL=0, MODE_HINGE, MODE_MIXED
	} jointMode;

	unsigned int soNSolids;
	unsigned int joNBallJoints;
	unsigned int joNHingeJoints;
	unsigned int joNJoints;

	/// サイズに関するパラメータ(radius, length)
	double radius, length;

	/// リンク一つの重さ
	double mass;

	/// 各BallJointのバネダンパ
	double spring, damper;

	/// HingeJoint可動域制限
	Vec2d  range;
	
	// BallJoint制御目標
	Quaterniond SetTargetPosition;

	/// BallJointのswing可動域:
	Vec2d limitSwing;

	/// BallJointのtwist可動域
	Vec2d limitTwist;

	// 関節の出せる力の最大値
	double fMax;

	/// 静止摩擦
	double mu0;

	/// 動摩擦
	double mu;


	CRDebugLinkBodyGenDesc(bool enableRange = false, bool enableFMax = false);
};



class CRDebugLinkBodyGen: public CRBodyGen, public CRDebugLinkBodyGenDesc{

private:
	PHBallJointDesc InitBallJointDesc();
	PHHingeJointDesc InitHingeJointDesc();
	void SolidFactory(CDShapeMode m);
	void JointFactory(PHJointMode m);
	void CreateBody();
	void InitBody();
	void InitContact();
	// void InitControlMode(PHJointDesc::PHControlMode m = PHJointDesc::MODE_POSITION);
public:

	CRDebugLinkBodyGen(){}
	CRDebugLinkBodyGen(const CRDebugLinkBodyGenDesc& desc, PHSceneIf* s =NULL)
		: CRDebugLinkBodyGenDesc(desc)
		, CRBodyGen((const CRBodyGenDesc&)desc, s)
	{
		CreateBody();
		InitBody();
		InitContact();
	}
	int NBallJoints(){return joNBallJoints;}
	int NHingeJoints(){return joNHingeJoints;}

};

}
//@}

#endif //< end of CRDEBUGLINKBODYGEN_H
/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRInverseKinematicsIf_H
#define SPR_CRInverseKinematicsIf_H

#include <SprFoundation.h>
#include <SprPhysics.h>

namespace Spr{;

//@{

// ------------------------------------------------------------------------------
/// ボディをIKで動作させるための制御点
struct CRIKControlIf : SceneObjectIf{
	IF_DEF(CRIKControl);

	/** @brief 目標地点を設定する
	*/
	virtual void SetGoal(Vec3d goal)= 0;
};

struct CRIKControlDesc{
	DESC_DEF_FOR_OBJECT(CRIKControl);

	PHSolidIf* solid;  ///< 制御点のある剛体
};

// --- 位置制御点
struct CRIKControlPosIf : CRIKControlIf{
	IF_DEF(CRIKControlPos);
};

struct CRIKControlPosDesc : CRIKControlDesc{
	DESC_DEF_FOR_OBJECT(CRIKControlPos);

	Vec3d pos;  ///< 制御点の位置（剛体ローカル座標系における）
};

// --- 姿勢制御点
struct CRIKControlOriIf : CRIKControlIf{
	IF_DEF(CRIKControlOri);
};

struct CRIKControlOriDesc : CRIKControlDesc{
	DESC_DEF_FOR_OBJECT(CRIKControlOri);
};

// ------------------------------------------------------------------------------
/// ボディをIKで動作させるための制御対象（関節・剛体）
struct CRIKMovableIf : SceneObjectIf{
	IF_DEF(CRIKMovable);

	/** @brief IKの計算準備をする
	*/
	virtual void PrepareSolve()= 0;

	/** @brief IKの計算繰返しの１ステップを実行する
	*/
	virtual void ProceedSolve()= 0;

	/** @brief 計算結果を取得する
	*/
	virtual PTM::VVector<double> GetValue()= 0;

	/** @brief 制御点を追加する
	*/
	virtual void AddIKControl(CRIKControlIf* control)= 0;
};

struct CRIKMovableDesc{
	DESC_DEF_FOR_OBJECT(CRIKMovable);

	float bias;  ///< 動かしやすさの係数

	CRIKMovableDesc(){ bias = 1.0f; }
};

// --- 位置制御可能な剛体
struct CRIKMovableSolidPosIf : CRIKMovableIf{
	IF_DEF(CRIKMovableSolidPos);
};

struct CRIKMovableSolidPosDesc : CRIKMovableDesc{
	DESC_DEF_FOR_OBJECT(CRIKMovableSolidPos);

	PHSolidIf* solid;  ///< 制御対象の剛体
};

// --- 姿勢制御可能な剛体
struct CRIKMovableSolidOriIf : CRIKMovableIf{
	IF_DEF(CRIKMovableSolidOri);
};

struct CRIKMovableSolidOriDesc : CRIKMovableDesc{
	DESC_DEF_FOR_OBJECT(CRIKMovableSolidOri);

	PHSolidIf* solid;  ///< 制御対象の剛体
};

// --- 角度制御可能なボールジョイント
struct CRIKMovableBallJointOriIf : CRIKMovableIf{
	IF_DEF(CRIKMovableBallJointOri);
};

struct CRIKMovableBallJointOriDesc : CRIKMovableDesc{
	DESC_DEF_FOR_OBJECT(CRIKMovableBallJointOri);

	PHBallJointIf* joint;  ///< 制御対象の関節
};

// --- 角度制御可能な三連ヒンジジョイント
struct CRIKMovable3HingeJointOriIf : CRIKMovableIf{
	IF_DEF(CRIKMovable3HingeJointOri);
};

struct CRIKMovable3HingeJointOriDesc : CRIKMovableDesc{
	DESC_DEF_FOR_OBJECT(CRIKMovable3HingeJointOri);

	PHHingeJointIf *joint1, *joint2, *joint3;  ///< 制御対象の関節
};

// --- 角度制御可能なヒンジジョイント
struct CRIKMovableHingeJointOriIf : CRIKMovableIf{
	IF_DEF(CRIKMovableHingeJointOri);
};

struct CRIKMovableHingeJointOriDesc : CRIKMovableDesc{
	DESC_DEF_FOR_OBJECT(CRIKMovableHingeJointOri);

	PHHingeJointIf *joint;  ///< 制御対象の関節
};

}

#endif//SPR_CRInverseKinematicsIf_H

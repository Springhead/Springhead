/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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
	SPR_IFDEF(CRIKControl);

	/** @brief 目標地点を設定する
	*/
	void SetGoal(Vec3d goal);

	/** @brief 目標地点を取得する
	*/
	Vec3d GetGoal();

	/** @brief 制御対象の剛体を設定する
	*/
	void SetSolid(PHSolidIf* solid);

	/** @brief 制御対象の剛体を取得する
	*/
	PHSolidIf* GetSolid();
};

struct CRIKControlDesc{
	SPR_DESCDEF(CRIKControl);

	PHSolidIf* solid;  ///< 制御点のある剛体
};

// --- 位置制御点
struct CRIKControlPosIf : CRIKControlIf{
	SPR_IFDEF(CRIKControlPos);
};

struct CRIKControlPosDesc : CRIKControlDesc{
	SPR_DESCDEF(CRIKControlPos);

	Vec3d pos;  ///< 制御点の位置（剛体ローカル座標系における）
};

// --- 姿勢制御点
struct CRIKControlOriIf : CRIKControlIf{
	SPR_IFDEF(CRIKControlOri);
};

struct CRIKControlOriDesc : CRIKControlDesc{
	SPR_DESCDEF(CRIKControlOri);
};

// ------------------------------------------------------------------------------
/// ボディをIKで動作させるための制御対象（関節・剛体）
struct CRIKMovableIf : SceneObjectIf{
	SPR_IFDEF(CRIKMovable);

	/** @brief IKの計算準備をする
	*/
	void PrepareSolve();

	/** @brief IKの計算繰返しの１ステップを実行する
	*/
	void ProceedSolve();

	/** @brief 計算結果を取得する
	*/
	PTM::VVector<double> GetValue();

	/** @brief 計算結果に従って制御対象を動かす
	*/
	void Move();

	/** @brief 制御点を追加する
	*/
	void AddIKControl(CRIKControlIf* control);
};

struct CRIKMovableDesc{
	SPR_DESCDEF(CRIKMovable);

	float bias;  ///< 動かしやすさの係数

	CRIKMovableDesc(){ bias = 1.0f; }
};

// --- 位置制御可能な剛体
struct CRIKMovableSolidPosIf : CRIKMovableIf{
	SPR_IFDEF(CRIKMovableSolidPos);
};

struct CRIKMovableSolidPosDesc : CRIKMovableDesc{
	SPR_DESCDEF(CRIKMovableSolidPos);

	PHSolidIf* solid;  ///< 制御対象の剛体
};

// --- 姿勢制御可能な剛体
struct CRIKMovableSolidOriIf : CRIKMovableIf{
	SPR_IFDEF(CRIKMovableSolidOri);
};

struct CRIKMovableSolidOriDesc : CRIKMovableDesc{
	SPR_DESCDEF(CRIKMovableSolidOri);

	PHSolidIf* solid;  ///< 制御対象の剛体
};

// --- 角度制御可能なボールジョイント
struct CRIKMovableBallJointOriIf : CRIKMovableIf{
	SPR_IFDEF(CRIKMovableBallJointOri);
};

struct CRIKMovableBallJointOriDesc : CRIKMovableDesc{
	SPR_DESCDEF(CRIKMovableBallJointOri);

	PHBallJointIf* joint;  ///< 制御対象の関節
};

// --- 角度制御可能な三連ヒンジジョイント
struct CRIKMovable3HingeJointOriIf : CRIKMovableIf{
	SPR_IFDEF(CRIKMovable3HingeJointOri);
};

struct CRIKMovable3HingeJointOriDesc : CRIKMovableDesc{
	SPR_DESCDEF(CRIKMovable3HingeJointOri);
	PHHingeJointIf *joint1;
	PHHingeJointIf *joint2;
	PHHingeJointIf* joint3;  ///< 制御対象の関節
};

// --- 角度制御可能なヒンジジョイント
struct CRIKMovableHingeJointOriIf : CRIKMovableIf{
	SPR_IFDEF(CRIKMovableHingeJointOri);
};

struct CRIKMovableHingeJointOriDesc : CRIKMovableDesc{
	SPR_DESCDEF(CRIKMovableHingeJointOri);

	PHHingeJointIf *joint;  ///< 制御対象の関節
};

}

#endif//SPR_CRInverseKinematicsIf_H

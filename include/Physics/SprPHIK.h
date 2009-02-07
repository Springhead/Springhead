/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_PHIKIf_H
#define SPR_PHIKIf_H

#include <SprFoundation.h>
#include <SprPhysics.h>

namespace Spr{;

//@{

// ------------------------------------------------------------------------------
/// IKの制御点
struct PHIKControlPointIf : SceneObjectIf{
	SPR_IFDEF(PHIKControlPoint);

	/** @brief 力を設定する
	*/
	void SetForce(Vec3d force);

	/** @brief 力を取得する
	*/
	Vec3d GetForce();

	/** @brief 制御対象の剛体を設定する
	*/
	void SetSolid(PHSolidIf* solid);

	/** @brief 制御対象の剛体を取得する
	*/
	PHSolidIf* GetSolid();

	/** @brief 番号を設定する
	*/
	void SetNumber(int i);

	/** @brief 有効・無効を設定する
	*/
	void Enable(bool enable);
};

struct PHIKControlPointDesc{
	SPR_DESCDEF(PHIKControlPoint);

	PHSolidIf* solid;  ///< 制御点のある剛体
};

// --- 位置制御点
struct PHIKPosCtlIf : PHIKControlPointIf{
	SPR_IFDEF(PHIKPosCtl);

	/** @brief 目標地点を設定する
	*/
	void SetGoal(Vec3d goal);

	/** @brief 目標地点を取得する
	*/
	Vec3d GetGoal();

	/** @brief 制御点の位置を設定する
	*/
	void SetPos(Vec3d pos);

	/** @brief 制御点の位置を取得する
	*/
	Vec3d GetPos();
};

struct PHIKPosCtlDesc : PHIKControlPointDesc{
	SPR_DESCDEF(PHIKPosCtl);

	Vec3d pos;  ///< 制御点の位置（剛体ローカル座標系における）
};

// --- 姿勢制御点
struct PHIKOriCtlIf : PHIKControlPointIf{
	SPR_IFDEF(PHIKOriCtl);

	/** @brief 目標地点を設定する
	*/
	void SetGoal(Quaterniond goal);

	/** @brief 目標地点を取得する
	*/
	Quaterniond GetGoal();
};

struct PHIKOriCtlDesc : PHIKControlPointDesc{
	SPR_DESCDEF(PHIKOriCtl);
};

// ------------------------------------------------------------------------------
/// IKの制御に用いることのできる可動部品（関節・剛体など）
struct PHIKNodeIf : SceneObjectIf{
	SPR_IFDEF(PHIKNode);

	/** @brief IKの計算準備をする
	*/
	void PrepareSolve();

	/** @brief IKの計算繰返しの１ステップを実行する
	*/
	void ProceedSolve();

	/** @brief 計算結果を取得する
	*/
	PTM::VVector<double> GetDTheta();

	/** @brief 計算結果に従って制御対象を動かす
	*/
	void Move();

	/** @brief 制御点を追加する
	*/
	void AddControlPoint(PHIKControlPointIf* control);

	/** @brief 動かしにくさを設定する
	*/
	void SetBias(float bias);

	/** @brief 動かしにくさを取得する
	*/
	float GetBias();

	/** @brief 番号を設定する
	*/
	void SetNumber(int i);
};

struct PHIKNodeDesc{
	SPR_DESCDEF(PHIKNode);

	float bias;  ///< 動かしにくさの係数

	PHIKNodeDesc(){ bias = 1.0f; }
};

// --- 位置制御可能な剛体
struct PHIKSolidIf : PHIKNodeIf{
	SPR_IFDEF(PHIKSolid);
};

struct PHIKSolidDesc : PHIKNodeDesc{
	SPR_DESCDEF(PHIKSolid);

	PHSolidIf* solid;  ///< 制御対象の剛体
};

// --- 角度制御可能なボールジョイント
struct PHIKBallJointIf : PHIKNodeIf{
	SPR_IFDEF(PHIKBallJoint);
};

struct PHIKBallJointDesc : PHIKNodeDesc{
	SPR_DESCDEF(PHIKBallJoint);

	PHBallJointIf* joint;  ///< 制御対象の関節
};

// --- 角度制御可能なヒンジジョイント
struct PHIKHingeJointIf : PHIKNodeIf{
	SPR_IFDEF(PHIKHingeJoint);
};

struct PHIKHingeJointDesc : PHIKNodeDesc{
	SPR_DESCDEF(PHIKHingeJoint);

	PHHingeJointIf *joint;  ///< 制御対象の関節
};

}

#endif

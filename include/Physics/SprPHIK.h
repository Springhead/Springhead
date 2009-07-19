/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprPHIK.h
 *	@brief 逆運動学(IK)計算
*/
#ifndef SPR_PHIKIf_H
#define SPR_PHIKIf_H

#include <SprFoundation.h>
#include <SprPhysics.h>

namespace Spr{;

/** \addtogroup gpPhysics */
//@{

/** \defgroup gpJoint 逆運動学(IK)計算*/
//@{

// ------------------------------------------------------------------------------
/// IKの制御点（目標位置を示すもの）
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

	/** @brief 有効・無効を取得する
	*/
	bool IsEnabled();
};

/// IK制御点のディスクリプタ
struct PHIKControlPointDesc{
	SPR_DESCDEF(PHIKControlPoint);

	bool isEnabled;  ///< 有効かどうか
};

/// 目標位置を指定する制御点
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

/// 位置制御点のディスクリプタ
struct PHIKPosCtlDesc : PHIKControlPointDesc{
	SPR_DESCDEF(PHIKPosCtl);

	Vec3d pos;  ///< 制御点の位置（剛体ローカル座標系における）
};

/// 目標姿勢を指定する制御点
struct PHIKOriCtlIf : PHIKControlPointIf{
	SPR_IFDEF(PHIKOriCtl);

	/** @brief 目標地点を設定する
	*/
	void SetGoal(Quaterniond goal);

	/** @brief 目標地点を取得する
	*/
	Quaterniond GetGoal();
};

/// 姿勢制御点のディスクリプタ
struct PHIKOriCtlDesc : PHIKControlPointDesc{
	SPR_DESCDEF(PHIKOriCtl);
};

// ------------------------------------------------------------------------------
/// IKノード（目標の達成のために用いることのできる可動部品（関節・剛体など））
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

	/** @brief 自然位置に戻る
	*/
	void MoveNatural();

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

	/** @brief 駆動のためのバネ係数を設定する
	*/
	void SetSpring(double spring);

	/** @brief 駆動のためのバネ係数を取得
	*/
	double GetSpring();

	/** @brief 駆動のためのダンパ係数を設定する
	*/
	void SetDamper(double damper);

	/** @brief 駆動のためのダンパ係数を取得
	*/
	double GetDamper();

	/** @brief 有効・無効を設定する
	*/
	void Enable(bool enable);

	/** @brief 有効・無効を取得する
	*/
	bool IsEnabled();
};

/// IKノードのディスクリプタ
struct PHIKNodeDesc{
	SPR_DESCDEF(PHIKNode);

	bool isEnabled;  ///< 有効かどうか

	float	bias;	///< 動かしにくさの係数
	double	spring;	///< 駆動用バネのバネ係数
	double	damper;	///< 駆動用バネのダンパ係数

	PHIKNodeDesc(){ bias = 1.0f; }
};

/// 位置が可動な剛体
struct PHIKSolidIf : PHIKNodeIf{
	SPR_IFDEF(PHIKSolid);
};

/// IK用剛体のディスクリプタ
struct PHIKSolidDesc : PHIKNodeDesc{
	SPR_DESCDEF(PHIKSolid);
};

/// 姿勢が可動なボールジョイント
struct PHIKBallJointIf : PHIKNodeIf{
	SPR_IFDEF(PHIKBallJoint);
};

/// IK用ボールジョイントのディスクリプタ
struct PHIKBallJointDesc : PHIKNodeDesc{
	SPR_DESCDEF(PHIKBallJoint);
};

/// 角度が可動なヒンジジョイント
struct PHIKHingeJointIf : PHIKNodeIf{
	SPR_IFDEF(PHIKHingeJoint);
};

/// IK用ヒンジジョイントのディスクリプタ
struct PHIKHingeJointDesc : PHIKNodeDesc{
	SPR_DESCDEF(PHIKHingeJoint);
};

//@}
//@}

}

#endif

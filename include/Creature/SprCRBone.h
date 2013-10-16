/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRBONEIF_H
#define SPR_CRBONEIF_H

#include <Foundation/SprObject.h>

namespace Spr{;

struct PHSolidIf;
struct PHJointIf;
struct PHIKActuatorIf;
struct PHIKEndEffectorIf;

/// 運動軌道の通過点
struct CRTrajectoryNode {
	float  time;
	Vec3d  r;
	Vec3d  v;
	Vec3d  a;
	Vec4d  priority;

	Vec6d  coeff[3];
	double length;

	CRTrajectoryNode(){
		time		= 0;
		r			= Vec3d();
		v			= Vec3d();
		a			= Vec3d();
		priority	= Vec4d();

		for (int i=0; i<3; ++i) { coeff[i] = Vec6d(); }
		length      = DBL_MAX;
	};

	CRTrajectoryNode(float t, Vec3d r, Vec3d v, Vec3d a, Vec4d pr) {
		time		= t;
		this->r		= r;
		this->v		= v;
		this->a		= a;
		priority	= pr;

		for (int i=0; i<3; ++i) { coeff[i] = Vec6d(); }
		length      = DBL_MAX;
	}
};

/// 接触リスト
struct CRTouchInfo{
	Vec3f      force;
	double     area;
	Vec3f      position;
	PHSolidIf* solid;
};

/// クリーチャのボーン（剛体一つ＋親ボーンへの関節一つ）
struct CRBoneIf : SceneObjectIf {
	SPR_IFDEF(CRBone);

	/** @brief ラベル（役割を示す文字列：Handなど）の取得
	 */
	const char* GetLabel() const;

	/** @brief ラベル（役割を示す文字列：Handなど）の設定
	 */
	void SetLabel(const char* str);

	/** @brief PHSolidを取得
	 */
	PHSolidIf* GetPHSolid();

	/** @brief PHSolidを設定
	 */
	void SetPHSolid(PHSolidIf* so);

	/** @brief IKエンドエフェクタを取得
	 */
	PHIKEndEffectorIf* GetIKEndEffector();

	/** @brief IKエンドエフェクタを設定
	 */
	void SetIKEndEffector(PHIKEndEffectorIf* ikEE);

	/** @brief PHJointを取得
	 */
	PHJointIf* GetPHJoint();

	/** @brief PHJointを設定
	 */
	void SetPHJoint(PHJointIf* jo);

	/** @brief IKアクチュエータを取得
	 */
	PHIKActuatorIf* GetIKActuator();

	/** @brief IKアクチュエータを設定
	 */
	void SetIKActuator(PHIKActuatorIf* ikAct);

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

	/** @brief 親ボーンを返す
	 */
	CRBoneIf* GetParentBone();

	/** @brief 子ボーンのリスト
	 */
	int       NChildBones();
	CRBoneIf* GetChildBone(int number);

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	//  軌道運動関連
	
	/** @brief 到達の相対基準となる剛体を設定 NULLだと絶対位置
		@param solid 基準剛体
	*/
	void SetOriginSolid(PHSolidIf* solid);

	/** @brief 軌道の通過点を追加する
		@param time     通過時刻
		@param pose     目標位置・姿勢
		@param dpose    目標速度・角速度
		@param priority 優先度ベクトル（位置・姿勢・速度・角速度 の各優先度．0でdisable）
	*/
	void AddTrajectoryNode(CRTrajectoryNode node, bool clear=0);

	/** @brief i番目（時刻ベース）の通過点を取得する
	*/
	CRTrajectoryNode GetTrajectoryNode(int i);

	/** @brief 通過点を数を取得する
	*/
	int NTrajectoryNodes();

	/** @brief 時刻tにおけるの通過点を取得する
	*/
	CRTrajectoryNode GetTrajectoryNodeAt(float time);

	/** @brief i番目（時刻ベース）の通過点を設定する
		（追加した軌道通過点を後から編集したい場合に使う．普通は使わない）
	*/
	void SetTrajectoryNode(int i, CRTrajectoryNode node);

	/** @brief 現在通過中の点を取得する
	*/
	CRTrajectoryNode GetCurrentNode();

	/** @brief 軌道の通過点を全削除する
	*/
	void ClearTrajectory(bool apply=0);

	/** @brief １ステップ
	*/
	void StepTrajectory();

	/** @brief 軌道を計画する
	*/
	void Plan();

	/** @brief 軌道計画中かどうかを返す
	*/
	bool IsPlanning();

	/** @brief 新しい軌道が開始されたかどうかを返す
	*/
	bool IsNewTrajectoryStarted();

	/** @brief 軌道が変更されたかどうかを返す
	    IsNewTrajectoryChangedより前に呼ぶとIsNewTrajectoryChangedがうまく判定できなくなるので注意
	*/
	bool IsTrajectoryChanged();
};

struct CRBoneDesc {
	SPR_DESCDEF(CRBone);
	std::string	label;			///<	ラベル（役割を示す文字列：Handなど）
};

}

#endif//SPR_CRBONEIF_H
/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRBONE_H
#define CRBONE_H

#include <Foundation/Object.h>
#include <Creature/SprCRBone.h>
#include <Physics/SprPHSolid.h>
#include <Physics/SprPHJoint.h>
#include <Physics/SprPHIK.h>

#include <queue>

//@{
namespace Spr{;

class CRBone : public SceneObject, public CRBoneDesc {
	PHSolidIf*			solid;
	PHJointIf*			joint;
	PHIKEndEffectorIf*	endeffector;
	PHIKActuatorIf*		actuator;

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	//  軌道運動関連
	
	/// 軌道の相対基準剛体
	PHSolidIf* originSolid;

	/// 軌道を構成する点群
	std::deque<CRTrajectoryNode> trajNodes;

	/// 現在時刻
	float time;

	/// 現在の状態
	CRTrajectoryNode current;

	/// 軌道計画中フラグ
	bool bPlan;

	/// 軌道変更フラグ
	bool bChanged;

	/// 軌道クリアフラグ
	bool bCleared;



	/// 到達目標時間
	float timeLimit;

	/// 最終及び初期の目標位置・速度・姿勢・角速度
	Vec3d		finalPos, initPos; bool bCtlPos;
	Quaterniond	finalOri, initOri; bool bCtlOri;
	Vec3d		finalVel, initVel; bool bCtlVel;
	Vec3d		finalAvl, initAvl; bool bCtlAvl;

	/// 有効か
	bool bEnable;
	bool bPause;

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	//  視覚関連
	
	/// 物体探索エリア
	Posed		relativePose;
	double		horizRange;
	double		vertRange;
	std::vector<PHSolidIf*> foundSolids;

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	//  触覚関連
	
	/// 接触リスト
	std::vector<CRTouchInfo> touchList;

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

	void InitVars() {
		solid		= NULL;
		endeffector	= NULL;
		joint		= NULL;
		actuator	= NULL;

		bCtlPos = bCtlOri = bCtlVel = bCtlAvl = false;
		bEnable = false;
		bPause  = false;

		originSolid = NULL;
		time        = 0.0f;
		bPlan       = false;
		bChanged    = false;
		bCleared    = false;
		ClearTrajectory();

		relativePose = Posed();
		horizRange = vertRange = -1;
		foundSolids.clear();
	}

public:
	SPR_OBJECTDEF(CRBone);

	CRBone() { InitVars(); }
	CRBone(const CRBoneDesc& desc) : CRBoneDesc(desc) { InitVars(); }

	/** @brief ラベル（役割を示す文字列：Handなど）の取得
	 */
	virtual const char* GetLabel() const { return label.c_str(); }

	/** @brief ラベル（役割を示す文字列：Handなど）の設定
	 */
	void SetLabel(const char* str) { label = std::string(str); }

	/** @brief PHSolidを取得
	 */
	PHSolidIf* GetPHSolid() { return solid; }

	/** @brief PHSolidを設定
	 */
	void SetPHSolid(PHSolidIf* so) { solid = so; }

	/** @brief PHJointを取得
	 */
	PHJointIf* GetPHJoint() { return joint; }

	/** @brief PHJointを設定
	 */
	void SetPHJoint(PHJointIf* jo) { joint = jo; }

	/** @brief IKエンドエフェクタを取得
	 */
	PHIKEndEffectorIf* GetIKEndEffector() { return endeffector; }

	/** @brief IKエンドエフェクタを設定
	 */
	void SetIKEndEffector(PHIKEndEffectorIf* ikEE) { endeffector = ikEE; }

	/** @brief IKアクチュエータを取得
	 */
	PHIKActuatorIf* GetIKActuator() { return actuator; }

	/** @brief IKアクチュエータを設定
	 */
	void SetIKActuator(PHIKActuatorIf* ikAct) { actuator = ikAct; }

	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const {
		return( ((solid==NULL)?0:1) + ((joint==NULL)?0:1) + ((endeffector==NULL)?0:1) + ((actuator==NULL)?0:1) );
	}

	virtual ObjectIf* GetChildObject(size_t i) {
		ObjectIf* objs[] = {solid->Cast(), joint->Cast(), endeffector->Cast(), actuator->Cast()};
		int pos = -1;
		for (int n=0; n<4; ++n) {
			if (objs[n]!=NULL) { pos++; }
			if (pos==i) { return objs[n]; }
		}
		return NULL;
	}

	virtual bool AddChildObject(ObjectIf* o) {
		if (!solid)			{ solid			= o->Cast(); if (solid)			{ return true; } }
		if (!joint)			{ joint			= o->Cast(); if (joint)			{ return true; } }
		if (!endeffector)	{ endeffector	= o->Cast(); if (endeffector)	{ return true; } }
		if (!actuator)		{ actuator		= o->Cast(); if (actuator)		{ return true; } }
		return false;
	}

	virtual bool DelChildObject(ObjectIf* o) {
		if (o==solid)		{ solid			= NULL; return true; }
		if (o==joint)		{ joint			= NULL; return true; }
		if (o==endeffector)	{ endeffector	= NULL; return true; }
		if (o==actuator)	{ actuator		= NULL; return true; }
		return false;
	}

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
	int NTrajectoryNodes() { return trajNodes.size(); }

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
	void ClearTrajectory();

	/** @brief １ステップ
	*/
	void StepTrajectory();

	/** @brief 軌道を計画する
	*/
	void Plan();
	void PlanSegment(CRTrajectoryNode &from, CRTrajectoryNode &to);

	/** @brief 軌道計画中かどうかを返す
	*/
	bool IsPlanning() { return bPlan; }

	/** @brief 新しい軌道が開始されたかどうかを返す
	*/
	bool IsNewTrajectoryStarted() { if (bCleared && bChanged) { bCleared=false; bChanged=false; return true; } else { return false; } }

	/** @brief 軌道が変更されたかどうかを返す
	    IsNewTrajectoryChangedより前に呼ぶとIsNewTrajectoryChangedがうまく判定できなくなるので注意
	*/
	bool IsTrajectoryChanged() { if (bChanged) { bChanged=false; return true; } else { return false; } }



	/// ----- そのうちObsoleteにするかも＜ここから＞ -----

	/** @brief 目標位置の設定
		@param pos 目標位置
	*/
	void SetTargetPos(Vec3d pos);

	/** @brief 現在設定されている目標位置の取得
	*/
	Vec3d GetTargetPos();

	/** @brief 目標姿勢の設定
		@param ori 目標姿勢
	*/
	void SetTargetOri(Quaterniond ori);

	/** @brief 現在設定されている目標姿勢の取得
	*/
	Vec3d GetTargetOri();

	/** @brief 目標位置・姿勢の設定
		@param pose 目標位置・姿勢
	*/
	void SetTargetPose(Posed pose);

	/** @brief 目標到達時刻の設定
		@param timeLimit 到達までの目標経過時間
	*/
	void SetTimeLimit(float timeLimit);

	/** @brief 動作開始
	*/
	void Start();

	/** @brief 動作の一時停止
	*/
	void Pause();

	/** @brief 動作停止
	*/
	void Stop();

	/// ----- そのうちObsoleteにするかも＜ここまで＞ -----

	// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
	// 視覚関連

	/** @brief 物体探索用の円錐形のエリアを設置する
		@param relativePose 円錐を置く相対座標系を設定：原点が円錐の頂点、Z軸+が円錐の軸、Y軸+が上方向
		@param horizRange 水平方向の幅（ラジアン） マイナスなら無効
		@param vertRange  垂直方向の幅（ラジアン） マイナスなら無効
	*/
	void SetViewArea(Posed relativePose, double horizRange, double vertRange);

	/** @brief 設定された円錐形のエリア内に存在する剛体の数
	*/
	int NVisibleSolids();

	/** @brief 設定された円錐形のエリア内に存在する剛体を返す
	*/
	PHSolidIf* GetVisibleSolid(int i);

	/** @brief １ステップ
	*/
	void StepSearchArea();

	// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
	// 触覚関連

	/** @brief 接触の数を返す
	*/
	int NTouches();

	/** @brief i番目の接触情報を返す
	*/
	CRTouchInfo GetTouchInfo(int i) {
		return touchList[i];
	}

	/** @brief 接触情報を追加する
	*/
	void AddTouchInfo(CRTouchInfo ci) {
		touchList.push_back(ci);
	}

	/** @brief 接触情報をクリアする
	*/
	void ClearTouchInfo() {
		touchList.clear();
	}



	/// --- Will be Obsoleted

	/** @brief i番目の接触の接触力を返す
	*/
	Vec3f GetContactForce(int i);

	/** @brief i番目の接触の面積を返す
	*/
	double GetContactArea(int i);

	/** @brief i番目の接触の位置を返す
	*/
	Vec3f GetContactPosition(int i);

	/** @brief i番目の接触の相手剛体を返す
	*/
	PHSolidIf* GetContactSolid(int i);

	/** @brief １ステップ
	*/
	void StepListContact();
};

}
//@}

#endif//CRBONE_H

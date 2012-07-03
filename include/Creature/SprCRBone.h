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
	//  軌道運動関連
	
	/** @brief 到達の相対基準となる剛体を設定 NULLだと絶対位置
		@param solid 基準剛体
	*/
	void SetOriginSolid(PHSolidIf* solid);

	/** @brief 目標位置の設定
		@param pos 目標位置
	*/
	void SetTargetPos(Vec3d pos);

	/** @brief 目標姿勢の設定
		@param ori 目標姿勢
	*/
	void SetTargetOri(Quaterniond ori);

	/** @brief 目標位置・姿勢の設定
		@param pose 目標位置・姿勢
	*/
	void SetTargetPose(Posed pose);

	/** @brief 目標到達時刻の設定
		@param timeLimit 到達までの目標経過時間
	*/
	void SetTimeLimit(float timeLimit);

	/** @brief １ステップ
	*/
	void StepTrajectory();

	/** @brief 動作開始
	*/
	void Start();

	/** @brief 動作の一時停止
	*/
	void Pause();

	/** @brief 動作停止
	*/
	void Stop();

	/** @brief 軌道を計画する
	*/
	void Plan();

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
	int NContacts();

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

struct CRBoneDesc {
	SPR_DESCDEF(CRBone);
	std::string	label;			///<	ラベル（役割を示す文字列：Handなど）
};

}

#endif//SPR_CRBONEIF_H
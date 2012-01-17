/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRBODYPART_H
#define CRBODYPART_H

#include <Foundation/Object.h>
#include <Creature/SprCRBodyPart.h>

//@{
namespace Spr{;

struct PHIKActuatorIf;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief ボディ構成要素のベースクラス
*/
class CRBodyPart : public SceneObject, public CRBodyPartDesc {
protected:
public:
	SPR_OBJECTDEF(CRBodyPart);
	// ACCESS_DESC(CRBodyPart);

	CRBodyPart(){}
	CRBodyPart(const CRBodyPartDesc& desc) 
		: CRBodyPartDesc(desc)
	{
	}

	/** @brief ラベル（役割を示す文字列：Handなど）の取得
	 */
	virtual const char* GetLabel() const { return label.c_str(); }

	/** @brief ラベル（役割を示す文字列：Handなど）の設定
	 */
	void SetLabel(const char* str) { label = std::string(str); }

	/** @brief １ステップ
	*/
	virtual void Step() {}
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief ボディ構成剛体
*/
class CRSolid : public CRBodyPart, public CRSolidDesc {
protected:
	PHSolidIf* solid;

public:
	SPR_OBJECTDEF(CRSolid);
	ACCESS_DESC(CRSolid);

	CRSolid()
		: solid(NULL)
	{
	}
	CRSolid(const CRSolidDesc& desc) 
		: solid(NULL), CRSolidDesc(desc), CRBodyPart(desc)
	{
	}

	/** @brief PHSolidを取得
	 */
	PHSolidIf* GetPHSolid() { return solid; }

	/** @brief PHSolidを設定
	 */
	void SetPHSolid(PHSolidIf* so) { solid = so; }

	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t i);
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief ボディ構成剛体（可動）
*/
class CRIKSolid : public CRSolid, public CRIKSolidDesc {
private:
	/// 予測シミュレーション用の状態格納変数
	// UTRef<ObjectStatesIf> state;

	/// 到達位置の相対基準剛体
	PHSolidIf* originSolid;

	/// 経過時間
	float time;

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

	/// 物体探索エリア
	Posed		relativePose;
	double		horizRange;
	double		vertRange;
	std::vector<PHSolidIf*> foundSolids;

	/// 接触リスト
	struct ContactInfo{
		Vec3f      force;
		double     area;
		Vec3f      position;
		PHSolidIf* solid;
	};
	std::vector<ContactInfo> contactList;

	virtual void InitVars() {
		ikEndEffector = NULL;
		bCtlPos = bCtlOri = bCtlVel = bCtlAvl = false;
		bEnable = false;
		bPause  = false;

		originSolid = NULL;

		relativePose = Posed();
		horizRange = vertRange = -1;
		foundSolids.clear();

		// state = ObjectStatesIf::Create();
	}

protected:
	PHIKEndEffectorIf *ikEndEffector;

public:
	SPR_OBJECTDEF(CRIKSolid);
	ACCESS_DESC(CRIKSolid);

	CRIKSolid()
		: ikEndEffector(NULL)
	{
		InitVars();
	}
	CRIKSolid(const CRIKSolidDesc& desc) 
		: CRIKSolidDesc(desc), CRSolid(desc)
	{
		InitVars();
	}

	// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
	// 基本機能

	/** @brief IKエンドエフェクタを取得
	 */
	PHIKEndEffectorIf* GetIKEndEffector() {
		return ikEndEffector;
	}

	/** @brief IKエンドエフェクタを設定
	 */
	void SetIKEndEffector(PHIKEndEffectorIf* ikEE) {
		ikEndEffector = ikEE;
	}

	/** @brief １ステップ
	*/
	virtual void Step();

	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t i);
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);

	// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
	// 軌道運動

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
	// 物体探索

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
	// 接触一覧

	/** @brief 接触の数を返す
	*/
	virtual int NContacts();

	/** @brief i番目の接触の接触力を返す
	*/
	virtual Vec3f GetContactForce(int i);

	/** @brief i番目の接触の面積を返す
	*/
	virtual double GetContactArea(int i);

	/** @brief i番目の接触の位置を返す
	*/
	virtual Vec3f GetContactPosition(int i);

	/** @brief i番目の接触の相手剛体を返す
	*/
	PHSolidIf* GetContactSolid(int i);

	/** @brief １ステップ
	*/
	void StepListContact();
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief ボディ構成関節
*/
class CRJoint : public CRBodyPart, public CRJointDesc {
protected:
	PHJointIf* joint;
	double spring, damper;

public:
	SPR_OBJECTDEF(CRJoint);
	ACCESS_DESC(CRJoint);

	CRJoint()
		: joint(NULL), spring(-1), damper(-1)
	{
	}
	CRJoint(const CRJointDesc& desc) 
		: joint(NULL), CRJointDesc(desc), CRBodyPart(desc), spring(-1), damper(-1)
	{
	}

	/** @brief PHJointを取得
	 */
	PHJointIf* GetPHJoint() { return joint; }

	/** @brief PHJointを設定
	 */
	void SetPHJoint(PHJointIf* jo) { joint = jo; }

	/** @brief バネダンパ係数の倍数を設定
	 */
	virtual void SetSpringRatio(double springRatio, double damperRatio);
	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t i);
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief ボディ構成関節（可動）
*/
class CRIKJoint : public CRJoint, public CRIKJointDesc {
protected:
	PHIKActuatorIf* ikActuator;
	double ikSpring, ikDamper;

public:
	SPR_OBJECTDEF(CRIKJoint);
	ACCESS_DESC(CRIKJoint);

	CRIKJoint()
		: ikActuator(NULL), ikSpring(-1), ikDamper(-1)
	{
	}
	CRIKJoint(const CRIKJointDesc& desc) 
		: ikActuator(NULL), CRIKJointDesc(desc), CRJoint(desc), ikSpring(-1), ikDamper(-1)
	{
	}

	/** @brief IKアクチュエータを取得
	 */
	PHIKActuatorIf* GetIKActuator() { return ikActuator; }

	/** @brief IKアクチュエータを設定
	 */
	void SetIKActuator(PHIKActuatorIf* ikAct);

	/** @brief バネダンパ係数の倍数を設定
	 */
	virtual void SetSpringRatio(double springRatio, double damperRatio);

	/** @brief バネダンパ係数の倍数を設定
	 */
	void SetIKSpringRatio(double springRatio, double damperRatio);

	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t i);
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
};

}
//@}

#endif//CRBODYPART_H

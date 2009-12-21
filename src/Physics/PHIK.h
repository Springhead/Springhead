/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PH_INVERSEKINEMATICS_H
#define PH_INVERSEKINEMATICS_H

#include <SprPhysics.h>
#include <Foundation/Object.h>
#include <Physics/PHEngine.h>

#include <vector>
#include <set>
#include <map>

namespace Spr{;

class PHIKNode;
class PHIKControlPoint;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// IKエンジン
// 
class PHIKEngine : public PHEngine{
public:
	SPR_OBJECTDEF(PHIKEngine);

	std::vector<PHIKNodeIf*>			nodes;
	std::vector<PHIKControlPointIf*>	controlpoints;

	std::vector<int>  nDOFsInCol;
	std::vector<int>  nDOFsInRow;
	PTM::VMatrixRow<double>   Jc;
	PTM::VMatrixRow<double> piJc;
	PTM::VVector<double>      v_;
	PTM::VVector<double>      w_;

	bool bEnabled;

	size_t numIter;
	PHIKEngine();

	int GetPriority() const {return SGBP_INVERSEKINEMATICS;}
	void Step();
	void Clear();
	void Enable(bool bEnabled){ this->bEnabled = bEnabled; };
	void SetNumIter(int numIter){ this->numIter = numIter; };

	PHIKNode* CreateIKNode(const IfInfo* ii, const PHIKNodeDesc& desc);
	PHIKControlPoint* CreateIKControlPoint(const IfInfo* ii, const PHIKControlPointDesc& desc);
	virtual bool AddChildObject(ObjectIf* o);
};

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// IKNode：IKで制御するノード
// 
class PHIKNode : public SceneObject{
public:
	/// 簡略表記用typedef
	typedef std::set<PHIKControlPointIf*>	CSet;
	typedef CSet::iterator					CSetIter;
	typedef std::set<PHIKNodeIf*>			NSet;
	typedef NSet::iterator					NSetIter;
	
	/// 番号
	int number;

	/// 動かしにくさ
	float bias;

	/// 駆動用のバネダンパ（関節本来のバネダンパは原点への引き戻しとして作用する）
	double spring, damper;

	/// 連結した制御点の有効・無効状態
	enum ENABLE_STATUS {
		ES_NONE=0,
		ES_NOORI,
		ES_ORI,
	} enableStat;

	/// この関節自身の有効・無効
	bool isEnabled;

	// 計算用の一時変数
	PTM::VVector<double>     alpha, beta;
	std::map< int, PTM::VMatrixRow<double> >  gamma;

public:
	SPR_OBJECTDEF(PHIKNode);

	/// 自由度
	int ndof;

	/// このNodeを動かせるControlPoint
	CSet linkedControlPoints;

	/// このNodeと連動するNode
	NSet linkedNodes;

	// ヤコビアン
	std::map< int,PTM::VMatrixRow<double> > Mj;

	/// IKのIterationの一回前の計算結果（収束判定用）
	PTM::VVector<double> dTheta_prev;

	/// IKの計算結果（角度）
	PTM::VVector<double> dTheta;

	/// IDの計算結果（トルク）
	PTM::VVector<double> tau;

	/** @brief デフォルトコンストラクタ
	*/
	PHIKNode(){ bias=1.0f; }

	/** @brief コンストラクタ
	*/
	PHIKNode(const PHIKNodeDesc& desc){
		SetDesc(&desc);
	}

	/** @brief デスクリプタを設定する
	*/
	virtual void SetDesc(const void* d){
		this->isEnabled = ((PHIKNodeDesc*)d)->isEnabled;
		this->bias		= ((PHIKNodeDesc*)d)->bias;
		this->spring	= ((PHIKNodeDesc*)d)->spring;
		this->damper	= ((PHIKNodeDesc*)d)->damper;
	}

	/** @brief 関係するすべての制御点とのヤコビアンをそれぞれ求める
	*/
	virtual void CalcAllJacobian();

	/** @brief ヤコビアンをクリアする
	*/
	virtual void ClearJacobian();

	/** @brief IKの計算準備をする
	*/
	virtual void PrepareSolve();

	/** @brief IKの計算繰返しの１ステップを実行する
	*/
	virtual void ProceedSolve();

	/** @brief 計算結果を取得する
	*/
	virtual PTM::VVector<double> GetDTheta(){ return dTheta; }

	/** @brief 計算結果に従って制御対象を動かす
	*/
	virtual void Move(){}

	/** @brief 自然位置に戻る
	*/
	virtual void MoveNatural(){}

	/** @brief 制御点を追加する
	*/
	virtual void AddControlPoint(PHIKControlPointIf* control);

	/** @brief 番号を設定する
	*/
	void SetNumber(int i){ number = i; }

	/** @brief 番号を取得する
	*/
	int  GetNumber(){ return number; }

	/** @brief 自由度を設定する
	*/
	void SetNDOF(int n);

	/** @brief 指定した制御点との間のヤコビアンを計算する
	*/
	virtual PTM::VMatrixRow<double> CalcJacobian(PHIKControlPointIf* control);

	/** @brief 動かしにくさを設定する
	*/
	void SetBias(float bias){ this->bias = bias; }

	/** @brief 動かしにくさを取得する
	*/
	float GetBias(){ return bias; }

	/** @brief 駆動のためのバネ係数を設定する
	*/
	void SetSpring(double spring){ this->spring = spring; }

	/** @brief 駆動のためのバネ係数を取得
	*/
	double GetSpring(){ return spring; }

	/** @brief 駆動のためのダンパ係数を設定する
	*/
	void SetDamper(double damper){ this->damper = damper; }

	/** @brief 駆動のためのダンパ係数を取得
	*/
	double GetDamper(){ return damper; }

	/** @brief 有効・無効を設定する
	*/
	void Enable(bool enable){ isEnabled = enable; if (!enable) { MoveNatural(); } }

	/** @brief 有効・無効を取得する
	*/
	bool IsEnabled(){ return isEnabled; }

	virtual bool AddChildObject(ObjectIf* o){
		PHIKControlPointIf* cp = o->Cast();
		if (cp) {
			AddControlPoint(cp);
			return true;
		}
		return false;
	}
	virtual ObjectIf* GetChildObject(size_t pos){
		for (CSetIter it=linkedControlPoints.begin(); it!=linkedControlPoints.end(); ++it) {
			if (pos == 0) {
				return *it;
			}
			pos--;
		}
		return NULL;
	}
	size_t NChildObject() {
		int cnt = 0;
		for (CSetIter it=linkedControlPoints.begin(); it!=linkedControlPoints.end(); ++it) {
			cnt++;
		}
		return cnt;
	}
};

class PHIKSolid : public PHIKNode{
protected:
	/// 制御対象の剛体
	PHSolidIf* solid;

public:
	SPR_OBJECTDEF(PHIKSolid);

	/** @brief デフォルトコンストラクタ
	*/
	PHIKSolid(){SetNDOF(3);}

	/** @brief コンストラクタ
	*/
	PHIKSolid(const PHIKSolidDesc& desc) {
		SetNDOF(3);
		SetDesc(&desc);
	}

	/** @brief デスクリプタを設定する
	*/
	virtual void SetDesc(const void* d){
		PHIKNode::SetDesc(d);
	}

	/** @brief 指定した制御点との間のヤコビアンを計算する
	*/
	virtual PTM::VMatrixRow<double> CalcJacobian(PHIKControlPointIf* control);

	virtual bool AddChildObject(ObjectIf* o){
		PHSolidIf* so = o->Cast();
		if (so) { this->solid = so; return true; }
		return false;
	}
	virtual ObjectIf* GetChildObject(size_t pos){
		if (pos == 0 && this->solid != NULL) { return this->solid; }
		if (this->solid != NULL) {
			return PHIKNode::GetChildObject(pos - 1);
		} else {
			return PHIKNode::GetChildObject(pos);
		}
		return NULL;
	}
	size_t NChildObject() {
		if (this->solid != NULL) { return 1 + PHIKNode::NChildObject(); }
		return PHIKNode::NChildObject();
	}
};

class PHIKBallJoint : public PHIKNode{
public:
	/// 制御対象の関節
	PHBallJointIf* joint;

	/// 制御対象関節のバネダンパ初期値と基準姿勢
	double			jSpring;
	double			jDamper;
	Quaterniond		jGoal;

	/// IKの回転軸
	Vec3d e[3];

public:
	SPR_OBJECTDEF(PHIKBallJoint);

	/** @brief デフォルトコンストラクタ
	*/
	PHIKBallJoint(){SetNDOF(2);}

	/** @brief コンストラクタ
	*/
	PHIKBallJoint(const PHIKBallJointDesc& desc) {
		SetNDOF(2);
		SetDesc(&desc);
		joint = NULL;
	}

	/** @brief デスクリプタを設定する
	*/
	virtual void SetDesc(const void* d){
		PHIKNode::SetDesc(d);
	}

	/** @brief 計算結果に従って制御対象を動かす
	*/
	virtual void Move();

	/** @brief 自然位置に戻る
	*/
	virtual void MoveNatural();

	/** @brief 指定した制御点との間のヤコビアンを計算する
	*/
	virtual PTM::VMatrixRow<double> CalcJacobian(PHIKControlPointIf* control);

	/** @brief 制御点を追加する
	    特に姿勢指定制御点が追加されたときにNDOFを2から3に上げるためにオーバーライド
	*/
	virtual void AddControlPoint(PHIKControlPointIf* control);

	/** @brief 関係するすべての制御点とのヤコビアンをそれぞれ求める
	*/
	virtual void CalcAllJacobian();

	/** @brief 回転軸を計算する
	*/
	virtual void CalcAxis();

	virtual bool AddChildObject(ObjectIf* o){
		PHBallJointIf* jo = o->Cast();
		if (jo) {
			this->joint = jo;
			PHBallJointDesc dJ; DCAST(PHBallJointIf,this->joint)->GetDesc(&dJ);
			this->jSpring = dJ.spring;
			this->jDamper = dJ.damper;
			this->jGoal   = dJ.targetPosition;
			return true;
		}
		return PHIKNode::AddChildObject(o);
	}
	virtual ObjectIf* GetChildObject(size_t pos){
		if (pos == 0 && this->joint != NULL) { return this->joint; }
		if (this->joint != NULL) {
			return PHIKNode::GetChildObject(pos - 1);
		} else {
			return PHIKNode::GetChildObject(pos);
		}
		return NULL;
	}
	size_t NChildObject() {
		if (this->joint != NULL) { return 1 + PHIKNode::NChildObject(); }
		return PHIKNode::NChildObject();
	}
};

class PHIKHingeJoint : public PHIKNode{
public:
	/// 制御対象の関節
	PHHingeJointIf *joint;

	/// 制御対象関節のバネダンパ初期値と基準姿勢
	double		jSpring;
	double		jDamper;
	double		jGoal;

public:
	SPR_OBJECTDEF(PHIKHingeJoint);

	/** @brief デフォルトコンストラクタ
	*/
	PHIKHingeJoint(){
		joint = NULL;
		SetNDOF(1);
	}

	/** @brief コンストラクタ
	*/
	PHIKHingeJoint(const PHIKHingeJointDesc& desc) {
		SetNDOF(1);
		SetDesc(&desc);
		joint = NULL;
	}

	/** @brief デスクリプタを設定する
	*/
	virtual void SetDesc(const void* d){
		PHIKNode::SetDesc(d);
	}

	/** @brief 計算結果に従って制御対象を動かす
	*/
	virtual void Move();

	/** @brief 自然位置に戻る
	*/
	virtual void MoveNatural();

	/** @brief 指定した制御点との間のヤコビアンを計算する
	*/
	virtual PTM::VMatrixRow<double> CalcJacobian(PHIKControlPointIf* control);

	virtual bool AddChildObject(ObjectIf* o){
		PHHingeJointIf* jo = o->Cast();
		if (jo) {
			this->joint = jo;
			this->jSpring = DCAST(PHHingeJointIf,this->joint)->GetSpring();
			this->jDamper = DCAST(PHHingeJointIf,this->joint)->GetDamper();
			this->jGoal   = DCAST(PHHingeJointIf,this->joint)->GetTargetPosition();
			return true;
		}
		return PHIKNode::AddChildObject(o);
	}
	virtual ObjectIf* GetChildObject(size_t pos){
		if (pos == 0 && this->joint != NULL) { return this->joint; }
		if (this->joint != NULL) {
			return PHIKNode::GetChildObject(pos - 1);
		} else {
			return PHIKNode::GetChildObject(pos);
		}
		return NULL;
	}
	size_t NChildObject() {
		if (this->joint != NULL) { return 1 + PHIKNode::NChildObject(); }
		return PHIKNode::NChildObject();
	}
};


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// IKControlPint：IKの制御点
// 

class PHIKControlPoint : public SceneObject{
public:
	/// 簡略表記用typedef
	typedef std::set<PHIKControlPointIf*>	CSet;
	typedef CSet::iterator					CSetIter;
	typedef std::set<PHIKNodeIf*>			NSet;
	typedef NSet::iterator					NSetIter;

	/// 番号
	int number;

	/// 制御点のある剛体
	PHSolidIf* solid;

	/// 力
	Vec3d force;

	/// 有効・無効
	bool isEnabled;

public:
	SPR_OBJECTDEF(PHIKControlPoint);

	/// このControlPointと連動するNode
	NSet linkedNodes;

	/** @brief デフォルトコンストラクタ
	*/
	PHIKControlPoint(){}

	/** @brief コンストラクタ
	*/
	PHIKControlPoint(const PHIKControlPointDesc& desc){
		SetDesc(&desc);
		force	= Vec3d(0,0,0);
		solid = NULL;
	}

	/** @brief デスクリプタを設定する
	*/
	virtual void SetDesc(const void* d){
		isEnabled = ((PHIKControlPointDesc*)d)->isEnabled;
	}

	/** @brief 力を設定する
	*/
	virtual void SetForce(Vec3d force){ this->force = force; }

	/** @brief 力を取得する
	*/
	Vec3d GetForce(){ return force; }

	/** @brief 暫定目標地点を取得する
	*/
	virtual Vec3d GetTmpGoal(){ return Vec3d(); }

	/** @brief 番号を設定する
	*/
	void SetNumber(int i){ number = i; }

	/** @brief 番号を取得する
	*/
	int  GetNumber(){ return number; }

	/** @brief 有効・無効を設定する
	*/
	void Enable(bool enable);

	/** @brief 有効・無効を取得する
	*/
	bool IsEnabled(){ return isEnabled; }

	/** @brief 制御対象の剛体を設定する
	*/
	void SetSolid(PHSolidIf* solid){ this->solid = solid; }

	/** @brief 制御対象の剛体を取得する
	*/
	PHSolidIf* GetSolid(){ return this->solid; }

	virtual bool AddChildObject(ObjectIf* o){
		PHSolidIf* so = o->Cast();
		if (so) { this->solid = so; return true; }
		return false;
	}
	virtual ObjectIf* GetChildObject(size_t pos){
		if (pos == 0 && this->solid != NULL) { return this->solid; }
		return NULL;
	}
	size_t NChildObject() const {
		if (this->solid != NULL) { return 1; }
		return 0;
	}
};

class PHIKPosCtl : public PHIKControlPoint{
	/// 目標値
	Vec3d targetPosition;

public:
	/// 制御点の位置（剛体ローカル座標系）
	Vec3d pos;

	SPR_OBJECTDEF(PHIKPosCtl);

	/** @brief デフォルトコンストラクタ
	*/
	PHIKPosCtl(){}

	/** @brief コンストラクタ
	*/
	PHIKPosCtl(const PHIKPosCtlDesc& desc) {
		SetDesc(&desc);
		targetPosition	= Vec3d(0,0,0);
	}

	/** @brief デスクリプタを設定する
	*/
	virtual void SetDesc(const void* d){
		PHIKControlPoint::SetDesc(d);
		this->pos = ((PHIKPosCtlDesc*)d)->pos;
	}

	/** @brief 暫定目標地点を取得する
	*/
	virtual Vec3d GetTmpGoal();

	/** @brief 目標地点を設定する
	*/
	virtual void SetTargetPosition(Vec3d targetPosition){ this->targetPosition = targetPosition; }

	/** @brief 目標地点を取得する
	*/
	virtual Vec3d GetTargetPosition(){ return targetPosition; }

	/** @brief 制御点の位置を設定する
	*/
	virtual void SetPos(Vec3d pos){ this->pos = pos; }

	/** @brief 制御点の位置を取得する
	*/
	virtual Vec3d GetPos(){ return pos; }
};

class PHIKOriCtl : public PHIKControlPoint{
	/// 目標値
	Quaterniond targetPosition;

public:
	SPR_OBJECTDEF(PHIKOriCtl);

	/** @brief デフォルトコンストラクタ
	*/
	PHIKOriCtl(){
		targetPosition	= Quaterniond();
	}

	/** @brief コンストラクタ
	*/
	PHIKOriCtl(const PHIKOriCtlDesc& desc) {
		SetDesc(&desc);
	}

	/** @brief デスクリプタを設定する
	*/
	virtual void SetDesc(const void* d){
		PHIKControlPoint::SetDesc(d);
	}

	/** @brief 暫定目標地点を取得する
	*/
	virtual Vec3d GetTmpGoal();

	/** @brief 目標地点を設定する
	*/
	virtual void SetTargetPosition(Quaterniond targetPosition){ this->targetPosition = targetPosition; }

	/** @brief 目標地点を取得する
	*/
	virtual Quaterniond GetTargetPosition(){ return targetPosition; }
};

}

#endif

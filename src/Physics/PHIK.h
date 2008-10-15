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

namespace Spr{;

class PHIKNode;
class PHIKControlPoint;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// IKエンジン
// 
class PHIKEngine : public PHEngine{
public:
	SPR_OBJECTDEF_NOIF(PHIKEngine);

	std::vector<PHIKNodeIf*>			nodes;
	std::vector<PHIKControlPointIf*>	controlpoints;

	size_t numIter;
	PHIKEngine():numIter(25){ }

	int GetPriority() const {return SGBP_INVERSEKINEMATICS;}
	void Step();
	void Clear();

	PHIKNode* CreateIKNode(const IfInfo* ii, const PHIKNodeDesc& desc);
	PHIKControlPoint* CreateIKControlPoint(const IfInfo* ii, const PHIKControlPointDesc& desc);
	virtual bool AddChildObject(ObjectIf* o);
};

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// IKNode：IKで制御するノード
// 
class PHIKNode : public SceneObject{
protected:
	/// 簡略表記用typedef
	typedef std::set<PHIKControlPointIf*>	CSet;
	typedef CSet::iterator					CSetIter;
	typedef std::set<PHIKNodeIf*>			NSet;
	typedef NSet::iterator					NSetIter;
	
	/// 番号
	int number;

	/// 自由度
	int ndof;

	/// 動かしにくさ
	float bias;

	// 計算用の一時変数
	PTM::VVector<double>     iDx, iD;
	PTM::VMatrixRow<double>  F;
	std::vector< PTM::VMatrixRow<double> >  K;

public:
	SPR_OBJECTDEF(PHIKNode);

	/// このNodeを動かせるControlPoint
	CSet linkedControlPoints;

	/// このNodeと連動するNode
	NSet linkedNodes;

	/// 計算結果
	PTM::VVector<double> dTheta;

	/** @brief デフォルトコンストラクタ
	*/
	PHIKNode(){};

	/** @brief コンストラクタ
	*/
	PHIKNode(const PHIKNodeDesc& desc){
		SetDesc(&desc);
	}

	/** @brief デスクリプタを設定する
	*/
	virtual void SetDesc(const void* d){
		this->bias = ((PHIKNodeDesc*)d)->bias;
	}

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
		this->solid = ((PHIKSolidDesc*)d)->solid;
	}

	/** @brief 指定した制御点との間のヤコビアンを計算する
	*/
	virtual PTM::VMatrixRow<double> CalcJacobian(PHIKControlPointIf* control);
};

class PHIKBallJoint : public PHIKNode{
protected:
	/// 制御対象の関節
	PHBallJointIf* joint;

public:
	SPR_OBJECTDEF(PHIKBallJoint);

	/** @brief デフォルトコンストラクタ
	*/
	PHIKBallJoint(){SetNDOF(3);}

	/** @brief コンストラクタ
	*/
	PHIKBallJoint(const PHIKBallJointDesc& desc) {
		SetNDOF(3);
		SetDesc(&desc);
	}

	/** @brief デスクリプタを設定する
	*/
	virtual void SetDesc(const void* d){
		PHIKNode::SetDesc(d);
		this->joint = ((PHIKBallJointDesc*)d)->joint;
	}

	/** @brief 指定した制御点との間のヤコビアンを計算する
	*/
	virtual PTM::VMatrixRow<double> CalcJacobian(PHIKControlPointIf* control);
};

class PHIKHingeJoint : public PHIKNode{
protected:
	/// 制御対象の関節
	PHHingeJointIf *joint;

public:
	SPR_OBJECTDEF(PHIKHingeJoint);

	/** @brief デフォルトコンストラクタ
	*/
	PHIKHingeJoint(){SetNDOF(1);}

	/** @brief コンストラクタ
	*/
	PHIKHingeJoint(const PHIKHingeJointDesc& desc) {
		SetNDOF(1);
		SetDesc(&desc);
	}

	/** @brief デスクリプタを設定する
	*/
	virtual void SetDesc(const void* d){
		PHIKNode::SetDesc(d);
		this->joint = ((PHIKHingeJointDesc*)d)->joint;
	}

	/** @brief 計算結果に従って制御対象を動かす
	*/
	virtual void Move();

	/** @brief 指定した制御点との間のヤコビアンを計算する
	*/
	virtual PTM::VMatrixRow<double> CalcJacobian(PHIKControlPointIf* control);
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

	/// 目標値
	Vec3d goal;

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
	}

	/** @brief デスクリプタを設定する
	*/
	virtual void SetDesc(const void* d){
		this->solid = ((PHIKPosCtlDesc*)d)->solid;
	}

	/** @brief 目標地点を設定する
	*/
	virtual void SetGoal(Vec3d goal){ this->goal = goal; }

	/** @brief 目標地点を取得する
	*/
	Vec3d GetGoal(){ return goal; }

	/** @brief 暫定目標地点を取得する
	*/
	virtual Vec3d GetTmpGoal(){ return goal; }

	/** @brief 番号を設定する
	*/
	void SetNumber(int i){ number = i; }

	/** @brief 番号を取得する
	*/
	int  GetNumber(){ return number; }

	/** @brief 制御対象の剛体を設定する
	*/
	void SetSolid(PHSolidIf* solid){ this->solid = solid; }

	/** @brief 制御対象の剛体を取得する
	*/
	PHSolidIf* GetSolid(){ return this->solid; }
};

class PHIKPosCtl : public PHIKControlPoint{
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
};

class PHIKOriCtl : public PHIKControlPoint{
public:
	SPR_OBJECTDEF(PHIKOriCtl);

	/** @brief デフォルトコンストラクタ
	*/
	PHIKOriCtl(){}

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
};

}

#endif

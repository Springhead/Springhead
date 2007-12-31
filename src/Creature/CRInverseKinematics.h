/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRINVERSEKINEMATICS_H
#define CRINVERSEKINEMATICS_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"

#include <set>
#include <vector>

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 制御点
*/
class CRIKControl : public SceneObject, public CRIKControlIfInit {
public:
	/// 簡略表記用typedef
	typedef std::set<CRIKControlIf*>           CSet;
	typedef std::set<CRIKControlIf*>::iterator CSetIter;
	typedef std::set<CRIKMovableIf*>           MSet;
	typedef std::set<CRIKMovableIf*>::iterator MSetIter;

	/// 番号
	int number;

	/// 制御点のある剛体
	PHSolidIf* solid;

	/// 目標値
	Vec3d goal;

public:
	OBJECTDEF(CRIKControl, SceneObject);

	/// このControlと連動するMovable
	MSet linkedMovables;

	/** @brief デフォルトコンストラクタ
	*/
	CRIKControl(){}

	/** @brief コンストラクタ
	*/
	CRIKControl(const CRIKControlDesc& desc){
		this->solid = desc.solid;
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
};

/** @brief 位置制御点
*/
class CRIKControlPos : public CRIKControl, CRIKControlPosIfInit {
public:
	/// 制御点の位置（剛体ローカル座標系）
	Vec3d pos;

public:
	OBJECTDEF(CRIKControlPos, CRIKControl);

	/** @brief デフォルトコンストラクタ
	*/
	CRIKControlPos(){}

	/** @brief コンストラクタ
	*/
	CRIKControlPos(const CRIKControlPosDesc& desc) : CRIKControl(desc) {
		this->pos = desc.pos;
	}

	/** @brief 暫定目標地点を取得する
	*/
	virtual Vec3d GetTmpGoal();
};

/** @brief 姿勢制御点
*/
class CRIKControlOri : public CRIKControl, CRIKControlOriIfInit {
public:
public:
	OBJECTDEF(CRIKControlOri, CRIKControl);

	/** @brief デフォルトコンストラクタ
	*/
	CRIKControlOri(){}

	/** @brief コンストラクタ
	*/
	CRIKControlOri(const CRIKControlOriDesc& desc) : CRIKControl(desc) {
	}

	/** @brief 暫定目標地点を取得する
	*/
	virtual Vec3d GetTmpGoal();
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 制御対象
*/
class CRIKMovable : public SceneObject, public CRIKMovableIfInit {
protected:
	/// 簡略表記用typedef
	typedef std::set<CRIKControlIf*>           CSet;
	typedef std::set<CRIKControlIf*>::iterator CSetIter;
	typedef std::set<CRIKMovableIf*>           MSet;
	typedef std::set<CRIKMovableIf*>::iterator MSetIter;
	
	/// 番号
	int number;

	/// 自由度
	int ndof;

	/// 動かしやすさ
	float bias;

	// 計算用の一時変数
	PTM::VVector<double>     iDx, iD;
	PTM::VMatrixRow<double>  F;
	std::vector< PTM::VMatrixRow<double> >  K;

public:
	OBJECTDEF(CRIKMovable, SceneObject);

	/// このMovableを動かせるControl
	CSet linkedControls;

	/// このMovableと連動するMovable
	MSet linkedMovables;

	/// 計算結果
	PTM::VVector<double> value;

	/** @brief デフォルトコンストラクタ
	*/
	CRIKMovable(){};

	/** @brief コンストラクタ
	*/
	CRIKMovable(const CRIKMovableDesc& desc){
		this->bias = desc.bias;
	}

	/** @brief IKの計算準備をする
	*/
	virtual void PrepareSolve();

	/** @brief IKの計算繰返しの１ステップを実行する
	*/
	virtual void ProceedSolve();

	/** @brief 計算結果を取得する
	*/
	virtual PTM::VVector<double> GetValue(){ return value; }

	/** @brief 計算結果に従って制御対象を動かす
	*/
	virtual void Move(){}

	/** @brief 制御点を追加する
	*/
	virtual void AddIKControl(CRIKControlIf* control);

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
	virtual PTM::VMatrixRow<double> CalcJacobian(CRIKControlIf* control);
};

/** @brief 位置制御可能な剛体
*/
class CRIKMovableSolidPos : public CRIKMovable, public CRIKMovableSolidPosIfInit {
protected:
	/// 制御対象の剛体
	PHSolidIf* solid;

public:
	OBJECTDEF(CRIKMovableSolidPos, CRIKMovable);

	/** @brief デフォルトコンストラクタ
	*/
	CRIKMovableSolidPos(){SetNDOF(3);}

	/** @brief コンストラクタ
	*/
	CRIKMovableSolidPos(const CRIKMovableSolidPosDesc& desc) : CRIKMovable(desc){
		SetNDOF(3);
		this->solid = desc.solid;
	}

	/** @brief 指定した制御点との間のヤコビアンを計算する
	*/
	virtual PTM::VMatrixRow<double> CalcJacobian(CRIKControlIf* control);
};

/** @brief 姿勢制御可能な剛体
*/
class CRIKMovableSolidOri : public CRIKMovable, public CRIKMovableSolidOriIfInit {
protected:
	/// 制御対象の剛体
	PHSolidIf* solid;

public:
	OBJECTDEF(CRIKMovableSolidOri, CRIKMovable);

	/** @brief デフォルトコンストラクタ
	*/
	CRIKMovableSolidOri(){SetNDOF(3);}

	/** @brief コンストラクタ
	*/
	CRIKMovableSolidOri(const CRIKMovableSolidOriDesc& desc) : CRIKMovable(desc){
		SetNDOF(3);
		this->solid = desc.solid;
	}

	/** @brief 指定した制御点との間のヤコビアンを計算する
	*/
	virtual PTM::VMatrixRow<double> CalcJacobian(CRIKControlIf* control);
};

/** @brief 角度制御可能なボールジョイント
*/
class CRIKMovableBallJointOri : public CRIKMovable, public CRIKMovableBallJointOriIfInit {
protected:
	/// 制御対象の関節
	PHBallJointIf* joint;

public:
	OBJECTDEF(CRIKMovableBallJointOri, CRIKMovable);

	/** @brief デフォルトコンストラクタ
	*/
	CRIKMovableBallJointOri(){SetNDOF(3);}

	/** @brief コンストラクタ
	*/
	CRIKMovableBallJointOri(const CRIKMovableBallJointOriDesc& desc) : CRIKMovable(desc){
		SetNDOF(3);
		this->joint = desc.joint;
	}

	/** @brief 指定した制御点との間のヤコビアンを計算する
	*/
	virtual PTM::VMatrixRow<double> CalcJacobian(CRIKControlIf* control);
};

/** @brief 角度制御可能な三連ヒンジジョイント
*/
class CRIKMovable3HingeJointOri : public CRIKMovable, public CRIKMovable3HingeJointOriIfInit {
protected:
	/// 制御対象の関節
	PHHingeJointIf *joint1, *joint2, *joint3;

public:
	OBJECTDEF(CRIKMovable3HingeJointOri, CRIKMovable);

	/** @brief デフォルトコンストラクタ
	*/
	CRIKMovable3HingeJointOri(){SetNDOF(3);}

	/** @brief コンストラクタ
	*/
	CRIKMovable3HingeJointOri(const CRIKMovable3HingeJointOriDesc& desc) : CRIKMovable(desc){
		SetNDOF(3);
		this->joint1 = desc.joint1;
		this->joint2 = desc.joint2;
		this->joint3 = desc.joint3;
	}

	/** @brief 計算結果に従って制御対象を動かす
	*/
	virtual void Move();

	/** @brief 指定した制御点との間のヤコビアンを計算する
	*/
	virtual PTM::VMatrixRow<double> CalcJacobian(CRIKControlIf* control);
};

/** @brief 角度制御可能なヒンジジョイント
*/
class CRIKMovableHingeJointOri : public CRIKMovable, public CRIKMovableHingeJointOriIfInit {
protected:
	/// 制御対象の関節
	PHHingeJointIf *joint;

public:
	OBJECTDEF(CRIKMovableHingeJointOri, CRIKMovable);

	/** @brief デフォルトコンストラクタ
	*/
	CRIKMovableHingeJointOri(){SetNDOF(1);}

	/** @brief コンストラクタ
	*/
	CRIKMovableHingeJointOri(const CRIKMovableHingeJointOriDesc& desc) : CRIKMovable(desc){
		SetNDOF(1);
		this->joint = desc.joint;
	}

	/** @brief 計算結果に従って制御対象を動かす
	*/
	virtual void Move();

	/** @brief 指定した制御点との間のヤコビアンを計算する
	*/
	virtual PTM::VMatrixRow<double> CalcJacobian(CRIKControlIf* control);
};
}
//@}
#endif//CRINVERSEKINEMATICS_H

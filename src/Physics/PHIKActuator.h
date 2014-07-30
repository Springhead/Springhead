/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PH_IKACTUATOR_H
#define PH_IKACTUATOR_H

#include <Physics/SprPHIK.h>
#include <Physics/SprPHJoint.h>
#include <Physics/SprPHSolid.h>
#include "../Foundation/Object.h"
#include "PHIKEndEffector.h"
#include "PhysicsDecl.hpp"
#include <set>
#include <map>

namespace Spr{;

class PHIKActuator;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// IKActuator：IKに使用できるアクチュエータ（作動部品）
// 

class PHIKActuator : public SceneObject{
public:
	SPR_OBJECTDEF(PHIKActuator);
	SPR_DECLMEMBEROF_PHIKActuatorState;
	SPR_DECLMEMBEROF_PHIKActuatorDesc;
	virtual size_t GetStateSize() const { return sizeof(PHIKActuatorState); }

	// 関節ツリー上でつながったアクチュエータ
	// ※ 計算に必要となるのは
	//    「先祖と子孫にあたるアクチュエータすべて」
	//      （“「このアクチュエータで動かせるいずれかのエンドエフェクタ」を動かせる他のアクチュエータ” にあたる）
	//    「子孫にあたるアクチュエータにつながれたエンドエフェクタすべて」
	//      （“このアクチュエータで動かせるエンドエフェクタ” にあたる）

	/// 直系祖先・子孫（自分含む）・直接の子供
	std::vector<PHIKActuator*> ascendant, descendant, children;
	/// 直接の親
	PHIKActuator* parent;
	/// 祖先・子孫にまとめてアクセスする関数
	PHIKActuator* Link(size_t i) { return (i<ascendant.size()) ? ascendant[i] : descendant[i-ascendant.size()]; }
	int NLinks() {return (int)(ascendant.size()+descendant.size());}

	/// このアクチュエータで直接つながれたエンドエフェクタ．1アクチュエータに対し1エンドエフェクタが対応
	PHIKEndEffector* eef;

	/// 制御対象の関節
	PHJointIf* joint;

	// --- --- --- --- ---

	/// 番号
	int number;

	/// 自由度
	int ndof;

	/// 自由度変化フラグ
	bool bNDOFChanged;

	/// アクチュエータ追加フラグ
	bool bActuatorAdded;

	// --- --- --- --- ---

	/// デバッグ表示用の姿勢履歴
	std::vector<Posed> solidTempPoseHistory;
	size_t historyCnt;

	// --- --- --- --- ---

	/// 計算用の一時変数
	PTM::VVector<double>						alpha, beta;
	std::map< int, PTM::VMatrixRow<double> >	gamma;

	/// ヤコビアン
	std::map< int,PTM::VMatrixRow<double> > Mj;

	/// 標準姿勢引戻し速度
	PTM::VVector<double> domega_pull;

	/// IKのIterationの一回前の計算結果（収束判定用）
	PTM::VVector<double> omega_prev;

	/// IKの計算結果（角度）
	PTM::VVector<double> omega;
	PTM::VVector<double> omega2; // 軌道速度用

	/// IDの計算結果（トルク）
	PTM::VVector<double> tau;

	/// Biasの逆数の平方根
	double sqsaib;

	// --- --- --- --- --- --- --- --- --- ---

	/// 剛体に実現されるべき速度
	Vec3d solidVelocity;

	/// 剛体に実現されるべき角速度
	Vec3d solidAngularVelocity;

	// --- --- --- --- --- --- --- --- --- ---

	/** @brief 初期化
	*/
	virtual void Init() {
		number         = -1;
		bNDOFChanged   = true;
		bActuatorAdded = false;
		parent         = NULL;
		eef            = NULL;
		joint          = NULL;
		solidTempPose  = Posed();
		sqsaib         = 1.0;

		solidTempPoseHistory.resize(2);
		for (size_t i=0; i<solidTempPoseHistory.size(); ++i) { solidTempPoseHistory[i] = Posed(); }
		historyCnt = 0;

		jointTempOri   = Quaterniond();
		if (descendant.size()==0) { descendant.push_back(this); }
	}

	/** @brief デフォルトコンストラクタ
	*/
	PHIKActuator(){
		Init();
	}

	/** @brief コンストラクタ
	*/
	PHIKActuator(const PHIKActuatorDesc& desc){
		Init();
		SetDesc(&desc);
	}

	// --- --- --- --- ---

	/** @brief IKの計算準備をする
	*/
	virtual void PrepareSolve();

	/** @brief IKの計算繰返しの１ステップを実行する
	*/
	virtual void ProceedSolve();

	/** @brief 擬似逆解を解いたままの"生の"計算結果を取得する
	*/
	virtual PTM::VVector<double> GetRawSolution(){ return omega; }

	// --- --- --- --- ---

	/** @brief 計算結果に従って制御対象を動かす
	*/
	virtual void Move(){}

	// --- --- --- --- ---

	/** @brief 動かしにくさを設定・取得する
	*/
	void  SetBias(float bias){ this->bias = bias; sqsaib = 1/sqrt(bias); }
	float GetBias()          { return bias; }

	/** @brief 標準姿勢への復帰率を取得・設定する
	*/
	void SetPullbackRate(double pullbackRate) { this->pullbackRate = pullbackRate; }
	double GetPullbackRate() { return this->pullbackRate; }

	/** @brief 有効・無効を設定・取得する
	*/
	void Enable(bool enable){ bEnabled = enable; }
	bool IsEnabled()        { return bEnabled; }

	/** @brief 祖先を取得する
	*/
	int NAncestors() { return (int)ascendant.size(); }
	PHIKActuatorIf* GetAncestor(int i) { return ascendant[i]->Cast(); }

	/** @brief 直接の親を取得する
	*/
	PHIKActuatorIf* GetParent() { return parent->Cast(); }

	/** @brief 直接の子アクチュエータを取得する
	*/
	int NChildActuators() { return (int)children.size(); }
	PHIKActuatorIf* GetChildActuator(int i) { return children[i]->Cast(); }

	/** @brief 子エンドエフェクタを取得する
	*/
	PHIKEndEffectorIf* GetChildEndEffector() { return eef->Cast(); }

	// --- --- --- --- ---

	virtual bool		AddChildObject(ObjectIf* o);
	virtual ObjectIf*	GetChildObject(size_t pos);
	virtual	size_t		NChildObject()const;

	// --- --- --- --- --- --- --- --- --- ---
	// Non API Methods

	/** @brief ヤコビアン計算前の処理
	*/
	virtual void BeforeCalcAllJacobian() {}

	/** @brief 関係するすべての制御点とのヤコビアンをそれぞれ求める
	*/
	void CalcAllJacobian();

	/** @brief 計算用変数を準備する前の処理
	*/
	virtual void BeforeSetupMatrix() {}

	/** @brief 計算用変数を準備する
	*/
	void SetupMatrix();

	/** @brief 指定した制御点との間のヤコビアンを計算する
	*/
	virtual void CalcJacobian(PHIKEndEffector* endeffector) {}

	/** @brief 引き戻し速度を計算する
	*/
	virtual void CalcPullbackVelocity() {}

	/** @brief 繰返し計算の一ステップの後に行う処理
	*/
	virtual void AfterProceedSolve() {}

	// --- --- --- --- --- --- --- --- --- ---

	/** @brief 一時変数の関節角度を現実の関節角度に合わせる
	*/
	virtual void ApplyExactState(bool reverse=false) { }

	/** @brief 一時変数の関節角度を可動域内にクリップする
	*/
	virtual bool LimitTempJoint() { return false; }

	/** @brief IK計算結果にしたがって一時変数の関節角度を動かす
	*/
	virtual void MoveTempJoint() {}

	/** @brief 順運動学（FK）計算
	*/
	void FK();

	/** @brief FK結果の保存（デバッグ・表示用）
	*/
	void SaveFKResult() {
		solidTempPoseHistory[historyCnt] = solidTempPose;
		historyCnt++;
		if (historyCnt >= solidTempPoseHistory.size()) { historyCnt = 0; }
	}

	/** @brief solidTempPoseにアクセスする
	*/
	virtual Posed GetSolidTempPose() { return solidTempPose; }
};

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

class PHIKBallActuator : public PHIKActuator{
public:
	SPR_OBJECTDEF(PHIKBallActuator);
	SPR_DECLMEMBEROF_PHIKBallActuatorDesc;

	/// IKの回転軸
	Vec3d e[3];

	/// 関節で実現すべき速度
	Vec3d jointVelocity;

	/// 補間後の関節角度（in 回転ベクトル）（IK条件の変更等によるTargetPositionの急激な変化を避けるため）
	Vec3d jointTempOriIntp;

	// --- --- --- --- --- --- --- --- --- ---

	/** @brief 初期化
	*/
	virtual void Init() {
		ndof = 2;
		PHIKActuator::Init();
		jointVelocity = Vec3d();
		solidVelocity = Vec3d();
		solidAngularVelocity = Vec3d();
		jointTempOriIntp = Vec3d();
	}

	/** @brief デフォルトコンストラクタ
	*/
	PHIKBallActuator(){
		Init();
	}

	/** @brief コンストラクタ
	*/
	PHIKBallActuator(const PHIKBallActuatorDesc& desc) {
		Init();
		SetDesc(&desc);
	}

	// --- --- --- --- ---

	/** @brief 計算結果に従って制御対象を動かす
	*/
	virtual void Move();

	// --- --- --- --- ---

	/** @brief 動作対象の関節を設定する（１アクチュエータにつき１関節が必ず対応する）
	*/
	virtual void SetJoint(PHBallJointIf* joint) { this->joint = joint; }

	/** @brief 動作対象として設定された関節を取得する
	*/
	virtual PHBallJointIf* GetJoint() { return this->joint->Cast(); }

	// --- --- --- --- ---

	virtual bool		AddChildObject(ObjectIf* o);
	virtual ObjectIf*	GetChildObject(size_t pos);
	virtual	size_t		NChildObject()const;

	// --- --- --- --- --- --- --- --- --- ---
	// Non API Methods

	/** @brief 回転軸を計算する
	*/
	virtual void CalcAxis();

	/** @brief ヤコビアン計算前の処理
	*/
	virtual void BeforeCalcAllJacobian();

	/** @brief 計算用変数を準備する前の処理
	*/
	virtual void BeforeSetupMatrix();

	/** @brief 指定した制御点との間のヤコビアンを計算する
	*/
	virtual void CalcJacobian(PHIKEndEffector* endeffector);

	/** @brief 引き戻し速度を計算する
	*/
	virtual void CalcPullbackVelocity();

	// --- --- --- --- --- --- --- --- --- ---

	/** @brief 一時変数の関節角度を現実の関節角度に合わせる
	*/
	virtual void ApplyExactState(bool reverse=false);

	/** @brief 一時変数の関節角度を可動域内にクリップする
	*/
	virtual bool LimitTempJoint();

	/** @brief IK計算結果にしたがって一時変数の関節角度を動かす
	*/
	virtual void MoveTempJoint();
};

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

class PHIKHingeActuator : public PHIKActuator{
public:
	SPR_OBJECTDEF(PHIKHingeActuator);
	SPR_DECLMEMBEROF_PHIKHingeActuatorDesc;

	// --- --- --- --- ---

	/// 関節で実現すべき速度
	double jointVelocity;

	/// 補間後の関節角度（IK条件の変更等によるTargetPositionの急激な変化を避けるため）
	double jointTempAngleIntp;

	// --- --- --- --- --- --- --- --- --- ---

	/** @brief 初期化
	*/
	virtual void Init() {
		ndof = 1;
		jointTempAngle = 0;
		jointTempAngleIntp = 0;
		PHIKActuator::Init();
	}

	/** @brief デフォルトコンストラクタ
	*/
	PHIKHingeActuator(){
		Init();
	}

	/** @brief コンストラクタ
	*/
	PHIKHingeActuator(const PHIKHingeActuatorDesc& desc) {
		Init();
		SetDesc(&desc);
	}

	// --- --- --- --- ---

	/** @brief 計算結果に従って制御対象を動かす
	*/
	virtual void Move();

	// --- --- --- --- ---

	/** @brief 動作対象の関節を設定する（１アクチュエータにつき１関節が必ず対応する）
	*/
	virtual void SetJoint(PHHingeJointIf* joint) { this->joint = joint; }

	/** @brief 動作対象として設定された関節を取得する
	*/
	virtual PHHingeJointIf* GetJoint() { return this->joint->Cast(); }

	// --- --- --- --- ---

	virtual bool		AddChildObject(ObjectIf* o);
	virtual ObjectIf*	GetChildObject(size_t pos);
	virtual	size_t		NChildObject()const;

	// --- --- --- --- --- --- --- --- --- ---
	// Non API Methods

	/** @brief 指定した制御点との間のヤコビアンを計算する
	*/
	virtual void CalcJacobian(PHIKEndEffector* endeffector);

	/** @brief 引き戻し速度を計算する
	*/
	virtual void CalcPullbackVelocity();

	// --- --- --- --- --- --- --- --- --- ---

	/** @brief 一時変数の関節角度を現実の関節角度に合わせる
	*/
	virtual void ApplyExactState(bool reverse=false);

	/** @brief 一時変数の関節角度を可動域内にクリップする
	*/
	virtual bool LimitTempJoint();

	/** @brief IK計算結果にしたがって一時変数の関節角度を動かす
	*/
	virtual void MoveTempJoint();


	// <!!>
	double GTA() { return jointTempAngle; }
};

}

#endif

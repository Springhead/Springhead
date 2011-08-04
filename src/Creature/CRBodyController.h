/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRBodyController_H
#define CRBodyController_H

#include <Creature/CREngine.h>
#include <Creature/SprCRBody.h>
#include <Creature/SprCRBodyController.h>

#include <vector>

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 各パーツの到達目標を管理する構造体
*/
struct CRBCTargetValue {
	/// 対象剛体
	CRIKSolidIf* solid;

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

	CRBCTargetValue() {
		bEnable = false;
	}
};


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 全身運動コントローラ
*/
class CRBodyController : public CREngine, public CRBodyControllerDesc {
private:
	/// 予測シミュレーション用の状態格納変数
	// UTRef<ObjectStatesIf> state;

	/// 制御対象のクリーチャ身体
	CRBodyIf* body;

	/// 各パーツの到達目標リスト
	std::vector<CRBCTargetValue*> targetValues;

	/// 目標リストから指定ラベルを持つパーツに関する目標値セットを探索（なければ作る）
	CRBCTargetValue* FindTargetValue(UTString label);

	/// デバッグ表示用剛体
	PHSolidIf* soPoint;

public:
	SPR_OBJECTDEF(CRBodyController);
	ACCESS_DESC(CRBodyController);

	CRBodyController(){
		body = NULL;
		soPoint = NULL;
	}
	CRBodyController(const CRBodyControllerDesc& desc) 
		: CRBodyControllerDesc(desc)
	{
		body = NULL;
		soPoint = NULL;
		Init();
	}

	/** @ brief 初期化を実行する
	*/
	virtual void Init();

	/** @ brief 制御のステップを実行する
	*/
	virtual void Step();

	/** @brief 目標位置の設定
		@param effector 動作させるパーツのラベル
		@param pos 目標位置
	*/
	virtual void SetTargetPos(UTString effector, Vec3d pos);

	/** @brief 目標姿勢の設定
		@param effector 動作させるパーツのラベル
		@param ori 目標姿勢
	*/
	virtual void SetTargetOri(UTString effector, Quaterniond ori);

	/** @brief 目標位置・姿勢の設定
		@param effector 動作させるパーツのラベル
		@param pose 目標位置・姿勢
	*/
	virtual void SetTargetPose(UTString effector, Posed pose);

	/** @brief 目標到達時刻の設定
		@param effector 動作させるパーツのラベル
		@param timeLimit 到達までの目標経過時間
	*/
	virtual void SetTimeLimit(UTString effector, float timeLimit);

	/** @brief 軌道を計画する
	*/
	virtual void Plan();

	/** @brief 運動開始
		@param effector 動作させるパーツのラベル
	*/
	virtual void Restart(UTString effector);

	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const { return body ? 1 : 0; }
	virtual ObjectIf* GetChildObject(size_t i) { return( ((i==0) ? body : NULL) ); }
	virtual bool AddChildObject(ObjectIf* o){ CRBodyIf* b=o->Cast(); if(b){body=b; return true;} return false; }
	virtual bool DelChildObject(ObjectIf* o){ if (o==body) { body = NULL; return true;} return false; }
};

}
//@}

#endif//CRBodyController_H

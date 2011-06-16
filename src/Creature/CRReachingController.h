/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRREACHINGCONTROLLER_H
#define CRREACHINGCONTROLLER_H

#include <Creature/CREngine.h>
#include <Creature/SprCRReachController.h>

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 到達運動コントローラ
*/
class CRReachingController : public CREngine, public CRReachingControllerDesc {
public:
	/// 経過時間
	float time;

	/// 到達目標時間
	float period;

	/// 最終目標位置・速度・姿勢・角速度
	Vec3f fP, fV;

	/// 有効かどうか
	bool bActive;

	/// 現在の目標
	Vec3f pos, vel;

	/// 目標までの初期距離
	double initLen;

	/// 予測シミュレーションのために格納しておく状態
	UTRef<ObjectStatesIf> state;

public:
	SPR_OBJECTDEF(CRReachingController);
	ACCESS_DESC(CRReachingController);

	/// 制御対象のクリーチャ用剛体
	CRIKSolidIf* cSolid;

	CRReachingController(){}
	CRReachingController(const CRReachingControllerDesc& desc) 
		: CRReachingControllerDesc(desc)
	{
		bActive = false;
		Init();
	}

	/** @ brief 初期化を実行する
	*/
	virtual void Init();

	/** @ brief 制御のステップを実行する
	*/
	virtual void Step();

	/** @brief デバッグ情報を表示する
	*/
	virtual void Render(GRRenderIf* render);

	/** @brief 位置を到達させる
	*/
	virtual void Start(Vec3d pos, Vec3d v, float t);

	/** @brief 到達状態を解除する
	*/
	virtual void Stop();

	/** @brief 目標位置の再設定
	*/
	virtual void SetPos(Vec3f pos){ fP = pos; }

	/** @brief 制御対象剛体の設定
	*/
	virtual void SetCRSolid(CRIKSolidIf* cso){ cSolid = cso; }

	/** @brief IK制御点の取得
	*/
	virtual CRIKSolidIf* GetCRSolid(){ return cSolid; }
};

/** @brief 到達運動コントローラの集合体
*/
class CRReachingControllers : public CREngine, public CRReachingControllersDesc {
private:
	std::vector<CRReachingControllerIf*> controllers;

public:
	SPR_OBJECTDEF(CRReachingControllers);
	ACCESS_DESC(CRReachingControllers);

	CRReachingControllers(){}
	CRReachingControllers(const CRReachingControllersDesc& desc)
		: CRReachingControllersDesc(desc)
	{
	}

	/** @ brief 制御のステップを実行する
	*/
	virtual void Step() { /* std::cout << "CRReachingControllers::Step() called" << std::endl; */ }

	/** @brief 到達運動コントローラを取得する
	*/
	CRReachingControllerIf* GetReachingController(PHSolidIf* solid){
		/*
		for (size_t i=0; i<controllers.size(); ++i) {
			CRReachingController* ct = controllers[i]->Cast();
			if (ct && ct->ikcp->GetSolid() == solid) {
				return controllers[i];
			}
		}

		{
			CRCreatureIf* creature = DCAST(SceneObject,this)->GetScene()->Cast();
			CRReachingControllerDesc reachDesc;
			CRReachingControllerIf*  reachCtl = creature->CreateEngine(reachDesc)->Cast();
			controllers.push_back(reachCtl);
			
			for (int i=0; i<creature->NBodies(); ++i) {
				CRBodyIf* body = creature->GetBody(i);
				for (int j=0; j<body->NControlPoints(); ++j) {
					PHIKPosCtlIf* posCtl = body->GetControlPoint(j)->Cast();
					if (posCtl && posCtl->GetSolid() == solid) {
						reachCtl->SetIKCP(posCtl);
						return reachCtl;
					}
				}
			}
		}
		*/

		return NULL;
	}

};
}
//@}

#endif//CRREACHINGCONTROLLER_H

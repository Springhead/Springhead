/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CR_BODY_CONTROLLER_IF_H
#define SPR_CR_BODY_CONTROLLER_IF_H

#include <Creature/SprCREngine.h>

namespace Spr{;

// ------------------------------------------------------------------------------

struct PHSolidIf;
struct CRIKSolidIf;

/// 全身運動コントローラのインターフェース
struct CRBodyControllerIf : CREngineIf{
	SPR_IFDEF(CRBodyController);

	/** @brief 目標位置の設定
		@param effector 動作させるパーツのラベル
		@param pos 目標位置
	*/
	void SetTargetPos(UTString effector, Vec3d pos);

	/** @brief 目標姿勢の設定
		@param effector 動作させるパーツのラベル
		@param ori 目標姿勢
	*/
	void SetTargetOri(UTString effector, Quaterniond ori);

	/** @brief 目標位置・姿勢の設定
		@param effector 動作させるパーツのラベル
		@param pose 目標位置・姿勢
	*/
	void SetTargetPose(UTString effector, Posed pose);

	/** @brief 目標到達時刻の設定
		@param effector 動作させるパーツのラベル
		@param timeLimit 到達までの目標経過時間
	*/
	void SetTimeLimit(UTString effector, float timeLimit);

	/** @brief 軌道を計画する
	*/
	void Plan();

	/** @brief 運動開始
		@param effector 動作させるパーツのラベル
	*/
	void Restart(UTString effector);
};

/// 全身運動コントローラのデスクリプタ
struct CRBodyControllerDesc : public CREngineDesc{
	SPR_DESCDEF(CRBodyController);
};

}

#endif // SPR_CR_BODY_CONTROLLER_IF_H

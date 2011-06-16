/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRGAZECONTROLLER_H
#define CRGAZECONTROLLER_H

#include <Creature/CREngine.h>
#include <Creature/SprCRGazeController.h>

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 視線運動コントローラ
*/
class CRGazeController : public CREngine, public CRGazeControllerDesc {
private:
	/// 注視点
	Vec3d pos, vel;

	/// 注意の度合
	float attractiveness;

	/// 下位のコントローラ：眼球運動
	CREyeControllerIf* eyeCtrl;

	/// 下位のコントローラ：首運動
	CRNeckControllerIf* neckCtrl;

public:
	SPR_OBJECTDEF(CRGazeController);
	ACCESS_DESC_STATE(CRGazeController);

	CRGazeController(){}
	CRGazeController(const CRGazeControllerDesc& desc) 
		: CRGazeControllerDesc(desc) 
	{
	}

	/** @brief 初期化を実行する
	*/
	virtual void Init();

	/** @brief 制御処理を実行する
	*/
	virtual void Step();

	/** @brief 注視点を設定する
		@param pos 注視点の３次元座標
		@param vel 注視点の移動速度ベクトル
	*/
	virtual void LookAt(Vec3f pos, Vec3f vel, float attractiveness);
};
}
//@}

#endif//CRGAZECONTROLLER_H

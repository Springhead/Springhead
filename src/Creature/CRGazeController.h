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
#include <Creature/SprCRController.h>

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 視線運動コントローラ
*/
class CRGazeController : public CRController, public CRGazeControllerDesc {
private:

public:
	SPR_OBJECTDEF(CRGazeController);
	ACCESS_DESC(CRGazeController);

	CRGazeController(){}
	CRGazeController(const CRGazeControllerDesc& desc) 
		: CRGazeControllerDesc(desc) 
	{
	}

	/// 初期化を実行する
	virtual void Init();

	/// 制御処理を実行する
	virtual void Step();

	///  状態をリセットする
	virtual void Reset();

	///  現状を返す
	virtual int  GetStatus();

	///  視対象位置をセットする
	virtual void SetTargetPosition(Vec3d pos);
};
}
//@}

#endif//CRGAZECONTROLLER_H

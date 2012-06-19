/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRREACHCONTROLLER_H
#define CRREACHCONTROLLER_H

#include <Creature/CREngine.h>
#include <Creature/SprCRController.h>

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 到達運動コントローラ
*/
class CRReachController : public CRController, public CRReachControllerDesc {
private:

public:
	SPR_OBJECTDEF(CRReachController);
	ACCESS_DESC(CRReachController);

	CRReachController(){}
	CRReachController(const CRReachControllerDesc& desc) 
		: CRReachControllerDesc(desc) 
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

	///  到達対象位置をセットする
	virtual void SetTargetPosition(Vec3d pos);
};
}
//@}

#endif//CRREACHCONTROLLER_H

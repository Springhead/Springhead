/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRCONTROLLERIF_H
#define SPR_CRCONTROLLERIF_H

#include <Foundation/SprObject.h>
#include <Creature/SprCREngine.h>

namespace Spr{;

struct PHSolidIf;

//@{

///	クリーチャ・コントローラの基本クラス
struct CRControllerIf : public CREngineIf{
	SPR_IFDEF(CRController);

	///	状態をリセットする
	void Reset();

	/// 現状を返す
	int GetStatus();
};

struct CRControllerDesc{
	SPR_DESCDEF(CRController);

	/// コントローラの現状を表すEnum
	enum CRControllerStatus {
		CS_WAITING = 0,
		CS_WORKING,
		CS_DONE,
		CS_FAILED,
	};

	CRControllerDesc(){
	}
};

///	視線コントローラ
struct CRGazeControllerIf : public CRControllerIf{
	SPR_IFDEF(CRGazeController);

	///  視対象位置をセットする
	void SetTargetPosition(Vec3d pos);
};

struct CRGazeControllerDesc{
	SPR_DESCDEF(CRGazeController);

	/// ラベル <!!>

	CRGazeControllerDesc(){
	}
};

///	到達運動コントローラ
struct CRReachControllerIf : public CRControllerIf{
	SPR_IFDEF(CRReachController);

	///  到達対象位置をセットする
	void SetTargetPosition(Vec3d pos);
};

struct CRReachControllerDesc{
	SPR_DESCDEF(CRReachController);

	/// ラベル <!!>

	CRReachControllerDesc(){
	}
};

///	把持コントローラ
struct CRGrabControllerIf : public CRControllerIf{
	SPR_IFDEF(CRGrabController);

	///  把持対象剛体をセットする
	void SetTargetSolid(PHSolidIf* targetSolid);

	/// つかんでいる物体を返す．つかんでいなければNULL
	PHSolidIf* GetGrabbingSolid();

	/// 把持に使用する剛体を返す
	PHSolidIf* GetSolid();
};

struct CRGrabControllerDesc{
	SPR_DESCDEF(CRGrabController);

	/// ラベル <!!>

	CRGrabControllerDesc(){
	}
};

//@}

}

#endif//SPR_CRENGINEIF_H

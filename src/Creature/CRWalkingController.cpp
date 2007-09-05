/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRWalkingController.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
IF_OBJECT_IMP(CRWalkingController, CRController);

void CRWalkingController::Init(){
	CRController::Init();

	/// 依存する他のオブジェクトの取得方法（もし必要なら）．

	/// < Body >
	/*
	body = DCAST(CRHingeHumanBodyIf, creature->GetBody());
	*/

	/// < PHScene >
	/*
	phScene = DCAST(PHSceneIf, creature->GetScene());
	*/

	/// < InternalScene >
	/*
	internalScene = creature->GetInternalScene();
	*/

	/// < ほかのController > 
	/*
	for (int i=0; i<creature->NControllers(); i++) {
		if (!gazeCtrl) {
			gazeCtrl = DCAST(CRGazeControllerIf, creature->GetController(i))
		}
	}
	*/

	// < ほかのSensor >
	/*
	for (int i=0; i<creature->NSensors(); i++) {
		if (!opticalSensor) {
			opticalSensor = DCAST(CROpticalSensorIf, creature->GetSensor(i))
		}
	}
	*/
}
	
void CRWalkingController::Step(){
	CRController::Step();

	/// 制御のステップを実行する．
}

/// 上記以外にCRWalkingControllerIfに追加したAPI関数の実装をここに追加．

}

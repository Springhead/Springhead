/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRVisualSensor.h>
#include <Creature/SprCRBody.h>
#include <Creature/SprCRBone.h>
#include <Creature/SprCRCreature.h>
#include <Physics/SprPHScene.h>

namespace Spr{

void CRVisualSensor::Step(){
	CRBodyIf*  body    = DCAST(CRCreatureIf,DCAST(SceneObjectIf,this)->GetScene())->GetBody(0);
	PHSceneIf* phScene = DCAST(CRCreatureIf,DCAST(SceneObjectIf,this)->GetScene())->GetPHScene();

	// 視覚リストの構築を開始する
	visibleList[write].clear();

	// PHScene中のすべての剛体についてチェック
	for (int i=0; i<phScene->NSolids(); ++i) {
		bool bMyBody = false;
		for (int n=0; n<body->NBones(); ++n) {
			if (body->GetBone(n) && body->GetBone(n)->GetPHSolid() == phScene->GetSolids()[i]) { bMyBody = true; }
		}

		PHSolidIf* so = phScene->GetSolids()[i];

		Vec3d pos      = so->GetPose().Pos();
		Vec3d localPos = (soVisualSensor->GetPose() * pose).Inv() * pos;
			
		double dist = localPos.norm();
		Vec3d direction = Vec3d();
		if (localPos.norm() != 0) { direction = localPos.unit(); }

		Vec2d theta = Vec2d(atan2(direction.x, -direction.y), atan2(direction.z, -direction.y));
		theta.y *= (range.X() / range.Y());

		Vec2d thetaC = Vec2d(atan2(direction.x, -direction.y), atan2(direction.z, -direction.y));
		thetaC.y *= (centerRange.X() / centerRange.Y());
		if (theta.norm() < (range.X() / 2.0) && dist < 30) { // 決め打ち<!!>
			// Visible
			CRVisualInfo visible;
			visible.posWorld = pos;
			visible.posLocal = localPos;
			visible.velWorld = so->GetVelocity();
			visible.velLocal = (so->GetPose() * pose).Inv() * so->GetVelocity();
			visible.solid    = so;
			visible.bMyBody  = bMyBody;
			visible.bCenter  = ( thetaC.norm() < (centerRange.X() / 2.0) );

			visibleList[write].push_back(visible);
		}
	}

	// 書き込み完了　→　バッファをローテーション
	if (empty >= 0) {
		// 読み終わったバッファがある場合：そのバッファに書く．
		keep  = write;
		write = empty;
		empty = -1;
	} else {
		// 読み終わったバッファがない場合：前に書き終わってkeepしているバッファに書く．
		int w = write;
		write = keep;
		keep  = w;
	}
}

}

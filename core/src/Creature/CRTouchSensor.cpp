/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRTouchSensor.h>
#include <Creature/SprCRBody.h>
#include <Creature/SprCRBone.h>
#include <Creature/SprCRCreature.h>
#include <Physics/SprPHScene.h>
#include <Physics/PHHapticEngine.h>
#include <Creature/CRBone.h>

namespace Spr{;

void CRTouchSensor::Step() {
	CRBodyIf*  body    = DCAST(CRCreatureIf,DCAST(SceneObjectIf,this)->GetScene())->GetBody(0);
	PHSceneIf* phScene = DCAST(CRCreatureIf,DCAST(SceneObjectIf,this)->GetScene())->GetPHScene();

	// 接触リストの構築を開始する
	contactList[write].clear();

	int sceneCnt = phScene->GetCount();

	PHSolidPairForLCPIf* solidPair;
	PHShapePairForLCPIf* shapePair;

	Vec3d totalForce;

	// PHScene中のすべての剛体についてチェック
	for (int i=0; i<phScene->NSolids(); ++i) {
		for (int j=i+1; j<phScene->NSolids(); ++j) {
			// 自分の体を構成する剛体 と それ以外の剛体 のペアのみに限定
			bool iIsMe = false, jIsMe = false;
			for (int n=0; n<body->NBones(); ++n) {
				if (body->GetBone(n) && body->GetBone(n)->GetPHSolid() == phScene->GetSolids()[i]) { iIsMe = true; }
				if (body->GetBone(n) && body->GetBone(n)->GetPHSolid() == phScene->GetSolids()[j]) { jIsMe = true; }
				if (iIsMe && jIsMe) { break; }
			}
			if ((iIsMe && jIsMe) || (!iIsMe && !jIsMe)) { continue; }

			PHSolidIf *so1, *so2, *soMe, *soOther;
			if (iIsMe) {
				so1 = soMe		= phScene->GetSolids()[i];
				so2 = soOther	= phScene->GetSolids()[j];
			} else {
				so1 = soOther	= phScene->GetSolids()[i];
				so2 = soMe		= phScene->GetSolids()[j];
			}

			// ハプティックポインタが含まれていないか確認
			PHHapticPointer* hp1 = so1->Cast();
			PHHapticPointer* hp2 = so2->Cast();

			if (hp1!=NULL && hp2!=NULL) {
				/// 両方ハプティックポインタである場合
				// <!!> 未対応

			} else if (hp1!=NULL || hp2!=NULL) {
				/// 一方がハプティックポインタである場合
				PHSolidIf* so = NULL; PHHapticPointer* hp = NULL;
				if (hp1) { hp = hp1; so = so2; }
				if (hp2) { hp = hp2; so = so1; }
				DSTR << so->GetName() << std::endl;
				
				for (int m=0; m<hp->NNeighborSolids(); ++m) {
					if (hp->GetNeighborSolid(m) == so) {
						int n = hp->neighborSolidIDs[m];
						PHHapticEngine* he = phScene->GetHapticEngine()->Cast();
						PHSolidPairForHaptic* sop = he->GetSolidPairTemp(n,0)->Cast();
						for (int x=0; x<sop->body[0]->NShape(); ++x) {
							for (int y=0; y<sop->body[0]->NShape(); ++y) {
								if (x < sop->shapePairs.height() && y < sop->shapePairs.width()) {
									PHShapePairForHaptic* shp = sop->GetShapePair(x,y)->Cast();
									Vec3d p0 = (shp->shapePoseW[0]*shp->closestPoint[0]);
									Vec3d p1 = (shp->shapePoseW[1]*shp->closestPoint[1]);
									Vec3d di = (p0-p1) * 100.0; // <!!>
									if (di.norm()!=0  &&  PTM::dot(di.unit(), shp->normal.unit()) > 0) {
										CRContactInfo contact;
										contact.pos			= soOther->GetPose().Pos(); // <!!>
										contact.soMe		= soMe;
										contact.soOther		= soOther;
										contact.force		= di;
										contact.pressure	= di.norm();

										contactList[write].push_back(contact);
									}
								}
							}
						}
					}
				}

			} else {
				/// どちらもハプティックポインタではない場合
				solidPair = phScene->GetSolidPair(i, j);
				PHConstraintIf* constraint = phScene->GetConstraintEngine()->GetContactPoints()->FindBySolidPair(so1, so2);

				if (!solidPair)  { continue; }
				if (!constraint) { continue; }

				Vec3d force = phScene->GetConstraintEngine()->GetContactPoints()->GetTotalForce(so1, so2);
				force = so1->GetPose() * force;

				CRContactInfo contact;
				contact.soMe		= soMe;
				contact.soOther		= soOther;
				contact.force		= force;
				contact.pressure	= force.norm();

				double num = 0;
				for (int s=0; s<so1->NShape(); ++s) {
					for (int t=0; t<so2->NShape(); ++t) {

						// この方法だと同じ剛体の異なるShapeについての接触は
						// 異なるContactとなる。
						// それを剛体ごとにまとめるには皮膚感覚の加算についての知見が必要。
						// とりあえずあとまわし (mitake, 09/02/07)

						shapePair = solidPair->GetShapePair(s, t);

						if (!shapePair) { continue; }

						int			contactStat	= solidPair->GetContactState(s, t);
						unsigned	lastContCnt	= solidPair->GetLastContactCount(s, t);


						if (contactStat == 1 || (contactStat == 2 && ((int)lastContCnt == sceneCnt-1))) {
							totalForce += force;

							double		depth			= solidPair->GetContactDepth(s, t);
							int			nSectionVtx		= shapePair->NSectionVertexes();

							contact.pos += solidPair->GetCommonPoint(s, t);
							num += 1.0;
						}
					}
				}
				contact.pos *= (1/num);

				contactList[write].push_back(contact);
			}
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

} // namespace Spr

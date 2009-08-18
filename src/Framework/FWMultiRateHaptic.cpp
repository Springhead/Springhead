#include <Framework/FWMultiRateHaptic.h>
#include <Framework/SprFWApp.h>
#include <Physics/PHConstraintEngine.h>
#include <sstream>

namespace Spr{;

FWMultiRateHaptic::FWMultiRateHaptic(){
	bSync = false;
	bCalcPhys = true;
	hapticcount = 1;
}
void FWMultiRateHaptic::Sync(){
	FWHapticLoopBase* hLoop = GetHapticLoop();
	if(bSync){ 
		/// 力覚ポインタの同期
		UpdatePointer();			
		/// 同期のための準備
		FWInteractSolids* piSolids = GetInteractSolids();
		FWInteractSolids* hiSolids = GetHapticLoop()->GetInteractSolids();
		FWInteractPointers* piPointers = GetInteractScene()->GetInteractPointers();
		std::vector<FWInteractPointer>* hiPointers = GetHapticLoop()->GetInteractPointers();
		FWInteractSolid *piSolid, *hiSolid;
		FWInteractPointer* piPointer,*hiPointer;
		FWInteractInfo* piInfo, *hiInfo;

		/// HapticLoop--->PhysicsLoop ///
		for(unsigned i = 0; i < (int)hiSolids->size(); i++){
			piSolid = GetInteractSolid(i);
			hiSolid = GetHapticLoop()->GetInteractSolid(i);
			/// bSim = ture かつ bfirstSim = falseなら結果を反映させる
			if(!hiSolid->bSim || hiSolid->bfirstSim) continue;
			double pdt = GetInteractScene()->GetScene()->GetPHScene()->GetTimeStep();			// physicsの刻み
		
			/// 各ポインタが持つ情報を取得
			for(int j = 0; j < (int)piPointers->size(); j++){
				piPointer = GetInteractScene()->GetInteractPointer(j)->Cast();
				hiPointer = GetHapticLoop()->GetInteractPointer(j)->Cast();
				piInfo = &piPointer->interactInfo[i];
				hiInfo = &hiPointer->interactInfo[i];
				piInfo->neighborInfo.test_force_norm = hiInfo->neighborInfo.test_force_norm;
			}
			/// 結果の反映
			SpatialVector b;
			b += (piSolid->b + 
					(piSolid->curb - piSolid->lastb)) * pdt;	// モビリティ定数項
			Vec3d v = hiSolid->copiedSolid.GetVelocity() + b.v();					// 反映速度
			Vec3d w = hiSolid->copiedSolid.GetAngularVelocity() + b.w();			// 反映角速度
			hiSolid->sceneSolid->SetVelocity(v);
			hiSolid->sceneSolid->SetAngularVelocity(w);
			hiSolid->sceneSolid->SetCenterPosition(hiSolid->copiedSolid.GetCenterPosition());
			hiSolid->sceneSolid->SetOrientation(hiSolid->copiedSolid.GetOrientation());
		}
	

		/// PhysicsLoop--->HapticLoop ///
		/// シーンで新しく生成された分を拡張
		for(int i = (int)hiPointers->size(); i < (int)piPointers->size(); i++){
			hiPointers->resize(hiPointers->size() + 1);
			FWInteractPointer* ip = GetInteractScene()->GetInteractPointer(i)->Cast(); 
			hiPointers->back() = *ip;
		}
		for(int i = (int)hiSolids->size(); i < (int)piSolids->size(); i++){
			hiSolids->resize(i + 1);
			hiSolids->back() = *GetInteractSolid(i);
			/// ポインタが持つ情報についても拡張
			for(int j = 0; j < (int)piPointers->size(); j++){
				hiPointer = GetHapticLoop()->GetInteractPointer(j);
				hiPointer->interactInfo.resize(i + 1);
				piPointer = GetInteractScene()->GetInteractPointer(j)->Cast();
				hiPointer->interactInfo.back() = piPointer->interactInfo[i];
			}
		}
		/// 情報の同期
		for(unsigned i = 0; i < hiSolids->size(); i++){
			piSolid = GetInteractSolid(i);
			hiSolid = GetHapticLoop()->GetInteractSolid(i);
			hiSolid->bSim = piSolid->bSim;
			hiSolid->bfirstSim = piSolid->bfirstSim;
			/// 初めてシミュレーション対象になった時
//			DSTR << piSolid->bfirstSim << std::endl;
			if(piSolid->bfirstSim){								
				hiSolid->copiedSolid = piSolid->copiedSolid;	// 最新の情報をコピー
				piSolid->bfirstSim = false;						// 初めて近傍になった場合の処理
			}
			/// ポインタごとに持つ情報の同期
			for(int j = 0; j < (int)piPointers->size(); j++){
				piPointer = GetInteractScene()->GetInteractPointer(j)->Cast();
				hiPointer = GetHapticLoop()->GetInteractPointer(j)->Cast();
				hiPointer->interactInfo[i] = piPointer->interactInfo[i];
			}
		}
		/* 同期終了処理 */
		hLoop->loopCount = 0;		// HapticLoopのカウント初期化
		bSync = false;				// 同期終了フラグ
	}
	#define COUNT_MAX 100
	if(hLoop->loopCount > COUNT_MAX) {
		DSTR << "Too Many Loops in HapticLoop!" << std::endl;
		DSTR << "Reset HapticLoop Count" << std::endl;
		hLoop->loopCount = 0;
		bSync = false;
	}
	hLoop->loopCount++;
	hapticcount++;
}

void FWMultiRateHaptic::Clear(){
	GetHapticLoop()->Clear();
	GetInteractSolids()->clear();
	bSync = false;
	bCalcPhys = true;
	hapticcount = 1;
}


}

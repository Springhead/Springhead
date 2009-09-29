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
	if(bSync){ 
		/// 力覚ポインタの同期
		UpdatePointer();			
		/// 同期のための準備
		FWInteractSolids* hiSolids = GetHapticLoop()->GetINSolids();
		std::vector<FWInteractPointer>* hiPointers = GetHapticLoop()->GetINPointers();
		FWInteractSolid *piSolid, *hiSolid;
		FWInteractPointer* hiPointer;
		FWInteractInfo* piInfo, *hiInfo;

		/// HapticLoop--->PhysicsLoop ///
		for(unsigned i = 0; i < (int)hiSolids->size(); i++){
			hiSolid = GetHapticLoop()->GetINSolid(i);
			/// bSim = ture かつ bfirstSim = falseなら結果を反映させる
			if(!hiSolid->bSim || hiSolid->bfirstSim) continue;
			piSolid = GetINSolid(i);
			double pdt = GetPHScene()->GetTimeStep();			// physicsの刻み
			SpatialVector b;
			b += (piSolid->b + 
					(piSolid->curb - piSolid->lastb)) * pdt;	// モビリティ定数項
			Vec3d v = hiSolid->copiedSolid.GetVelocity() + b.v();					// 反映速度
			Vec3d w = hiSolid->copiedSolid.GetAngularVelocity() + b.w();			// 反映角速度
			
			hiSolid->sceneSolid->SetVelocity(v);
			hiSolid->sceneSolid->SetAngularVelocity(w);
			hiSolid->sceneSolid->SetCenterPosition(hiSolid->copiedSolid.GetCenterPosition());
			hiSolid->sceneSolid->SetOrientation(hiSolid->copiedSolid.GetOrientation());
			/// 各ポインタが持つ情報を同期
			for(int j = 0; j < NINPointers(); j++){
				piInfo = &GetINPointer(j)->interactInfo[i];
				hiPointer = GetHapticLoop()->GetINPointer(j)->Cast();
				hiInfo = &hiPointer->interactInfo[i];
				piInfo->neighborInfo.test_force_norm = hiInfo->neighborInfo.test_force_norm;
			}
		}
	
		/// PhysicsLoop--->HapticLoop ///
		/// シーンで新しく生成された分を拡張
		for(int i = (int)hiPointers->size(); i < NINPointers(); i++){
			hiPointers->resize(i + 1);
			hiPointers->back() = *GetINPointer(i);
		}
		for(int i = (int)hiSolids->size(); i < (int)NINSolids(); i++){
			hiSolids->resize(i + 1);
			hiSolids->back() = *GetINSolid(i);
			/// ポインタが持つ情報についても拡張
			for(int j = 0; j < NINPointers(); j++){
				hiPointer = GetHapticLoop()->GetINPointer(j);
				hiPointer->interactInfo.resize(i + 1);
				hiPointer->interactInfo.back() = GetINPointer(j)->interactInfo[i];
			}
		}
		/// 情報の同期
		for(unsigned i = 0; i < hiSolids->size(); i++){
			piSolid = GetINSolid(i);
			hiSolid = GetHapticLoop()->GetINSolid(i);
			hiSolid->bSim = piSolid->bSim;
			hiSolid->bfirstSim = piSolid->bfirstSim;
			/// 初めてシミュレーション対象になった時
			if(piSolid->bfirstSim){								
				hiSolid->copiedSolid = piSolid->copiedSolid;	// 最新の情報をコピー
				piSolid->bfirstSim = false;						// 初めて近傍になった場合の処理
			}
			//DSTR<<"sync:"<<piSolid->b<<std::endl; //naga
			//hiSolid->b = piSolid->b;
			hiSolid->curb = piSolid->curb;
			hiSolid->lastb = piSolid->lastb;
			/// ポインタごとに持つ情報の同期
			for(int j = 0; j < NINPointers(); j++){
				hiPointer = GetHapticLoop()->GetINPointer(j)->Cast();
				hiPointer->interactInfo[i] = GetINPointer(j)->interactInfo[i];
				hiPointer->bForce = GetINPointer(j)->bForce;
				hiPointer->bVibration = GetINPointer(j)->bVibration;
			}
		}
		/* 同期終了処理 */
		GetHapticLoop()->loopCount = 0;		// HapticLoopのカウント初期化
		bSync = false;						// 同期終了フラグ
	}
	#define COUNT_MAX 100
	if(GetHapticLoop()->loopCount > COUNT_MAX) {
		DSTR << "Too Many Loops in HapticLoop!" << std::endl;
		DSTR << "Reset HapticLoop Count" << std::endl;
		GetHapticLoop()->loopCount = 0;
		bSync = false;
	}
	GetHapticLoop()->loopCount++;
	hapticcount++;
}

void FWMultiRateHaptic::SyncPointer(){
	std::vector<FWInteractPointer>* hiPointers = GetHapticLoop()->GetINPointers();
	for(int i = 0; i < NINPointers(); i++){
		hiPointers->at(i)= *GetINPointer(i);
	}
}

void FWMultiRateHaptic::Clear(){
	GetHapticLoop()->Clear();
	bSync = false;
	bCalcPhys = true;
	hapticcount = 1;
}


}

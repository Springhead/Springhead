#ifndef FWINTERACT_ADAPTEE_H
#define FWINTERACT_ADAPTEE_H

#include <Springhead.h>
#include <Framework/FWInteractPointer.h>
#include <Framework/FWInteractSolid.h>

namespace Spr{;

class FWInteractScene;
class FWInteractPointer;
enum FWHapticMode;

/** Hapticインタラクションのためのクラス*/
class FWHapticLoopBase : public UTRefCount{
protected:
	std::vector<FWInteractPointer>	interactPointers;
	std::vector<FWInteractSolid>	interactSolids;
public:
	double pdt, hdt;
	int loopCount;
	FWHapticMode hmode;
	FWHapticLoopBase(){}
	FWInteractPointer* GetINPointer(int i){ return &interactPointers[i]; }
	std::vector<FWInteractPointer>* GetINPointers(){ return &interactPointers;}
	int NINPointers(){ return (int)interactPointers.size();}
	FWInteractSolid* GetINSolid(int i){ return &interactSolids[i]; } 
	FWInteractSolids* GetINSolids(){ return &interactSolids; }
	int NINSolids(){ return (int)interactSolids.size(); }
	int GetLoopCount(){ return loopCount; }

	virtual void Clear();
	void Init(double physicdt, double hapticdt){
		pdt = physicdt;
		hdt = hapticdt;
		loopCount = 1;
	}
	virtual void Step(){};
	virtual void HapticRendering(){}
};


/** HumanInterfaceを使ってインタラクションを可能にするクラス 
	このクラスを継承させて実装を呼ぶ．
*/
class FWInteractAdaptee : public UTRefCount{
protected:
	FWInteractScene*	interactScene;
public:
	FWInteractAdaptee();

	/// INScene関係
	void SetINScene(FWInteractScene* inScene);
	FWInteractScene* GetINScene();
	PHSceneIf* GetPHScene();

	/// HapticLoop関係
	virtual FWHapticLoopBase* GetHapticLoop();
	void SetHMode(FWHapticMode hMode);

	/// INPointer関係(INSceneを介して取得)
	FWInteractPointer* GetINPointer(int i = -1);
	FWInteractPointers* GetINPointers();
	int NINPointers();

	/// INSolid関係(INSceneを介して取得)
	FWInteractSolid* GetINSolid(int i = -1);
	FWInteractSolids* GetINSolids();
	int NINSolids();

	void UpdateSolidList();

	virtual void UpdatePointer(){};													///< ポインタの位置を更新する
	void NeighborObjectFromPointer();												///< ポインタ近傍の物体を探す
	/// 以下2つの関数はNeighborObjectFromPointer()で呼ばれる
	virtual void UpdateInteractSolid(int index, FWInteractPointer* iPointer);		///< InteractSolidの情報を更新する(オーバーライドして使用)
	double FindNearestPoint(const CDConvexIf* a, const CDConvexIf* b,
											const Posed& a2w, const Posed& b2w, const Vec3d pc, Vec3d& dir, 
											Vec3d& normal, Vec3d& pa, Vec3d& pb);	///< ポインタ近傍の物体との近傍点を探す

	virtual void Clear(){};
	virtual void Init(){};
	virtual void CallBackHapticLoop(){};
	virtual void Step(){};
	virtual void BeginKeyboard(){};
	virtual void EndKeyboard(){};
};

}
#endif
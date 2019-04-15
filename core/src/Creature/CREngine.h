/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRENGINE_H
#define CRENGINE_H

#include <Foundation/Object.h>
#include <Creature/SprCREngine.h>
#include <Creature/SprCRController.h>
//#include <Framework/SprFWObject.h>

#include <vector>

namespace Spr {;

///	クリーチャのエンジンの基本クラス．
class CREngine : public SceneObject {
protected:
	/// 有効か・無効か
	bool enabled;

public:
	SPR_OBJECTDEF(CREngine);

	CREngine() { enabled = true; }

	///	実行順序を決めるプライオリティ値．小さいほど早い
	virtual int GetPriority() const { return CREngineDesc::CREP_CONTROLLER; }

	///	初期化する
	virtual void Init(){}

	///	処理を１ステップ実行する
	virtual void Step(){}

	/// 有効・無効を切り替える
	virtual void Enable(bool enable) { enabled = enable; }

	/// 有効・無効かを返す
	virtual bool IsEnabled() { return enabled; }
};

inline bool operator < (const CREngine& e1, const CREngine& e2){
	return e1.GetPriority() < e2.GetPriority();
}

///	クリーチャのコントローラの基本クラス．
class CRController : public CREngine {
public:
	SPR_OBJECTDEF(CRController);

	CRController() {}

	///	状態をリセットする
	virtual void Reset(){}

	///	現状を返す
	virtual int GetStatus(){ return CRControllerDesc::CS_WAITING; }
};

}

#endif // CRENGINE_H

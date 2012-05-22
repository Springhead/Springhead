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
#include <Framework/SprFWObject.h>

#include <vector>

namespace Spr {;

struct GRRenderIf;
struct FWObjectGroupIf;

///	クリーチャのエンジンの基本クラス．
class CREngine : public SceneObject {
	/// 有効か・無効か
	bool enabled;

protected:
	/// 子要素となるFWObjectグループ
	std::vector<FWObjectGroupIf*> groups;

public:
	SPR_OBJECTDEF(CREngine);

	CREngine() { enabled = true; }

	/// 子要素
	virtual bool       AddChildObject(ObjectIf* o);
	virtual ObjectIf*  GetChildObject(size_t pos);
	virtual size_t     NChildObject() const;

	///	実行順序を決めるプライオリティ値．小さいほど早い
	virtual int GetPriority() const { return CREngineDesc::CREP_CONTROLLER; }

	///	処理を１ステップ実行する
	virtual void Step(){}

	/// 有効・無効を切り替える
	virtual void SetEnable(bool enable) { enabled = enable; }

	/// 有効・無効かを返す
	virtual bool IsEnabled() { return enabled; }

	/// デバッグ情報を描画する
	virtual void Render(GRRenderIf* render){}

	// 非API
	/// グループから必要なオブジェクトを取り出してくる（要オーバーライド）
	virtual void AssignObject() {}
};

inline bool operator < (const CREngine& e1, const CREngine& e2){
	return e1.GetPriority() < e2.GetPriority();
}

}

#endif // CRENGINE_H

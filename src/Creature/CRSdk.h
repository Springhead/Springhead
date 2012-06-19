/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRSDK_H
#define CRSDK_H

#include <Foundation/Scene.h>
#include <Creature/SprCRSdk.h>
#include <Creature/SprCRCreature.h>

namespace Spr {;

class CRSdkFactory : public FactoryBase {
public:
	const IfInfo* GetIfInfo() const {
		return CRSdkIf::GetIfInfoStatic();
	}
	ObjectIf* Create(const void* desc, const ObjectIf*){
		return CRSdkIf::CreateSdk();
	}
};

class SPR_DLL CRSdk : public Sdk {
protected:
	///	クリーチャ
	typedef std::vector< UTRef<CRCreatureIf> > CRCreatures;
	CRCreatures creatures;

public:
	SPR_OBJECTDEF(CRSdk);
	CRSdk();
	~CRSdk();

	/** @brief クリーチャを一体作成する
	 */
	virtual CRCreatureIf* CreateCreature(const IfInfo* ii, const CRCreatureDesc& desc);

	/** @brief クリーチャの個数（人数？）を取得する
	 */
	virtual int NCreatures() const {
		return (int)creatures.size();
	}

	/** @brief クリーチャ一体を取得する
	 */
	virtual CRCreatureIf* GetCreature(int index) {
		return creatures[index];
	}

	/** @brief 全クリーチャについて１ステップ処理を実行する
	 */
	virtual void Step();

	/** @brief 一度全てをクリアしてSDKを作り直す
	 */
	virtual void Clear();

	/** @brief 子要素の扱い
	 */
	virtual size_t NChildObject() const { return NCreatures(); }
	virtual ObjectIf* GetChildObject(size_t i){ return GetCreature((int)i)->Cast(); }
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);

};

}
#endif // CRSDK_H

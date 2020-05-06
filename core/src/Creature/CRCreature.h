/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRCREATURE_H
#define CRCREATURE_H

#include <Foundation/Scene.h>
#include <Creature/SprCRCreature.h>
#include <vector>

//@{
namespace Spr{;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief クリーチャの実装
*/
class CRCreature : public Scene, public CRCreatureDesc {
protected:
	///	エンジン
	typedef std::vector< UTRef<CREngineIf> > CREngines;
	CREngines engines;

	///	ボディ
	typedef std::vector< UTRef<CRBodyIf> > CRBodies;
	CRBodies bodies;

	/// 関連するPHScene
	PHSceneIf* phScene;

public:
	SPR_OBJECTDEF(CRCreature);
	ACCESS_DESC(CRCreature);

	CRCreature(){
		CRRegisterTypeDescs();
	}
	CRCreature(const CRCreatureDesc& desc) : CRCreatureDesc(desc) {
		CRRegisterTypeDescs();
	}

	/** @brief 感覚→情報処理→運動 の１ステップを実行する
	*/
	virtual void Step();

	/** @brief ボディをつくる
	*/
	virtual CRBodyIf* CreateBody(const IfInfo* ii, const CRBodyDesc& desc);

	/** @brief ボディを取得する
	*/
	virtual CRBodyIf* GetBody(int i) { return bodies[i]; }

	/** @brief ボディの数を取得する
	*/
	virtual int NBodies() { return (int)bodies.size(); }

	/** @brief CREngineを作成する
	*/
	virtual CREngineIf* CreateEngine(const IfInfo* ii, const CREngineDesc& desc);

	/** @brief CREngineを取得する
	*/
	virtual CREngineIf* GetEngine(int i) { return engines[i]; }

	/** @brief CREngineの数を取得する
	*/
	virtual int NEngines() { return (int)engines.size(); }

	/** @brief 関連するPHSceneを取得する
	*/
	PHSceneIf* GetPHScene() { return phScene; }

	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const { return bodies.size()+engines.size(); }
	virtual ObjectIf* GetChildObject(size_t i);
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
};

}
//@}

#endif//CRCREATURE_H

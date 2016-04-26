/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHEngine_H
#define PHEngine_H

#include "../Foundation/Object.h"
#include <Physics/SprPHEngine.h>
#include <Physics/SprPHScene.h>

namespace Spr {;

///	シーンの振舞い（ビヘイビア）を管理するエンジン
/// ビヘイビアの分類と優先順位
///	100の倍数は分類を表し、以下の位はその中での実行順序を規定する
enum PHBehaviorPriority{
	//初期化モジュール
	SGBP_INITIALIZER			= 100,
		SGBP_HAPTICENGINE1,
		SGBP_SOLIDINITIALIZER,
		//SGBP_CLEARFORCE,
	//力生成モジュール
	SGBP_FORCEGENERATOR			= 200,
		SGBP_GRAVITYENGINE,
		SGBP_COLLISIONENGINE,
		SGBP_PENALTYENGINE,
		SGBP_CREATURE,
		SGBP_INVERSEKINEMATICS,
		//SGBP_WATERCONTACTENGINE,
	//積分の対象
	SGBP_DYNAMICALSYSTEM		= 300,
		SGBP_JOINTENGINE,
		SGBP_CONSTRAINTENGINE,
		SGBP_SOLIDCONTAINER,
		SGBP_HAPTICENGINE2,
		//SGBP_WATERENGINE,
	SGBP_MOTION					= 400,
		//SGBP_CHANGEOBJECT,
		//SGBP_CONSTRAINT,
	//他のエンジンのリスナ
	SGBP_LISTENER				= 500,
		//SGBP_CONTACTENGINE,
		//SGBP_STICKENGINE,
		//SGBP_WARPENGINE,
	//コールしない
	SGBP_NONE					= 600,

};

class PHScene;

///	動作エンジンの基本クラス．
class PHEngine : public SceneObject{
	SPR_OBJECTDEF(PHEngine);

	bool bEnabled;

public:
	///	実行順序を決めるプライオリティ値．小さいほど早い
	virtual int GetPriority() const { return SGBP_FORCEGENERATOR; }
	
	///	時間を dt 進める
	virtual void Step(){}

	void Enable(bool on){ bEnabled = on; }

	bool IsEnabled(){ return bEnabled; }
	
	///
	virtual PHSceneIf* GetScene(){ return SceneObject::GetScene()->Cast(); }

	PHEngine();
};

inline bool operator < (const PHEngine& e1, const PHEngine& e2){
	return e1.GetPriority() < e2.GetPriority();
}

///	PHEngineの配列
class PHEngines: private std::vector< UTRef<PHEngine> >{
public:
	iterator initializer;
	iterator forceGenerator;
	iterator dynamicalSystem;
	iterator motion;
	iterator listener;
protected:
	friend class PHScene;
	PHScene* scene;

	iterator begin2() { return base::begin(); }
	iterator end2() { return base::end(); }
public:
	typedef std::vector< UTRef<PHEngine> > base;
	typedef base::iterator iterator;
	typedef base::const_iterator const_iterator;

	PHEngines();
	virtual ~PHEngines(){}
	
	const_iterator begin() const { return base::begin(); }
	const_iterator end() const { return base::end(); }
	size_t size(){ return base::size(); }
	size_t size() const { return base::size(); }
	UTRef<PHEngine>& operator [] (int id){ return base::begin()[id]; }
	const UTRef<PHEngine>& operator [] (int id) const { return base::begin()[id]; }
	///@name シミュレーションを進めるメソッド
	//@{	
	///	力のクリア．
	virtual void ClearForce();
	///	力を生成(計算)する
	virtual void GenerateForce();
	///	ODE(積分処理)．力→速度，速度→位置 の計算を行う．
	virtual void Integrate();
	//@}
	void Add(PHEngine* e);
	bool Del(PHEngine* e);
	void Sort();
	///	シーンをクリアするときの処理
	virtual void Clear();
	///	指定した型を継承したエンジンを見つける (最初に見つかったものを返す)
	PHEngine* FindInherit(const UTTypeInfo& tinfo, UTString name) const;
	PHEngine* FindInherit(const UTTypeInfo& tinfo) const;
	///	指定した型かその派生クラスのエンジンを見つける (最初に見つかったものを返す)
	template <class T> bool Find(T*& t) const{
		t = (T*)FindInherit(*T::GetTypeInfoStatic());
		return t != NULL;
	}
	template <class T> bool Find(UTRef<T>& t) const{
		t = UTRef<T>((T*)FindInherit(*T::GetTypeInfoStatic()));
		return t != NULL;
	}
	///	指定した型かその派生クラスのエンジンで名前が一致するものを見つける (最初に見つかったものを返す)
	template <class T> bool Find(T*& t, UTString name) const{
		t = (T*)FindInherit(*T::GetTypeInfoStatic(), name);
		return t != NULL;
	}
	template <class T> bool Find(UTRef<T>& t, UTString name) const{
		t = UTRef<T>((T*)FindInherit(*T::GetTypeInfoStatic(), name));
		return t != NULL;
	}
};


}
#endif

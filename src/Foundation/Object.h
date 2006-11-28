/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FOUNDATION_OBJECT_H
#define FOUNDATION_OBJECT_H

#include <SprFoundation.h>
#include <Base/Base.h>

/*	hase memo
	Object が ObjectIfを継承しない方式の提案．大体良い感じだと思います．
	概要：
	- Ifクラスは今までどおり．ObjectはIfを継承しない．
	  IfImpがObjectの関数を呼び出す（これを書くのが面倒なので要自動化）
		- ObjectがIfBuf(Ifのvtableを持っておくためのメモリ)を継承
		- IntfInit<Type>が，TypeIfStubにIfBufを初期化
			class Object:IfBuf{}
			class PHSolid:PHObject, IfInit<PHSolid, PHSolidIf>{}
		- TypeIfStubは，TypeIfを継承，(this-4)->Type::Func()を呼び出す．
	
	良い点：
		- Objectの継承関係がすっきりする．InheritObject<>が不要に．デバッグしやすい．
		- vtable_ptrが2つで済む．Ifのvtable_ptrが複数入らない．メモリ節約になる．
		- 呼び出し時に参照するVirtualはIfの分１回のみ．
		- Ifのポインタオフセットが固定なので，Static_Castで If<-->Object の変換ができる．
		- Debuggerでも，強制キャストすれば見られる:(Obj*)((char*)intf-4)
	悪い点：
		- Stubを書くのが面倒→要自動生成．従来と違って末端の派生クラスまでStubが必要．
		- IfBufの書き換えがトリッキー．
		- Ifは絶対に変数を持てない．
*/


namespace Spr{;

//	型情報 IfInfo と TypeInfoのポインタを保持しておいて、最後に開放するクラス
class TypeInfoManager:public UTRefCount{
protected:
	std::vector< UTRef< IfInfo > > ifs;
	std::vector< UTRef< UTTypeInfo > > objects;
	static TypeInfoManager* typeInfoManager;
public:
	TypeInfoManager();
	~TypeInfoManager();
	static TypeInfoManager* SPR_CDECL Get();
	void RegisterIf(IfInfo* i);
	void RegisterObject(UTTypeInfo* t);
};

#define IF_IMP_COMMON(cls)															\
	IfInfoImp<cls##If> cls##If::ifInfo = IfInfoImp<cls##If>(#cls, cls##_BASEIF);	\
	const IfInfo* SPR_CDECL cls##If::GetIfInfoStatic(){								\
		static IfInfoImp<cls##If>* i;												\
		if (!i){																	\
			i= DBG_NEW IfInfoImp<cls##If>(#cls, cls##_BASEIF);						\
			TypeInfoManager::Get()->RegisterIf(i);									\
		}																			\
		return i;																	\
	}																				\

///	ObjectIf インタフェースクラスの実行時型情報
#define IF_IMP_BASE(cls)															\
	IfInfo* cls##_BASEIF[] = {NULL};												\
	IF_IMP_COMMON(cls)

///	インタフェースクラスの実行時型情報
#define IF_IMP(cls, base1)															\
	IfInfo* cls##_BASEIF[] = {(IfInfo*)base1##If::GetIfInfoStatic(), NULL};			\
	IF_IMP_COMMON(cls)


///	Object派生クラスの実行時型情報
#define OBJECT_GETSELFDEF(cls)											\
	static cls* GetSelfFromIf(const ObjectIf* p){						\
		if (!p) return NULL;											\
		Object* o = p->GetObj<Object>();								\
		if (o->GetTypeInfo()->Inherit(cls::GetTypeInfoStatic()))		\
			return (cls*)o;												\
		return NULL;													\
	}																	\
	XCastPtr<cls>& Cast() const{										\
		return *(XCastPtr<cls>*)(void*)this; }							\

#define	OBJECT_DEF_ABST_NOIF(cls)		DEF_UTTYPEINFOABSTDEF(cls) OBJECT_GETSELFDEF(cls)
#define	OBJECT_DEF_NOIF(cls)			DEF_UTTYPEINFODEF(cls) OBJECT_GETSELFDEF(cls)

#define OBJECT_GETSELFDEF_IF(cls)										\
	static cls* GetSelfFromIf(const cls##If* p){						\
		if (!p) return NULL;											\
		return p->GetObj<cls>();										\
	}																	\

#define OBJECT_IF_UTILITY(cls)	\
	cls##If* GetIf(){ return (cls##If*)(ObjectIfBuf*) this; }	\
	const cls##If* GetIf() const { return (const cls##If*)(ObjectIfBuf*) this; }

#define	OBJECT_DEF_ABST(cls)	OBJECT_DEF_ABST_NOIF(cls) OBJECT_IF_UTILITY(cls) OBJECT_GETSELFDEF_IF(cls)
#define	OBJECT_DEF(cls)			OBJECT_DEF_NOIF(cls) OBJECT_IF_UTILITY(cls) OBJECT_GETSELFDEF_IF(cls)
#define	OBJECT_DEF_FOR_OBJ(cls)	OBJECT_DEF_NOIF(cls) OBJECT_IF_UTILITY(cls)

///	実行時型情報を持つObjectの派生クラスが持つべきメンバの実装．
#define OBJECT_IMP_BASEABST(cls)		DEF_UTTYPEINFOABST(cls)
#define OBJECT_IMP_BASE(cls)			DEF_UTTYPEINFO(cls)
#define OBJECT_IMP(cls, base)			DEF_UTTYPEINFO1(cls, base)
#define OBJECT_IMP_ABST(cls, base)		DEF_UTTYPEINFOABST1(cls, base)
#define IF_OBJECT_IMP(cls, base)		IF_IMP(cls, base) OBJECT_IMP(cls, base)
#define IF_OBJECT_IMP_ABST(cls, base)	IF_IMP(cls, base) OBJECT_IMP_ABST(cls, base)

///	ステートの作成と破棄の関数定義
#define ACCESS_STATE_NOINHERIT(cls)													\
	virtual size_t GetStateSize() const { return sizeof(cls##State); }				\
	virtual void ConstructState(void* m) const { new(m) cls##State;}				\
	virtual void DestructState(void* m) const { ((cls##State*)m)->~cls##State(); }	\

///	ステートの設定・取得を含めたアクセス用関数の定義
#define ACCESS_STATE(cls)															\
	ACCESS_STATE_NOINHERIT(cls)														\
	virtual const void* GetStateAddress() const { return (cls##State*)this; }		\
	virtual bool GetState(void* s) const { *(cls##State*)s=*this; return true; }	\
	virtual void SetState(const void* s){ *(cls##State*)this = *(cls##State*)s;}	\

///	デスクリプタの設定・取得などアクセス用関数の定義
#define ACCESS_DESC(cls)															\
	virtual const void* GetDescAddress() const { return (cls##Desc*)this; }			\
	virtual bool GetDesc(void* d) const { *(cls##Desc*)d=*this; return true; }		\
	virtual size_t GetDescSize() const { return sizeof(cls##Desc); }				\

///	ステートとデスクリプタをまとめて定義
#define ACCESS_DESC_STATE(cls) ACCESS_STATE(cls) ACCESS_DESC(cls)


///	ObjectIfとその派生クラスのオブジェクトのためのバッファ
class ObjectIfBuf{
	void* vtable;
};
///	ObjectIfBufを派生クラスのインタフェースに書き換える
template <class I, class O>
struct IfInitTemplate{
	IfInitTemplate(){
		const int toBuf = int((char*)(ObjectIfBuf*)(O*)0x1000 - (char*)0x1000);
		const int fromThis = int((char*)0x1000 - (char*)(IfInitTemplate<I,O>*)(O*)0x1000);
		ObjectIfBuf* buf = (ObjectIfBuf*)(((char*)this) + fromThis + toBuf);
		::new (buf) I;
	}
};

}	//	namespace Spr;


#include "IfStubDumpFoundation.h"

namespace Spr{;

/**	全Objectの基本型	*/
class Object:public ObjectIfBuf, ObjectIfInit, public UTTypeInfoObjectBase, public UTRefCount{
	void GetIfInfo() { assert(0); }	//	don't call me
public:
	OBJECT_DEF_FOR_OBJ(Object);		///<	クラス名の取得などの基本機能の実装

	virtual int AddRef(){return UTRefCount::AddRef();}
	virtual int DelRef(){return UTRefCount::DelRef();}
	virtual int RefCount(){return UTRefCount::RefCount();}

	///	デバッグ用の表示
	virtual void Print(std::ostream& os) const;
	virtual void PrintShort(std::ostream& os) const;
	///	オブジェクトの作成
	virtual ObjectIf* CreateObject(const IfInfo* info, const void* desc);
	///	子オブジェクトの数
	virtual size_t NChildObject() const { return 0; }
	///	子オブジェクトの取得
	virtual ObjectIf* GetChildObject(size_t pos) { return NULL; }
	virtual const ObjectIf* GetChildObject(size_t pos) const { 
		return ((Object*) this)->GetChildObject(pos);
	}
	///	子オブジェクトの追加
	virtual bool AddChildObject(ObjectIf* o){ return false; }
	///	子オブジェクトの削除
	virtual bool DelChildObject(ObjectIf* o){ return false; }
	///	すべての子オブジェクトの削除とプロパティのクリア
	virtual void Clear(){}

	///	デスクリプタの読み出し(コピー版)
	virtual bool GetDesc(void* desc) const { return false; }
	///	デスクリプタの読み出し(参照版)
	virtual const void* GetDescAddress() const { return NULL; }
	///	デスクリプタのサイズ
	virtual size_t GetDescSize() const { return 0; };
	///	状態の読み出し(コピー版)
	virtual bool GetState(void* state) const { return false; }
	///	状態の読み出し(参照版)
	virtual const void* GetStateAddress() const { return NULL; }
	///	状態の設定
	virtual void SetState(const void* state){}
	///	状態のサイズ
	virtual size_t GetStateSize() const { return 0; };
	///	メモリブロックを状態型に初期化
	virtual void ConstructState(void* m) const {}
	///	状態型をメモリブロックに戻す
	virtual void DestructState(void* m) const {}
protected:
	virtual void PrintHeader(std::ostream& os, bool bClose) const;
	virtual void PrintChildren(std::ostream& os) const;
	virtual void PrintFooter(std::ostream& os) const;
	///	sをoのStateとして初期化する．
	static void ConstructState(ObjectIf* o, char*& s);
	///	sをoのStateからメモリブロックに戻す．
	static void DestructState(ObjectIf* o, char*& s);
};
template <class T> T* DCastImp(const Object* o){
	if (!o) return NULL;
	return T::GetSelfFromIf(o->GetIf());
}

class NameManager;
/**	名前を持つObject型．
	SDKやSceneに所有される．	*/
class NamedObject: public Object, NamedObjectIfInit{
	OBJECT_DEF(NamedObject);		///<	クラス名の取得などの基本機能の実装
protected:
	friend class ObjectNames;
	UTString name;					///<	名前
	NameManager* nameManager;	///<	名前の検索や重複管理をするもの．SceneやSDKなど．
public:
	NamedObject():nameManager(NULL){}
	~NamedObject();
	///	名前の取得
	const char* GetName() const { return name.c_str(); }
	///	名前の設定
	void SetName(const char* n);
	///	NameManagerを設定
	void SetNameManager(NameManagerIf* s);
	///	NameManagerの取得
	virtual NameManagerIf* GetNameManager();
protected:
	virtual void PrintHeader(std::ostream& os, bool bClose) const;
};

class Scene;
/**	Sceneが所有するObject型．
	所属するSceneへのポインタを持つ	*/
class SceneObject:public NamedObject, SceneObjectIfInit{
	OBJECT_DEF(SceneObject);		///<	クラス名の取得などの基本機能の実装
public:
	virtual void SetScene(SceneIf* s);
	virtual SceneIf* GetScene();
};

///	Objectのポインタの配列
class Objects:public UTStack< UTRef<Object> >{
public:
	///	Tの派生クラスをスタックのTop側から探す．
	template <class T> bool Find(T*& t){
		for(iterator it = end(); it != begin();){
			--it;
			T* tmp = DCAST(T, *it);
			if (tmp){
				t = tmp;
				return true;
			}
		}
		return false;
	}
};

///	ファクトリーの実装
template <class T, class IF, class DESC>
class FactoryImpTemplate: public FactoryBase{
public:
	virtual ObjectIf* Create(const void* desc, ObjectIf* parent){
		T* t = DBG_NEW T(*(DESC*)desc);

		//	シーンの設定
		SceneObject* o = DCAST(SceneObject, t);
		SceneIf* s = DCAST(SceneIf, parent);
		if (o && !s){		//	親がシーンでは無い場合，親を持つsceneに登録
			SceneObject* po = DCAST(SceneObject, parent);
			if (po) s = po->GetScene();
		}
		if (o && s){
			o->SetScene(s);
		}else{	//	シーンに設定できない場合，名前管理オブジェクトの設定
			NamedObject* o = DCAST(NamedObject, t);
			NameManagerIf* m = DCAST(NameManagerIf, parent);
			if (o && !m){	//	親がNameMangerではない場合，親のNameManagerに登録
				NamedObject* po = DCAST(NamedObject, parent);
				if (po) m = po->GetNameManager();
			}
			if (o && m) o->SetNameManager(m);
		}
		return t->GetIf();
	}
	virtual const IfInfo* GetIfInfo() const {
		return IF::GetIfInfoStatic();
	}
};
#define FactoryImp(cls)	FactoryImpTemplate<cls, cls##If, cls##Desc>

///	シーングラフの状態を保存．再生する仕組み
class ObjectStates:public Object, ObjectStatesIfInit{
protected:
	char* state;	///<	状態(XXxxxxState)を並べたもの
	size_t size;	///<	状態の長さ
	///	状態セーブの再起部分
	void SaveState(ObjectIf* o, char*& s);
	///	状態ロードの再起部分
	void LoadState(ObjectIf* o, char*& s);

public:
	OBJECT_DEF(ObjectStates);
	ObjectStates():state(NULL), size(0){}
	~ObjectStates(){ delete state; }
	///	oとその子孫をセーブするために必要なメモリを確保する．
	void AllocateState(ObjectIf* o);
	///	状態をセーブする．
	void SaveState(ObjectIf* o);
	///	状態をロードする．
	void LoadState(ObjectIf* o);
	///	状態のメモリを解放する
	void ReleaseState(ObjectIf* o);
	///	状態のサイズを求める
	size_t CalcStateSize(ObjectIf* o);
};


}
#endif

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FOUNDATION_OBJECT_H
#define FOUNDATION_OBJECT_H

#include <Foundation/SprObject.h>
#include "../Base/Base.h"
#include <iostream>
#include <fstream>
/*	hase memo
	1. ObjectはObjectIfを継承しない．
	2. ObjectIfはバーチャル関数を持たない．
	3. ObjectIfのアドレス = Objectのアドレス
	概要：
	- IfクラスはObjectはIfを継承しない．
	  自動生成されたObjectIfの関数の実装がObjectの関数を呼び出す．
	
	良い点：
		- Objectの継承関係がすっきりする．
		- vtable_ptrが1つで済む．Ifのvtable_ptrは不要．
		- 呼び出し時に参照するVirtualはObjectの分１回のみ．
		- Ifのポインタオフセットが固定なので，Static_Castで If<-->Object の変換ができる．
		- Debuggerでも，強制キャストすれば見られる:(Obj*)(Object*)intf
	悪い点：
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
	IfInfo* FindIfInfo(const char* cn);
	UTTypeInfo* FindTypeInfo(const char* cn);
};

///	以下インタフェースの型情報の実装
//@{
#define SPR_DESCIMP0(cls)	
#define SPR_DESCIMP1(cls, b1)	
#define SPR_DESCIMP2(cls, b1, b2)	

#define SPR_IFIMP_COMMON(cls)										\
	const IfInfo* SPR_CDECL cls##If::GetIfInfo() const {			\
		cls* p = DCAST(cls, (Object*)(ObjectIf*)this);				\
		return p->GetIfInfo();	/* 派生の情報が得られるかも */		\
	}																\
	template<> const UTTypeInfo* SPR_CDECL 							\
		IfInfoImp<cls##If>::GetTypeInfo(){							\
		return cls::GetTypeInfoStatic();							\
	}																\

#define SPR_IFIMP0(cls)	SPR_IFIMP_COMMON(cls)										\
	const IfInfo* SPR_CDECL cls##If::GetIfInfoStatic(){								\
		static IfInfoImp<cls##If>* i;												\
		if (!i){																	\
			static const IfInfo* base[] = {NULL};									\
			i= DBG_NEW IfInfoImp<cls##If>(#cls, base, cls::GetTypeInfoStatic());	\
			TypeInfoManager::Get()->RegisterIf(i);									\
		}																			\
		return i;																	\
	}																				\

#define SPR_IFIMP1(cls, b1)	SPR_IFIMP_COMMON(cls)									\
	const IfInfo* SPR_CDECL cls##If::GetIfInfoStatic(){								\
		static IfInfoImp<cls##If>* i;												\
		if (!i){																	\
			static const IfInfo* base[] = {											\
				b1##If::GetIfInfoStatic(), NULL};									\
			i= DBG_NEW IfInfoImp<cls##If>(#cls, base, cls::GetTypeInfoStatic());	\
			TypeInfoManager::Get()->RegisterIf(i);									\
		}																			\
		return i;																	\
	}																				\

#define SPR_IFIMP2(cls, b1, b2)	SPR_IFIMP_COMMON(cls)								\
	const IfInfo* SPR_CDECL cls##If::GetIfInfoStatic(){								\
		static IfInfoImp<cls##If>* i;												\
		if (!i){																	\
			static const IfInfo* base[] = {											\
				b1##If::GetIfInfoStatic(), b3##If::GetIfInfoStatic(), NULL};		\
			i= DBG_NEW IfInfoImp<cls##If>(#cls, base, cls::GetTypeInfoStatic());	\
			TypeInfoManager::Get()->RegisterIf(i);									\
		}																			\
		return i;																	\
	}																				\

//@}

///	Object派生クラスの実行時型情報
//@{
//	すべてのクラスに共通
#define OBJECTDEF_COMMON(cls)														\
	/*	If Object どちらにもなる 自動型変換型 XCastPtrを返す．*/					\
	XCastPtr<cls>& Cast() const{													\
		return *(XCastPtr<cls>*)(void*)this;										\
	}																				\
	/*	異型のIfからObjectへの動的変換	*/											\
	static cls* GetMe(const ObjectIf* p) {											\
		return (cls*)(((Object*)p)->GetBaseAddress(GetTypeInfoStatic()));			\
	}																				\
	/*	異型のObjectからObjectへの動的変換	*/										\
	static cls* GetMe(const Object* p) {											\
		return (cls*)(p)->GetBaseAddress(GetTypeInfoStatic());						\
	}																				\
	/*	Objectをファイルにダンプ	*/												\
	virtual void DumpObject(std::ostream& os) const {										\
		os.write((char*)(void*)this, sizeof(cls)); }								\

#ifdef SWIGSPR

#define DEF_UTTYPEINFOABSTDEF(cls)
#define DEF_UTTYPEINFODEF(cls)

#else


#define	SPR_OBJECTDEF_NOIF(cls)	DEF_UTTYPEINFODEF(cls) OBJECTDEF_COMMON(cls)
#define	SPR_OBJECTDEF(cls)	SPR_OBJECTDEF_NOIF(cls)									\
	virtual const IfInfo* GetIfInfo() {												\
		return cls##If::GetIfInfoStatic();											\
	}																				\

#define	SPR_OBJECTDEF_ABST_NOIF(cls)	DEF_UTTYPEINFOABSTDEF(cls) OBJECTDEF_COMMON(cls)
#define	SPR_OBJECTDEF_ABST(cls)	SPR_OBJECTDEF_ABST_NOIF(cls)						\
	virtual const IfInfo* GetIfInfo() {												\
		return cls##If::GetIfInfoStatic();											\
	}																				\

#define SPR_OBJECTDEF_NOIF1(cls, base)		SPR_OBJECTDEF_NOIF(cls)
#define SPR_OBJECTDEF1(cls, base)			SPR_OBJECTDEF(cls)
#define SPR_OBJECTDEF_ABST_NOIF1(cls, base)	SPR_OBJECTDEF_ABST_NOIF(cls)
#define SPR_OBJECTDEF_ABST1(cls, base)		SPR_OBJECTDEF_ABST(cls)

#define SPR_OBJECTDEF_NOIF2(cls, b1, b2)		SPR_OBJECTDEF_NOIF(cls)
#define SPR_OBJECTDEF2(cls, b1, b2)				SPR_OBJECTDEF(cls)
#define SPR_OBJECTDEF_ABST_NOIF2(cls, b1, b2)	SPR_OBJECTDEF_ABST_NOIF(cls)
#define SPR_OBJECTDEF_ABST2(cls, b1, b2)		SPR_OBJECTDEF_ABST(cls)

#endif

//@}

///	実行時型情報を持つObjectの派生クラスが持つべきメンバの実装．
//@{
#define SPR_OBJECTIMP0(cls)					DEF_UTTYPEINFO(cls)
#define SPR_OBJECTIMP1(cls, base)			DEF_UTTYPEINFO1(cls, base)
#define SPR_OBJECTIMP2(cls, b1,b2)			DEF_UTTYPEINFO2(cls, b1, b2)
#define SPR_OBJECTIMP_ABST0(cls)			DEF_UTTYPEINFOABST(cls)
#define SPR_OBJECTIMP_ABST1(cls, base)		DEF_UTTYPEINFOABST1(cls, base)
//@}


///	ステートのアクセス用関数の定義
#define ACCESS_STATE_PRIVATE(cls)													\
	virtual size_t GetStateSize() const {											\
		return sizeof(cls##State) + sizeof(cls##StatePrivate); }					\
	virtual void ConstructState(void* m) const {									\
		new(m) cls##State; new ((char*)m + sizeof(cls##State)) cls##StatePrivate; }	\
	virtual void DestructState(void* m) const {										\
		((cls##State*)m)->~cls##State();											\
		((cls##StatePrivate*)((char*)m+sizeof(cls##State)))->~cls##StatePrivate(); }\
	virtual bool GetState(void* s) const { *(cls##State*) s = *this;				\
		*(cls##StatePrivate*)((char*)s+sizeof(cls##State)) = *this;	return true; }	\
	virtual void SetState(const void* s){ *(cls##State*)this = *(cls##State*)s;		\
		*(cls##StatePrivate*)this =													\
			*(cls##StatePrivate*) ((char*)s + sizeof(cls##State) ); }				\

#define ACCESS_STATE(cls)															\
	virtual size_t GetStateSize() const { return sizeof(cls##State); }				\
	virtual void ConstructState(void* m) const { new(m) cls##State;}				\
	virtual void DestructState(void* m) const { ((cls##State*)m)->~cls##State(); }	\
	virtual const void* GetStateAddress() const { return (cls##State*)this; }		\
	virtual bool GetState(void* s) const { *(cls##State*)s=*this; return true; }	\
	virtual void SetState(const void* s){ *(cls##State*)this = *(cls##State*)s;}	\

#define ACCESS_PRIVATE(cls)																			\
	virtual size_t GetStateSize() const { return sizeof(cls##StatePrivate); }						\
	virtual void ConstructState(void* m) const { new(m) cls##StatePrivate;}							\
	virtual void DestructState(void* m) const { ((cls##StatePrivate*)m)->~cls##StatePrivate(); }	\
	virtual const void* GetStateAddress() const { return (cls##StatePrivate*)this; }				\
	virtual bool GetState(void* s) const { *(cls##StatePrivate*)s=*this; return true; }				\
	virtual void SetState(const void* s){ *(cls##StatePrivate*)this = *(cls##StatePrivate*)s;}		\

///	デスクリプタの設定・取得などアクセス用関数の定義
#define ACCESS_DESC(cls)															\
	virtual const void* GetDescAddress() const {									\
		((cls*)this)->BeforeGetDesc(); return (cls##Desc*)this; }					\
	virtual bool GetDesc(void* d) const {											\
		((cls*)this)->BeforeGetDesc(); *(cls##Desc*)d=*this; return true; }			\
	virtual void SetDesc(const void* d) {											\
		*(cls##Desc*)this = *(const cls##Desc*)d; ((cls*)this)->AfterSetDesc();}	\
	virtual size_t GetDescSize() const { return sizeof(cls##Desc); }				\

///	ステートとデスクリプタをまとめて定義
#define ACCESS_DESC_STATE(cls) ACCESS_STATE(cls) ACCESS_DESC(cls)
#define ACCESS_DESC_STATE_PRIVATE(cls) ACCESS_STATE_PRIVATE(cls) ACCESS_DESC(cls)
#define ACCESS_DESC_PRIVATE(cls) ACCESS_PRIVATE(cls) ACCESS_DESC(cls)

}	//	namespace Spr;


namespace Spr{;
class UTLoadContext;

/**	全Objectの基本型	*/
class Object: public UTTypeInfoObjectBase, public UTRefCount{
public:
	SPR_OBJECTDEF(Object);		///<	クラス名の取得などの基本機能の実装
	ObjectIf* GetObjectIf(){
		return (ObjectIf*) this; 
	}
	const ObjectIf* GetObjectIf() const {
		return (ObjectIf*) this; 
	}

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
	/**	ファイルロード後に一度呼ばれる。ノードの設定に使う。
		呼び出し順序に依存する処理がしたい場合は、UTLoadHandeler と UTLoadTask を使うと良い。　*/
	virtual void Loaded(UTLoadContext*){ Setup(); }
	///	ノードの設定をするための関数
	virtual void Setup(){}

	///	デスクリプタの読み出し(コピー版)
	virtual bool GetDesc(void* desc) const { return false; }
	/// デスクリプタの設定
	virtual void SetDesc(const void* desc) {}
	///	デスクリプタの読み出し(参照版)
	virtual const void* GetDescAddress() const { return NULL; }
	///	デスクリプタのサイズ
	virtual size_t GetDescSize() const { return 0; };
	///	状態の読み出し(コピー版)
	virtual bool GetState(void* state) const { return false; }
	///	状態の読み出し(参照版)
	virtual const void* GetStateAddress() const { return NULL; }
	///	状態の再帰的な読み出し
	virtual void GetStateR(char*& state);
	///	状態の設定
	virtual void SetState(const void* state){}
	///	状態の再帰的な設定
	virtual void SetStateR(const char*& state);
	/// 状態のファイルへの書き出し
	virtual bool WriteStateR(std::ostream& fout);
	virtual bool WriteState(std::string fileName);
	/// 状態の読み込み
	virtual bool ReadStateR(std::istream& fin);
	virtual bool ReadState(std::string fileName);
	///	状態のサイズ
	virtual size_t GetStateSize() const { return 0; };
	///	メモリブロックを状態型に初期化
	virtual void ConstructState(void* m) const {}
	///	状態型をメモリブロックに戻す
	virtual void DestructState(void* m) const {}
	///	メモリデバッグ用。単純にツリーのオブジェクトをファイルにダンプする。
	virtual void DumpObjectR(std::ostream& os, int level=0) const;

protected:
	/// 開き括弧
	virtual void PrintHeader(std::ostream& os, bool bClose) const;
	/// 子オブジェクトをPrint
	virtual void PrintChildren(std::ostream& os) const;
	/// 閉じ括弧
	virtual void PrintFooter(std::ostream& os) const;

	///	GetDesc()の前に呼ばれる
	virtual void BeforeGetDesc() const {}
	///	SetDesc()のあとに呼ばれる
	virtual void AfterSetDesc(){}

	///	sをoのStateとして初期化する．
	static void ConstructState(ObjectIf* o, char*& s);
	///	sをoのStateからメモリブロックに戻す．
	static void DestructState(ObjectIf* o, char*& s);
};
///	coutなどのストリームにObjectを出力する演算子．ObjectのPrintを呼び出す．
inline std::ostream& operator << (std::ostream& os, const Object& o){
	o.Print(os);
	return os;
}

class NameManager;
/**	名前を持つObject型．
	SDKやSceneに所有される．	*/
class NamedObject: public Object{
	SPR_OBJECTDEF(NamedObject);			///<	クラス名の取得などの基本機能の実装.
protected:
	friend class ObjectNames;
	UTString name;					///<	名前
	NameManager* nameManager;		///<	名前の検索や重複管理をするもの．SceneやSDKなど．
public:
	NamedObject():nameManager(NULL){}
	NamedObject(const NamedObject& n);
	NamedObject& operator=(const NamedObject& n);
	virtual ~NamedObject();
	///	名前の取得
	virtual const char* GetName() const { return name.c_str(); }
	///	名前の設定
	virtual void SetName(const char* n);
	///	NameManagerを設定
	virtual void SetNameManager(NameManagerIf* s);
	///	NameManagerの取得
	virtual NameManagerIf* GetNameManager() const ;
protected:
	virtual void PrintHeader(std::ostream& os, bool bClose) const;
};

class Scene;
/**	Sceneが所有するObject型．
	所属するSceneへのポインタを持つ	*/
class SceneObject:public NamedObject{
	SPR_OBJECTDEF(SceneObject);		///<	クラス名の取得などの基本機能の実装
public:
	virtual void SetScene(SceneIf* s);
	virtual SceneIf* GetScene() const;

	virtual SceneObjectIf* CloneObject();
};

///	Objectへの参照の配列
class ObjectRefs:public UTStack< UTRef<Object> >{
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
		T* t = (desc ? DBG_NEW T(*(DESC*)desc) : DBG_NEW T());

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
		return t->Cast();
	}
	virtual const IfInfo* GetIfInfo() const {
		return IF::GetIfInfoStatic();
	}
};
///	ファクトリーの実装(作り手オブジェクトが所有する場合、最後に作り手にAddChildObjectする)
template <class T, class IF, class DESC>
class FactoryImpTemplateOwned: public FactoryImpTemplate<T, IF, DESC>{
public:
	virtual ObjectIf* Create(const void* desc, ObjectIf* parent){
		ObjectIf* rv = FactoryImpTemplate<T, IF, DESC>::Create(desc, parent);
		if (rv) parent->AddChildObject(rv);
		return rv;
	}
};
///	ファクトリーの実装(デスクリプタがないオブジェクト用)
template <class T, class IF>
class FactoryImpTemplateNoDesc: public FactoryBase{
public:
	virtual ObjectIf* Create(const void* desc, ObjectIf* parent){
		T* t = DBG_NEW T;

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
		return t->Cast();
	}
	virtual const IfInfo* GetIfInfo() const {
		return IF::GetIfInfoStatic();
	}
};

#define FactoryImp(cls)	FactoryImpTemplate<cls, cls##If, cls##Desc>
#define FactoryImpOwned(cls)	FactoryImpTemplateOwned<cls, cls##If, cls##Desc>
#define FactoryImpNoDesc(cls)	FactoryImpTemplateNoDesc<cls, cls##If>

///	シーングラフの状態を保存．再生する仕組み
class ObjectStates:public Object{
protected:
	char* state;	///<	状態(XXxxxxState)を並べたもの
	size_t size;	///<	状態の長さ
public:
	SPR_OBJECTDEF(ObjectStates);
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
	/// SprBlender用再帰的ではないセーブ
	void SingleSave(ObjectIf* o);
	/// SprBlender用再帰的ではないロード
	void SingleLoad(ObjectIf* o);
	///	アロケート済みかどうか
	bool IsAllocated() { return state != NULL; }
};


}
#endif

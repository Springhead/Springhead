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

namespace Spr{;

#define IF_IMP_COMMON(cls)															\
	IfInfoImp<cls##If> cls##If::ifInfo = IfInfoImp<cls##If>(#cls, cls##_BASEIF);	\
	template <> \
	void* IfInfoImp<cls##If>::GetSprObject(ObjectIf* i)const{						\
		return (Object*)(cls*)(cls##If*)i;											\
	}																				\
	template <> \
	ObjectIf* IfInfoImp<cls##If>::GetIf(void* obj)const{							\
		return (ObjectIf*)(cls##If*)DCAST(cls, (Object*)obj);						\
	}																				\

///	ObjectIf インタフェースクラスの実行時型情報
#define IF_IMP_BASE(cls)															\
	IfInfo* cls##_BASEIF[] = {NULL};												\
	IF_IMP_COMMON(cls)

///	インタフェースクラスの実行時型情報
#define IF_IMP(cls, base1)															\
	IfInfo* cls##_BASEIF[] = {&base1##If::ifInfo, NULL};							\
	IF_IMP_COMMON(cls)


#define OBJECT_CAST(cls, p)											\
	(((Object*)p)->GetTypeInfo()->Inherit(cls::GetTypeInfoStatic())	\
		? (cls*)(Object*)p :  NULL)									\

///	Object派生クラスの実行時型情報
#define OBJECT_DEF_ABST_NOIF(cls)		DEF_UTTYPEINFOABSTDEF(cls)	\
	static cls* GetSelfFromObject(void* o) {						\
		return OBJECT_CAST(cls, o);									\
	}																\

#define OBJECT_DEF_NOIF(cls)			DEF_UTTYPEINFODEF(cls)		\
	static cls* GetSelfFromObject(void* o) {						\
		return OBJECT_CAST(cls, o);									\
	}																\

#define	OBJECT_DEF_ABST(cls)			OBJECT_DEF_ABST_NOIF(cls)	\
	virtual ObjectIf* GetIf() const { return (cls##If*)this; }		\

#define	OBJECT_DEF(cls)					OBJECT_DEF_NOIF(cls)		\
	virtual ObjectIf* GetIf() const { return (cls##If*)this; }		\


///	実行時型情報を持つObjectの派生クラスが持つべきメンバの実装．
#define OBJECT_IMP_BASEABST(cls)			DEF_UTTYPEINFOABST(cls)
#define OBJECT_IMP_BASE(cls)				DEF_UTTYPEINFO(cls)
#define OBJECT_IMP(cls, base)				DEF_UTTYPEINFO1(cls, base)
#define OBJECT_IMP2(cls, b1, b2)			DEF_UTTYPEINFO2(cls, b1, b2)
#define OBJECT_IMP3(cls, b1, b2, b3)		DEF_UTTYPEINFO3(cls, b1, b2, b3)
#define OBJECT_IMP_ABST(cls, base)			DEF_UTTYPEINFOABST1(cls, base)
#define OBJECT_IMP_ABST2(cls, b1, b2)		DEF_UTTYPEINFOABST2(cls, b1,b2)
#define OBJECT_IMP_ABST3(cls, b1, b2, b3)	DEF_UTTYPEINFOABST3(cls, b1,b2,b3)

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

/**	全Objectの基本型	*/
class Object:private ObjectIf, public UTTypeInfoObjectBase, public UTRefCount{
	void GetIfInfo() { assert(0); }	///	don't call me
public:
	OBJECT_DEF(Object);		///<	クラス名の取得などの基本機能の実装


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
	///	すべての子オブジェクトの削除
	virtual void ClearChildObjects(){}

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
	virtual void PrintHeader(std::ostream& os) const;
	virtual void PrintChildren(std::ostream& os) const;
	virtual void PrintFooter(std::ostream& os) const;
};

template <class T> T* SprDcastImp(const Object* p){
	if (p && p->GetTypeInfo()->Inherit(T::GetTypeInfoStatic())) return (T*)&*(p);
	return NULL;
}


template <class intf, class base>
struct InheritObject:public intf, public base{
	typedef base baseType;
	virtual int AddRef(){return base::AddRef();}
	virtual int DelRef(){return base::DelRef();}
	virtual int RefCount(){return base::RefCount();}				
	virtual ObjectIf* CreateObject(const IfInfo* i, const void* d){
		return base::CreateObject(i,d);
	}
	virtual void Print(std::ostream& os) const{ base::Print(os); }
	virtual void PrintShort(std::ostream& os) const{ base::PrintShort(os); }
	virtual size_t NChildObject() const { return base::NChildObject(); }
	virtual ObjectIf* GetChildObject(size_t pos){ return base::GetChildObject(pos); }
	virtual const ObjectIf* GetChildObject(size_t pos) const{
		return base::GetChildObject(pos);
	}
	virtual bool AddChildObject(ObjectIf* o){
		return base::AddChildObject(o);		
	}
	virtual bool DelChildObject(ObjectIf* o){
		return base::DelChildObject(o);		
	}
	virtual void ClearChildObjects(){
		base::ClearChildObjects();		
	}
	virtual bool GetDesc(void* desc) const { return base::GetDesc(desc); }
	virtual const void* GetDescAddress() const { return base::GetDescAddress(); }
	virtual size_t GetDescSize() const { return base::GetDescSize(); }
	virtual bool GetState(void* state) const { return base::GetState(state); }
	virtual const void* GetStateAddress() const { return base::GetStateAddress(); }
	virtual size_t GetStateSize() const { return base::GetStateSize(); }
	virtual void SetState(const void* s){ return base::SetState(s); }
	virtual void ConstructState(void* m) const { base::ConstructState(m); }
	virtual void DestructState(void* m) const { base::DestructState(m); }
};

class NameManager;
/**	名前を持つObject型．
	SDKやSceneに所有される．	*/
class NamedObject: public InheritObject<NamedObjectIf, Object>{
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
	///	デバッグ用の表示
	virtual void Print(std::ostream& os) const;
	virtual void PrintShort(std::ostream& os) const;
	virtual void SetNameManager(NameManager* s);
	virtual NameManager* GetNameManager(){ return nameManager; }
	///	デバッグ用の表示
};

template <class intf, class base>
struct InheritNamedObject:public InheritObject<intf, base>{
	const char* GetName() const { return base::GetName(); }
	void SetName(const char* n) { base::SetName(n); }
};

class Scene;
/**	Sceneが所有するObject型．
	所属するSceneへのポインタを持つ	*/
class SceneObject:public InheritNamedObject<SceneObjectIf, NamedObject>{
	OBJECT_DEF(SceneObject);		///<	クラス名の取得などの基本機能の実装
public:
	virtual void SetScene(SceneIf* s);
	virtual SceneIf* GetScene();
};
template <class intf, class base>
struct InheritSceneObject:public InheritNamedObject<intf, base>{
	SceneIf* GetScene() { return base::GetScene(); }
	void SetScene(SceneIf* s) { base::SetScene(s); }
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
			NameManager* m = DCAST(NameManager, parent);
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
class ObjectStates:public InheritObject<ObjectStatesIf, Object>{
protected:
	char* state;	///<	状態(XXxxxxState)を並べたもの
	size_t size;	///<	状態の長さ
	///	sをoのStateとして初期化する．
	void ConstructState(ObjectIf* o, char*& s);
	///	sをoのStateからメモリブロックに戻す．
	void DestructState(ObjectIf* o, char*& s);
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

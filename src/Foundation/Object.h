#ifndef FOUNDATION_OBJECT_H
#define FOUNDATION_OBJECT_H

#include <SprFoundation.h>
#include <Base/Base.h>

namespace Spr{;


#define IF_IMP_COMMON(cls)															\
	IfInfoImp<cls##If> cls##If::ifInfo = IfInfoImp<cls##If>(#cls, cls##_BASEIF);	\
	template <> \
	void* IfInfoImp<cls##If>::GetSprObject(ObjectIf* i)const{							\
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

///	Object派生クラスの実行時型情報

#define OBJECTDEF(cls)					DEF_UTTYPEINFODEF(cls)
#define OBJECTDEFABST(cls)				DEF_UTTYPEINFOABSTDEF(cls)
#define OBJECTIMPBASE(cls)				DEF_UTTYPEINFO(cls)
#define OBJECTIMPBASEABST(cls)			DEF_UTTYPEINFOABST(cls)
#define OBJECTIMP(cls, base)			DEF_UTTYPEINFO1(cls, base)
#define OBJECTIMP2(cls, b1, b2)			DEF_UTTYPEINFO2(cls, b1, b2)
#define OBJECTIMP3(cls, b1, b2, b3)		DEF_UTTYPEINFO3(cls, b1, b2, b3)
#define OBJECTIMPABST(cls, base)		DEF_UTTYPEINFOABST1(cls, base)
#define OBJECTIMPABST2(cls, b1, b2)		DEF_UTTYPEINFOABST2(cls, b1,b2)
#define OBJECTIMPABST3(cls, b1, b2, b3)	DEF_UTTYPEINFOABST3(cls, b1,b2,b3)

class Object;	
///	インタフェース->オブジェクトへのキャスト
#define OCAST(T, i)	OcastImp<T>(i)
template <class T, class I> T* OcastImp(const I* i){
	ObjectIf* oi = (ObjectIf*)i;
	void* obj = i ? i->GetIfInfo()->GetSprObject(oi) : NULL;
	return (T*)(Object*)obj;
}
	
/**	全Objectの基本型	*/
class Object:public ObjectIf, public UTTypeInfoBase, public UTRefCount{
public:
	OBJECTDEF(Object);		///<	クラス名の取得などの基本機能の実装

	virtual int AddRef(){return UTRefCount::AddRef();}
	virtual int DelRef(){return UTRefCount::DelRef();}
	virtual int RefCount(){return UTRefCount::RefCount();}

	///	デバッグ用の表示
	virtual void Print(std::ostream& os) const;
	///	オブジェクトの作成
	virtual ObjectIf* CreateObject(const IfInfo* info, const void* desc){ return NULL; }
	///	子オブジェクトの数
	virtual size_t NChildObject() const { return 0; }
	///	子オブジェクトの取得
	virtual ObjectIf* GetChildObject(size_t pos) { return NULL; }
	virtual const ObjectIf* GetChildObject(size_t pos) const { 
		return ((Object*) this)->GetChildObject(pos);
	}
	///	子オブジェクトの追加
	virtual bool AddChildObject(ObjectIf* o){ return false; }
	///	データの読み出し
	virtual bool GetDesc(void* desc){ return false; }
	///	データの読み出し
	virtual void* GetDescAddress(){ return NULL; }
};
template <class intf, class base>
struct InheritObject:public intf, base{
	virtual int AddRef(){return base::AddRef();}
	virtual int DelRef(){return base::DelRef();}
	virtual int RefCount(){return base::RefCount();}				
	virtual ObjectIf* CreateObject(const IfInfo* i, const void* d){
		return base::CreateObject(i,d);
	}
	virtual void Print(std::ostream& os) const{ base::Print(os); }
	virtual size_t NChildObject() const { return base::NChildObject(); }
	virtual ObjectIf* GetChildObject(size_t pos){ return base::GetChildObject(pos); }
	virtual const ObjectIf* GetChildObject(size_t pos) const{
		return base::GetChildObject(pos);
	}
	virtual bool AddChildObject(ObjectIf* o){
		return base::AddChildObject(o);		
	}
	virtual bool GetDesc(void* desc){ return base::GetDesc(desc); }
	virtual void* GetDescAddress(){ return base::GetDescAddress(); }
};

class NameManager;
/**	名前を持つObject型．
	SDKやSceneに所有される．	*/
class NamedObject: public InheritObject<NamedObjectIf, Object>{
	OBJECTDEF(NamedObject);		///<	クラス名の取得などの基本機能の実装
protected:
	friend class ObjectNames;
	UTString name;				///<	名前
	NameManager* nameManager;	///<	名前の検索や重複管理をするもの．SceneやSDKなど．
public:
	NamedObject():nameManager(NULL){}
	///	名前の取得
	const char* GetName() const { return name.c_str(); }
	///	名前の設定
	void SetName(const char* n);
	///	デバッグ用の表示
	virtual void Print(std::ostream& os) const;
	virtual void SetNameManager(NameManager* s){ nameManager = s; }
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
	OBJECTDEF(SceneObject);		///<	クラス名の取得などの基本機能の実装
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


}
#endif

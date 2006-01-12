#ifndef FOUNDATION_OBJECT_H
#define FOUNDATION_OBJECT_H

#include <SprFoundation.h>
#include <Base/Base.h>

namespace Spr{;


#define IF_IMP_COMMON(cls)															\
	IfInfoImp<cls##If> cls##If::ifInfo = IfInfoImp<cls##If>(#cls, cls##_BASEIF);	\
	void* IfInfoImp<cls##If>::GetObject(ObjectIf* i)const{							\
		return (Object*)(cls*)(cls##If*)i;											\
	}																				\
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

///	インタフェース->オブジェクトへのキャスト
#define OCAST(T, i)	OcastImp<T>(i)
template <class T, class I> T* OcastImp(I* i){
	ObjectIf* oi = i;
	void* obj = i->GetIfInfo()->GetObject(oi);
	return (T*)(Object*)obj;
}

/**	全Objectの基本型	*/
class Object:public ObjectIf, public UTTypeInfoBase, public UTRefCount{
public:
	OBJECTDEF(Object);		///<	クラス名の取得などの基本機能の実装

	int AddRef(){return UTRefCount::AddRef();}
	int DelRef(){return UTRefCount::DelRef();}
	int RefCount(){return UTRefCount::RefCount();}

	///	デバッグ用の表示
	virtual void Print(std::ostream& os) const;
};
#define BASEIMP_OBJECT(base)											\
	int AddRef(){return base::AddRef();}								\
	int DelRef(){return base::DelRef();}								\
	int RefCount(){return base::RefCount();}							\


class NameManager;
/**	名前を持つObject型．
	SDKやSceneに所有される．	*/
class NamedObject:public NamedObjectIf, public Object{
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
};
#define BASEIMP_NAMEDOBJECT(base)										\
	BASEIMP_OBJECT(base)												\
	const char* GetName() const { return base::GetName(); }				\
	void SetName(const char* n) { base::SetName(n); }					\

class Scene;
/**	Sceneが所有するObject型．
	所属するSceneへのポインタを持つ	*/
class SceneObject:public SceneObjectIf, public NamedObject{
	OBJECTDEF(SceneObject);		///<	クラス名の取得などの基本機能の実装
public:
	virtual void SetScene(SceneIf* s);
	virtual SceneIf* GetScene();
};
#define BASEIMP_SCENEOBJECT(base)										\
	BASEIMP_NAMEDOBJECT(base)											\
	SceneIf* GetScene() { return base::GetScene(); }					\
	void SetScene(SceneIf* s) { base::SetScene(s); }					\

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

#ifndef FOUNDATION_OBJECT_H
#define FOUNDATION_OBJECT_H

#include <Springhead.h>
#include <Base/Base.h>


namespace Spr{;


///	Object を継承したクラスはメンバとしてこのマクロを持つ．
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

class Scene;
/**	シーングラフのオブジェクト型．
	クラス名を返す関数を持つ．	*/
class Object:public ObjectIf, public UTTypeInfoBase, public UTRefCount{
	friend class ObjectNames;
	UTString name;				///<	名前
	UTString nameSpace;			///<	名前
protected:
	Scene* scene;
public:
	Object():scene(NULL){}
	///	クラス名の取得などの基本機能の実装
	OBJECTDEF(Object);

	virtual void SetScene(Scene*s){ scene = s; }
	///	名前の取得
	const char* GetName() const { return name.c_str(); }
	const char* GetNameSpace() const { return nameSpace.c_str(); }
	///	名前の設定
	void SetName(const char* n);
	int AddRef(){return UTRefCount::AddRef();}
	int DelRef(){return UTRefCount::DelRef();}
	int RefCount(){return UTRefCount::RefCount();}

#define BASEIMP_OBJECT(base)											\
	const char* GetName() const { return base::GetName(); }				\
	const char* GetNameSpace() const { return base::GetNameSpace(); }	\
	void SetName(const char* n) { base::SetName(n); }					\
	int AddRef(){return base::AddRef();}								\
	int DelRef(){return base::AddRef();}								\
	int RefCount(){return base::AddRef();}								\


	///	デバッグ用の表示
	virtual void Print(std::ostream& os) const;

/*
	///	所有しているオブジェクトの数
	virtual size_t NChildObjects(){ return 0; }
	///	所有しているオブジェクト
	virtual Object* ChildObject(size_t i){ return NULL; }
	///	参照しているオブジェクトの数
	virtual size_t NReferenceObjects(){ return 0; }
	///	参照しているオブジェクト
	virtual Object* ReferenceObject(size_t i){ return NULL; };
	///	子オブジェクトの追加(所有・参照を問わない)
	virtual bool AddChildObject(Object* o){ return false; }
	///	子オブジェクトの削除
	virtual bool DelChildObject(Object* o){ return false; }
	///	子になりえるオブジェクトの型情報の配列
	virtual const UTTypeInfo** ChildCandidates(){ return NULL; }
*/
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

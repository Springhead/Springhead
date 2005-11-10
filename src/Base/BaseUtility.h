#ifndef Spr_BASE_UTILITIES_H
#define Spr_BASE_UTILITIES_H
#include "BaseDebug.h"
#include <algorithm>
#include <iosfwd>
#include <vector>
#include <typeinfo>

/**	@file	BaseUtility.h	その他のユーティリティークラス・関数．*/

namespace Spr {

typedef std::string UTString;
inline bool operator < (const UTString& u1, const UTString& u2){
	return u1.compare(u2) < 0;
}
struct UTStringLess{
	bool operator ()(const UTString& t1, const UTString& t2) const{
		return t1.compare(t2) < 0;
	}
};

template <class T>
bool UTContentsEqual(const T& t1, const T& t2){
	return *t1 == *t2;
}

template <class T>
struct UTContentsLess{
	bool operator ()(const T& t1, const T& t2) const{
		return *t1 < *t2;
	}
};

template <class T, class M, M T::* m>
struct UTMemberLess{
	bool operator ()(const T& t1, const T& t2) const{
		return *t1.*m < *t2.*m;
	}
};

class SPR_DLL UTPadding{
public:
	int len;
	UTPadding(int i){len = i;}
};
std::ostream& operator << (std::ostream& os, UTPadding p);

struct UTEatWhite{
	UTEatWhite(){}
};
std::istream& operator >> (std::istream& is, const UTEatWhite& e);

///	参照カウンタ．カウントが0になっても勝手に消えはしない．
class SPR_DLL UTRefCount{
	mutable int refCount;
public:
#ifndef _DEBUG
//	UTRefCount(){refCount = 0;}
//	UTRefCount(const UTRefCount&){refCount = 0;}
//	~UTRefCount(){ assert(refCount==0); }
	UTRefCount();
	UTRefCount(const UTRefCount& r);
	~UTRefCount();
#else
	static int nObject;
	UTRefCount();
	UTRefCount(const UTRefCount& r);
	~UTRefCount();
#endif
	UTRefCount& operator = (const UTRefCount& r){ return *this; }

	int AddRef() const { return ++ refCount; }
	int DelRef() const {
		assert(refCount > 0);
		return -- refCount;
	}
	int RefCount() const { return refCount; }
};

/**	参照カウンタ用のポインタ．自動的に参照カウンタを増減，
	オブジェクトのdeleteをする．
*/
template <class T>
class UTRef{
	T* obj;
	T*& Obj() const {return (T*&) obj;}
public:
	UTRef(T* t = NULL){
		Obj() = t;
		if (Obj()) Obj()->AddRef();
	}
	template <class E>
	UTRef(const UTRef<E>& r){
		Obj() = (T*)(E*)r;
		if (Obj()) Obj()->AddRef();
	}
	UTRef(const UTRef<T>& r){
		Obj() = r.Obj();
		if (Obj()) Obj()->AddRef();
	}
	~UTRef(){ if (Obj() && Obj()->DelRef() == 0) delete Obj(); }
	UTRef& operator =(T* t){
		if (Obj() != t){
			if (Obj() && Obj()->DelRef() == 0) delete Obj();
			Obj() = t;
			if (Obj()) Obj()->AddRef();
		}
		return *this;
	}
	template <class E>
	UTRef& operator =(const UTRef<E>& r){
		if (Obj() != r){
			if (Obj() && Obj()->DelRef() == 0) delete Obj();
			Obj() = r;
			if (Obj()) Obj()->AddRef();
		}
		return *this;
	}
	UTRef& operator =(const UTRef<T>& r){
		if (Obj() != r.Obj()){
			if (Obj() && Obj()->DelRef() == 0) delete Obj();
			Obj() = r;
			if (Obj()) Obj()->AddRef();
		}
		return *this;
	}
	operator T*() const {return Obj();}
	T* operator->() const {return Obj();}
	bool operator <(const UTRef& r) const { return Obj() < r.Obj(); }
};

/// UTRefの配列
template<class T, class ARRAY = std::vector< UTRef<T> > >
class UTRefArray : public ARRAY{
public:
	UTRef<T> Erase(const UTRef<T>& ref){
		iterator it = std::find(begin(), end(), ref);
		if (it == end()) return NULL;
		UTRef<T> rv = *it;
		erase(it);
		return rv;
	}
	UTRef<T>* Find(const UTRef<T>& ref){
		iterator it = std::find(begin(), end(), s);
		if (it == end()) return NULL;
		else return &*it;
	}
	UTRef<T>* Find(const UTRef<T>& ref) const {
		return ((UTRefArray<T, ARRAY>*)this)->Find(ref);
	}
};


///	シングルトンクラス
template <class T>
T& Singleton(){
	static T t;
	return t;
}

///	スタックつき vector 
template <class T, class Cont=std::vector<T> >
class UTStack: public Cont{
public:
	T Pop(){
		assert(size());
		T t=back(); pop_back(); return t;
	}
	void Push(const T& t=T()){ push_back(t); }
	T& Top(){
		assert(size());
		return back();
	}
};

///	ツリーのノード(親子両方向参照)
template <class T, class ARRAY=std DOUBLECOLON vector< UTRef<T> > >
class UTTreeNode{
private:
	void clear();
protected:
	T* parent;
	ARRAY children;
public:
	typedef ARRAY container_t;
	///
	UTTreeNode():parent(NULL){}
	virtual ~UTTreeNode(){
/*		for(ARRAY::iterator it = children.begin(); it != children.end(); ++it){
			(*it)->parent = NULL;
		}
		children.clear();
*/
		while(children.size()){
			 children.back()->parent = NULL;
			 children.pop_back();
		}
	}
	///@name ツリーの操作
	//@{
	///	親ノードを取得する．
	T* GetParent(){ return parent; }
	const T* GetParent() const { return parent; }
	///	親ノードを設定する．
	void SetParent(T* n){
		if (parent == n) return;						//	変更がなければ何もしない．
		//	途中でRefCountが0になって消えないように，先に新しいノードの子にする．
		if (n) n->children.push_back((T*)this);
		if (parent) {									//	古い親ノードの子リストから削除
			TYPENAME ARRAY::iterator it = std::find(parent->children.begin(), parent->children.end(), UTRef<T>((T*)this));
			if (it != parent->children.end()) parent->children.erase(it);
		}
		parent = n;										//	parent を新しいノードに切り替える．
	}
	///	子ノード．
	ARRAY& Children(){ return children; }
	///	子ノード．
	const ARRAY& Children() const { return children; }
	///	子ノードを追加する
	void AddChild(UTRef<T> c){ c->SetParent((T*)this); }
	///	子ノードを削除する
	void DelChild(UTRef<T> c){ if(c->GetParent() == this) c->SetParent(NULL); }
	///	子ノードをすべて削除する．
	void ClearChildren(){
		while(children.size()){
			 children.back()->parent = NULL;
			 children.pop_back();
		}
	}
	///
	template <class M>
	void ForEachChild(M m){
		for(TYPENAME ARRAY::iterator it = children.begin(); it !=children.end(); ++it){
			T* t = *it;
			(t->*m)();
		}
	}
	///
	template <class M>
	void Traverse(M m){
		  m(this);
		for(TYPENAME ARRAY::iterator it = children.begin(); it !=children.end(); ++it){
			(*it)->Traverse(m);
		}
	}
	template <class M, class A>
	void Traverse(M m, A a){
		m((T*)this, a);
		for(TYPENAME ARRAY::iterator it = children.begin(); it !=children.end(); ++it){
			(*it)->Traverse(m, a);
		}
	}
	template <class T2, class M>
	void MemberTraverse(T2 t, M m){
		(t->*m)(this);
		for(TYPENAME ARRAY::iterator it = children.begin(); it !=children.end(); ++it){
			(*it)->MemberTraverse(t, m);
		}
	}
	template <class E, class M, class A>
	void MemberTraverse(E e, M m, A& a){
		(e->*m)((T*)this, a);
		for(TYPENAME ARRAY::iterator it = children.begin(); it !=children.end(); ++it){
			(*it)->MemberTraverse(e, m, a);
		}
	}
};

	
/* assert_cast
		 SPR_DEBUG定義時はdynamic_cast、それ以外ではstatic_castとして働く。
		 dynamic_castに失敗するとstd::bad_cast例外を発生する。
*/
template <class T, class U>
inline T assert_cast(U u){
#ifdef SPR_DEBUG
	T t= dynamic_cast<T>(u);
	if (u && !t) throw std::bad_cast();
	return t;
#else
	return static_cast<T>(u);
#endif
}


}	//	namespace Spr

#endif

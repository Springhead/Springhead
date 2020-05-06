/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef Spr_BASE_UTILITIES_H
#define Spr_BASE_UTILITIES_H
#include "BaseDebug.h"
#include <algorithm>
#include <iosfwd>
#include <vector>
#include <typeinfo>

namespace Spr {

/**	\defgroup gpBaseUtility ユーティリティクラス	*/
//@{

/**	@file	BaseUtility.h	その他のユーティリティークラス・関数．*/

///	文字列．str::stringの別名
typedef std::string UTString;
inline bool operator < (const UTString& u1, const UTString& u2){
	return u1.compare(u2) < 0;
}
/**	文字列(UTString)を比較する関数オブジェクト型．
	set<UTString, UTStringLess> などを作るために使える．	*/
struct UTStringLess{
	bool operator ()(const UTString& t1, const UTString& t2) const{
		return t1.compare(t2) < 0;
	}
};

///	ポインタの中身が等しいか調べる関数オブジェクト型．
template <class T>
bool UTContentsEqual(const T& t1, const T& t2){
	return *t1 == *t2;
}

/**	ポインタの中身を比較する関数オブジェクト型．
	set< int*, UTContentsLess<int> > などに使える．*/
template <class T>
struct UTContentsLess{
	bool operator ()(const T& t1, const T& t2) const{
		return *t1 < *t2;
	}
};

/**	クラスのメンバを比較する関数オブジェクト型．
	set<C, UTMemberLess<C, C::member> > などに使える．*/
template <class T, class M, M T::* m>
struct UTMemberLess{
	bool operator ()(const T& t1, const T& t2) const{
		return *t1.*m < *t2.*m;
	}
};

///	ostreamに指定の数のスペースを出力するストリームオペレータ(std::endl などの仲間)
class UTPadding{
public:
	int len;
	UTPadding(int i){len = i;}
};
std::ostream& operator << (std::ostream& os, UTPadding p);

///	istreamから，空白文字(スペース タブ 改行(CR LF) )を読み出すストリームオペレータ
struct UTEatWhite{
	UTEatWhite(){}
};
std::istream& operator >> (std::istream& is, const UTEatWhite& e);


/**	\defgroup gpUTRef 参照カウンタ/参照ポインタクラス 
参照カウンタは，複数のポインタで共有されたオブジェクトのメモリの開放を
参照するポインタが無くなった時に自動的に行う仕組み．
これを使うと delete を書く必要がなくなる．
ポインタ
@verbatim
	T* p = new T;
@endverbatim
の代わりに，
@verbatim
	UTRef<T> p = new T;
	UTRef<T> p2 = p;
@endverbatim
とすると，pとp2の両方が消えたときに， p/p2 が指すオブジェクトもdeleteされる．

*/
//@{

/**	参照カウンタ．UTRef<T>で指されるクラスTは，
	これを1つだけ継承する必要がある．	*/
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
	カウンタが0になったら，オブジェクトをdeleteする．
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
	~UTRef(){ if (Obj() && Obj()->DelRef() == 0) delete Obj(); obj=NULL;}
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

/// UTRefの配列．
template<class T, class CO = std::vector< UTRef<T> > >
class UTRefArray : public CO{
public:
	typedef typename CO::iterator iterator;
	UTRef<T> Erase(const UTRef<T>& ref){
		iterator it = std::find(CO::begin(), CO::end(), ref);
		if (it == CO::end()) return NULL;
		UTRef<T> rv = *it;
		erase(it);
		return rv;
	}
	UTRef<T>* Find(const UTRef<T>& ref){
		iterator it = std::find(CO::begin(), CO::end(), ref);
		if (it == CO::end()) return NULL;
		else return &*it;
	}
	UTRef<T>* Find(const UTRef<T>& ref) const {
		return ((UTRefArray<T, CO>*)this)->Find(ref);
	}
};

/**	一般オブジェクト用，自動delete ポインタ
	ポインタが消えるときにオブジェクトをdeleteをする．
	カウンタはついてないので，複数の UTDeleteRef でオブジェクトを
	指すことはできない．*/
template <class T>
class UTDeleteRef{
	T* obj;
	T*& Obj() const {return (T*&) obj;}
	UTDeleteRef(const UTDeleteRef<T>& r){
		assert(0);	//	コピーしちゃだめ．
	}
public:
	UTDeleteRef(T* t = NULL){
		Obj() = t;
	}
	~UTDeleteRef(){ delete Obj(); }
	UTDeleteRef& operator =(T* t){
		Obj() = t;
		return *this;
	}
	operator T*() const {return Obj();}
	T* operator->() const {return Obj();}
	bool operator <(const UTDeleteRef& r) const { return Obj() < r.Obj(); }
};
//@}


/**	シングルトンクラス．
	プログラム内に，オブジェクトを1つだけ作りたく，
	いつでも使いたい場合に利用するもの．*/
template <class T>
T& Singleton(){
	static T t;
	return t;
}

/**	\defgroup gpExCont コンテナの拡張
	stlのコンテナクラスを拡張したクラス類．
	スタック，ツリー，	*/
//@{
///	スタックつき vector 
template <class T, class CO=std::vector<T> >
class UTStack: public CO{
public:
	typedef CO container;
	T Pop(){
		assert(CO::size());
		T t=CO::back(); CO::pop_back(); return t;
	}
	void Push(const T& t=T()){ CO::push_back(t); }
	T& Top(){
		assert(CO::size());
		return CO::back();
	}
};

///	ツリーのノード(親子両方向参照)
template <class T, class CO=std::vector< UTRef<T> > >
class UTTreeNode{
private:
	void clear();
protected:
	T* parent;
	CO children;
public:
	typedef CO container_t;
	///
	UTTreeNode():parent(NULL){}
	virtual ~UTTreeNode(){
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
		//	変更がなければ何もしない．
		if (parent == n)
			return;
		//	途中でRefCountが0になって消えないように，先に新しいノードの子にする．
		if (n) n->children.push_back((T*)this);
		//	parent を新しいノードに切り替える．
		T* parOld = parent;
		parent = n;

		//	古い親ノードの子リストから削除（親以外から参照されていない場合はここでdeleteされる）
		if (parOld) {
			TYPENAME CO::iterator it = std::find(parOld->children.begin(), parOld->children.end(), UTRef<T>((T*)this));
			if (it != parOld->children.end())
				parOld->children.erase(it);
		}
	}
	///	子ノード．
	CO& Children(){ return children; }
	///	子ノード．
	const CO& Children() const { return children; }
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
		for(TYPENAME CO::iterator it = children.begin(); it !=children.end(); ++it){
			T* t = *it;
			(t->*m)();
		}
	}
	///
	template <class M>
	void Traverse(M m){
		  m(this);
		for(TYPENAME CO::iterator it = children.begin(); it !=children.end(); ++it){
			(*it)->Traverse(m);
		}
	}
	template <class M, class A>
	void Traverse(M m, A a){
		m((T*)this, a);
		for(TYPENAME CO::iterator it = children.begin(); it !=children.end(); ++it){
			(*it)->Traverse(m, a);
		}
	}
	template <class T2, class M>
	void MemberTraverse(T2 t, M m){
		(t->*m)(this);
		for(TYPENAME CO::iterator it = children.begin(); it !=children.end(); ++it){
			(*it)->MemberTraverse(t, m);
		}
	}
	template <class E, class M, class A>
	void MemberTraverse(E e, M m, A& a){
		(e->*m)((T*)this, a);
		for(TYPENAME CO::iterator it = children.begin(); it !=children.end(); ++it){
			(*it)->MemberTraverse(e, m, a);
		}
	}
};


template <class T, class CO = std::vector< T > >
class RingBuffer
{
private:
	volatile size_t read_index;
	volatile size_t write_index;
	CO buffer;
public:
	RingBuffer(int s=10){
		read_index = 0;
		write_index = 0;
		buffer.resize(s);
	}
	size_t Size() {
		return buffer.size();
	}
	size_t Length(){
		int diff = (int)write_index - (int)read_index;
		if (diff < 0) diff += Size();
		return diff;
	}
	bool Read(T &result){
		volatile size_t local_read;
		local_read = read_index;

		if (local_read == write_index)
			return false;

		++local_read;
		if (local_read == buffer.size())
			local_read = 0;

		result = buffer[local_read];
		read_index = local_read;
		return true;
	}

	bool Write(const T& element){
		volatile size_t local_write;
		local_write = write_index;

		++local_write;
		if (local_write == buffer.size())
			local_write = 0;

		if (local_write != read_index) {
			buffer[local_write] = element;
			write_index = local_write;
			return true;
		}
		return false;
	}
	void WriteNoLimit(const T& element)
	{
		volatile unsigned local_write;
		local_write = (unsigned)write_index;

		++local_write;
		if (local_write == buffer.size())
			local_write = 0;

		buffer[local_write] = element;
		write_index = local_write;
	}
	void GetLastOne(T& t)
	{
		volatile size_t local_write;
		local_write = write_index;
		t = buffer[local_write];
	}
};

//@}
	
/** 	assert_cast
		SPR_DEBUG定義時はdynamic_cast、それ以外ではstatic_castとして働く。
		dynamic_castに失敗するとstd::bad_cast例外を発生する。
		RTTIを使うC++のdynamic_castを使用する．DCASTの類は使わない	*/
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
//@}

}	//	namespace Spr
#endif

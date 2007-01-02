/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/** @file SprObject.h
	実行時型情報を持つクラスの基本クラス
 */
#ifndef SPR_OBJECT_H
#define SPR_OBJECT_H
#include "../SprBase.h"
#include <iosfwd>

namespace Spr{;

class Object;

#undef DCAST
#define DCAST(T,p) DCastImp((T*)0, p)

///	同じ型へのキャスト
///	静的変換が可能な場合の If->Obj のキャスト
template <class TO, class FROM> TO* DCastImp(TO* dmmy, FROM* p){
	return TO::GetSelf(p);
}
template <class TO, class FROM> TO* DCastImp(TO* dmmy, UTRef<FROM> p){
	return TO::GetSelf(&*p);
}

#undef XCAST
#define XCAST(p) CastToXCastPtr(p)
///	インタフェースクラスのキャスト
template <class T>
class XCastPtr{
public:
	template <class X> operator X*() const {
		T* t = (T*) this;
		return DCastImp((X*)0, t);
	}
	template <class X> operator UTRef<X>() const {
		T* t = (T*) this;
		return DCastImp((X*)0, t);
	}
};

template <class T> XCastPtr<T>& CastToXCastPtr(const T* ptr){
	return *(XCastPtr<T>*)(void*)ptr;
}
template <class T> XCastPtr<T>& CastToXCastPtr(const UTRef<T> ptr){
	return *(XCastPtr<T>*)(void*)&*ptr;
}


struct ObjectIf;
class IfInfo;
class UTTypeDesc;

/**	Factory (オブジェクトを生成するクラス)の基本クラス
*/
class FactoryBase:public UTRefCount{
public:
	virtual ~FactoryBase(){}
	///	作成するIfInfo
	virtual const IfInfo* GetIfInfo() const =0;
	///	iiのオブジェクトを desc から parentの子として生成する．
	virtual ObjectIf* Create(const void* desc, ObjectIf* parent) =0;
};

/**	インタフェースの型情報クラスの基本クラス．クラス名や継承関係を持っていて，DCAST()などが利用する．
1つのインタフェースクラスに付き1個のインスタンスができる．	*/
class IfInfo: public UTRefCount{
public:
	///	IfInfoのid クラスを示す自然数．
	int id;
	///	idの最大値
	static int maxId;
	///	desc型についての詳細な情報
	UTTypeDesc* desc;
	///	クラス名
	const char* className;
	///	基本クラスたち
	IfInfo** base;

	typedef std::vector< UTRef<FactoryBase> > Factories;
	///	子オブジェクトを生成するクラス(ファクトリー)
	Factories factories;
	///	コンストラクタ
	IfInfo(const char* cn, IfInfo** b);
	///	デストラクタ
	virtual ~IfInfo() {};
	///	クラス名
	virtual const char* ClassName() const =0;
	///	key を継承しているかどうかチェック
	bool Inherit(const char* key) const;
	///	key を継承しているかどうかチェック
	bool Inherit(const IfInfo* key) const;
	///	ファクトリ(オブジェクトを生成するクラス)の登録
	void RegisterFactory(FactoryBase* f) const ;
	///	指定(info)のオブジェクトを作るファクトリを検索
	FactoryBase* FindFactory(const IfInfo* info) const;
	///
	static IfInfo* Find(const char* cname);
};
///	IfInfoの実装．1クラス1インスタンス
template <class T>
class IfInfoImp: public IfInfo{
public:
	IfInfoImp(const char* cn, IfInfo** b): IfInfo(cn, b){}
	virtual const char* ClassName() const { return className; }
	virtual void* CreateInstance() const{ return 0;}
};

///	インタフェースが持つべきメンバの宣言部．
#define IF_DEF_FOR_OBJECTIF(cls)										\
public:																	\
	static IfInfoImp<cls##If> ifInfo;	/*	Ifの型情報	*/				\
	virtual const IfInfo* GetIfInfo() const {							\
		return GetIfInfoStatic();										\
	}																	\
	static const IfInfo* SPR_CDECL GetIfInfoStatic();					\
	static void* operator new(size_t) {									\
        assert(0);	/*	Don't allocate interfaces	*/					\
        return (void*)1;												\
    }																	\
	static void operator delete(void* pv) { /*	nothing	to do */ }		\
	/*	? -> If	*/														\
	template <class O>													\
	static cls##If* GetSelf(const O * p){								\
		return p ? p->GetIf((cls##If*)0) : NULL;						\
	}																	\
	/*	同型のIf -> If	*/												\
	cls##If* GetIf(cls##If* i) const { return (cls##If*)this; }			\
	/*	相互キャスト	*/												\
	XCastPtr<cls##If>& Cast() const{									\
		return *(XCastPtr<cls##If>*)(void*)this; }						\
	/*	異型のIf -> Ifへの型変換 */										\
	template <class I> I* GetIf(I*) const {									\
		if (GetIfInfo()->Inherit(I::GetIfInfoStatic())) return (I*)this;	\
		return (I*)GetIfDynamic(I::GetIfInfoStatic());						\
	}																		\


#define IF_DEF(cls)	IF_DEF_FOR_OBJECTIF(cls)

///	すべてのインタフェースクラスの基本クラス
struct ObjectIf{
	IF_DEF_FOR_OBJECTIF(Object);
	virtual ~ObjectIf();
	///	オブジェクト取得
	virtual Object* GetObj(const UTTypeInfo* info) const =0;
	///	動的なインタフェースの取得
	virtual ObjectIf* GetIfDynamic(const IfInfo* info) const = 0;
	///	デバッグ用の表示。子オブジェクトを含む。
	virtual void Print(std::ostream& os) const =0;	
	///	デバッグ用の表示。子オブジェクトを含まない。
	virtual void PrintShort(std::ostream& os) const =0;	

	///	@name 参照カウンタ関係
	//@{
	///
	virtual int AddRef()=0;
	///
	virtual int DelRef()=0;
	///
	virtual int RefCount()=0;
	//@}


	///	@name 子オブジェクト
	//@{
	///	子オブジェクトの数
	virtual size_t NChildObject() const =0;
	///	子オブジェクトの取得
	virtual ObjectIf* GetChildObject(size_t pos) = 0;
	virtual const ObjectIf* GetChildObject(size_t pos) const = 0;
	/**	子オブジェクトの追加．複数のオブジェクトの子オブジェクトとして追加してよい．
		例えば，GRFrameはツリーを作るが，全ノードがGRSceneの子でもある．*/
	virtual bool AddChildObject(ObjectIf* o)=0;
	///	子オブジェクトの削除
	virtual bool DelChildObject(ObjectIf* o)=0;
	///	すべての子オブジェクトの削除と、プロパティのクリア
	virtual void Clear()=0;
	///	オブジェクトを作成し，AddChildObject()を呼ぶ．
	virtual ObjectIf* CreateObject(const IfInfo* info, const void* desc)=0;
	//@}

	///	@name デスクリプタと状態
	//@{
	/**	デスクリプタの読み出し(参照版 NULLを返すこともある)．
		これが実装されていなくても，ObjectIf::GetDesc()は実装されていることが多い．	*/
	virtual const void* GetDescAddress() const = 0;
	/**	デスクリプタの読み出し(コピー版 失敗する(falseを返す)こともある)．
		ObjectIf::GetDescAddress() が実装されていなくても，こちらは実装されていることがある．	*/
	virtual bool GetDesc(void* desc) const = 0;
	/**	ディスクリプタのサイズ	*/
	virtual size_t GetDescSize() const = 0;
	/**	状態の読み出し(参照版 NULLを返すこともある)．
		これが実装されていなくても，ObjectIf::GetState()は実装されていることがある．	*/
	virtual const void* GetStateAddress() const = 0;
	/**	状態の読み出し(コピー版 失敗する(falseを返す)こともある)．
		ObjectIf::GetStateAddress() が実装されていなくても，こちらは実装されていることがある．	*/
	virtual bool GetState(void* state) const = 0;
	/**	状態の設定	*/
	virtual void SetState(const void* state) = 0;
	/**	状態のサイズ	*/
	virtual size_t GetStateSize() const = 0;
	/**	メモリブロックを状態型に初期化	*/
	virtual void ConstructState(void* m) const = 0;
	/**	状態型をメモリブロックに戻す	*/
	virtual void DestructState(void* m) const = 0;
	//@}
};

///	インタフェースクラスへのポインタの配列
struct ObjectIfs: public UTStack<ObjectIf*>{
	void PrintShort(std::ostream& os) const{
		for(const_iterator it = begin(); it!=end(); ++it){
			(*it)->PrintShort(os);
		}
	}
	void Print(std::ostream& os) const{
		for(const_iterator it = begin(); it!=end(); ++it){
			(*it)->Print(os);
		}
	}
};

struct NameManagerIf;

///	名前を持つオブジェクトのインタフェース
struct NamedObjectIf: public ObjectIf{
	IF_DEF(NamedObject);
	///	名前の取得
	virtual const char* GetName() const =0;
	///	名前の設定
	virtual void SetName(const char* n) =0;
	///	名前管理オブジェクトの取得
	virtual NameManagerIf* GetNameManager() =0;
};

struct SceneIf;
///	シーングラフを構成するノードのインタフェース
struct SceneObjectIf: NamedObjectIf{
	IF_DEF(SceneObject);
	///	所属Sceneの取得
	virtual SceneIf* GetScene() =0;
};


///	オブジェクトツリーの状態をメモリ上に保存しておくクラス
struct ObjectStatesIf: public ObjectIf{
	IF_DEF(ObjectStates);

	///	oとその子孫をセーブするために必要なメモリを確保する．
	virtual void AllocateState(ObjectIf* o) = 0;
	///	状態のメモリを解放する
	virtual void ReleaseState(ObjectIf* o) = 0;
	///	状態のサイズを求める
	virtual size_t CalcStateSize(ObjectIf* o) = 0;

	///	状態をセーブする．
	virtual void SaveState(ObjectIf* o) = 0;
	///	状態をロードする．
	virtual void LoadState(ObjectIf* o) = 0;
};
ObjectStatesIf* SPR_CDECL CreateObjectStates();

}

#endif

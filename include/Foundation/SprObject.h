/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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
#include <iostream>
#include <fstream>

namespace Spr{;

class Object;

#if !defined SWIG

#undef DCAST
#define DCAST(T,p) DCastImp((T*)0, p)

///	同じ型へのキャスト
///	静的変換が可能な場合の If->Obj のキャスト
template <class TO, class FROM> TO* DCastImp(TO* dmmy, FROM* p){
	return p ? TO::GetMe(p) : NULL;
}
template <class TO, class FROM> UTRef<TO> DCastImp(TO* dmmy, UTRef<FROM> p){
	return p ? TO::GetMe(&*p) : NULL;
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
	///	対応するデスクリプタ(Desc)型についての詳細な情報
	UTTypeDesc* desc;
	///	対応するステート(State)型についての詳細な情報
	UTTypeDesc* state;
	///	基本インタフェースリスト
	const IfInfo** baseList;
	///	対応するオブジェクトの型情報
	const UTTypeInfo* objInfo;
	///	クラス名
	const char* className;
	///	この型のオブジェクトをCreateできる親（RegisterFactory時に登録する）
	IfInfo* creator;

	typedef std::vector< UTRef<FactoryBase> > Factories;
	///	子オブジェクトを生成するクラス(ファクトリー)
	Factories factories;
	///	コンストラクタ
	IfInfo(const char* cn, const IfInfo** bl, const UTTypeInfo* o):
		desc(NULL), state(NULL), baseList(bl), objInfo(o), className(cn), creator(NULL){id = ++maxId;}
	///	デストラクタ
	virtual ~IfInfo() {};
	///	クラス名
	virtual const char* ClassName() const =0;
	///	ファクトリ(オブジェクトを生成するクラス)の登録
	void RegisterFactory(FactoryBase* f) const ;
	///	指定(info)のオブジェクトを作るファクトリを検索
	FactoryBase* FindFactory(const IfInfo* info) const;
	///
	static IfInfo* Find(const char* cname);
	///	infoを継承しているかどうか．
	bool Inherit(const IfInfo* info) const;
};
///	IfInfoの実装．1クラス1インスタンス
template <class T>
class IfInfoImp: public IfInfo{
public:
	IfInfoImp(const char* cn, const IfInfo** b, const UTTypeInfo* o): IfInfo(cn, b, o){}
	virtual const char* ClassName() const { return className; }
	virtual void* CreateInstance() const{ return 0;}
	static const UTTypeInfo* SPR_CDECL GetTypeInfo();
};

///	インタフェースが持つべきメンバの宣言部．
#define SPR_IFDEF_BASE(cls)												\
public:																	\
	const IfInfo* SPR_CDECL GetIfInfo() const ;							\
	static const IfInfo* SPR_CDECL GetIfInfoStatic();					\
	static void* operator new(size_t) {									\
		return (void*)0;												\
	}																	\
	static void operator delete(void* pv) { /*	nothing	to do */ }		\
	/*	相互キャスト	*/												\
	XCastPtr<cls##If>& Cast() const{									\
		return *(XCastPtr<cls##If>*)(void*)this; }						\
	/*	キャストの実装	*/												\
	template <typename FROM> static cls##If* GetMe(FROM* f){			\
		if ( f && f->GetObjectIf()->GetIfInfo()->Inherit(				\
			cls##If::GetIfInfoStatic()) )								\
			return (cls##If*)f->GetObjectIf();							\
		return NULL;													\
	}																	\

#endif // !SWIG

///	ディスクリプタが持つべきメンバの宣言部．
#ifndef SWIG
#define SPR_DESCDEF(cls)												\
public:																	\
	const static IfInfo* GetIfInfo(){									\
		return cls##If::GetIfInfoStatic();								\
	}																	\

#define SPR_DESCDEF_VIRTUAL(cls)										\
public:																	\
	const static IfInfo* GetIfInfo(){									\
		return cls##If::GetIfInfoStatic();								\
	}																	\
	virtual const IfInfo* GetIfInfoVirtual(){							\
		return cls##If::GetIfInfoStatic();								\
	}																	\


#else
#define SPR_DESCDEF(cls)
#endif
// Rubyなどのポートで使用されるキャスト
#define SPR_IF_HLANG_CAST(cls) static cls##If* Cast(ObjectIf* o){return DCAST(cls##If, o);}

#ifdef SWIGSPR
 #ifdef SWIG_CS_SPR
  #define SPR_IFDEF(cls)	SPR_IFDEF(cls); const IfInfo* SPR_CDECL GetIfInfo() const; static const IfInfo* SPR_CDECL GetIfInfoStatic();
  #define SPR_VIFDEF(cls)	SPR_IFDEF(cls); const IfInfo* SPR_CDECL GetIfInfo() const; static const IfInfo* SPR_CDECL GetIfInfoStatic();
 #else
  #define SPR_IFDEF(cls)	SPR_IFDEF(cls)
  #define SPR_VIFDEF(cls)	SPR_VIFDEF(cls)
 #endif
#else
 #define SPR_VIFDEF(cls)	SPR_IFDEF(cls)
 #ifdef SWIG
 #define SPR_IFDEF(cls)	SPR_IF_HLANG_CAST(cls)
 #else
 #define SPR_IFDEF(cls)	SPR_IFDEF_BASE(cls) SPR_IF_HLANG_CAST(cls)
 #endif
#endif

#ifndef SWIG
#define SPR_OVERRIDEMEMBERFUNCOF(cls, base)	SPR_OVERRIDEMEMBERFUNCOF_##cls(base)
#else
#define SPR_OVERRIDEMEMBERFUNCOF(cls, base)
#endif

struct ObjectIfs;

///	すべてのインタフェースクラスの基本クラス
struct ObjectIf{
	SPR_IFDEF(Object);
	~ObjectIf();
#ifndef SWIGSPR
	ObjectIf* GetObjectIf(){ return this; }
	const ObjectIf* GetObjectIf() const { return this; }
	//	DelRefのみオーバーライド．
	//	単にObjectのDelRefを呼ぶのではなく， delete の制御が必要なため．
	int DelRef() const ;
#endif

	///	デバッグ用の表示。子オブジェクトを含む。
	void Print(std::ostream& os) const;	
	///	デバッグ用の表示。子オブジェクトを含まない。
	void PrintShort(std::ostream& os) const;	

	///	@name 参照カウンタ関係
	//@{
	///
	int AddRef();
	///
	int RefCount();
	//@}


	///	@name 子オブジェクト
	//@{
	///	子オブジェクトの数
	size_t NChildObject() const;
	///	子オブジェクトの取得
	ObjectIf* GetChildObject(size_t pos);
	const ObjectIf* GetChildObject(size_t pos) const;
	/**	子オブジェクトの追加．複数のオブジェクトの子オブジェクトとして追加してよい．
		例えば，GRFrameはツリーを作るが，全ノードがGRSceneの子でもある．*/
	bool AddChildObject(ObjectIf* o);
	///	子オブジェクトの削除
	bool DelChildObject(ObjectIf* o);
	///	すべての子オブジェクトの削除と、プロパティのクリア
	void Clear();
	///	オブジェクトを作成し，AddChildObject()を呼ぶ．
	ObjectIf* CreateObject(const IfInfo* info, const void* desc);
	///	CreateObjectを呼び出すユーティリティ関数
	template <class T> ObjectIf* CreateObject(const T& desc){
		return CreateObject(desc.GetIfInfo(), &desc);
	}
	//@}

	///	@name デスクリプタと状態
	//@{
	/**	デスクリプタの読み出し(参照版 NULLを返すこともある)．
		これが実装されていなくても，ObjectIf::GetDesc()は実装されていることが多い．	*/
	const void* GetDescAddress() const;
	/**	デスクリプタの読み出し(コピー版 失敗する(falseを返す)こともある)．
		ObjectIf::GetDescAddress() が実装されていなくても，こちらは実装されていることがある．	*/
	bool GetDesc(void* desc) const;
	/** デスクリプタの設定 */
	void SetDesc(const void* desc);
	/**	デスクリプタのサイズ	*/
	size_t GetDescSize() const;
	/**	状態の読み出し(参照版 NULLを返すこともある)．
		これが実装されていなくても，ObjectIf::GetState()は実装されていることがある．	*/
	const void* GetStateAddress() const;
	/**	状態の読み出し(コピー版 失敗する(falseを返す)こともある)．
		ObjectIf::GetStateAddress() が実装されていなくても，こちらは実装されていることがある．	*/
	bool GetState(void* state) const;
	/**	状態の設定	*/
	void SetState(const void* state);
	/** 状態の書き出し */
	bool WriteStateR(std::ostream& fout);
	bool WriteState(std::string fileName);
	/** 状態の読み込み */
	bool ReadStateR(std::istream& fin);
	bool ReadState(std::string fileName);
	/**	状態のサイズ	*/
	size_t GetStateSize() const;
	/**	メモリブロックを状態型に初期化	*/
	void ConstructState(void* m) const;
	/**	状態型をメモリブロックに戻す	*/
	void DestructState(void* m) const;
	//@}
	///	オブジェクトツリーのメモリイメージをダンプ
	void DumpObjectR(std::ostream& os, int level=0) const;
};

///	インタフェースクラスへのポインタの配列
struct ObjectIfs
#if !defined SWIG	
	: public UTStack<ObjectIf*>
#endif
{
	/*void PrintShort(std::ostream& os) const{
		for(const_iterator it = begin(); it!=end(); ++it){
			(*it)->PrintShort(os);
		}
	}
	void Print(std::ostream& os) const{
		for(const_iterator it = begin(); it!=end(); ++it){
			(*it)->Print(os);
		}
	}*/
	typedef UTStack<ObjectIf*> container_t;
	void Push(ObjectIf* obj){container_t::Push(obj);}
	void Pop(){container_t::Pop();}
	ObjectIf* Top(){return container_t::Top();}
};

struct NameManagerIf;

///	名前を持つオブジェクトのインタフェース
struct NamedObjectIf: public ObjectIf{
	SPR_IFDEF(NamedObject);
	///	名前の取得
	const char* GetName() const;
	///	名前の設定
	void SetName(const char* n);
	///	名前管理オブジェクトの取得
	NameManagerIf* GetNameManager();
};

struct SceneIf;
///	シーングラフを構成するノードのインタフェース
struct SceneObjectIf: NamedObjectIf{
	SPR_IFDEF(SceneObject);
	///	所属Sceneの取得
	SceneIf* GetScene();
	/// 自分を所属Sceneに複製する
	SceneObjectIf* CloneObject();
};


/**	オブジェクトツリーの状態をメモリ上に保存するためのクラス．
	SaveState(scene) のように呼び出すと，sceneの子孫全体の状態をメモリに保存する．
	セーブした後，LoadState(scene) を呼び出すと，sceneの状態を保存時の状態に戻す．
	セーブ後，シーンのオブジェクトの数や構造が変わってしまうと整合性がとれず，
	メモリを破壊してしまうので注意が必要．

	初めて SaveState(scene) を呼び出すと，まず内部で AllocateState(scene)を呼び出して
	メモリを確保する．次に確保したメモリにオブジェクトの状態を保存する．
	
	セーブ・ロードに使い終わったときは，ReleaseState(scene)を呼び出してメモリを開放する
	必要がある．ReleaseState(scene)は，AllocateState(scene)時とシーンの構造が同じでないと
	整合性がとれず，メモリを破壊してしまう．

	シーンの構造を変更するときは，一度ReleaseState()を呼び出して，再度セーブしなおす必要がある．
*/
struct ObjectStatesIf: public ObjectIf{
	SPR_IFDEF(ObjectStates);

	///	oとその子孫をセーブするために必要なメモリを確保する．
	void AllocateState(ObjectIf* o);
	///	状態のメモリを解放する
	void ReleaseState(ObjectIf* o);
	///	状態のサイズを求める
	size_t CalcStateSize(ObjectIf* o);

	///	状態をセーブする．
	void SaveState(ObjectIf* o);
	///	状態をロードする．
	void LoadState(ObjectIf* o);
	/// シングルセーブ
	void SingleSave(ObjectIf* o);
	/// シングルロード
	void SingleLoad(ObjectIf* o);
	///	ObjectStateオブジェクトを作成する．
	static ObjectStatesIf* SPR_CDECL Create();
};

struct UTTypeDescIf{
	SPR_IFDEF(UTTypeDesc);
	enum FieldType {
		SINGLE, ARRAY, VECTOR
	};
	static UTTypeDescIf* FindTypeDesc(const char* typeName, const char* moduleName);
	static void PrintPool(std::ostream& os);
		///
	void Print(std::ostream& os) const;
	///	型名
	std::string GetTypeName() const;
	///	型名
	void SetTypeName(const char* s);
	///	型のサイズ
	size_t GetSize();
	///	組み立て型かどうか
	bool IsPrimitive();
	///	文字列やvectorや参照を含まなければ true
	bool IsSimple();
	///	組み立て型のフィールドの数
	int NFields();
	///	フィールドのTypeDesc
	UTTypeDesc* GetFieldType(int i);
	///	Fieldが配列の場合の配列の長さを返す
	int GetFieldLength(int i);
	///	Fieldのvector場合のvectorの長さを返す
	int GetFieldVectorSize(int i, const void* base);
	///	要素数を別のフィールドからとる場合のフィールド名
	const char* GetFieldLengthName(int i);
	///	vector/配列かどうか
	UTTypeDescIf::FieldType GetFieldVarType(int i);
	///	参照かどうか
	bool GetFieldIsReference(int i);
	///	フィールド名
	const char* GetFieldName(int i);
	///	フィールドのアドレスの取得
	void* GetFieldAddress(int i, void* base, int pos);
	const void* GetFieldAddress(int i, const void* base, int pos);
	///	フィールドのアドレスを計算．vectorを拡張する．
	void* GetFieldAddressEx(int i, void* base, int pos);
	///	文字列にフィールドを読み出す
	std::string ReadToString(int i, void* base, int pos);
	///	文字列からフィールドに書き込む
	void WriteFromString(std::string from, int i, void* base, int pos);
	///
	const IfInfo* GetIfInfoOfType();

	//	ユーティリティ関数
	bool IsBool();
	bool IsNumber();
	bool IsString();
	///	TypeDescがboolの単純型の場合に，boolを読み出す関数
	bool ReadBool(const void* ptr);
	///	TypeDescが数値の単純型の場合に，数値を書き込む関数
	void WriteBool(bool val, void* ptr);
	///	TypeDescが数値の単純型の場合に，数値を読み出す関数
	double ReadNumber(const void* ptr);
	///	TypeDescが数値の単純型の場合に，数値を書き込む関数
	void WriteNumber(double val, void* ptr);
	///	文字列読み出し
	std::string ReadString(const void* ptr);
	///	文字列書き込み
	void WriteString(const char* val, void* ptr);

	///	オブジェクトの構築
	void* Create();
	///	オブジェクトの後始末
	void Delete(void* ptr);
	///	vector::push_back() return &vector::back();
	void* VectorPush(void* v);
	///	vector::pop_back();
	void VectorPop(void* v);
	///	return &vector::at(pos);
	void* VectorAt(void* v, int pos);
	const void* VectorAt(const void* v, int pos);
	///	return vector::size();
	size_t VectorSize(const void * v);
	///
	size_t SizeOfVector();
	//	シリアライズ
	///	ストリームに書き出し
	void Write(std::ostream& os, void* base);
	///	ストリームから読み出し
	void Read(std::istream& is, void* base);
};

}

#endif

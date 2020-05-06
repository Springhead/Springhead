/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef UTTYPEDESC_H
#define UTTYPEDESC_H

#include <Base/BaseUtility.h>
#include <Base/BaseTypeInfo.h>
#include <Foundation/Object.h>
#include <set>
#include <map>
#include <algorithm>

/*
	ファイルローダは，型→データを持ってくる．
	ポインタはあけておいて，後でリンクする．
	あるオブジェクトのどこにポインタを足す/上書きするのか指示する仕組みが必要．


	配列は，可変長データ．メモリ上はvectorなどなので，ちょっと違うけど．
	vector のままだと，強引なキャストが必要になる．どうする？
	⇒vectorアクセスメソッドをソースから自動生成．

	文字列を数値に変換するところまではパーサの仕事．
	double を 数値型に変換するのはTypeDescの仕事．
	パーサが扱う型は，数値，文字列，ポインタ，組み立て
	typedescが扱うのは，数値→char int float double
	
	typedesc->CreateInstance();
	typedesc->SetData("メンバ名", データ);
		:
		:
	typedesc->GetData("メンバ名", データ);


	データはNodeのツリー．
	Nodeは，フィールドの列
	フィールドは，数値，文字列，ポインタ，組み立て
	組み立てはTypeDescを持つ．
*/
/*
	Fieldと構造体の関係の見直し→Field = Cの型にしたい．

	Cの型は，単純型とstruct (の配列)
	∴ Field = 単純型 or struct (の配列)
		TypeDesc は単純型とstruct
*/

namespace Spr{;
/**	@page UTTypeDesc ドキュメントオブジェクトと型記述
	C++の構造体からデータを書き出す場合など，構造体やデータには，
	変数名や変数の型と言った情報はない．
	そこで，型記述型オブジェクト(UTTypeDesc)を使って記述する．
	UTTypeDescのオブジェクトを作るためのソースはヘッダファイルを typedesc.exe が
	パースして自動生成する．*/

///	対象の型にアクセスするためのクラス
class UTAccessBase:public UTRefCount{
public:
	virtual ~UTAccessBase(){}
	///	オブジェクトの構築
	virtual void* Create()=0;
	///	オブジェクトの破棄
	virtual void Delete(void* ptr)=0;
	///	vector<T>::push_back(); return &back();
	virtual void* VectorPush(void* v)=0;
	///	vector<T>::pop_back();
	virtual void VectorPop(void* v)=0;
	///	vector<T>::at(pos);
	virtual void* VectorAt(void* v, int pos)=0;
	///	vector<T>::size();
	virtual size_t VectorSize(const void* v)=0;
	///
	virtual size_t SizeOfVector()=0;
};
template <class T>
class UTAccess:public UTAccessBase{
	virtual void* Create(){ return DBG_NEW T; }
	virtual void Delete(void* ptr){delete (T*)ptr; }
#if defined _MSC_VER
# pragma warning (push)
# pragma warning (disable : 4172)
#endif
	virtual void* VectorPush(void* v){
		((std::vector<T>*)v)->push_back(T());
#ifdef	_WIN32
		return &((std::vector<T>*)v)->back();
#else
		T tmp = ((std::vector<T>*)v)->back();
		T* ptmp = &tmp;
		return ptmp;
#endif
	}
#if defined _MSC_VER
# pragma warning (pop)
#endif
	virtual void VectorPop(void* v){
		((std::vector<T>*)v)->pop_back();
	}
#if defined _MSC_VER
# pragma warning (push)
# pragma warning (disable : 4172)
#endif
	virtual void* VectorAt(void* v, int pos) {
#ifdef	_WIN32
		return &((std::vector<T>*)v)->at(pos);
#else
		T tmp = ((std::vector<T>*)v)->at(pos);
		T* ptmp = &tmp;
		return ptmp;
#endif
	}
#if defined _MSC_VER
# pragma warning (pop)
#endif
	size_t VectorSize(const void* v){
		return ((const std::vector<T>*)v)->size();
	}
	virtual size_t SizeOfVector(){
		return sizeof(std::vector<T>);
	}
};

class UTTypeDescDb;

///	型を表す
class SPR_DLL UTTypeDesc:public Object{
public:
	SPR_OBJECTDEF(UTTypeDesc);
	enum { BIGVALUE= 0x40000000 };
	///	レコードのフィールドを表す
	class SPR_DLL Field{
	public:
		typedef std::vector<std::pair<std::string, int> > Enums;
		Enums enums;
		///	メンバ名
		std::string name;
		///	型名
		std::string typeName;
		///	型
		UTRef<UTTypeDesc> type;
		///	オフセット
		int offset;
		///	配列の要素数．
		int length;
		///	要素数を別のフィールドからとる場合のフィールド名
		std::string lengthFieldName;
		///	vector/配列かどうか
		UTTypeDescIf::FieldType varType;
		///	参照かどうか
		bool isReference;

		Field(): offset(-1), length(1), varType(UTTypeDescIf::SINGLE), isReference(false){}
		~Field();
		///	データのサイズ
		size_t GetSize();
		///
		void Print(std::ostream& os) const;
		///
		void AddEnumConst(std::string name, int val);
		void AddEnumConst(std::string name);
		///	フィールドのアドレスを計算
		const void* GetAddress(const void* base, int pos);
		void* GetAddress(void* base, int pos){
			return (void*)GetAddress((const void*)base, pos); 
		}
		///	フィールドのアドレスを計算．vectorを拡張する．
		void* GetAddressEx(void* base, int pos);
		///	フィールドがstd::vectorの場合，vector::size() を呼び出す．
		size_t VectorSize(const void * base){ 
			return type->access->VectorSize(((char*)base)+offset); 
		}
		///	typeがboolの単純型の場合に，boolを読み出す関数
		bool ReadBool(const void* base, int pos=0){
			return type->ReadBool(GetAddress(base, pos));
		}
		///	typeがboolの単純型の場合に，boolを書き込む関数
		void WriteBool(void* base, bool val, int pos = 0){
			if (base) type->WriteBool(val, GetAddressEx(base, pos));
		}
		///	typeが数値の単純型の場合に，数値を読み出す関数
		double ReadNumber(const void* base, int pos=0){
			return type->ReadNumber(GetAddress(base, pos));
		}
		///	typeが数値の単純型の場合に，数値を書き込む関数
		void WriteNumber(void* base, double val, int pos = 0){
			if (base) type->WriteNumber(val, GetAddressEx(base, pos));
		}
		///	文字列読み出し
		std::string ReadString(const void* base, int pos=0){
			return type->ReadString(GetAddress(base, pos));
		}
		///	文字列書き込み
		void WriteString(void* base, const char* val, int pos=0){
			if (base) type->WriteString(val, GetAddressEx(base, pos));
		}
	};
	///	組み立て型をあらわす場合に使う
	class SPR_DLL Composit: public std::vector<Field>{
	public:
		///	データのサイズを返す
		int Size(){ return Size(""); }
		///	データのサイズを返す
		int Size(std::string id);
		///
		void Link(UTTypeDescDb* db);
		///
		void Print(std::ostream& os) const;
		///
		Field* Find(const char* id);
	};

	///	型名
	std::string typeName;
	///	データの長さ
	size_t size;
	///	組み立て型の中身の記述．
	Composit composit;
	///	プリミティブかどうか
	bool bPrimitive;
	///	バイナリダンプが可能かどうか
	enum BinaryType{UNKNOWN_BINARY, SIMPLE_BINARY, COMPLEX_BINARY} bSimple;
	///	IfInfo
	const IfInfo* ifInfo;
	///
	UTRef<UTAccessBase> access;

	friend class UTTypeDescDb;
public:
	///	コンストラクタ
	UTTypeDesc():size(0), bPrimitive(false), bSimple(UNKNOWN_BINARY), ifInfo(NULL){}
	///	コンストラクタ
	UTTypeDesc(std::string tn, int sz=0): 
		typeName(tn), size(sz), bPrimitive(false), bSimple(UNKNOWN_BINARY), ifInfo(NULL){}
	///	
	virtual ~UTTypeDesc(){}
	///
	void Print(std::ostream& os) const;
	///	型名
	std::string GetTypeName() const { return typeName; }
	///	型名
	void SetTypeName(const char* s) { typeName = s; }
	///	型のサイズ
	size_t GetSize() { return size; }
	///	フィールドの追加
	Field* AddField(std::string pre, std::string ty, std::string n, std::string post);
	///	baseの追加
	Field* AddBase(std::string tn);

	///	組み立て型かどうか
	bool IsPrimitive(){ return bPrimitive; }
	///	文字列やvectorや参照を含まなければ true
	bool IsSimple(){ return bSimple == SIMPLE_BINARY; }
	///	bFlatの初期化
	BinaryType CheckSimple();
	///	組み立て型の要素
	Composit& GetComposit(){ return composit; }
	///	組み立て型のフィールドの数
	int NFields() { return (int)composit.size(); }
	///	フィールドのTypeDesc
	UTTypeDesc* GetFieldType(int i) { return composit[i].type; }
	///	Fieldが配列の場合の配列の長さを返す
	int GetFieldLength(int i) { return composit[i].length; }
	///	Fieldのvector場合のvectorの長さを返す
	int GetFieldVectorSize(int i, const void* base) { return (int)composit[i].VectorSize(base); }
	///	要素数を別のフィールドからとる場合のフィールド名
	const char* GetFieldLengthName(int i) { return composit[i].lengthFieldName.c_str(); }
	///	vector/配列かどうか
	UTTypeDescIf::FieldType GetFieldVarType(int i) { return composit[i].varType; }
	///	参照かどうか
	bool GetFieldIsReference(int i) { return composit[i].isReference; }
	///	フィールド名
	const char* GetFieldName(int i) { return composit[i].name.c_str(); }
	///	フィールドのアドレスの取得
	void* GetFieldAddress(int i, void*base, int pos) { return composit[i].GetAddress(base, pos); }
	const void* GetFieldAddress(int i, const void*base, int pos) { return composit[i].GetAddress(base, pos); }
	///	フィールドのアドレスを計算．vectorを拡張する．
	void* GetFieldAddressEx(int i, void* base, int pos) { return composit[i].GetAddressEx(base, pos); }
	///	文字列からフィールドに書き込む
	std::string ReadToString(int i, void* base, int pos);
	///	文字列からフィールドに書き込む
	void WriteFromString(std::string from, int i, void* base, int pos);

	///	フィールドの型情報のリンク
	void Link(UTTypeDescDb* db);
	///	リンクの確認
	bool LinkCheck();
	///
	const IfInfo* GetIfInfoOfType() { return ifInfo; }

	//	ユーティリティ関数
	virtual bool IsBool(){ return false; }
	virtual bool IsNumber(){ return false; }
	virtual bool IsString(){ return false; }
	///	TypeDescがboolの単純型の場合に，boolを読み出す関数
	virtual bool ReadBool(const void* ptr){ assert(0); return 0;}
	///	TypeDescが数値の単純型の場合に，数値を書き込む関数
	virtual void WriteBool(bool val, void* ptr){ assert(0);}
	///	TypeDescが数値の単純型の場合に，数値を読み出す関数
	virtual double ReadNumber(const void* ptr){ assert(0); return 0;}
	///	TypeDescが数値の単純型の場合に，数値を書き込む関数
	virtual void WriteNumber(double val, void* ptr){ assert(0);}
	///	文字列読み出し
	virtual std::string ReadString(const void* ptr){ assert(0);  return 0;}
	///	文字列書き込み
	virtual void WriteString(const char* val, void* ptr){ assert(0); }
	
	///	オブジェクトの構築
	void* Create(){
		return access ? access->Create() : NULL;
	}
	///	オブジェクトの後始末
	void Delete(void* ptr){ 
		if (ptr){
			assert(access);
			access->Delete(ptr);
		}
	}
	///	vector::push_back() return &vector::back();
	void* VectorPush(void* v){ return access->VectorPush(v); }
	///	vector::pop_back();
	void VectorPop(void* v){ access->VectorPop(v); }
	///	return &vector::at(pos);
	void* VectorAt(void* v, int pos){ return access->VectorAt(v, pos); }
	const void* VectorAt(const void* v, int pos){ return VectorAt((void*)v, pos); }
	///	return vector::size();
	size_t VectorSize(const void * v){ return access->VectorSize(v); }
	///
	size_t SizeOfVector(){ return access->SizeOfVector(); }
	
	//	シリアライズ
	///	ストリームに書き出し
	void Write(std::ostream& os, void* base);
	///	ストリームから読み出し
	void Read(std::istream& is, void* base);
};
inline bool operator < (const UTTypeDesc& d1, const UTTypeDesc& d2){
	return d1.GetTypeName().compare(d2.GetTypeName()) < 0;
}

template <class N>
class SPR_DLL UTTypeDescNumber:public UTTypeDesc{
public:
	UTTypeDescNumber(){
		access = DBG_NEW UTAccess<N>;
		bPrimitive = true;
	}
	UTTypeDescNumber(std::string tn): UTTypeDesc(tn, sizeof(N)){
		access = DBG_NEW UTAccess<N>;
		bPrimitive = true;
	}
protected:
	///	数値読み出し
	virtual double ReadNumber(const void* ptr){
		return *(const N*)ptr;
	}
	///	数値書き込み
	virtual void WriteNumber(double val, void* ptr){
		*(N*)ptr = (N)val;
	}
	virtual bool IsNumber(){ return true; }
};

template <class N>
class SPR_DLL UTTypeDescBool:public UTTypeDesc{
public:
	UTTypeDescBool(){
		access = DBG_NEW UTAccess<N>;
		bPrimitive = true;
	}
	UTTypeDescBool(std::string tn): UTTypeDesc(tn, sizeof(N)){
		access = DBG_NEW UTAccess<N>;
		bPrimitive = true;
	}
protected:
	///	bool読み出し
	virtual bool ReadBool(const void* ptr){
		return *(const N*)ptr != 0;
	}
	///	bool書き込み
	virtual void WriteBool(bool val, void* ptr){
		*(N*)ptr = (N)val;
	}
	virtual bool IsBool(){ return true; }
};

class SPR_DLL UTTypeDescString:public UTTypeDesc{
public:
	UTTypeDescString(){
		access = DBG_NEW UTAccess<std::string>;
		bPrimitive = true;
	}
	UTTypeDescString(std::string tn): UTTypeDesc(tn, sizeof(std::string)){
		access = DBG_NEW UTAccess<std::string>;
		bPrimitive = true;
	}
protected:
	virtual bool IsString(){ return true; }
	///	文字列読み出し
	virtual std::string ReadString(const void* ptr){
		return *(std::string*)ptr;
	}
	///	文字列書き込み
	virtual void WriteString(const char* val, void* ptr){
		*(std::string*)ptr = val;
	}
};

///	型のデータベース
class SPR_DLL UTTypeDescDb: public UTRefCount{
	std::set<UTString, UTStringLess> addedGroups;
public:
	///	コンテナの型
	typedef std::set< UTRef<UTTypeDesc>, UTContentsLess< UTRef<UTTypeDesc> > > Db;
protected:
	std::string group;			///<	グループ名
	std::string prefix;			///<	名前のうちプレフィックスの部分
	Db db;						///<	UTTypeDesc を入れておくコンテナ
public:
	UTTypeDescDb(UTString gp=""):group(gp){}
	///	
	~UTTypeDescDb();
	/**	型情報をデータベースに登録．	*/
	void RegisterDesc(UTTypeDesc* n){
		if (prefix.length() && n->typeName.compare(0, prefix.length(), prefix) == 0){
			n->typeName = n->typeName.substr(prefix.length());
		}
		db.insert(n);
	}
	/**	型名のAliasを登録	*/
	void RegisterAlias(const char* src, const char* dest){
		UTTypeDesc* srcDesc = Find(src);
		assert(srcDesc);
		UTTypeDesc* destDesc =DBG_NEW UTTypeDesc(*srcDesc);
		destDesc->typeName = dest;
		RegisterDesc(destDesc);
	}
	/**	型情報をプロトタイプリストに登録	*/
	void RegisterProto(UTTypeDesc* n);
	/**	型名のPrefix を設定．
		型名をFindで検索する際に，検索キーにPrefixをつけたキーでも型名を検索する．	*/
	void SetPrefix(std::string p);
	/**	型情報を名前から検索する．
		@param tn	型名．prefix は省略してよい．	*/
	UTTypeDesc* Find(std::string tn);
	///	DB内の型情報を、引数dbにリンク．
	void Link(UTTypeDescDb* db=NULL);
	///	リンクの確認
	bool LinkCheck();
	///	DB内の型情報の表示
	void Print(std::ostream& os) const;
	UTTypeDescDb& operator += (const UTTypeDescDb& b){
		if (b.group.compare(group)!=0 && 
			addedGroups.find(b.group) == addedGroups.end()){
			db.insert(b.db.begin(), b.db.end());
			Link();
			addedGroups.insert(b.group);
		}
		return *this;
	}
	UTString GetGroup() const {
		return group;
	}
	void Clear(){
		db.clear();
	}
	friend class UTTypeDescDbPool;
};
inline bool operator < (const UTTypeDescDb& d1, const UTTypeDescDb& d2){
	return d1.GetGroup() < d2.GetGroup();
}

class UTTypeDescDbPool: public UTRefCount, 
	public std::set< UTRef<UTTypeDescDb>, UTContentsLess< UTRef<UTTypeDescDb> > >{
protected:
	static UTRef<UTTypeDescDbPool> pool;
public:
	static UTTypeDescDbPool* SPR_CDECL GetPool();
	static UTTypeDescDb* SPR_CDECL Get(std::string gp);
	static void SPR_CDECL Print(std::ostream& os);
};

/**	TypeDescのフィールドのイタレータ
	バイナリファイルやXファイルから，ある型のデータを順に読み出していく場合，
	読み出し中のデータがUTTypeDescのツリーのどこに対応するかを保持しておく必要がある．
*/
class UTTypeDescFieldIt{
public:
	/**	フィールドの種類を示すフラグ．
		ほとんどのファイルフォーマットで，整数，実数，文字列で，異なるパーサが必要になる．
		そこで，それらで分類．
		組み立て型は，UTTypeDescを参照して読み出すので，F_BLOCKを用意した．
	*/
	enum FieldType{
		F_NONE, F_BOOL, F_INT, F_REAL, F_STR, F_BLOCK
	};
	UTTypeDesc* type;						///<	読み出し中のUTTypeDesc
	UTTypeDesc::Composit::iterator field;	///<	組み立て型の場合，その中のどのフィールドか
	int arrayPos;							///<	配列の場合，読み出し中の添え字
	int arrayLength;						///<	固定長の場合の配列の長さ
	FieldType fieldType;					///<	読み出すフィールドの型

	UTTypeDescFieldIt(UTTypeDesc* d);		///<	コンストラクタ
	bool NextField(void* base);				///<	次のフィールドに進む
	bool PrevField(void* base);				///<	前のフィールドに進む
	bool HaveField(UTString name);			///<	指定の名前のフィールドを持っているならtrue。継承元のフィールドも検索する。
protected:
	void SetFieldInfo(void* base);			///<	NextFieldが使う
	FieldType GetTypeId(UTTypeDesc* t);
	friend class UTLoadContext;
};
class UTTypeDescFieldIts:public UTStack<UTTypeDescFieldIt>{
public:
	///
	void PushType(UTTypeDesc* t){
		Push(UTTypeDescFieldIt(t));
	}
	///	次のフィールドに進む
	bool NextField(void* base){
		if(size()) return back().NextField(base);
		return false;
	}
	///	配列中での位置
	int ArrayPos(){
		if(size()) return back().arrayPos;
		return -1;
	}
	///	配列の長さ
	int ArrayLength(){
		if(size()) return back().arrayLength;
		return 0;
	}
	bool IncArrayPos(){
		if(!size()) return false;
		if (back().arrayPos == -2) return true;
		++ back().arrayPos;
		return back().arrayPos < back().arrayLength;
	}
	bool IsArrayRest(){
		if (!size()) return false;
		return back().arrayPos < back().arrayLength;
	}
	bool IsBool(){
		return back().fieldType==UTTypeDescFieldIt::F_BOOL;
	}
	bool IsNumber(){
		return back().fieldType==UTTypeDescFieldIt::F_INT || back().fieldType==UTTypeDescFieldIt::F_REAL;
	}
	bool IsInteger(){
		return back().fieldType==UTTypeDescFieldIt::F_INT;
	}
	bool IsReal(){
		return back().fieldType==UTTypeDescFieldIt::F_REAL;
	}
	bool IsString(){
		return back().fieldType==UTTypeDescFieldIt::F_STR;
	}
	bool IsBlock(){
		return back().fieldType==UTTypeDescFieldIt::F_BLOCK;
	}
	bool IsValid(){
		return back().fieldType!=UTTypeDescFieldIt::F_NONE;
	}
};

void SPR_CDECL UTRegisterTypeDescs();

///	単純型を登録する．
#define REG_FIELD(type)	RegisterDesc( DBG_NEW UTTypeDesc(#type, sizeof(type)) )
/**	ドキュメントからロード可能なクラスの定義．
	クラスが基本クラスを持つ場合	*/
#define DEF_RECORDBASE(Type, Base, Def)					\
	struct Type: public Base Def;						\
	static const char* classNameOf##Type = #Type;		\
	static const char* classDefOf##Type = #Def;			\

/**	ドキュメントからロード可能なクラスの定義．
	基本クラスが無い場合	*/
#define DEF_RECORD(Type, Def)							\
	struct Type Def;									\
	static const char* classNameOf##Type = #Type;		\
	static const char* classDefOf##Type = #Def;			\

/**	DER_RECORD で定義した型を登録する．*/
#define REG_RECORD(Type)	\
	Register(classNameOf##Type, classDefOf##Type)

/**	DER_RECORD で定義した型をプロトタイプとしても登録する．*/
#define REG_RECORD_PROTO(Type)	\
	Register(classNameOf##Type, classDefOf##Type, true)

/**	@page TypeDesc 型説明クラス
C++の構造体宣言を文字列として受け取り，型を解析する．
使い方：ヘッダファイルで，定義
DEF_RECORD( Test, {
	int x;
	char y;
}
)

を書いて，クラスの定義を行い，
.cppファイルの何かの関数の中で
	REG_RECORD(Test);
を書いて登録します．
*/


}
#endif

#ifndef FITYPEDESC_H
#define FITYPEDESC_H

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
	メソッドをソースから自動生成？TypeDescが仕事すればそれで済む．

	文字列を数値に変換するところまではパーサの仕事．
	double を 数値型に変換するのはTYpeDescの仕事．

	パーサが扱う型は，数値，文字列，ポインタ，組み立て
	Typedescが扱うのは，数値→char int float double
	
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


namespace Spr{;
/**	@page FITypeDesc ドキュメントオブジェクトと型記述
	C++の構造体からデータを書き出す場合など，構造体やデータには，
	変数名や変数の型と言った情報はない．
	そこで，型記述型オブジェクト(FITypeDesc)を使って記述しておく．
*/

/**	FIVVectorの基本クラス．
	ベクタへのロードとセーブのサポートに必要な仮想関数を用意	*/
class FIVVectorBase{
public:
	virtual size_t VItemSize() = 0;
	virtual size_t VSize() = 0;
	virtual void VResize(size_t s) = 0;
	virtual char* VBegin() = 0;
	virtual char* VEnd() = 0;
	virtual void VInsert(int pos, void* v) = 0;
	virtual void VPushBack(void* v) = 0;
	virtual void VPushBack() = 0;
	virtual void VPopBack() = 0;
};

/**	FITypeDesc が理解できるstd::vector
	ベクタへのロードとセーブのサポートに必要な仮想関数が実装される．
	FITypeDesc は普通のvector を含むクラスはサポートしない．*/
template <class T>
class FIVVector: public FIVVectorBase, public std::vector<T>{
	virtual size_t VItemSize(){ return sizeof(T); }
	virtual size_t VSize(){ return size(); }
	virtual void VResize(size_t s){ resize(s); }
	virtual char* VBegin(){ return (char*)&*begin(); }
	virtual char* VEnd(){ return (char*) &*end(); }
	virtual void VInsert(int pos, void* v){ insert(begin()+pos, *(T*)v); }
	virtual void VPushBack(void* v){ push_back(*(T*)v); }
	virtual void VPushBack(){ push_back(T()); }
	virtual void VPopBack(){ pop_back(); }
public:
	FIVVector(){}
	FIVVector(const std::vector<T>& v):std::vector<T>(v){}
	FIVVector& operator=(const std::vector<T>& v){ *(std::vector<T>*)this = v; return *this; }
};

///	対象の型にアクセスするためのクラス
class FIAccessBase:public UTRefCount{
public:
	///	コンストラクタ呼び出し
	virtual void* Construct(void* mem)=0;
	///	デストラクタ呼び出し
	virtual void Destruct(void* ptr)=0;
	///	vector<T>::push_back(); return &back();
	virtual void* VectorPush(void* v)=0;
	///	vector<T>::pop_back();
	virtual void VectorPop(void* v)=0;
	///	vector<T>::at(pos);
	virtual void* VectorAt(void* v, int pos)=0;
	///
	virtual size_t SizeOfVector()=0;
};
template <class T>
class FIAccess:public FIAccessBase{
	virtual void* Construct(void* mem){ return new(mem) T; }
	virtual void Destruct(void* ptr){((T*)ptr)->~T(); }
	virtual void* VectorPush(void* v){
		((std::vector<T>*)v)->push_back(T());
		return &((std::vector<T>*)v)->back();
	}
	virtual void VectorPop(void* v){
		((std::vector<T>*)v)->pop_back();
	}
	virtual void* VectorAt(void* v, int pos){
		return &((std::vector<T>*)v)->at(pos);
	}
	virtual size_t SizeOfVector(){
		return sizeof(std::vector<T>);
	}
};

class FITypeDescDb;
///	型を表す
class SPR_DLL FITypeDesc:public UTRefCount{
public:
	///	レコードのフィールドを表す
	class SPR_DLL Field{
	public:
		typedef std::vector<std::pair<std::string, int> > Enums;
		Enums enums;
		Field(): length(1), offset(-1), isReference(false), varType(SINGLE){}
		~Field();
		///	メンバ名
		std::string name;
		///	型名
		std::string typeName;
		///	オフセット
		int offset;
		///	型
		UTRef<FITypeDesc> type;
		///	配列の要素数．
		int length;
		///	要素数を別のフィールドからとる場合のフィールド名
		std::string lengthFieldName;
		///	vector/配列かどうか
		enum VarType{
			SINGLE, ARRAY, VECTOR
		} varType;
		///	参照かどうか
		bool isReference;
		///	データのサイズ
		size_t GetSize();
		///
		void Print(std::ostream& os) const;
		///
		void AddEnumConst(std::string name, int val);
		void AddEnumConst(std::string name);
		///	フィールドのアドレスを計算
		const void* FITypeDesc::Field::GetAddress(const void* base, int pos);
		void* FITypeDesc::Field::GetAddress(void* base, int pos){
			return (void*)GetAddress((const void*)base, pos); 
		}
		///	typeが数値の単純型の場合に，数値を読み出す関数
		double ReadNumber(const void* base, int pos=0){
			return type->ReadNumber(GetAddress(base, pos));
		}
		///	typeが数値の単純型の場合に，数値を書き込む関数
		void WriteNumber(void* base, double val, int pos = 0){
			type->WriteNumber(val, GetAddress(base, pos));
		}
		///	文字列読み出し
		std::string ReadString(const void* base, int pos=0){
			return type->ReadString(GetAddress(base, pos));
		}
		///	文字列書き込み
		void WriteString(void* base, const char* val, int pos=0){
			type->WriteString(val, GetAddress(base, pos));
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
		void Link(FITypeDescDb* db);
		///
		void Print(std::ostream& os) const;
	};
protected:
	///	型名
	std::string typeName;
	///	データの長さ
	size_t size;
	///	組み立て型の中身の記述．単純型の場合は，size() == 0
	Composit composit;
	///	IfInfo
	const IfInfo* ifInfo;
	///
	UTRef<FIAccessBase> access;

	friend class FITypeDescDb;
	friend void RegisterTypes();
public:
	///	コンストラクタ
	FITypeDesc():size(0), ifInfo(NULL){}
	///	コンストラクタ
	FITypeDesc(std::string tn, int sz=0): typeName(tn), size(sz), 
		ifInfo(NULL){}
	///	
	~FITypeDesc(){}
	///
	void Print(std::ostream& os) const;
	///	型名
	std::string GetTypeName() const { return typeName; }
	///	型名
	std::string SetTypeName(const char* s) { typeName = s; }
	///	型のサイズ
	size_t GetSize() { return size; }
	///	フィールドの追加
	Field* AddField(std::string pre, std::string ty, std::string n, std::string post);
	///	baseの追加
	Field* AddBase(std::string tn);

	///	組み立て型かどうか
	bool IsComposit(){ return composit.size()!=0; }
	///	組み立て型の要素
	Composit& GetComposit(){ return composit; }
	///	フィールドの型情報のリンク
	void Link(FITypeDescDb* db);
	///
	const IfInfo* GetIfInfo(){ return ifInfo; }

	//	ユーティリティ関数
	///	TypeDescが数値の単純型の場合に，数値を読み出す関数
	virtual double ReadNumber(const void* ptr){ assert(0); return 0;}
	///	TypeDescが数値の単純型の場合に，数値を書き込む関数
	virtual void WriteNumber(double val, void* ptr){ assert(0);}
	///	文字列読み出し
	virtual std::string ReadString(const void* ptr){ assert(0);  return 0;}
	///	文字列書き込み
	virtual void WriteString(const char* val, void* ptr){ assert(0); }

	///	オブジェクトの構築
	void* Construct(void* mem=NULL){
		if (!mem) mem = new char[GetSize()];
		access->Construct(mem);
		return mem;
	}
	///	オブジェクトの後始末
	void Destruct(void* ptr){ access->Destruct(ptr); }
	///	オブジェクトの削除
	void Delete(void* ptr){ Destruct(ptr); delete ptr; };
	///	vector::push_back() return &vector::back();
	void* VectorPush(void* v){ return access->VectorPush(v); }
	///	vector::pop_back();
	void VectorPop(void* v){ access->VectorPop(v); }
	///	return &vector::at(pos);
	void* VectorAt(void* v, int pos){ return access->VectorAt(v, pos); }
	const void* VectorAt(const void* v, int pos){ return VectorAt((void*)v, pos); }
	///
	size_t SizeOfVector(){ return access->SizeOfVector(); }
};
inline bool operator < (const FITypeDesc& d1, const FITypeDesc& d2){
	return d1.GetTypeName().compare(d2.GetTypeName()) < 0;
}

template <class N>
class SPR_DLL FITypeDescNumber:public FITypeDesc{
public:
	FITypeDescNumber(){}
	FITypeDescNumber(std::string tn): FITypeDesc(tn, sizeof(N)){}
protected:
	///	数値読み出し
	virtual double ReadNumber(const void* ptr){
		return *(const N*)ptr;
	}
	///	数値書き込み
	virtual void WriteNumber(double val, void* ptr){
		*(N*)ptr = (N)val;
	}
};

///	型のデータベース
class SPR_DLL FITypeDescDb: public UTRefCount{
public:
	///	コンテナの型
	typedef std::set< UTRef<FITypeDesc>, UTContentsLess< UTRef<FITypeDesc> > > Db;
	struct ProtoDesc{
		std::string fileType;
		UTRef<FITypeDesc> desc;
	};
	typedef std::vector<ProtoDesc> ProtoDescs;
protected:
	std::string prefix;			///<	名前のうちプレフィックスの部分
	Db db;						///<	FITypeDesc を入れておくコンテナ
	static ProtoDescs protoDescs;
public:
	///	
	~FITypeDescDb();
	static const ProtoDescs& GetProtoDescs(){ return protoDescs; }
	/**	型情報をデータベースに登録．	*/
	void RegisterDesc(FITypeDesc* n){
		if (prefix.length() && n->typeName.compare(0, prefix.length(), prefix) == 0){
			n->typeName = n->typeName.substr(prefix.length());
		}
		db.insert(n);
	}
	/**	型情報をプロトタイプリストに登録	*/
	void RegisterProto(FITypeDesc* n);
	/**	型名のPrefix を設定．
		型名をFindで検索する際に，検索キーにPrefixをつけたキーでも型名を検索する．	*/
	void SetPrefix(std::string p);
	/**	型情報を名前から検索する．
		@param tn	型名．prefix は省略してよい．	*/
	FITypeDesc* Find(std::string tn);
	///	DB内の型情報をリンク．
	void Link();
	///	DB内の型情報の表示
	void Print(std::ostream& os) const ;
};

///	FIVVectorのサイズ指定．x にはフィールド名(メンバ変数名)を指定する．
#define VSIZE(x)

///	単純型を登録する．
#define REG_FIELD(type)	RegisterDesc( new FITypeDesc(#type, sizeof(type)) )
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

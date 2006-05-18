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
	そこで，型記述型オブジェクト(FITypeDesc)を使って記述する．
	FITypeDescのオブジェクトを作るためのソースはヘッダファイルを typedesc.exe が
	パースして自動生成する．*/

///	対象の型にアクセスするためのクラス
class FIAccessBase:public UTRefCount{
public:
	virtual ~FIAccessBase(){}
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
class FIAccess:public FIAccessBase{
	virtual void* Create(){ return DBG_NEW T; }
	virtual void Delete(void* ptr){delete (T*)ptr; }
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
	size_t VectorSize(const void* v){
		return ((const std::vector<T>*)v)->size();
	}
	virtual size_t SizeOfVector(){
		return sizeof(std::vector<T>);
	}
};

class FITypeDescDb;
///	型を表す
class SPR_DLL FITypeDesc:public UTRefCount{
public:
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
		UTRef<FITypeDesc> type;
		///	オフセット
		int offset;
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

		Field(): offset(-1), length(1), varType(SINGLE), isReference(false){}
		~Field();
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
		///	フィールドのアドレスを計算．vectorを拡張する．
		void* FITypeDesc::Field::GetAddressEx(void* base, int pos);
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
			type->WriteBool(val, GetAddressEx(base, pos));
		}
		///	typeが数値の単純型の場合に，数値を読み出す関数
		double ReadNumber(const void* base, int pos=0){
			return type->ReadNumber(GetAddress(base, pos));
		}
		///	typeが数値の単純型の場合に，数値を書き込む関数
		void WriteNumber(void* base, double val, int pos = 0){
			type->WriteNumber(val, GetAddressEx(base, pos));
		}
		///	文字列読み出し
		std::string ReadString(const void* base, int pos=0){
			return type->ReadString(GetAddress(base, pos));
		}
		///	文字列書き込み
		void WriteString(void* base, const char* val, int pos=0){
			type->WriteString(val, GetAddressEx(base, pos));
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
	virtual ~FITypeDesc(){}
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
	bool IsComposit(){ return composit.size()!=0; }
	///	組み立て型の要素
	Composit& GetComposit(){ return composit; }
	///	フィールドの型情報のリンク
	void Link(FITypeDescDb* db);
	///
	const IfInfo* GetIfInfo(){ return ifInfo; }

	//	ユーティリティ関数
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
		return access->Create();
	}
	///	オブジェクトの後始末
	void Delete(void* ptr){ access->Delete(ptr); }
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
};
inline bool operator < (const FITypeDesc& d1, const FITypeDesc& d2){
	return d1.GetTypeName().compare(d2.GetTypeName()) < 0;
}

template <class N>
class SPR_DLL FITypeDescNumber:public FITypeDesc{
public:
	FITypeDescNumber(){
		access = DBG_NEW FIAccess<N>;
	}
	FITypeDescNumber(std::string tn): FITypeDesc(tn, sizeof(N)){
		access = DBG_NEW FIAccess<N>;
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
};

template <class N>
class SPR_DLL FITypeDescBool:public FITypeDesc{
public:
	FITypeDescBool(){
		access = DBG_NEW FIAccess<N>;
	}
	FITypeDescBool(std::string tn): FITypeDesc(tn, sizeof(N)){
		access = DBG_NEW FIAccess<N>;
	}
protected:
	///	数値読み出し
	virtual bool ReadBool(const void* ptr){
		return *(const N*)ptr != 0;
	}
	///	数値書き込み
	virtual void WriteBool(bool val, void* ptr){
		*(N*)ptr = (N)val;
	}
};

class SPR_DLL FITypeDescString:public FITypeDesc{
public:
	FITypeDescString(){
		access = DBG_NEW FIAccess<std::string>;
	}
	FITypeDescString(std::string tn): FITypeDesc(tn, sizeof(std::string)){
		access = DBG_NEW FIAccess<std::string>;
	}
protected:
	///	数値読み出し
	virtual std::string ReadString(const void* ptr){
		return *(std::string*)ptr;
	}
	///	数値書き込み
	virtual void WriteString(const char* val, void* ptr){
		*(std::string*)ptr = val;
	}
};

///	型のデータベース
class SPR_DLL FITypeDescDb: public UTRefCount{
public:
	///
	static UTRef<FITypeDescDb> theTypeDescDb;
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
	/**	型名のAliasを登録	*/
	void RegisterAlias(const char* src, const char* dest){
		FITypeDesc* srcDesc = Find(src);
		assert(srcDesc);
		FITypeDesc* destDesc =DBG_NEW FITypeDesc(*srcDesc);
		destDesc->typeName = dest;
		RegisterDesc(destDesc);
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


/**	ファイルからObjectDescを読み出したり，ファイルに書き込んだりするためのデータ．
	ObjectDesc へのポインタ(data) と 型情報 (type) を持つ．
	メモリの管理も行う．	*/
class FINodeData: public UTRefCount{
public:
	FITypeDesc* type;	///<	データの型 
	UTString name;		///<	名前
	void* data;			///<	ロードしたデータ
	bool haveData;		///<	dataをdeleteすべきかどうか．
	FINodeData(FITypeDesc* t=NULL, void* d=NULL);
	~FINodeData();
};
/**	TypeDescのフィールドのイタレータ
	バイナリファイルやXファイルから，ある型のデータを順に読み出していく場合，
	読み出し中のデータがFITypeDescのツリーのどこに対応するかを保持しておく必要がある．
*/
class FIFieldIt{
public:
	/**	フィールドの種類を示すフラグ．
		ほとんどのファイルフォーマットで，整数，実数，文字列で，異なるパーサが必要になる．
		そこで，それらで分類．
		組み立て型は，FITypeDescを参照して読み出すので，F_BLOCKを用意した．
	*/
	enum FieldType{
		F_NONE, F_BOOL, F_INT, F_REAL, F_STR, F_BLOCK
	};
	FITypeDesc* type;						///<	読み出し中のFITypeDesc
//	FITypeDesc::Composit::iterator field;	///<	組み立て型の場合，その中のどのフィールドか
	FITypeDesc::Field* field;	///<	組み立て型の場合，その中のどのフィールドか
	int arrayPos;							///<	配列の場合，読み出し中の添え字
	int arrayLength;						///<	固定長の場合の配列の長さ
	FieldType fieldType;					///<	読み出すフィールドの型

	FIFieldIt(FITypeDesc* d);					///<	コンストラクタ
	bool NextField();						///<	次のフィールドに進む
};
class FIFieldIts:public UTStack<FIFieldIt>{
public:
	///
	void PushType(FITypeDesc* t){
		Push(FIFieldIt(t));
	}
	///	次のフィールドに進む
	bool NextField(){
		if(size()) return back().NextField();
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
		++ back().arrayPos;
		return back().arrayPos < back().arrayLength;
	}
	bool IsArrayRest(){
		if (!size()) return false;
		return back().arrayPos < back().arrayLength;
	}
	bool IsBool(){
		return back().fieldType==FIFieldIt::F_BOOL;
	}
	bool IsNumber(){
		return back().fieldType==FIFieldIt::F_INT || back().fieldType==FIFieldIt::F_REAL;
	}
	bool IsString(){
		return back().fieldType==FIFieldIt::F_STR;
	}
};


///	単純型を登録する．
#define REG_FIELD(type)	RegisterDesc( DBG_NEW FITypeDesc(#type, sizeof(type)) )
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

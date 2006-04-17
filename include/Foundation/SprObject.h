#ifndef SPR_OBJECT_H
#define SPR_OBJECT_H
#include "../SprBase.h"
#include <iosfwd>

namespace Spr{;

struct ObjectIf;

/**	インタフェースの型情報クラスの基本クラス．クラス名や継承関係を持っていて，ICAST()などが利用する．
1つのインタフェースクラスに付き1個のインスタンスができる．	*/
class IfInfo{
public:
//	static int maxId;
//	int id;
	const char* className;
	IfInfo** base;
	IfInfo(const char* cn, IfInfo** b): className(cn), base(b){}
	virtual const char* ClassName() const =0;
	virtual void* GetSprObject(ObjectIf* intf)const =0;
	virtual ObjectIf* GetIf(void* obj)const =0;
	bool Inherit(const char* key) const;
	bool Inherit(const IfInfo* key) const;
protected:
	virtual ~IfInfo() {};
};
///	IfInfoの実装．1クラス1インスタンス
template <class T>
class IfInfoImp: public IfInfo{
public:
	IfInfoImp(const char* cn, IfInfo** b): IfInfo(cn, b){}
	virtual const char* ClassName() const { return className; }
	virtual void* CreateInstance() const{ return 0;}
	virtual void* GetSprObject(ObjectIf* intf)const;
	virtual ObjectIf* GetIf(void* obj)const;
};

///	実行時型情報を持つクラスが持つべきメンバの宣言部．
#define IF_DEF(cls)										\
public:													\
	static IfInfoImp<cls##If> ifInfo;					\
	virtual const IfInfo* GetIfInfo() const {			\
		return &ifInfo;									\
	}													\
	static const IfInfo* GetIfInfoStatic(){				\
		return &ifInfo;									\
	}													\

///	インタフェースクラスのキャスト
#define ICAST(T, p)	UTIcastImp<T>(p)
template <class T, class P> T* UTIcastImp(P p){
	void* obj = p ? p->GetIfInfo()->GetSprObject((ObjectIf*)p) : NULL;
	return (T*)T::GetIfInfoStatic()->GetIf(obj);
}

///	すべてのインタフェースクラスの基本クラス
struct ObjectIf{
	IF_DEF(Object);
	virtual int AddRef()=0;
	virtual int DelRef()=0;
	virtual int RefCount()=0;

	virtual ~ObjectIf(){}
	///	デバッグ用の表示
	virtual void Print(std::ostream& os) const =0;	
	///	オブジェクトの作成
	virtual ObjectIf* CreateObject(const IfInfo* info, const void* desc)=0;
	///	子オブジェクトの数
	virtual size_t NChildObject() const =0;
	///	子オブジェクトの取得
	virtual ObjectIf* GetChildObject(size_t pos) = 0;
	virtual const ObjectIf* GetChildObject(size_t pos) const = 0;
	///	子オブジェクトの追加
	virtual bool AddChildObject(ObjectIf* o)=0;
	///	データの読み出し(参照版 NULLを返すこともある)
	virtual void* GetDescAddress() = 0;
	///	データの読み出し(コピー版 必ず実装されている(はず))
	virtual void GetDesc(void* desc) = 0;
};

///	インタフェースクラスへのポインタの配列
struct ObjectIfs: public UTStack<ObjectIf*>{
};

///	名前を持つオブジェクトのインタフェース
struct NamedObjectIf: public ObjectIf{
	IF_DEF(NamedObject);
	///	名前の取得
	virtual const char* GetName() const =0;
	///	名前の設定
	virtual void SetName(const char* n) =0;
};

struct SceneIf;
///	シーングラフを構成するノードのインタフェース
struct SceneObjectIf: NamedObjectIf{
	IF_DEF(SceneObject);
	///	所属Sceneの取得
	virtual SceneIf* GetScene() =0;
};

}

#endif

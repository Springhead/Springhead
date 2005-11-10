#ifndef BASETYPEINFO_H
#define BASETYPEINFO_H
/**	実行時型情報テンプレートライブラリ
	コンパイラのrttiを使わずマクロとテンプレートで実装してある．
*/
namespace Spr{;

///	実行時型情報
class UTTypeInfo {
public:
	const char* className;
	UTTypeInfo** base;

	UTTypeInfo(const char* cn, UTTypeInfo** b): className(cn), base(b){}
	virtual bool Inherit(const UTTypeInfo* key) const ;
	virtual bool Inherit(const char* str) const ;
	virtual const char* ClassName() const = 0;
	virtual void* CreateInstance() const = 0;
};

///	実行時型情報クラスの実装
template <class T>
class UTTypeInfoImp: public UTTypeInfo{
public:
	UTTypeInfoImp(const char* cn, UTTypeInfo** b): UTTypeInfo(cn, b){}
	virtual void* CreateInstance() const { return new T; }
	virtual const char* ClassName() const { return className; }
};

///	実行時型情報クラスの実装．抽象クラス版
template <class T>
class UTTypeInfoImpAbst: public UTTypeInfo{
public:
	UTTypeInfoImpAbst(const char* cn, UTTypeInfo** b): UTTypeInfo(cn, b){}
	virtual void* CreateInstance() const { return 0; }
	virtual const char* ClassName() const { return className; }
};

///	実行時型情報を持つクラスの基本クラス
class UTTypeInfoBase{
public:
	virtual const UTTypeInfo* GetTypeInfo() const =0;
};

//----------------------------------------------------------------------
//	クラスの宣言(ヘッダ)に書く部分
///	実行時型情報を持つクラスが持つべきメンバの宣言部
#define DEF_UTTYPEINFODEF(cls)							\
public:													\
	static UTTypeInfoImp<cls> typeInfo;					\
	virtual const UTTypeInfo* GetTypeInfo() const {		\
		return &typeInfo;								\
	}													\
	static const UTTypeInfo* GetTypeInfoStatic(){		\
		return &typeInfo;								\
	}													\

///	実行時型情報を持つクラスが持つべきメンバの宣言部．抽象クラス版
#define DEF_UTTYPEINFOABSTDEF(cls)						\
public:													\
	static UTTypeInfoImpAbst<cls> typeInfo;				\
	virtual const UTTypeInfo* GetTypeInfo() const {		\
		return &typeInfo;								\
	}													\
	static const UTTypeInfo* GetTypeInfoStatic(){		\
		return &typeInfo;								\
	}													\


///	実行時型情報を持つクラスが持つべきメンバの実装．
#define DEF_UTTYPEINFO(cls)									\
	UTTypeInfo* cls##_BASE[] = {NULL};						\
	UTTypeInfoImp<cls> cls::typeInfo = UTTypeInfoImp<cls>(#cls, cls##_BASE);

///	実行時型情報を持つクラスが持つべきメンバの実装．1つのクラス継承をする場合
#define DEF_UTTYPEINFO1(cls, base1)							\
	UTTypeInfo* cls##_BASE[] = {&base1::typeInfo, NULL};	\
	UTTypeInfoImp<cls> cls::typeInfo = UTTypeInfoImp<cls>(#cls, cls##_BASE);

///	実行時型情報を持つクラスが持つべきメンバの実装．2つのクラス継承をする場合
#define DEF_UTTYPEINFO2(cls, base1, base2)									\
	UTTypeInfo* cls##_BASE[] = {&base1::typeInfo, &base2::typeInfo, NULL};	\
	UTTypeInfoImp<cls> cls::typeInfo = UTTypeInfoImp<cls>(#cls, cls##_BASE);

///	実行時型情報を持つクラスが持つべきメンバの実装．3つのクラス継承をする場合
#define DEF_UTTYPEINFO3(cls, base1, base2, base3)												\
	UTTypeInfo* cls##_BASE[] = {&base1::typeInfo, &base2::typeInfo, &base3::typeInfo, NULL};	\
	UTTypeInfoImp<cls> cls::typeInfo = UTTypeInfoImp<cls>(#cls, cls##_BASE);

///	実行時型情報を持つクラスが持つべきメンバの実装．抽象クラス版
#define DEF_UTTYPEINFOABST(cls)								\
	UTTypeInfo* cls##_BASE[] = {NULL};						\
	UTTypeInfoImpAbst<cls> cls::typeInfo = UTTypeInfoImpAbst<cls>(#cls, cls##_BASE);

///	実行時型情報を持つクラスが持つべきメンバの実装．抽象クラス版．1つのクラスを継承する場合
#define DEF_UTTYPEINFOABST1(cls, base)						\
	UTTypeInfo* cls##_BASE[] = {&base::typeInfo,NULL};		\
	UTTypeInfoImpAbst<cls> cls::typeInfo = UTTypeInfoImpAbst<cls>(#cls, cls##_BASE);

///	実行時型情報を持つクラスが持つべきメンバの実装．抽象クラス版．2つのクラスを継承する場合
#define DEF_UTTYPEINFOABST2(cls, base1, base2)									\
	UTTypeInfo* cls##_BASE[] = {&base1::typeInfo, &base2::typeInfo, NULL};		\
	UTTypeInfoImpAbst<cls> cls::typeInfo = UTTypeInfoImpAbst<cls>(#cls, cls##_BASE);

///	実行時型情報を持つクラスが持つべきメンバの実装．抽象クラス版．2つのクラスを継承する場合
#define DEF_UTTYPEINFOABST3(cls, base)															\
	UTTypeInfo* cls##_BASE[] = {&base1::typeInfo, &base2::typeInfo, &base3::typeInfo, NULL};	\
	UTTypeInfoImpAbst<cls> cls::typeInfo = UTTypeInfoImpAbst<cls>(#cls, cls##_BASE);

#define GETCLASSNAME(p)		(p->GetTypeInfo()->className)
#define GETCLASSNAMES(T)	(T::GetTypeInfoStatic()->className)

///	ダイナミックキャスト
#define DCAST(T, p)	UTDcastImp<T>(p)
template <class T, class P> T* UTDcastImp(P p){
	if (p && p->GetTypeInfo()->Inherit(T::GetTypeInfoStatic())) return (T*)&*(p);
	return NULL;
}

///	キャストに失敗するとassertするダイナミックキャスト
#define ACAST(T, p)	UTAcastImp<T>(p)
template <class T, class P> T* UTAcastImp(P p){
	if (p && p->GetTypeInfo()->Inherit(T::GetTypeInfoStatic())) return (T*)&*(p);
	UTAcastError((p)->GetTypeInfo()->className);
	return NULL;
}
///	ダイナミックキャスト．型名にポインタ型を使うバージョン
#define DCASTP(TP, p)	UTDcastpImp<TP>(p)
template <class TP, class P> TP UTDcastpImp(P p){
	TP tp=NULL;
	if (p && p->GetTypeInfo()->Inherit(tp->GetTypeInfoStatic())) return (TP)&*(p);
	return NULL;
}
///	キャストに失敗するとassertするダイナミックキャスト．型名にポインタ型を使うバージョン
#define ACASTP(TP, p)	UTAcastpImp<TP>(p)
template <class TP, class P> TP UTAcastpImp(P p){
	TP tp=NULL;
	if (p && p->GetTypeInfo()->Inherit(tp->GetTypeInfoStatic())) return (TP)&*(p);
	UTAcastError((p)->GetTypeInfo()->className);
	return NULL;
}
///	キャストに失敗した場合のassert関数
char* UTAcastError(const char* );

}
#endif

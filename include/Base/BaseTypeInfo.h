/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef BASETYPEINFO_H
#define BASETYPEINFO_H
#include "BaseUtility.h"
/**	@file BaseTypeInfo.h
	実行時型情報テンプレートライブラリ.
	コンパイラのrttiを使わずマクロとテンプレートで実装してある．
*/
namespace Spr{;

///	実行時型情報
class UTTypeInfo: public UTRefCount{
public:
	const char* className;
	const UTTypeInfo** base;

	UTTypeInfo(const char* cn, const UTTypeInfo** b): className(cn), base(b){}
	virtual bool Inherit(const UTTypeInfo* key) const ;
	virtual bool Inherit(const char* str) const ;
	virtual const char* ClassName() const = 0;
	virtual void* CreateInstance() const = 0;
	virtual ~UTTypeInfo() {};
};

///	実行時型情報クラスの実装
template <class T>
class UTTypeInfoImp: public UTTypeInfo{
public:
	UTTypeInfoImp(const char* cn, const UTTypeInfo** b): UTTypeInfo(cn, b){}
	virtual void* CreateInstance() const { return new T; }
	virtual const char* ClassName() const { return className; }
};

///	実行時型情報クラスの実装．抽象クラス版
template <class T>
class UTTypeInfoImpAbst: public UTTypeInfo{
public:
	UTTypeInfoImpAbst(const char* cn, const UTTypeInfo** b): UTTypeInfo(cn, b){}
	virtual void* CreateInstance() const { return 0; }
	virtual const char* ClassName() const { return className; }
};

///	実行時型情報を持つクラスの基本クラス
class UTTypeInfoObjectBase{
public:
	virtual const UTTypeInfo* GetTypeInfo() const =0;
protected:
	virtual ~UTTypeInfoObjectBase() {};
};

//----------------------------------------------------------------------
//	クラスの宣言(ヘッダ)に書く部分
///	実行時型情報を持つクラスが持つべきメンバの宣言部
#define DEF_UTTYPEINFODEF(cls)							\
public:													\
	virtual const UTTypeInfo* GetTypeInfo() const {		\
		return GetTypeInfoStatic();						\
	}													\
	static const UTTypeInfo* GetTypeInfoStatic();		\
	virtual void* /*cls::*/GetBaseAddress(const UTTypeInfo* to) const;	\

///	実行時型情報を持つクラスが持つべきメンバの宣言部．抽象クラス版
#define DEF_UTTYPEINFOABSTDEF(cls)						\
public:													\
	virtual const UTTypeInfo* GetTypeInfo() const {		\
		return GetTypeInfoStatic();						\
	}													\
	static const UTTypeInfo* GetTypeInfoStatic();		\
	virtual void* /*cls::*/GetBaseAddress(const UTTypeInfo* to) const;	\


///	実行時型情報を持つクラスが持つべきメンバの実装．
#define DEF_UTTYPEINFO(cls)									\
	const UTTypeInfo* cls::GetTypeInfoStatic(){				\
		static const UTTypeInfo* base[] = {NULL};			\
		static UTTypeInfoImp<cls> info(#cls, base);			\
		return &info;										\
	}														\
	void* cls::GetBaseAddress(const UTTypeInfo* to) const{		\
		if (to==GetTypeInfoStatic()) return (cls*)this;		\
		return NULL;										\
	}														\

///	実行時型情報を持つクラスが持つべきメンバの実装．1つのクラス継承をする場合
#define DEF_UTTYPEINFO1(cls, base1)										\
	const UTTypeInfo* cls::GetTypeInfoStatic(){							\
		static const UTTypeInfo* base[] = {base1::GetTypeInfoStatic(),NULL};	\
		static UTTypeInfoImp<cls> info(#cls, base);						\
		return &info;													\
	}																	\
	void* cls::GetBaseAddress(const UTTypeInfo* to) const{				\
		if (to==GetTypeInfoStatic()) return (cls*)this;					\
		return base1::GetBaseAddress(to);								\
	}																	\

///	実行時型情報を持つクラスが持つべきメンバの実装．2つのクラス継承をする場合
#define DEF_UTTYPEINFO2(cls, base1, base2)								\
	const UTTypeInfo* cls::GetTypeInfoStatic(){							\
		static const UTTypeInfo* base[] = {								\
			base1::GetTypeInfoStatic(), base2::GetTypeInfoStatic(),NULL};	\
		static UTTypeInfoImp<cls> info(#cls, base);						\
		return &info;													\
	}																	\
	void* cls::GetBaseAddress(const UTTypeInfo* to) const{				\
		if (to==GetTypeInfoStatic()) return (cls*)this;					\
		void* rv = base1::GetBaseAddress(to);							\
		if (!rv) rv = base2::GetBaseAddress(to);						\
		return rv;														\
	}																	\

///	実行時型情報を持つクラスが持つべきメンバの実装．3つのクラス継承をする場合
#define DEF_UTTYPEINFO3(cls, base1, base2, base3)						\
	const UTTypeInfo* cls::GetTypeInfoStatic(){							\
		static const UTTypeInfo* base[] = {								\
			base1::GetTypeInfoStatic(), base2::GetTypeInfoStatic(),		\
			base3::GetTypeInfoStatic(), NULL};							\
		static UTTypeInfoImp<cls> info(#cls, base);						\
		return &info;													\
	}																	\
	void* cls::GetBaseAddress(const UTTypeInfo* to) const{				\
		if (to==GetTypeInfoStatic()) return (cls*)this;					\
		void* rv = base1::GetBaseAddress(to);							\
		if (!rv) rv = base2::GetBaseAddress(to);						\
		if (!rv) rv = base3::GetBaseAddress(to);						\
		return rv;														\
	}																	\


///	実行時型情報を持つクラスが持つべきメンバの実装．抽象クラス版
#define DEF_UTTYPEINFOABST(cls)											\
	const UTTypeInfo* cls::GetTypeInfoStatic(){							\
		static const UTTypeInfo* base[] = { NULL };						\
		static UTTypeInfoImpAbst<cls> info(#cls, base);					\
		return &info;													\
	}																	\
	void* cls::GetBaseAddress(const UTTypeInfo* to) const{				\
		if (to==GetTypeInfoStatic()) return (cls*)this;					\
		return NULL;													\
	}																	\

///	実行時型情報を持つクラスが持つべきメンバの実装．抽象クラス版．1つのクラスを継承する場合
#define DEF_UTTYPEINFOABST1(cls, base1)									\
	const UTTypeInfo* cls::GetTypeInfoStatic(){							\
		static const UTTypeInfo* base[] = { base1::GetTypeInfoStatic(), NULL };	\
		static UTTypeInfoImpAbst<cls> info(#cls, base);					\
		return &info;													\
	}																	\
	void* cls::GetBaseAddress(const UTTypeInfo* to) const{				\
		if (to==GetTypeInfoStatic()) return (cls*)this;					\
		void* rv = base1::GetBaseAddress(to);							\
		return rv;														\
	}																	\

///	実行時型情報を持つクラスが持つべきメンバの実装．抽象クラス版．2つのクラスを継承する場合
#define DEF_UTTYPEINFOABST2(cls, base1, base2)							\
	const UTTypeInfo* cls::GetTypeInfoStatic(){							\
		static const UTTypeInfo* base[] = { base1::GetTypeInfoStatic(), \
			base2::GetTypeInfoStatic(), NULL };							\
		static UTTypeInfoImpAbst<cls> info(#cls, base);					\
		return &info;													\
	}																	\
	void* cls::GetBaseAddress(const UTTypeInfo* to) const{				\
		if (to==GetTypeInfoStatic()) return (cls*)this;					\
		void* rv = base1::GetBaseAddress(to);							\
		if (!rv) rv = base2::GetBaseAddress(to);						\
		return rv;														\
	}																	\

///	実行時型情報を持つクラスが持つべきメンバの実装．抽象クラス版．2つのクラスを継承する場合
#define DEF_UTTYPEINFOABST3(cls, base1, base2, base3)					\
	const UTTypeInfo* cls::GetTypeInfoStatic(){							\
		static const UTTypeInfo* base[] = { base1::GetTypeInfoStatic(), \
			base2::GetTypeInfoStatic(), base3::GetTypeInfoStatic(), NULL };	\
		static UTTypeInfoImpAbst<cls> info(#cls, base);					\
		return &info;													\
	}																	\
	void* cls::GetBaseAddress(const UTTypeInfo* to) const{				\
		if (to==GetTypeInfoStatic()) return (cls*)this;					\
		void* rv = base1::GetBaseAddress(to);							\
		if (!rv) rv = base2::GetBaseAddress(to);						\
		if (!rv) rv = base3::GetBaseAddress(to);						\
		return rv;														\
	}																	\

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

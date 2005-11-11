#ifndef SPR_TYPE_H
#define SPR_TYPE_H
#include <iosfwd>

namespace Spr{;

struct ObjectIf{
	virtual int AddRef()=0;
	virtual int DelRef()=0;
	virtual int RefCount()=0;

	virtual ~ObjectIf(){}
	///	名前の取得
	virtual const char* GetName() const =0;
	///	名前の設定
	virtual void SetName(const char* n) =0;
	///	デバッグ用の表示
	virtual void Print(std::ostream& os) const =0;

/*	
//	以下は本当に必要か要検討
	///	所有しているオブジェクトの数
	virtual size_t NChildObjectIfs(){ return 0; }
	///	所有しているオブジェクト
	virtual ObjectIf* ChildObjectIf(size_t i){ return NULL; }
	///	参照しているオブジェクトの数
	virtual size_t NReferenceObjectIfs(){ return 0; }
	///	参照しているオブジェクト
	virtual ObjectIf* ReferenceObjectIf(size_t i){ return NULL; }
	///	子オブジェクトの追加(所有・参照を問わない)
	virtual bool AddChildObjectIf(ObjectIf* o){ return false; }
	///	子オブジェクトの削除
	virtual bool DelChildObjectIf(ObjectIf* o){ return false; }
	///	子になりえるオブジェクトの型情報の配列
	virtual const UTTypeInfo** ChildCandidates(){ return NULL; }
*/
	
};
}

#endif

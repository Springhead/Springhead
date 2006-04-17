#ifndef FISAVECONTEXT_H
#define FISAVECONTEXT_H

#include <Foundation/Object.h>
#include <FileIO/FITypeDesc.h>
#include <fstream>

namespace Spr{;

class FINodeHandlers;
class FISaveContext{
public:
	UTString fileName;
	std::fstream file;
	///	ノードハンドラ
	FINodeHandlers* handlers;
	///	型DB
	FITypeDescDb* typeDb;
	///	FITypeDescのフィールドへのイタレータのスタック
	FIFieldIts fieldIts;
	///	セーブするディスクリプタのスタック．ネストした組み立て型に備えてスタックになっている．
	UTStack< UTRef<FINodeData> > datas;
	
	///	セーブ中のオブジェクトのスタック
	ObjectIfs objects;
	///
	ObjectIfs rootObjects;
	///
	void Open(const char* fn);
};

}

#endif

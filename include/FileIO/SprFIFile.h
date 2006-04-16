#ifndef SPR_FIFile_H
#define SPR_FIFile_H
#include <Foundation/SprObject.h>

/**	\addtogroup gpFileIO	*/
//@{
namespace Spr{;

///	ファイルローダ・セーバ
struct FIFileIf: public ObjectIf{
	IF_DEF(FIFile);
	///	ロード
	virtual bool Load(ObjectIfs& objs, const char* fn)=0;
	///	セーブ
	virtual bool Save(const ObjectIfs& objs, const char* fn)=0;
};

///	DirectXファイルのファイルローダとセーバ
struct FIFileXIf: public FIFileIf{
	IF_DEF(FIFileX);
	///	ロード
	virtual bool Load(ObjectIfs& objs, const char* fn)=0;
	///	セーブ
	virtual bool Save(const ObjectIfs& objs, const char* fn)=0;
};


}

#endif

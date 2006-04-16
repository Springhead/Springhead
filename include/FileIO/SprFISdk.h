#ifndef SPR_FISDKIF_H
#define SPR_FISDKIF_H
#include <Foundation/SprObject.h>

namespace Spr{;


/** \addtogroup gpFileIO	*/
//@{

struct FIFileXIf;

///	ファイル入出力SDK
struct FISdkIf : public ObjectIf{
	IF_DEF(FISdk);
	virtual FIFileXIf* CreateFileX()=0;
	virtual void Clear()=0;
};
///	ファイル入出力SDKの作成
FISdkIf* SPR_CDECL CreateFISdk();
///	いろいろなSDKの作成
ObjectIf* SPR_CDECL CreateSdk(const IfInfo* info, const void* desc);

//@}

}	//	namespace Spr
#endif

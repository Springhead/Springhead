#ifndef FISdk_H
#define FISdk_H
#include <Foundation/Scene.h>
#include <FileIO/SprFISdk.h>

namespace Spr {;

class FIFile;
class SPR_DLL FISdk:public InheritObject<FISdkIf, Object>{
public:
	OBJECTDEF(FISdk);
	FISdk();
	~FISdk();
	FIFileXIf* CreateFileX();
	void Clear();
public:
	ObjectIf* CreateObject(const IfInfo* info, const void* desc);
	std::vector< UTRef<FIFile> > files;
};

}
#endif

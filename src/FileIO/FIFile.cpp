#include "FileIO.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "FIFile.h"
namespace Spr{;

IF_IMP(FIFile, Object);
OBJECTIMPABST(FIFile, Object);

bool FIFile::Load(ObjectIfs& objs, const char* fn){
	FIFileContext fc;
	fc.objects.insert(fc.objects.end(), objs.begin(), objs.end());
	fc.fileInfo.Push();
	fc.fileInfo.back().Map(fn);
	Load(&fc);
	if (fc.rootObjects.size()){
		objs.insert(objs.end(), fc.rootObjects.begin(), fc.rootObjects.end());
		return true;
	}
	return false;
}
void FIFile::Load(FIFileContext* fc){
	if (fc->IsGood()){
		fc->typeDb = &typeDb;
		fc->handlers = &handlers;
		LoadImp(fc);
	}
	fc->Link();
	fc->PostTask();
}

};

/**
 Springhead2/src/tests/FileIO/FILoadTestX/main.cpp

【概要】
  ・Xファイルをロードし、ロードしたデータを出力する。
  
【終了基準】
  ・プログラムが正常終了したら0を返す。

【処理の流れ】
　・test.xファイルをロードする。
　・ロードした情報を出力する。

 */
#include <FileIO/FITypeDesc.h>
#include <FileIO/FIFileContext.h>
#include <FileIO/FIFileX.h>
#include <Springhead.h>
namespace Spr{
extern void RegisterTypes();
extern UTRef<FITypeDescDb> typeDescDb;
}

int main(){
	using namespace Spr;
	RegisterTypes();
	FIFileContext fc;
	fc.fileInfo.Push();
	fc.fileInfo.back().Map("test.x");
	PHSdkIf* sdk = CreatePHSdk();
	fc.objects.Push(sdk);
	FIFileX fileX;
	fileX.Init(typeDescDb);
	fileX.Load(&fc);
	sdk->Print(DSTR);

//	Spr::typeDescDb->Print(DSTR);
}


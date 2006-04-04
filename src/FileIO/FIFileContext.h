#ifndef FIFILECONTEXT_H
#define FIFILECONTEXT_H

#include <Foundation/Object.h>
#include <string>
#ifdef _WIN32
#include <WinBasis/WinBasis.h>
#else 
#include <sys/stat.h>
#endif


namespace Spr{;
class FITypeDesc;

/**	ファイルロード時に使用するコンテキスト
	ファイルをロードする際は，データをノードごとにロードして，
	オブジェクトを作るためのディスクリプタ構造体(PHSolidDescなど)を
	まずロードする．
	そのあと，オブジェクトを生成する．	*/
class FIFileContext{
public:
	
	//--------------------------------------------------------------------------
	//	クラス定義
	struct FileInfo{
		~FileInfo();
		std::string name;	///<	ファイル名
		int line;			///<	行番号
		const char* start;	///<	メモリマップされたファイルの先頭
		const char* end;	///<	メモリマップされたファイルの終端
#ifdef _WIN32
		HANDLE hFile, hFileMap;		///<	ファイルハンドル、ファイルマッピングオブジェクト
#else 
		//FILE *hFile;
		//char *buffer;
		int fd;					///<	ファイルディスクリプタ
		struct stat filestat;	///<	ファイルサイズを得るのに使う
		void *sourceptr;
#endif
		/// コンストラクタ
		FileInfo():line(0),start(NULL), end(NULL){}
		/// ファイル マッピング		
		bool Map(std::string fn);
		/// ファイル アンマッピング
		void Unmap();
		///	ロードできる状態ならtrue
		bool IsGood();
	};
	///	ロードしたデスクリプタ．Createした場合は消えるときにDeleteする．
	struct Primitive: UTRefCount{
		FITypeDesc* desc;
		void* obj;
		Primitive(FITypeDesc* d=NULL, void* o=NULL);
		~Primitive();
	};

	//--------------------------------------------------------------------------
	//	変数
	///	ロード中のファイルの名前と中身．includeした場合に備えてstackになっている．
	UTStack<FileInfo> fileInfo;	
	///	
	typedef UTStack<ObjectIf*> IfStack;
	///	現在ロード中のオブジェクト
	IfStack objects;
	///	ロードしたディスクリプタのスタック．ブロックのロードのためにスタックになっている．
	UTStack< UTRef<Primitive> > primitives;
	///	エラーメッセージ出力用のストリーム cout とか DSTR を指定する．
	std::ostream* errorStream;

	//---------------------------------------------------------------------------
	//	関数
	///
	FIFileContext():errorStream(NULL){
		errorStream=&DSTR;
	}
	///	オブジェクトの生成
	ObjectIf* Create(const IfInfo* ifInfo, const void* desc);
	///	エラーメッセージの出力．posをファイル名と行番号に変換する．
	void ErrorMessage(const char* pos, const char* msg);
	///	メッセージの作成．posをファイル名と行番号に変換する．
	void Message(const char* pos, const char* msg);
	///	ロードできる状態ならtrue
	bool IsGood();
};


}

#endif	// FIFILECONTEXT_H

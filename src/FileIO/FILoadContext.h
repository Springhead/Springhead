/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FILoadContext_H
#define FILoadContext_H

#include <Foundation/Object.h>
#include <Foundation/UTTypeDesc.h>
#include <string>
#ifdef _WIN32
#include <WinBasis/WinBasis.h>
#else 
#include <sys/stat.h>
#endif


namespace Spr{;

/**	ファイルからObjectDescを読み出したり，ファイルに書き込んだりするためのデータ．
	ObjectDesc へのポインタ(data) と 型情報 (type) を持つ．
	メモリの管理も行う．	*/
class FINodeData: public UTRefCount{
public:
	UTTypeDesc* type;	///<	データの型 
	UTString name;		///<	名前
	void* data;			///<	ロードしたデータ
	bool haveData;		///<	dataをdeleteすべきかどうか．
	FINodeData(UTTypeDesc* t=NULL, void* d=NULL);
	~FINodeData();
};

class FINodeHandlers;
/**	ファイルロード時に使用するコンテキスト
	ファイルをロードする際は，データをノードごとにロードして，
	オブジェクトを作るためのディスクリプタ構造体(PHSolidDescなど)を
	まずロードする．
	そのあと，オブジェクトを生成する．	*/
class FILoadContext{
public:
	//--------------------------------------------------------------------------
	//	クラス定義
	///
	struct FileInfo{
		~FileInfo();
		std::string name;	///<		ファイル名
		const char* start;	///<		メモリマップされたファイルの先頭
		const char* end;	///<		メモリマップされたファイルの終端
		const char* parsingPos;	///<	現在のパース位置
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
		FileInfo():start(NULL), end(NULL){}
		/// ファイル マッピング		
		bool Map(std::string fn);
		/// ファイル アンマッピング
		void Unmap();
		///	ロードできる状態ならtrue
		bool IsGood();
	};
	///	タスククラス．ロード後にまとめて仕事をさせるためのもの．
	class Task:public UTRefCount{
	public:
		virtual ~Task(){}
		virtual void Execute(FILoadContext* ctx)=0;	
	};
	///	タスクリスト
	class Tasks:public std::vector< UTRef<Task> >{
	public:
		void Execute(FILoadContext* ctx);
	};
	///	ノードへの参照を記録しておくクラス．全部ロードできてからリンクする．
	class LinkTask: public Task{
	public:
		std::vector<NameManagerIf*> nameManagers;
		std::string ref;
		ObjectIf* object;
		const char* pos;
		LinkTask(const ObjectIfs& objs, const char* p, ObjectIf* o, std::string r);
		void Execute(FILoadContext* ctx);
	};
	
	//--------------------------------------------------------------------------
	//	変数
	///	ロード中のファイルの名前と中身．ファイルincludeに備えてstackになっている．
	UTStack<FileInfo> fileInfo;	
	///	現在ロード中のオブジェクト．ネストしたオブジェクトに備えてスタックになっている．
	ObjectIfs objects;
	///	スタックに最初に詰まれたオブジェクト＝ファイルの一番外側＝ルートのオブジェクトの記録．
	ObjectIfs rootObjects;
	///	ロードしたディスクリプタのスタック．ネストした組み立て型に備えてスタックになっている．
	UTStack< UTRef<FINodeData> > datas;
	///	ロード中のFITypedescのフィールドの位置．組み立て型のフィールドに備えてスタックになっている．
	UTTypeDescFieldIts fieldIts;
	///	エラーメッセージ出力用のストリーム cout とか DSTR を指定する．
	std::ostream* errorStream;
	///	リファレンスを後でリンクするための記録．
	Tasks links;
	///	ロードとリンクが終わってから処理するタスク
	Tasks postTasks;
	///	型DB
	UTTypeDescDb* typeDb;

	//---------------------------------------------------------------------------
	//	関数
	///
	FILoadContext():errorStream(NULL){
		errorStream=&DSTR;
	}
	///	エラーメッセージの出力．posをファイル名と行番号に変換する．
	void ErrorMessage(const char* pos, const char* msg);
	///	メッセージの作成．posをファイル名と行番号に変換する．
	void Message(const char* pos, const char* msg);
	///	ロードできる状態ならtrue
	bool IsGood();
	///	typeを処理する準備をする(typeをセットし，XXDescを用意する)
	void PushType(UTTypeDesc* type);
	////
	void PopType();
	///
	void WriteString(std::string v);
	///
	void WriteNumber(double d);
	///
	void WriteBool(bool b);
	///
	void AddLink(std::string ref, const char* pos);
	///
	void Link();
	///
	void PostTask();
	///	ノードの作成
	void PushCreateNode(const IfInfo* info, const void* data);
};


}

#endif	// FILoadContext_H

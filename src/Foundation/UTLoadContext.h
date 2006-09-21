/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef UTLoadContext_H
#define UTLoadContext_H

#include <Foundation/Object.h>
#include <Foundation/UTTypeDesc.h>


namespace Spr{;

///	ファイルマップ(今のところファイルのロード専用)
class UTFileMap: public UTRefCount{
public:
	std::string name;	///<		ファイル名
	const char* start;	///<		メモリマップされたファイルの先頭
	const char* end;	///<		メモリマップされたファイルの終端
	const char* curr;	///<		現在の位置
	/// コンストラクタ
	UTFileMap():start(NULL), end(NULL), curr(NULL){}
	///
	/// ファイル マッピング		
	virtual bool Map(const UTString fn)=0;
	/// ファイル アンマッピング
	virtual void Unmap()=0;
	///	ロードできる状態ならtrue
	bool IsGood();
};

/**	ファイルからObjectDescを読み出したり，ファイルに書き込んだりするためのデータ．
	ObjectDesc へのポインタ(data) と 型情報 (type) を持つ．
	メモリの管理も行う．	*/
class UTLoadData: public UTRefCount{
public:
	UTTypeDesc* type;	///<	データの型 
	UTString name;		///<	名前
	void* data;			///<	ロードしたデータ
	bool haveData;		///<	dataをdeleteすべきかどうか．
	UTLoadData(UTTypeDesc* t=NULL, void* d=NULL);
	~UTLoadData();
};

class UTLoadContext;
///	ロード後に処理をさせるために、ロード時に生成され、ロード後に実行される．
class UTLoadTask:public NamedObject{
public:
	OBJECT_DEF_NOIF(UTLoadTask);
	virtual ~UTLoadTask(){}
	virtual void Execute(UTLoadContext* ctx){};
};

///	ロード後の処理を行うためのタスクリスト
class UTLoadTasks:public std::vector< UTRef<UTLoadTask> >{
public:
	void Execute(UTLoadContext* ctx);
};

typedef std::pair<UTRef<ObjectIf>, UTRef<ObjectIf> > UTPairObject;
typedef std::map<UTRef<ObjectIf>, UTRef<ObjectIf> >  UTMapObject;

/**	ファイルロード時に使用するコンテキスト
	ファイルをロードする際は，データをノードごとにロードして，
	オブジェクトを作るためのディスクリプタ構造体(PHSolidDescなど)を
	まずロードする．
	そのあと，オブジェクトを生成する．	*/
class UTLoadContext{
public:	
	//--------------------------------------------------------------------------
	//	変数
	///	ロード中のファイルの名前と中身．ファイルincludeに備えてstackになっている．
	UTStack< UTRef<UTFileMap> > fileMaps;
	///	現在ロード中のオブジェクト．ネストしたオブジェクトに備えてスタックになっている．
	ObjectIfs objects;
	///	スタックに最初に詰まれたオブジェクト＝ファイルの一番外側＝ルートのオブジェクトの記録．
	ObjectIfs rootObjects;
	///	ロードしたディスクリプタのスタック．ネストした組み立て型に備えてスタックになっている．
	UTStack< UTRef<UTLoadData> > datas;
	///	ロード中のFITypedescのフィールドの位置．組み立て型のフィールドに備えてスタックになっている．
	UTTypeDescFieldIts fieldIts;
	///	エラーメッセージ出力用のストリーム cout とか DSTR を指定する．
	std::ostream* errorStream;
	///	リファレンスを後でリンクするための記録．
	UTLoadTasks links;
	///	ロードとリンクが終わってから処理するタスク
	UTLoadTasks postTasks;
	///	型DB
	UTTypeDescDb* typeDb;
	// GRMeshのマップ
	UTMapObject meshMap;	

	//---------------------------------------------------------------------------
	//	関数
	///
	UTLoadContext():errorStream(NULL){
		errorStream=&DSTR;
	}
	///	エラーメッセージの出力．posをファイル名と行番号に変換する．
	void ErrorMessage(UTFileMap* info, const char* pos, const char* msg);
	///	メッセージの作成．posをファイル名と行番号に変換する．
	void Message(UTFileMap* info, const char* pos, const char* msg);
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
	///	ファイルマップを作成してスタック(fileMaps)に積む
	virtual void PushFileMap(const UTString fn)=0;
};

}

#endif	// UTLoadContext_H

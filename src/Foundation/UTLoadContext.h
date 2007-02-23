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
	///	デストラクタ
	virtual ~UTFileMap(){}
	/// ファイル マッピング		
	virtual bool Map(const UTString fn)=0;
	/// ファイル アンマッピング
	virtual void Unmap()=0;
	///	ロードできる状態ならtrue
	bool IsGood();
};

class UTLoadContext;
class UTLoadedData;
class UTLoadedDatas: public std::vector< UTLoadedData* >{
};
class UTLoadedDataRefs: public std::vector< UTRef<UTLoadedData> > {
};
class UTNameManagerForData;
/**	ファイルロードの際にDOMノードのような役割をするノード
	ファイルからObjectDescを読み出したり，ファイルに書き込んだりするためのデータ．
	ObjectDesc へのポインタ(data) と 型情報 (type) を持つ．
	メモリの管理も行う．	*/
class UTLoadedData: public UTRefCount{
	UTString name;					///<	ノード名
public:
	UTRef<UTFileMap> fileInfo;		///<	ファイル情報
	const char* filePos;			///<	ファイル内での位置
	UTLoadedData* parent;			///<	親ノード
	UTLoadedDataRefs children;		///<	子ノード
	UTLoadedDatas linkFrom;			///<	参照元ノード
	UTLoadedDatas linkTo;			///<	参照先ノード
	UTTypeDesc* type;				///<	型
	void* data;						///<	ロードしたディスクリプタ
	bool haveData;					///<	dataを所有するかどうか

	///	typeがNameManagerの派生クラスの場合だけ，Data用NameMangerを持つ
	UTRef<UTNameManagerForData> nameMan;	
	UTLoadedData* man;				///<	このデータの名前を管理するNameMangerを持つデータ
	
	///	このデータから作られたオブジェクトたち
	ObjectIfs loadedObjects;

	UTLoadedData(UTLoadContext* fc, UTTypeDesc* t, void* data=NULL);
	~UTLoadedData();
	void AddLink(UTLoadedData* to);	///<	参照の追加
	void AddChild(UTLoadedData* c);	///<	子ノードのデータの追加
	void SetType(UTTypeDesc* t);
	void SetName(UTString n);
	void SetupNameManager();
	UTString GetName(){ return name; }
	friend class UTNameManagerForData;
	bool operator < (const UTLoadedData& d2) const {
		return name < d2.name;
	}
	UTLoadedData* FindAncestor(UTString tn);
	UTLoadedData* FindDescendant(UTString tn);
	UTLoadedData* FindLinkAncestor(UTString tn);
	UTLoadedData* FindLinkDescendant(UTString tn);
	void EnumLinkAncestor(std::vector<UTLoadedDatas>& res, UTString tn);
	void EnumLinkDescendant(std::vector<UTLoadedDatas>& res, UTString tn);
	void Print(std::ostream& os);
};

///	UTLoadedData 用の名前管理
class UTNameManagerForData: public UTRefCount{
public:

	typedef std::set<UTLoadedData*, UTContentsLess<UTLoadedData*> > DataSet;
	typedef std::map<UTString, UTString> NameMap;
	///@name	名前空間を構成するためのツリー
	//@{
	///
	typedef std::vector<UTNameManagerForData*> NameManagers;
	///	持ち主のデータ
	UTLoadedData* data;
	///	子名前空間
	NameManagers childManagers;
	///	親名前空間
	UTNameManagerForData* parent;
	//@}

	UTLoadedData* FindData(UTString name, UTString cls="");
protected:
	NameMap nameMap;
	DataSet dataSet;
public:
	UTNameManagerForData();

	///	UTLoadedDataを名前表に追加
	bool AddData(UTLoadedData* data);

	/**	名前の変換（名前がぶつかった場合，ノードの名前を変換するが，
		この関数は，元の名前→変換後の名前の変換をする．	
		@param n 元の名前
		@return 変換後の名前
	*/
	UTString MapName(UTString n);

protected:
	UTLoadedData* FindDataFromAncestor(UTString name, UTString cls);
	UTLoadedData* FindDataFromDescendant(UTString name, UTString cls);
	UTLoadedData* FindDataExact(UTString name, UTString cls);
	UTLoadedData* SearchSet(UTString name, UTString cls);
};

class UTLoadContext;
///	ロード後に処理をさせるために、ロード時に生成され、ロード後に実行される．
class UTLoadTask:public NamedObject{
public:
	OBJECTDEF_NOIF(UTLoadTask, NamedObject);
	virtual ~UTLoadTask(){}
	virtual void Execute(UTLoadContext* ctx){};
};

///	ロード後にリンクするためのタスク．普通は使わない．
class UTLinkTask:public UTLoadTask{
public:
	ObjectIf* linkTo;
	ObjectIf* linkFrom;
	UTLinkTask(ObjectIf* from, ObjectIf* to):linkFrom(from), linkTo(to){}
	virtual void Execute(UTLoadContext* ctx){
		linkFrom->AddChildObject(linkTo);
	};
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
	UTStack< UTRef<UTLoadedData> > datas;
	struct LoadedDatas:public std::vector< UTRef<UTLoadedData> >{
		void Print(std::ostream& os);
	};
	///	ロードしたディスクリプタの記録．
	LoadedDatas loadedDatas;
	///	ロード中のFITypedescのフィールドの位置．組み立て型のフィールドに備えてスタックになっている．
	UTTypeDescFieldIts fieldIts;
	///	フラグのスタック
	UTStack<bool> flags;

	///	エラーメッセージ出力用のストリーム cout とか DSTR を指定する．
	std::ostream* errorStream;
	///	データのリンクを後でするための記録．
	UTLoadTasks dataLinks;
	///	リファレンスを後でリンクするための記録．
	UTLoadTasks links;
	///	ロードとリンクが終わってから処理するタスク
	UTLoadTasks postTasks;
	// マップオブジェクト．ノード間で関係性を持たせたい場合に使用．
	UTMapObject mapObj;	
	//	データ用のルートNameManagerのためのオブジェクト
	UTRef<UTLoadedData> rootNameManagerForData;

	//---------------------------------------------------------------------------
	///	コンストラクタ
	UTLoadContext();
	//@name ローダ，ハンドラを書くためのユーティリティ
	//@{
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
	void AddDataLink(std::string ref, const char* pos);	
	//@}
	//@name データからオブジェクトを作るときのユーティリティ
	//@{
	///	データからオブジェクトを作成
	ObjectIf* CreateObject(const IfInfo* info,  const void* data, UTString name="");
	///	
	UTLoadedData* FindAncestor(UTString tn);
	///	
	UTLoadedData* FindDescendant(UTString tn);
	///
	UTLoadedData* FindLinkAncestor(UTString tn);
	///
	UTLoadedData* FindLinkDescendant(UTString tn);

	//@}
	void LinkData();
	///
	void LinkNode();
	///
	void PostTask();
	///	ファイルマップを作成してスタック(fileMaps)に積む
	virtual void PushFileMap(const UTString fn)=0;
protected:
	void LinkNode(UTLoadedData* ld);
};

}

#endif	// UTLoadContext_H

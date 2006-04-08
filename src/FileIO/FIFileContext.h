#ifndef FIFILECONTEXT_H
#define FIFILECONTEXT_H

#include <Foundation/Object.h>
#include <FileIO/FITypeDesc.h>
#include <string>
#ifdef _WIN32
#include <WinBasis/WinBasis.h>
#else 
#include <sys/stat.h>
#endif


namespace Spr{;

class FINodeHandlers;
/**	ファイルロード時に使用するコンテキスト
	ファイルをロードする際は，データをノードごとにロードして，
	オブジェクトを作るためのディスクリプタ構造体(PHSolidDescなど)を
	まずロードする．
	そのあと，オブジェクトを生成する．	*/
class FIFileContext{
public:
	//--------------------------------------------------------------------------
	//	クラス定義
	///
	typedef UTStack<ObjectIf*> IfStack;
	///
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
	/**	ファイルから自動的に読み出したり，ファイルに自動的に書き込んだりする
	データ．メモリの管理も行う．	*/
	struct Data: UTRefCount{
		FITypeDesc* type;	///<	データの型 
		UTString name;		///<	名前
		void* data;			///<	ロードしたデータ
		bool haveData;		///<	dataをdeleteすべきかどうか．
		Data(FITypeDesc* t=NULL, void* d=NULL);
		~Data();
	};
	/**	フィールドの種類を示すフラグ．
		ほとんどのファイルフォーマットで，整数，実数，文字列で，異なるパーサが必要になる．
		そこで，それらで分類．
		組み立て型は，FITypeDescを参照して読み出すので，F_BLOCKを用意した．
	*/
	enum FieldType{
		F_NONE, F_BOOL, F_INT, F_REAL, F_STR, F_BLOCK
	};
	/**	TypeDescのフィールドのイタレータ
		バイナリファイルやXファイルから，ある型のデータを順に読み出していく場合，
		読み出し中のデータがFITypeDescのツリーのどこに対応するかを保持しておく必要がある．
	*/
	struct FieldIt{
		FITypeDesc* type;						///<	読み出し中のFITypeDesc
		FITypeDesc::Composit::iterator field;	///<	組み立て型の場合，その中のどのフィールドか
		int arrayPos;							///<	配列の場合，読み出し中の添え字
		int arrayLength;						///<	固定長の場合の配列の長さ
		FieldType nextField;					///<	読み出すフィールドの型

		FieldIt(FITypeDesc* d);					///<	コンストラクタ
		bool NextField();						///<	次のフィールドに進む
	};
	class FieldItStack:public UTStack<FieldIt>{
	public:
		///
		void PushType(FITypeDesc* t){
			Push(FIFileContext::FieldIt(t));
		}
		///	次のフィールドに進む
		bool NextField(){
			if(size()) return back().NextField();
			return false;
		}
		///	配列中での位置
		int ArrayPos(){
			if(size()) return back().arrayPos;
			return -1;
		}
		///	配列の長さ
		int ArrayLength(){
			if(size()) return back().arrayLength;
			return 0;
		}
		bool IncArrayPos(){
			if(!size()) return false;
			++ back().arrayPos;
			return back().arrayPos < back().arrayLength;
		}
		bool IsArrayRest(){
			if (!size()) return false;
			return back().arrayPos < back().arrayLength;
		}
		bool IsBool(){
			return back().nextField==F_BOOL;
		}
		bool IsNumber(){
			return back().nextField==F_INT || back().nextField==F_REAL;
		}
		bool IsString(){
			return back().nextField==F_STR;
		}
	};
	///	ノードへの参照を記録しておくクラス．全部ロードできてからリンクする．
	struct Link{
		std::vector<NameManagerIf*> nameManagers;
		std::string ref;
		ObjectIf* object;
		const char* pos;
		Link(const IfStack& objs, const char* p, ObjectIf* o, std::string r);
	};
	
	//--------------------------------------------------------------------------
	//	変数
	///	ロード中のファイルの名前と中身．ファイルincludeに備えてstackになっている．
	UTStack<FileInfo> fileInfo;	
	///	現在ロード中のオブジェクト．ネストしたオブジェクトに備えてスタックになっている．
	IfStack objects;
	///	ロードしたディスクリプタのスタック．ネストした組み立て型に備えてスタックになっている．
	UTStack< UTRef<Data> > datas;
	///	ロード中のFITypedescのフィールドの位置．組み立て型のフィールドに備えてスタックになっている．
	FieldItStack fieldIts;
	///	エラーメッセージ出力用のストリーム cout とか DSTR を指定する．
	std::ostream* errorStream;
	///	リファレンスを後でリンクするための記録．
	UTStack<Link> links;
	///	型DB
	FITypeDescDb* typeDb;
	///	ノードハンドラ
	FINodeHandlers* handlers;

	//---------------------------------------------------------------------------
	//	関数
	///
	FIFileContext():errorStream(NULL){
		errorStream=&DSTR;
	}
	///	ノードのロード
	void LoadNode();
	///	ノードのロードの終了
	void EndNode();
	///	ブロック(組み立て型)に入る
	void EnterBlock();
	///	ブロック(組み立て型)から出る
	void LeaveBlock();
	///	オブジェクトの生成
	ObjectIf* Create(const IfInfo* ifInfo, const void* desc);
	///	エラーメッセージの出力．posをファイル名と行番号に変換する．
	void ErrorMessage(const char* pos, const char* msg);
	///	メッセージの作成．posをファイル名と行番号に変換する．
	void Message(const char* pos, const char* msg);
	///	ロードできる状態ならtrue
	bool IsGood();
	///	typeを処理する準備をする(typeをセットし，XXDescを用意する)
	void PushType(FITypeDesc* type);
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
	void DoLink();
};


}

#endif	// FIFILECONTEXT_H

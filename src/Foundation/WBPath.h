#ifndef WINCMP_MISC_CPATH_H
#define WINCMP_MISC_CPATH_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <Base/BaseUtility.h>

namespace Spr {;

class SPR_DLL WBPath{
protected:
	UTString path;
public:
	///	ファイルパスの取得
	UTString Path(){ return path; }
	///	ファイルパスの設定
	void Path(UTString p);
	///	ファイルパスのドライブ名
	UTString Drive();
	///	ファイルパスのドライブ名
	UTString Dir();
	///	ファイルパスのファイル名部
	UTString File();
	///	ファイルパスのファイル名の拡張子を除いた部分を返す．
	UTString Main();
	///	ファイルパスの拡張子を返す．
	UTString Ext();
	///	カレントディレクトリを取得する
	UTString GetCwd();
	///	カレントディレクトリを設定する
	bool SetCwd(UTString cwd);

	///	フルパスを取得する．
	UTString FullPath();
	///	相対パスを取得する．
	UTString RelPath();

	///	ディレクトリをフルパスで
	UTString FullDir();

	///	ファイル名を環境変数 PATH から検索して，ファイルパスを設定．
	bool Search(UTString file);
};

}	//	namespace Spr

#endif


#ifndef SPR_UTPATH_H
#define SPR_UTPATH_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <Base/BaseUtility.h>

namespace Spr {;

class SPR_DLL UTPath{
protected:
	UTString path;
public:
	///	カレントディレクトリを取得する
	static UTString SPR_CDECL GetCwd();
	///	カレントディレクトリを設定する
	static bool SPR_CDECL SetCwd(UTString cwd);
	/// ディレクトリを作成する
	static void SPR_CDECL CreateDir(UTString dirname);

	///	ファイルパスの取得
	UTString Path() const { return path; }
	operator UTString() const { return Path(); }
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

	/**	@brief フルパスを取得する．
		元々絶対パスの場合はそのまま返す．
		相対パスの場合はカレントディレクトリを付加してフルパスにする．
	 */
	UTString FullPath();

	/** @brief 相対パスを取得する．
		元々相対パスの場合はそのまま返す．
		絶対パスの場合はカレントディレクトリとの共通部分を除いて相対パスにする．
	 */
	UTString RelPath();

	///	ディレクトリをフルパスで返す
	UTString FullDir();

	///	ファイル名を環境変数 PATH から検索して，ファイルパスを設定．
	bool Search(UTString file);

	UTPath(){}
	UTPath(UTString filename){ Path(filename); }
};

}	//	namespace Spr

#endif


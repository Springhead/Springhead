/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FWSDKIF_H
#define SPR_FWSDKIF_H

#include <Framework/SprFWScene.h>
#include <Physics/SprPHSdk.h>
#include <Graphics/SprGRSdk.h>
#include <FileIO/SprFISdk.h>

namespace Spr{;

struct FWWin{
	int id;
	UTRef<GRRenderIf> render;
	FWWin(int i=0, GRRenderIf* r=NULL)
		:render(r), id(i){
	}
	UTRef<FWSceneIf> scene;
};

struct FWSdkDesc{
};

struct FWSceneIf;
struct FWSdkIf : ObjectIf {
	IF_DEF(FWSdk);

	/** @brief SDKオブジェクトを作成する
	 */
	static FWSdkIf* SPR_CDECL CreateSdk();

	/** @brief 空のシーンを作成する
		空のシーンを作成し、シーンリストに追加する．
		作成されたシーンはアクティブとなる．
	 */
	virtual FWSceneIf* CreateScene(const PHSceneDesc& phdesc = PHSceneDesc(), const GRSceneDesc& grdesc = GRSceneDesc())=0;

	/** @brief シーンをファイルからロードする
		@param filename ファイル名
		指定されたファイルからシーンをロードし、シーンリストに追加する．
		ロード後は，ロードされたシーンがアクティブとなる．
		ファイル中に複数のシーンが含まれていた場合は，最後にロードされたシーンがアクティブとなる．
	 */
	virtual bool LoadScene(UTString filename)=0;

	/** @brief シーンをファイルへセーブする
		@param filename ファイル名
	 */
	virtual bool SaveScene(UTString filename)=0;

	/// シーンの個数を取得する
	virtual int NScene() const=0;

	/** @brief シーンを切り替える
		@param index
		index番目のシーンをアクティブにする
	 */
	virtual void	SwitchScene(FWSceneIf* scene)=0;

	/** @brief シーンを取得する
		@param index 取得するシーンのインデックス．省略するとアクティブシーンを取得する．
	 */
	virtual FWSceneIf* GetScene(int index = -1)=0;

	/** @brief シーンを混ぜる
		scene1に含まれる全ての要素をscene0に移動し，scene1を削除する．
	 */
	virtual void	MergeScene(FWSceneIf* scene0, FWSceneIf* scene1) = 0;
	
	/// PHSdkオブジェクトを取得する
	virtual PHSdkIf* GetPHSdk()=0;

	/// GRSdkオブジェクトを取得する
	virtual GRSdkIf* GetGRSdk()=0;

	/// FISdkオブジェクトを取得する
	virtual FISdkIf* GetFISdk()=0;

	/// ウィンドウに対応するコンテキストを作る
	virtual FWWin* CreateWin(int wid, GRRenderIf* r)=0;

	/** @brief デバッグ描画モードの取得
	 */
	virtual bool GetDebugMode() = 0;

	/** @brief デバッグ描画モードの設定
	 */
	virtual void SetDebugMode(bool debug = true) = 0;

	/** @brief シミュレーションの実行
	 */
	virtual void Step()=0;

	/** @brief 描画を実行
	 */
	virtual void Draw(FWWin* cur) = 0;

	/** @brief 描画領域のサイズを設定する
		@param w 描画領域の横幅
		@param h 描画領域の縦幅
	 */
	virtual void Reshape(FWWin* cur, int w, int h)=0;

	virtual FWWin* GetWinFromId(int wid)=0;
	virtual int NWin()=0;
	virtual FWWin* GetWin(int pos)=0;

	static void SPR_CDECL RegisterSdk();
};
inline bool operator < (const FWWin& a, const FWWin& b){
	return a.id < b.id;
}

}

#endif

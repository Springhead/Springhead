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

struct FWSdkDesc{
};

struct FWSceneIf;
struct FWSdkIf : SdkIf {
	SPR_IFDEF(FWSdk);

	/** @brief SDKオブジェクトを作成する
	 */
	static FWSdkIf* SPR_CDECL CreateSdk();

	/** @brief 空のシーンを作成する
		空のシーンを作成し、シーンリストに追加する．
		作成されたシーンはアクティブとなる．
	 */
	FWSceneIf* CreateScene(const PHSceneDesc& phdesc = PHSceneDesc(), const GRSceneDesc& grdesc = GRSceneDesc());

	/** @brief シーンをファイルからロードする
		@param filename ファイル名
		指定されたファイルからシーンをロードし、シーンリストに追加する．
		ロード後は，ロードされたシーンがアクティブとなる．
		ファイル中に複数のシーンが含まれていた場合は，最後にロードされたシーンがアクティブとなる．
	 */
	bool LoadScene(UTString filename);

	/** @brief シーンをファイルへセーブする
		@param filename ファイル名
	 */
	bool SaveScene(UTString filename);

	/// シーンの個数を取得する
	int NScene() const;

	/** @brief シーンを切り替える
		@param scene
		シーンsceneをアクティブにする
	 */
	void	SwitchScene(FWSceneIf* scene);

	/** @brief シーンを取得する
		@param index 取得するシーンのインデックス．省略するとアクティブシーンを取得する．
	 */
	FWSceneIf* GetScene(int index = -1);
	/** @brief シーンを混ぜる
		scene1に含まれる全ての要素をscene0に移動し，scene1を削除する．
	 */
	void	MergeScene(FWSceneIf* scene0, FWSceneIf* scene1);

	/** @brief レンダラを作成する
		レンダラを作成し，レンダラリストに追加する．
		
		＊引数でDeviceをGLかD3Dかを指定できるようにするべき
	 */
	GRRenderIf*	CreateRender();
	/** @brief レンダラの個数を取得する
	 */
	int NRender() const;
	/** @brief レンダラを取得する
	 */
	GRRenderIf* GetRender(int index = -1);
	/** @brief レンダラをアクティブにする
	 */
	void SwitchRender(GRRenderIf* render);
	
	/// PHSdkオブジェクトを取得する
	PHSdkIf* GetPHSdk();

	/// GRSdkオブジェクトを取得する
	GRSdkIf* GetGRSdk();

	/// FISdkオブジェクトを取得する
	FISdkIf* GetFISdk();

	/** @brief デバッグ描画モードの取得
	 */
	bool GetDebugMode();

	/** @brief デバッグ描画モードの設定
	 */
	void SetDebugMode(bool debug = true);

	/** @brief シミュレーションの実行
	 */
	void Step();

	/** @brief 描画を実行
		アクティブなシーンとアクティブなレンダラを使って描画を行う．
	 */
	void Draw();

	/** @brief 描画領域のサイズを設定する
		@param w 描画領域の横幅
		@param h 描画領域の縦幅
		アクティブなレンダラの描画領域を設定する
	 */
	void Reshape(int w, int h);

	static void SPR_CDECL RegisterSdk();
};

}

#endif

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FWSDKIF_H
#define SPR_FWSDKIF_H

#include <Physics/SprPHScene.h>
#include <Graphics/SprGRScene.h>
#include <Framework/SprFWInteractScene.h>

namespace Spr{;

struct ImportIf;
struct FWSceneIf;
struct FISdkIf;
struct HISdkIf;

struct FWSdkDesc{};

/** @brief Framework SDK
	FWSdk -
		PHSdk
		GRSdk
		FISdk
		HISdk
		FWScene[]
		GRRender[]
 */
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
		@param ii		ファイルの種類を示すIfInfo
		@param objs		親オブジェクトの配列
		指定されたファイルからシーンをロードし、シーンリストに追加する．
		ロード後は，ロードされたシーンがアクティブとなる．
		ファイル中に複数のシーンが含まれていた場合は，最後にロードされたシーンがアクティブとなる．

		iiがNULLの場合は拡張子からファイルタイプが判定される．
		ファイルタイプを明示したい場合はiiに対応するIfInfoを指定する(FIFileXIf::GetIfInfoStatic()など)
		
		親オブジェクトを指定したい場合は，objsに設定する．
		objsがNULLの場合，シーンはFWSdkの子オブジェクトとしてロードされる．

		内部でインポートされるファイルのタイプは必ず拡張子より判定される．
	 */
	bool LoadScene(UTString filename, ImportIf* ex=NULL, const IfInfo* ii = NULL, ObjectIfs* objs = NULL);

	/** @brief シーンをファイルへセーブする
		@param filename ファイル名
		@param ex		エクスポート情報
		@param ii		ファイルの種類を示すIfInfo
		@param objs		保存するオブジェクトの配列
		ファイルの種類の指定についてはLoadSceneを参照．

		保存するオブジェクトを指定したい場合は，それらをobjsに設定する．
		objsがNULLの場合は全てのFWSceneが保存される．

		部分的に別ファイルに保存したい場合はエクスポート情報をexに指定する．
		exがNULLの場合は全てが1つのファイルに保存される．
	 */
	bool SaveScene(UTString filename, ImportIf* ex = NULL, const IfInfo* ii = NULL, ObjectIfs* objs = NULL);

	/// シーンの個数を取得する
	int NScene() const;

	/** @brief シーンを切り替える
		@param scene
		シーンsceneをアクティブにする
	 */
	//void	SwitchScene(FWSceneIf* scene);

	/** @brief シーンを取得する
		@param index 取得するシーンのインデックス
	 */
	FWSceneIf* GetScene(int index);

	/** @brief シーンを混ぜる
		scene1に含まれる全ての要素をscene0に移動し，scene1を削除する．
	 */
	void	MergeScene(FWSceneIf* scene0, FWSceneIf* scene1);

	/// インタラクションシーンを作成する
	FWInteractSceneIf*	CreateIAScene(const FWInteractSceneDesc& desc);
	/// インタラクションシーンを取得する
	FWInteractSceneIf*	GetIAScene(int index);
	/// インタラクションシーンの数
	int		NIAScenes();
	/// インタラクションシーンをクリアする
	void	ClearIAScenes();

	/** @brief レンダラを作成する
		レンダラを作成し，レンダラリストに追加する．
		
		＊引数でDeviceをGLかD3Dかを指定できるようにするべき
	 */
	//GRRenderIf*	CreateRender();

	/** @brief レンダラの個数を取得する */
	//int NRender() const;
	
	/** @brief レンダラを取得する */
	//GRRenderIf* GetRender(int index = -1);
	
	/** @brief レンダラをアクティブにする */
	//void SwitchRender(GRRenderIf* render);

	
	/// Physics SDK(PHSdk)オブジェクトを取得する
	PHSdkIf* GetPHSdk();

	/// Graphics SDK(GRSdk)オブジェクトを取得する
	GRSdkIf* GetGRSdk();

	/// FileIO SDK(FISdk)オブジェクトを取得する
	FISdkIf* GetFISdk();

	/// HumanInterface SDK(HISdk)オブジェクトを取得する
	HISdkIf* GetHISdk();

	/** @brief デバッグ描画モードの取得
	 */
	//bool GetDebugMode();

	/** @brief デバッグ描画モードの設定
	 */
	//void SetDebugMode(bool debug = true);

	/** @brief シミュレーションの実行
	 */
	//void Step();

	/** @brief 描画を実行
		アクティブなシーンとアクティブなレンダラを使って描画を行う．

		デバッグ描画モードではGraphicsのシーングラフ情報を使わずに，
		Physicsシーンの剛体が持つ衝突判定用形状データや，拘束力などの描画が行われる．
		
		通常描画モードではまずSync関数によりPhysicsシーンの状態がGraphicsシーングラフに反映された後，
		シーングラフの形状データやマテリアル、テクスチャを利用して描画が行われる。
	 */
	//void Draw();

	/** @brief 描画領域のサイズを設定する
		@param w 描画領域の横幅
		@param h 描画領域の縦幅
		アクティブなレンダラの描画領域を設定する
	 */
	//void Reshape(int w, int h);

	/** @brief DSTR出力のデバッグ情報の表示の可否を設定
	 */
	//void SetDSTR(bool f);

	static void SPR_CDECL RegisterSdk();
};

}

#endif

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file GRDeviceD3D.h
 *	@brief Direct3Dによるグラフィックス描画の実装　　
 */
#ifndef GRDEVICED3D_H
#define GRDEVICED3D_H

#include "GraphicsD3D.h"
#include <d3d9.h>
#include <atlcomcli.h>

namespace Spr{;

/**	Direct3Dによるグラフィックス描画の実装	*/
class GRDeviceD3D: public GRDevice, public GRDeviceD3DIfInit{
	SPR_OBJECTDEF(GRDeviceD3D, GRDevice);
protected:
	CComPtr<IDirect3D9>			direct3D;				///< Direct3D
	CComPtr<IDirect3DDevice9>	d3ddevice;				///< Direct3Dデバイス
	D3DPRESENT_PARAMETERS		pp;						///< プレゼンテーションパラメータ
	std::stack<Affinef>			modelMatrixStack;		///< モデル行列スタック
	int							nLights;				///< 光源の数
	const GRVertexElement*		currentVertexFormat;	///< 頂点フォーマット
	std::vector<CComPtr<IDirect3DTexture9> >	textures;			///< テクスチャ

	struct DisplayList{
		DWORD									fvf;		///< 頂点フォーマット
		CComPtr<IDirect3DVertexDeclaration9>	decl;		///< 頂点フォーマット
		size_t									stride;		///< 頂点サイズ
		CComPtr<IDirect3DVertexBuffer9>			vtxBuf;		///< 頂点バッファ
		int										vtxCount;	///< 頂点数
		CComPtr<IDirect3DIndexBuffer9>			idxBuf;		///< インデックスバッファ
		D3DPRIMITIVETYPE						primType;	///< プリミティブタイプ
		int										primCount;	///< プリミティブ数
		GRMaterialIf*							mat;		///< マテリアル
		int										texid;		///< テクスチャID
	};
	std::vector<DisplayList> displayLists;				///< ディスプレイリスト

	GRDeviceD3DConfirmDeviceFunc confirmDeviceFunc;

	bool deviceLost;		///< デバイスはロストしているか？
	std::vector<GRDeviceD3DListenerFunc> lostDeviceListeners;
	std::vector<GRDeviceD3DListenerFunc> resetDeviceListeners;

	D3DVIEWPORT9	viewportStore;
	RECT			windowRectStore;
	bool			shouldResetWindowRect;

	static IDirect3DDevice9* s_d3ddevice;

public:
	///	コンストラクタ
	GRDeviceD3D() : confirmDeviceFunc(NULL) {}
	/// 初期設定
	virtual void Init();
	///	Viewport設定
	virtual void SetViewport(Vec2f pos, Vec2f sz);
	///	バッファクリア
	virtual void ClearBuffer();
	///	レンダリングの開始前に呼ぶ関数
	virtual void BeginScene();
	///	レンダリングの終了後に呼ぶ関数
	virtual void EndScene();
	///	カレントの視点行列をafvで置き換える
	virtual void SetViewMatrix(const Affinef& afv);
	///	カレントの投影行列を取得する
	virtual void SetProjectionMatrix(const Affinef& afp);
	///	カレントの投影行列をafpで置き換える
	virtual void GetProjectionMatrix(const Affinef& afp);
	///	カレントのモデル行列をafwで置き換える
	virtual void SetModelMatrix(const Affinef& afw);
	///	カレントのモデル行列に対してafwを掛ける
	virtual void MultModelMatrix(const Affinef& afw);
	///	カレントのモデル行列をモデル行列スタックへ保存する
	virtual void PushModelMatrix();
	///	モデル行列スタックから取り出し、カレントのモデル行列とする
	virtual void PopModelMatrix();
	/// ブレンド変換行列の全要素を削除する
	virtual void ClearBlendMatrix();
	/// ブレンド変換行列を設定する
	virtual bool SetBlendMatrix(const Affinef& afb);
	/// ブレンド変換行列を設定する
	virtual bool SetBlendMatrix(const Affinef& afb, unsigned int id);
	///	頂点フォーマットの指定
	virtual void SetVertexFormat(const GRVertexElement* e);
	///	頂点シェーダーの指定
	virtual void SetVertexShader(void* s);
	///	頂点座標を指定してプリミティブを描画
	virtual void DrawDirect(GRRenderBaseIf::TPrimitiveType ty, void* begin, size_t count, size_t stride=0);
	///	頂点座標とインデックスを指定してプリミティブを描画
	virtual void DrawIndexed(GRRenderBaseIf::TPrimitiveType ty, GLuint* idx, void* vtx, size_t count, size_t stride=0);
	///	DisplayListの表示
	virtual void DrawList(int i);
	///	DisplayListの解放
	virtual void ReleaseList(int i);
	/// 3次元テキストの描画（GLオンリー版でfontは指定なし）
	virtual void DrawFont(Vec2f pos, const std::string str);
	/// 3次元テキストの描画（GLオンリー版でfontは指定なし）
	virtual void DrawFont(Vec3f pos, const std::string str);
	///	3次元テキストの描画
	virtual void DrawFont(Vec2f pos, const std::string str, const GRFont& font);
	///	3次元テキストの描画
	virtual void DrawFont(Vec3f pos, const std::string str, const GRFont& font);
	///	描画の材質の設定
	virtual void SetMaterial(const GRMaterialDesc& mat);
	virtual void SetMaterial(const GRMaterialIf*& mat){ GRDevice::SetMaterial(mat); }
	///	描画する点・線の太さの設定
	virtual void SetLineWidth(float w);
	///	光源スタックをPush
	virtual void PushLight(const GRLightDesc& light);
	virtual void PushLight(const GRLightIf* light){ GRDevice::PushLight(light); }
	///	光源スタックをPop
	virtual void PopLight();
	///	デプスバッファへの書き込みを許可/禁止する
	virtual void SetDepthWrite(bool b);
	///	デプステストを有効/無効にする
	virtual void SetDepthTest(bool b);
	///	デプスバッファ法に用いる判定条件を指定する
	virtual void SetDepthFunc(GRRenderBaseIf::TDepthFunc f);
	/// アルファブレンディングを有効/無効にする
	virtual void SetAlphaTest(bool b);
	///	アルファブレンディングのモード設定(SRCの混合係数, DEST混合係数)
	virtual void SetAlphaMode(GRRenderBaseIf::TBlendFunc src, GRRenderBaseIf::TBlendFunc dest);
	/// テクスチャのロード（戻り値：テクスチャID）
	virtual unsigned int LoadTexture(const std::string filename);
	/// シェーダの初期化
	virtual void InitShader();
	/// シェーダフォーマットの設定
	virtual void SetShaderFormat(GRShaderFormat::ShaderType type);	
	/// シェーダオブジェクトの作成
	virtual bool CreateShader(std::string vShaderFile, std::string fShaderFile, GRHandler& shader);
	/// シェーダオブジェクトの作成、GRDeviceGL::shaderへの登録（あらかじめShaderFile名を登録しておく必要がある）
	virtual GRHandler CreateShader();
	/// シェーダのソースプログラムをメモリに読み込み、シェーダオブジェクトと関連付ける
	virtual bool ReadShaderSource(GRHandler shader, std::string file);	
	/// ロケーション情報の取得（SetShaderFormat()でシェーダフォーマットを設定しておく必要あり）
	virtual void GetShaderLocation(GRHandler shader, void* location);		
	/// デバイス選定用のコールバック関数を登録
	virtual void SetConfirmDeviceFunc(GRDeviceD3DConfirmDeviceFunc func) { confirmDeviceFunc = func; }
	/// デバイスがロストしたときに呼ばれるコールバック関数を登録
	virtual void AddLostDeviceListener(GRDeviceD3DListenerFunc func)  { lostDeviceListeners.push_back(func); }
	/// デバイスがリセットされたときに呼ばれるコールバック関数を登録
	virtual void AddResetDeviceListener(GRDeviceD3DListenerFunc func) { resetDeviceListeners.push_back(func); }
	/// ウインドウモード<->フルスクリーン 切り替え
	virtual void ToggleFullScreen();
	/// フルスクリーンモードか？
	virtual bool IsFullScreen() { return !pp.Windowed; }

	/// D3Dデバイスを取得する
	static IDirect3DDevice9* GetD3DDevice() { return s_d3ddevice; }

protected:
	/// アンチエイリアスを使用するための設定
	void SetMultiSampleType();
	/// デバイスがロストしたら呼ぶ
	void LostDevice();
	/// デバイスをリセットする
	void ResetDevice();
};

}
#endif

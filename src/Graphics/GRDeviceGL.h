/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file GRDeviceGL.h
 *	@brief OpenGLによるグラフィックス描画の実装　　
 */
#ifndef GRDEVICEGL_H
#define GRDEVICEGL_H

#include <map>
#include <stack>
#include <SprGraphics.h>

namespace Spr{;

/**	@class	GRDeviceGL
    @brief	OpenGLによるグラフィックス描画の実装　 */
class GRDeviceGL: public GRDevice{
	SPR_OBJECTDEF(GRDeviceGL);
protected:
	int		nLights;					///< 光源の数
	int		vertexFormatGl;				///< glInterleavedArraysで使う，GLの頂点フォーマットID
	size_t	vertexSize;					///< 頂点のサイズ
	bool	vertexColor;				///< 頂点が色を持つかどうか
	GRMaterialDesc currentMaterial;		///< 現在のマテリアル
	
	/**
	 *	@name	マトリックス変数
	 *　　 GLではModelを変えずにViewだけを変えるということができない。 \n
	 *　　 視点を動かすには、ユーザがModelを覚えておく必要がある。	\n
	 *　　 （Direct3Dの場合は、Modelを書き換えずにViewだけ書き換え、視点を動かすことができる）
	 *	@{ 
	 */
	Affinef								viewMatrix;				///< カレント視点行列
	Affinef								modelMatrix;			///< カレントモデル行列 
	std::stack<Affinef>                	modelMatrixStack;		///< モデル行列スタック
	std::vector<Affinef>				blendMatrix;			///< ブレンド変換行列
	/** @} */

	/**
	 *	@name	テクスチャマネージャ
	 *　　 ロードされたテクスチャのIDを覚えておくmap
	 *
	 */
	typedef std::map<std::string, unsigned int, UTStringLess> GRTexnameMap;
	GRTexnameMap texnameMap;

	/**
	 *	@name	フォント変数
	 *　　 新規に指定されたフォントはfontListに格納される。
	 *	@{
	 */
	std::map<unsigned int, GRFont> fontList;		///< フォントリスト<DisplayListのindex, font>    
	unsigned int	fontBase;						///< ディスプレイリストのindex numberの基底数 
	GRFont			font;							///< フォント情報
	/** @} */	

	/**
	 *	@name	シェーダ変数
	 *	@{
	 */
	std::string vertexShaderFile;						///< VertexShader ファイル名
	std::string fragmentShaderFile;						///< FragmentShader ファイル名
	GRShaderFormat::ShaderType shaderType;				///< シェーダのロケーションタイプ
	/** @} */	

public:
	///	コンストラクタ
	GRDeviceGL(){}
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
	virtual void GetViewMatrix(Affinef& afv);
	///	カレントの投影行列を取得する
	virtual void SetProjectionMatrix(const Affinef& afp);
	///	カレントの投影行列をafpで置き換える
	virtual void GetProjectionMatrix(Affinef& afp);
	///	カレントのモデル行列をafwで置き換える
	virtual void SetModelMatrix(const Affinef& afw);
	virtual void GetModelMatrix(Affinef& afw);
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
	virtual void DrawIndexed(GRRenderBaseIf::TPrimitiveType ty, size_t* idx, void* vtx, size_t count, size_t stride=0);
	///	球体の描画
	void DrawSphere(float radius, int slices, int stacks, bool solid=true);
	/// 円錐の描画
	void DrawCone(float radius, float height, int slice, bool solid=true);
	/// 円筒の描画
	void DrawCylinder(float radius, float height, int slice, bool solid=true);
	///	DiplayList の作成(記録開始)
	virtual int StartList();
	///	DiplayList の作成の終了(記録終了)
	virtual void EndList();
	///	DisplayListの表示
	virtual void DrawList(int i);
	///	DisplayListの解放
	virtual void ReleaseList(int i);
	virtual void SetFont(const GRFont& font);
	/// 3次元テキストの描画（GLオンリー版でfontは指定なし）
	virtual void DrawFont(Vec2f pos, const std::string str);
	/// 3次元テキストの描画（GLオンリー版でfontは指定なし）
	virtual void DrawFont(Vec3f pos, const std::string str);
	///	描画の材質の設定
	virtual void SetMaterial(const GRMaterialDesc& mat);
	virtual void SetMaterial(const GRMaterialIf* mat);
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
	void SetLighting(bool on);
	/// テクスチャのロード（戻り値：テクスチャID）
	virtual unsigned int LoadTexture(const std::string filename);
	/// テクスチャ画像の設定
	virtual void SetTextureImage(const std::string id, int components, int xsize, int ysize, int format, char* tb);
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
};

}
#endif

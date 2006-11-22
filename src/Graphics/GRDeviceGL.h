/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
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
class GRDeviceGL: public InheritGRDevice<GRDeviceGLIf, GRDevice>{
	OBJECT_DEF(GRDeviceGL);
protected:
	int		window;					///<	ウィンドウID
	int		vertexFormatGl;			///<	glInterleavedArraysで使う，頂点フォーマットID
	size_t	vertexSize;				///<	頂点のサイズ
	bool	vertexColor;			///<	頂点が色を持つかどうか
	GRMaterialDesc currentMaterial;	///<	現在のマテリアル
	/**
	 *	@name	マトリックス変数
	 *　　 GLではModelを変えずにViewだけを変えるということができない。 \n
	 *　　 視点を動かすには、ユーザがModelを覚えておく必要がある。	\n
	 *　　 （Direct3Dの場合は、Modelを書き換えずにViewだけ書き換え、視点を動かすことができる）
	 *	@{ 
	 */
	Affinef								viewMatrix;				///< カレント視点行列
	Affinef								projectionMatrix;		///< カレント射影行列 
	Affinef								modelMatrix;			///< カレントモデル行列 
	std::stack<Affinef>                	modelMatrixStack;		///< モデル行列スタック
	std::vector< std::vector<Affinef> > modelMatrices;			///< 複数モデル行列（使用用途：modelMatrices[N][0]はモデル行列、modelMatrices[N][1..]はブレンド行列）
	/** @} */
	/**
	 *	@name	フォント変数
	 *　　 新規に指定されたフォントはfontListに格納される。
	 *	@{
	 */
	std::map<unsigned int, GRFont> fontList;		///<	フォントリスト<DisplayListのindex, font>    
	unsigned int	fontBase;						///<	ディスプレイリストのindex numberの基底数 
	/** @} */	
	int				nLights;		///<	光源の数
	
public:
	///	コンストラクタ
	GRDeviceGL(int w=0):window(w){}
	///	ウィンドウIDの設定
	virtual void SetWindow(int w){window=w;}
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
	///	カレントの投影行列をafpで置き換える
	virtual void SetProjectionMatrix(const Affinef& afp);
	///	カレントのモデル行列をafwで置き換える
	virtual void SetModelMatrix(const Affinef& afw);
	///	カレントのモデル行列に対してafwを掛ける
	virtual void MultModelMatrix(const Affinef& afw);
	///	カレントのモデル行列をモデル行列スタックへ保存する
	virtual void PushModelMatrix();
	///	モデル行列スタックから取り出し、カレントのモデル行列とする
	virtual void PopModelMatrix();
	/// 複数モデル行列の modelMatrices[matrixId][0] をカレントのモデル行列として置き換える
	virtual bool SetModelMatrices(const Affinef& afw, unsigned int matrixId, unsigned int elementId);
	/// 複数モデル行列において、モデル行列afwを掛ける（modelMatrices[matrixId][0] *= afw;）
	virtual bool MultModelMatrices(const Affinef& afw, unsigned int matrixId);
	/// 複数モデル行列の modelMatrices[matrixId] にafwを追加する
	virtual bool PushModelMatrices(const Affinef& afw, unsigned int matrixId);
	/// 複数モデル行列の modelMatrices[matrixId] の最後尾の要素を削除する
	virtual bool PopModelMatrices(unsigned int matrixId);	
	///	頂点フォーマットの指定
	virtual void SetVertexFormat(const GRVertexElement* e);
	///	頂点シェーダーの指定
	virtual void SetVertexShader(void* s);
	///	頂点座標を指定してプリミティブを描画
	virtual void DrawDirect(TPrimitiveType ty, void* begin, size_t count, size_t stride=0);
	///	頂点座標とインデックスを指定してプリミティブを描画
	virtual void DrawIndexed(TPrimitiveType ty, size_t* idx, void* vtx, size_t count, size_t stride=0);
	///	ダイレクト形式による DiplayList の作成
	virtual int CreateList(TPrimitiveType ty, void* vtx, size_t count, size_t stride=0);
	///	ダイレクト形式による DiplayList の作成（マテリアル、テクスチャの設定も行う）
	virtual int CreateList(GRMaterialIf* mat, unsigned int texid, 
						   TPrimitiveType ty, void* vtx, size_t count, size_t stride=0);
	/// 球オブジェクトの DisplayList の作成
	virtual int CreateList(float radius, int slices, int stacks);
	/// 球オブジェクトの DisplayList の作成（マテリアル、テクスチャの設定も行う）
	virtual int CreateList(GRMaterialIf* mat,  float radius, int slices, int stacks);
	///	インデックス形式によるDiplayListの作成
	virtual int CreateIndexedList(TPrimitiveType ty, size_t* idx, void* vtx, size_t count, size_t stride=0);
	///	インデックス形式による DiplayList の作成（マテリアル、テクスチャの設定も行う）
	virtual int CreateIndexedList(GRMaterialIf* mat, 
								  TPrimitiveType ty, size_t* idx, void* vtx, size_t count, size_t stride=0);
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
	///	描画する点・線の太さの設定
	virtual void SetLineWidth(float w);
	///	光源スタックをPush
	virtual void PushLight(const GRLightDesc& light);
	///	光源スタックをPop
	virtual void PopLight();
	///	デプスバッファへの書き込みを許可/禁止する
	virtual void SetDepthWrite(bool b);
	///	デプステストを有効/無効にする
	virtual void SetDepthTest(bool b);
	///	デプスバッファ法に用いる判定条件を指定する
	virtual void SetDepthFunc(TDepthFunc f);
	/// アルファブレンディングを有効/無効にする
	virtual void SetAlphaTest(bool b);
	///	アルファブレンディングのモード設定(SRCの混合係数, DEST混合係数)
	virtual void SetAlphaMode(TBlendFunc src, TBlendFunc dest);
	/// テクスチャのロード（戻り値：テクスチャID）
	virtual unsigned int LoadTexture(const std::string filename);
	/// シェーダの初期化
	virtual void InitShader();
	/// シェーダオブジェクトの作成
	virtual bool CreateShader(std::string vShaderFile, std::string fShaderFile, GRHandler& shaderProgram);
	/// シェーダオブジェクトの作成
	virtual bool CreateShader(std::string vShaderFile, GRHandler& shaderProgram);
	/// シェーダのソースプログラムをメモリに読み込み、シェーダオブジェクトと関連付ける
	virtual bool ReadShaderSource(GRHandler shader, std::string file);	
	
};

}
#endif

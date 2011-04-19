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
	
	virtual void Init();
	virtual void SetViewport(Vec2f pos, Vec2f sz);
	virtual void ClearBuffer();
	virtual void BeginScene();
	virtual void EndScene();
	///	バッファを表示するための呼ぶ関数
	virtual void SwapBuffers();
	///	カレントの視点行列をafvで置き換える
	virtual void SetViewMatrix(const Affinef& afv);
	virtual void GetViewMatrix(Affinef& afv);
	///	カレントの投影行列を取得する
	virtual void SetProjectionMatrix(const Affinef& afp);
	virtual void GetProjectionMatrix(Affinef& afp);
	virtual void SetModelMatrix(const Affinef& afw);
	virtual void GetModelMatrix(Affinef& afw);
	///	カレントのモデル行列に対してafwを掛ける
	virtual void MultModelMatrix(const Affinef& afw);
	virtual void PushModelMatrix();
	virtual void PopModelMatrix();
	virtual void ClearBlendMatrix();
	virtual bool SetBlendMatrix(const Affinef& afb);
	virtual bool SetBlendMatrix(const Affinef& afb, unsigned int id);
	virtual void SetVertexFormat(const GRVertexElement* e);
	virtual void SetVertexShader(void* s);
	virtual void DrawDirect(GRRenderBaseIf::TPrimitiveType ty, void* begin, size_t count, size_t stride=0);
	virtual void DrawIndexed(GRRenderBaseIf::TPrimitiveType ty, size_t* idx, void* vtx, size_t count, size_t stride=0);
	virtual void DrawLine		(Vec3f p0, Vec3f p1);
	virtual void DrawArrow		(Vec3f p0, Vec3f p1, float rbar, float rhead, float lhead, int slice, bool solid);
	virtual void DrawBox		(float sx, float sy, float sz, bool solid=true);
	virtual void DrawSphere		(float radius, int slices, int stacks, bool solid=true);
	virtual void DrawCone		(float radius, float height, int slice, bool solid=true);
	virtual void DrawCylinder	(float radius, float height, int slice, bool solid=true);
	virtual void DrawCapsule	(float radius, float height, int slice=20, bool solid=true);
	virtual void DrawRoundCone	(float rbottom, float rtop, float height, int slice=20, bool solid=true);
	virtual void DrawGrid		(float size, int slice, float lineWidth);

	virtual int  StartList();
	virtual void EndList();
	virtual void DrawList(int i);
	virtual void ReleaseList(int i);
	virtual void SetFont(const GRFont& font);
	virtual void DrawFont(Vec2f pos, const std::string str);
	virtual void DrawFont(Vec3f pos, const std::string str);
	virtual void SetMaterial(const GRMaterialDesc& mat);
	virtual void SetMaterial(const GRMaterialIf* mat);
	///	描画する点・線の太さの設定
	virtual void SetLineWidth(float w);
	virtual void PushLight(const GRLightDesc& light);
	virtual void PushLight(const GRLightIf* light){ GRDevice::PushLight(light); }
	virtual void PopLight();
	virtual int  NLights();
	virtual void SetDepthWrite(bool b);
	virtual void SetDepthTest(bool b);
	virtual void SetDepthFunc(GRRenderBaseIf::TDepthFunc f);
	virtual void SetAlphaTest(bool b);
	virtual void SetAlphaMode(GRRenderBaseIf::TBlendFunc src, GRRenderBaseIf::TBlendFunc dest);
	virtual void SetLighting(bool on);
	virtual unsigned int LoadTexture(const std::string filename);
	virtual void SetTextureImage(const std::string id, int components, int xsize, int ysize, int format, char* tb);
	virtual void InitShader();
	virtual void SetShaderFormat(GRShaderFormat::ShaderType type);	
	virtual bool CreateShader(std::string vShaderFile, std::string fShaderFile, GRHandler& shader);
	virtual GRHandler CreateShader();
	virtual bool ReadShaderSource(GRHandler shader, std::string file);	
	virtual void GetShaderLocation(GRHandler shader, void* location);		
};

}
#endif

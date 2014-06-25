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

#include <Graphics/GRRender.h>
#include <map>
#include <stack>

namespace Spr{;

/**	@class	GRDeviceGL
    @brief	OpenGLによるグラフィックス描画の実装　 */
class GRDeviceGL: public GRDevice{
	SPR_OBJECTDEF(GRDeviceGL);
protected:
	int            majorVersion;		///< GLコンテキストのメジャーバージョン
	int            minorVersion;		///< GLコンテキストのマイナーバージョン

	int            nLights;				///< 光源の数
	int            vertexFormatGl;		///< glInterleavedArraysで使う，GLの頂点フォーマットID
	size_t         vertexSize;			///< 頂点のサイズ
	bool           vertexColor;			///< 頂点が色を持つかどうか
	GRMaterialDesc currentMaterial;		///< 現在のマテリアル

	bool	       bPointSmooth;		///< DrawPointにアンチエイリアスかけるか
	bool	       bLineSmooth;			///< DrawLineにアンチエイリアスかけるか
	
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
	std::vector<Affinef>				blendMatrices;			///< ブレンド変換行列
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
	//std::string vertexShaderFile;						///< VertexShader ファイル名
	//std::string fragmentShaderFile;						///< FragmentShader ファイル名
	//GRShaderFormat::ShaderType shaderType;				///< シェーダのロケーションタイプ

	/// シェーダ情報
	typedef std::vector< UTRef<GRShader> >		Shaders;
	Shaders		shaders;
	GRShader*	curShader;			//< 使用中のシェーダインデックス

	/* 組み込み + 拡張頂点属性インデックス
		- 普通のビデオカードで16個までは使えると思ってよい
	 */
	struct VertexAttribute{
		enum{
			// 組み込み属性のPosition ～ MultiTexCoordは参考のみ; nVidia仕様に準拠
			Position       = 0,
			Normal         = 2,
			Color          = 3,
			SecondaryColor = 4,
			FogCoord       = 5,
			MultiTexCoord0 = 8,
			/// 以下Springhead拡張属性
			BlendIndex     = 11,
			BlendWeight    = 12,
		};
	};

	/// シャドウマッピング
	GRShadowLightDesc	shadowDesc;	//< 現在のシャドウ設定
	unsigned shadowTexId;			//< シャドウテクスチャのID
	unsigned shadowBufferId;		//< シャドウテクスチャをバインドするフレームバッファのID
	Affinef  shadowView;			//< 光源変換
	Affinef  shadowProj;			//< 
	Affinef  shadowMatrix;			//< 頂点座標からシャドウテクスチャ座標を得るための変換
	Vec2f    shadowVpPosTmp;		//< ビューポートの退避用
	Vec2f    shadowVpSizeTmp;		//<
	Affinef  shadowViewTmp;			//< カメラ変換の退避用
	Affinef  shadowProjTmp;			//<
	
	/** @} */	

	/** sin, cosのキャッシュ
	 */
	struct tri_array{
		std::vector<float>	_sin;
		std::vector<float>	_cos;

		void Create(int slice){
			_sin.resize(slice);
			_cos.resize(slice);
			float step = (2.0f * (float)M_PI) / (float)slice;
			float t = 0.0f;
			for(int i = 0; i < slice; i++){
				_sin[i] = sinf(t);
				_cos[i] = cosf(t);
				t += step;
			}
		}
	};
	std::map<int, tri_array>	tri_cache;

	float	GetSin(int i, int slice){
		tri_array& arr = tri_cache[slice];
		if(arr._cos.empty())
			arr.Create(slice);
		return arr._sin[i%slice];
	}
	float	GetCos(int i, int slice){
		tri_array& arr = tri_cache[slice];
		if(arr._cos.empty())
			arr.Create(slice);
		return arr._cos[i%slice];
	}
	
	/// シェーダソース読み込み
	bool ReadShaderSource(const char* filename, std::string& src);
	/// シェーダのコンパイル・リンクレポート
	void PrintShaderInfoLog(int id, bool prog_or_shader);
	/// シャドウマッピング用変換を計算
	void CalcShadowMatrix();

public:
	///	コンストラクタ
	GRDeviceGL(){}
	
	virtual void Init();
	virtual void SetViewport(Vec2f pos, Vec2f sz);
	virtual Vec2f GetViewportPos();
	virtual Vec2f GetViewportSize();
	virtual void ClearBuffer(bool color, bool depth);
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
	//virtual void ClearBlendMatrix();
	//virtual bool SetBlendMatrix(const Affinef& afb);
	virtual void SetBlendMatrix (const Affinef& afb, unsigned int id);
	virtual void SetVertexFormat(const GRVertexElement* e);
	//virtual void SetVertexShader(void* s);
	virtual void DrawDirect		(GRRenderBaseIf::TPrimitiveType ty, void* begin, size_t count, size_t stride=0);
	virtual void DrawIndexed	(GRRenderBaseIf::TPrimitiveType ty, GLuint* idx, void* vtx, size_t count, size_t stride=0);
	virtual void DrawPoint		(Vec3f p);
	virtual void DrawLine		(Vec3f p0, Vec3f p1);
	virtual void DrawSpline		(Vec3f p0, Vec3f v0, Vec3f p1, Vec3f v1, int ndiv);
	virtual void DrawArrow		(Vec3f p0, Vec3f p1, float rbar, float rhead, float lhead, int slice, bool solid);
	virtual void DrawBox		(float sx, float sy, float sz, bool solid=true);
	virtual void DrawSphere		(float radius, int slices, int stacks, bool solid=true);
	virtual void DrawCone		(float radius, float height, int slice, bool solid=true);
	virtual void DrawCylinder	(float radius, float height, int slice, bool solid=true, bool cap=true);
	virtual void DrawDisk		(float radius, int slice, bool solid=true);
	virtual void DrawCapsule	(float radius, float height, int slice=20, bool solid=true);
	virtual void DrawRoundCone	(float rbottom, float rtop, float height, int slice=20, bool solid=true);
	virtual void DrawCurve		(const Curve3f& curve);
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
	virtual void SetPointSize(float sz, bool smooth);
	virtual void SetLineWidth(float w, bool smooth);
	virtual void PushLight(const GRLightDesc& light);
	virtual void PushLight(const GRLightIf* light){ GRDevice::PushLight(light); }
	virtual void PopLight();
	virtual int  NLights();
	virtual void SetDepthWrite(bool b);
	virtual void SetDepthTest (bool b);
	virtual void SetDepthFunc (GRRenderBaseIf::TDepthFunc f);
	virtual void SetAlphaTest (bool b);
	virtual void SetAlphaMode (GRRenderBaseIf::TBlendFunc src, GRRenderBaseIf::TBlendFunc dest);
	virtual void SetLighting  (bool b);
	virtual void SetTexture2D (bool b);
	virtual void SetTexture3D (bool b);
	virtual void SetBlending  (bool b);
	virtual unsigned int LoadTexture(const std::string filename);
	virtual void SetTextureImage(const std::string id, int components, int xsize, int ysize, int format,const char* tb);
	//virtual void InitShader();
	//virtual void SetShaderFormat(GRShaderFormat::ShaderType type);	
	//virtual bool CreateShader(std::string vShaderFile, std::string fShaderFile, GRHandler& shader);
	//virtual GRHandler CreateShader();
	//virtual bool ReadShaderSource(GRHandler shader, std::string file);	
	//virtual void GetShaderLocation(GRHandler shader, void* location);		
	virtual GRShaderIf* CreateShader(const GRShaderDesc& sd);
	virtual bool SetShader(GRShaderIf* sh);
	virtual void SetShadowLight(const GRShadowLightDesc& sld);
	virtual void EnterShadowMapGeneration();
	virtual void LeaveShadowMapGeneration();

	/// OpenGLバージョンチェック
	bool CheckGLVersion(int major, int minor);
	int  GetGLMajorVersion();
	int  GetGLMinorVersion();
	void SetGLVersion(int major, int minor);
};

}
#endif

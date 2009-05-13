/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_GRRENDER_H
#define SPR_GRRENDER_H

#include <float.h>
#include <Physics/SprPHSolid.h>
#include <Physics/SprPHScene.h>
#include <Collision/SprCDShape.h>
#include <Graphics/SprGRFrame.h>
#include <Graphics/SprGRVertex.h>
#include <Graphics/SprGRShader.h>
namespace Spr{;

/**	\addtogroup	gpGraphics	*/
//@{

/**
 *	@file SprGRRender.h
 *	@brief グラフィックスレンダラーのインタフェース、基本インタフェース
 */

/**	@brief	テキスト描画のフォント */
class SPR_DLL GRFont{
public:
	int height;				///<	フォントの高さ
	int width;				///<	平均文字幅
	int weight;				///<	フォントの太さ（0 - 900）
	std::string face;		///<	タイプフェイス
	unsigned long color;	///<	フォントの色
	bool bItalic;			///<	イタリック体
	GRFont(int h=20, const char* f=NULL){
		height=h;
		face=f ? f : "";
		weight = 400; color=0xffffffff; bItalic=false;
	}
	void SetColor(unsigned long c){ color = c; }
	bool operator < (GRFont& f){
		if (face < f.face) return true;
		if (height < f.height) return true;
		if (color < f.color) return true;
		return false;
	}
	GRFont& operator=(const GRFont& rhs) {
		if (this==&rhs)	return *this;
		height	= rhs.height;
		width	= rhs.width;
		weight	= rhs.weight;
		face	= rhs.face;
		color	= rhs.color;
		bItalic	= rhs.bItalic;
		return *this;
	}
};
	
/** @brief 光源のインタフェース		*/
struct GRLightIf: public GRVisualIf{
	SPR_IFDEF(GRLight);
};
/**	@brief	光源		*/
struct GRLightDesc : GRVisualDesc{
	SPR_DESCDEF(GRLight);
    Vec4f ambient;		///<	環境光
    Vec4f diffuse;		///<	拡散光
    Vec4f specular;		///<	鏡面光
    Vec4f position;		///<	光源位置（w=1.0で点光源、w=0.0で平行光源）
    float range;		///<	光が届く範囲（deviceがDirectXの場合のみ利用可能）
	/**
	 *	@name	減衰パラメータ
	 *　　 これらを設定することで光の減衰を表現できる。 \n
	 *　　 減衰のパラメータを設定すると、光源から離れるほど高原による効果は小さくなる。 \n
	 *　　 デフォルトでは、減衰なしに設定されている。 \n
	 *　　 平行光源の場合、減衰はさせない。 \n
	 *　　　　 減衰係数 = 1/( att0 + att1 * d + att2 * d^2)　　　d:距離
	 *	@{
	 */
	float attenuation0;	///<	一定減衰率
    float attenuation1;	///<	線形減衰率
    float attenuation2;	///<	2次減衰率
	/** @} */	// end of 減衰パラメータ

	Vec3f spotDirection;///<	スポットライトの向き
    float spotFalloff;	///<	スポットライトの円錐内での減衰率(大きいほど急峻) 0..∞
	float spotInner;	///<	スポットライトの中心部分(内部コーン)（deviceがDirectXの場合のみ利用可能） 0..spotCutoff
	float spotCutoff;	///<	スポットライトの広がり角度(度)(外部コーン) 0..π(pi)
	GRLightDesc(){
		ambient = Vec4f(0.0, 0.0, 0.0, 1.0);
		diffuse = Vec4f(1.0, 1.0, 1.0, 1.0);
		specular = Vec4f(1.0, 1.0, 1.0, 1.0);
		position = Vec4f(0.0, 0.0, 1.0, 1.0);
		range = FLT_MAX;
		attenuation0 = 1.0f;
		attenuation1 = 0.0f;
		attenuation2 = 0.0f;
		spotDirection = Vec3f(0.0, -1.0, 0.0);
		spotFalloff  = 0.0f;
		spotInner    = 0.0f;
		spotCutoff   = 180.0f;
	}
};



/** @brief　材質のインタフェース　　	*/
struct GRMaterialIf: public GRVisualIf{
	SPR_IFDEF(GRMaterial);
	bool IsOpaque() const;
};
/**	@brief	材質	*/
struct GRMaterialDesc : GRVisualDesc{
	SPR_DESCDEF(GRMaterial);
	Vec4f ambient;					///<	環境光に対する反射率
	Vec4f diffuse;					///<	拡散光に対する反射率
	Vec4f specular;					///<	鏡面光に対する反射率
	Vec4f emissive;					///<	放射輝度
	float power;					///<	鏡面反射の強度、鏡面係数
	std::string	texname;			///<	テクスチャファイル名

	GRMaterialDesc(){
		ambient = Vec4f(0.2, 0.2, 0.2, 1.0);
		diffuse = Vec4f(0.8, 0.8, 0.8, 1.0);
		specular = Vec4f(1.0, 1.0, 1.0, 1.0);
		emissive = Vec4f(0.0, 0.0, 0.0, 1.0);
		power = 20.0;
	}
	GRMaterialDesc(Vec4f a, Vec4f d, Vec4f s, Vec4f e, float p):
		ambient(a), diffuse(d), specular(s), emissive(e), power(p){}
	GRMaterialDesc(Vec4f c, float p):
		ambient(c), diffuse(c), specular(c), emissive(c), power(p){}
	GRMaterialDesc(Vec4f c):
		ambient(c), diffuse(c), specular(c), emissive(c), power(0.0f){}
	/**	W()要素は、アルファ値(0.0～1.0で透明度を表す). 1.0が不透明を表す.
		materialのW()要素を判定して、不透明物体か、透明物体かを判定する. 
		透明なオブジェクトを描くとき、遠くのものから順番に描画しないと、意図に反した結果となる. */
	bool IsOpaque() const {		
		return ambient.W() >= 1.0 && diffuse.W() >= 1.0 && specular.W() >= 1.0 && emissive.W() >= 1.0;
	}
	bool Is3D() const ;
};

struct GRFrameIf;
struct GRCameraDesc;
///	カメラのインタフェース
struct GRCameraIf: public GRVisualIf{
	SPR_IFDEF(GRCamera);
	GRFrameIf* GetFrame();
	void SetFrame(GRFrameIf* fr);
};
/**	@brief	カメラの情報			*/
struct GRCameraDesc : GRVisualDesc{
	SPR_DESCDEF(GRCamera);
	Vec2f size;				///<	スクリーンのサイズ
	Vec2f center;			///<	カメラからのスクリーンのずれ
	float front, back;		///<	視点からクリップ面までの相対距離（正の値で指定）
	//GRCameraDesc():center(Vec2f()), size(Vec2f(0.2f, 0)), front(0.1f), back(500.0f){}
	GRCameraDesc(Vec2f sz = Vec2f(0.2f, 0.0f), Vec2f c = Vec2f(), float f = 0.1f, float b = 500.0f):
		size(sz), center(c), front(f), back(b) {}
};

struct GRDeviceIf;
typedef unsigned GRHandler;

/**	@brief	グラフィックスレンダラーのインタフェース（ユーザインタフェース） */
struct GRRenderBaseIf: public ObjectIf{
	SPR_IFDEF(GRRenderBase);

	///	プリミティブの種類
	enum TPrimitiveType {
		POINTS,
		LINES,
		LINE_STRIP,
		LINE_LOOP,
		TRIANGLES,
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
		QUADS
	};
	/**
	 *	@name	デプスバッファ法に用いる判定条件
	 *	@{
	 */
	enum TDepthFunc{
		DF_NEVER,		///<	新しいZ値に関係なく更新しない
		DF_LESS,		///<	新しいZ値が小さければ更新する（default)
		DF_EQUAL,		///<	新しいZ値が等しければ描かれる
		DF_LEQUAL,		///<	新しいZ値が大きくなければ描かれる
		DF_GREATER,		///<	新しいZ値が大きければ描かれる
		DF_NOTEQUAL,	///<	新しいZ値と等しくなければ描かれる
		DF_GEQUAL,		///<	新しいZ値が小さくなければ描かれる
		DF_ALWAYS		///<	新しいZ値に関係なく更新する
	};
	/** @} */	
	/**
	 *	@name	アルファブレンディングの混合係数
	 *　　 SRCがこれから描画される合成させたい色、DESTがすでに描かれたカラーバッファの色    \n
	 *　　　　 合成結果 =SRC * SRCのブレンディング係数 + DEST * DESTのブレンディング係数 			\n
	 *　　 ただし、BF_BOTHINVSRCALPHA、BF_BLENDFACTOR、BF_INVBLENDFACTOR に関しては、			\n
	 *　　 deviceがDirectXのときのみ利用可能。指定してもBF_ZEROとして扱う。
	 *	@{
	 */
	enum TBlendFunc{
		BF_ZERO,				///<	(0,0,0,0)	
		BF_ONE,					///<	(1,1,1,1)
		BF_SRCCOLOR,			///<	(Rs, Gs, Bs, As)
		BF_INVSRCCOLOR,			///<	(1-Rs, 1-Gs, 1-Bs, 1-As) 
		BF_SRCALPHA,			///<	(As, As, As, As) 
		BF_INVSRCALPHA,			///<	(1-As, 1-As, 1-As, 1-As)
		BF_DESTALPHA,			///<	(Ad, Ad, Ad, Ad)  
		BF_INVDESTALPHA,		///<	(1-Ad, 1-Ad, 1-Ad, 1-Ad) 
		BF_DESTCOLOR,			///<	(Rd, Gd, Bd, Ad)
		BF_INVDESTCOLOR,		///<	(1-Rd, 1-Gd, 1-Bd, 1-Ad) 
		BF_SRCALPHASAT,			///<	(f, f, f, 1)、f = min(As, 1-Ad) 
		BF_BOTHINVSRCALPHA,		///<	SRCブレンディング係数は(1-As, 1-As, 1-As, 1-As)。DSTブレンディング係数は(As, As, As, As)。
		BF_BLENDFACTOR,			///<	フレームバッファ合成時に使われる定数色で合成
		BF_INVBLENDFACTOR		///<	フレームバッファ合成時に使われる定数色を反転した色で合成
	};
	/** @} */

	///	ビューポートの設定
	void SetViewport(Vec2f pos, Vec2f sz);
	///	バッファクリア
	void ClearBuffer();
	/// 背景色の取得
	void GetClearColor(Vec4f& color);
	/// 背景色の設定
	void SetClearColor(const Vec4f& color);
	///	レンダリングの開始前に呼ぶ関数
	void BeginScene();
	///	レンダリングの終了後に呼ぶ関数
	void EndScene();
	///	カレントの視点行列をafvで置き換える
	void SetViewMatrix(const Affinef& afv);
	///	カレントの投影行列をafpで置き換える
	void SetProjectionMatrix(const Affinef& afp);
	///	カレントの投影行列を取得する
	void GetProjectionMatrix(Affinef& afp);
	///	カレントのモデル行列をafwで置き換える
	void SetModelMatrix(const Affinef& afw);
	///	カレントのモデル行列に対してafwを掛ける
	void MultModelMatrix(const Affinef& afw);
	///	カレントのモデル行列をモデル行列スタックへ保存する
	void PushModelMatrix();
	///	モデル行列スタックから取り出し、カレントのモデル行列とする
	void PopModelMatrix();
	/// ブレンド変換行列の全要素を削除する
	void ClearBlendMatrix();
	/// ブレンド変換行列を設定する
	bool SetBlendMatrix(const Affinef& afb, unsigned int id=0);
	///	頂点フォーマットの指定
	void SetVertexFormat(const GRVertexElement* e);
	///	頂点シェーダーの指定	API化候補．引数など要検討 2006.6.7 hase
	void SetVertexShader(void* shader);
	///	頂点を指定してプリミティブを描画
	void DrawDirect(GRRenderBaseIf::TPrimitiveType ty, void* vtx, size_t count, size_t stride=0);
	///	頂点とインデックスを指定してプリミティブを描画
	void DrawIndexed(GRRenderBaseIf::TPrimitiveType ty, size_t* idx, void* vtx, size_t count, size_t stride=0);
 	///	頂点の成分ごとの配列を指定して，プリミティブを描画
	void DrawArrays(GRRenderBaseIf::TPrimitiveType ty, GRVertexArray* arrays, size_t count);
 	///	インデックスと頂点の成分ごとの配列を指定して，プリミティブを描画
	void DrawArrays(GRRenderBaseIf::TPrimitiveType ty, size_t* idx, GRVertexArray* arrays, size_t count);	///	球体を描画
	/// 球体を描画
	void DrawSphere(float radius, int slices, int stacks, bool solid=true);
	/// 円錐の描画
	void DrawCone(float radius, float height, int slice, bool solid=true);
	/// 円筒の描画
	void DrawCylinder(float radius, float height, int slice, bool solid=true);
	
	
	///	DiplayList の作成(記録開始)
	int StartList();
	///	DiplayList の終了(記録終了)
	void EndList();
	///	DisplayListの表示
	void DrawList(int i);
	///	DisplayListの解放
	void ReleaseList(int i);
	///	2次元テキストの描画　　 Windows環境(VC)でのみfontをサポートし、他の環境ではfontを指定しても利用されない。
	void DrawFont(Vec2f pos, const std::string str, const GRFont& font=0);
	/** @brief	3次元テキストの描画　　 Windows環境(VC)でのみfontをサポートし、他の環境ではfontを指定しても利用されない。	
		@param  Vec3f pos				: World座標系での書き始めの位置，ビューポートをはみ出すと何も表示されなくなるので注意．
				const std::string str	: 書く文字列(bitmapfontで書ける物のみ)
				const GRFont& font		: フォントに関する設定
	*/
	void DrawFont(Vec3f pos, const std::string str, const GRFont& font=0);
	///	描画の材質の設定
	void SetMaterial(const GRMaterialDesc& mat);
	void SetMaterial(const GRMaterialIf* mat);
	///	描画する点・線の太さの設定
	void SetLineWidth(float w);
	///	光源スタックをPush
	void PushLight(const GRLightDesc& light);
	void PushLight(const GRLightIf* light);
	///	光源スタックをPop
	void PopLight();
	///	デプスバッファへの書き込みを許可/禁止する
	void SetDepthWrite(bool b);
	///	デプステストを有効/無効にする
	void SetDepthTest(bool b);
	///	デプスバッファ法に用いる判定条件を指定する
	void SetDepthFunc(GRRenderBaseIf::TDepthFunc f);
	/// アルファブレンディングを有効/無効にする
	void SetAlphaTest(bool b);
	///	アルファブレンディングのモード設定(SRCの混合係数, DEST混合係数)
	void SetAlphaMode(GRRenderBaseIf::TBlendFunc src, GRRenderBaseIf::TBlendFunc dest);
	///	シェーディングON(glMaterial) or OFF（glColor)
	void SetLighting(bool l);
	/// テクスチャのロード（戻り値：テクスチャID）
	unsigned int LoadTexture(const std::string filename);
	/// テクスチャ画像の設定
	void SetTextureImage(const std::string id, int components, int xsize, int ysize, int format, char* tb);
	/// シェーダの初期化
	void InitShader();
	/// シェーダフォーマットの設定
	void SetShaderFormat(GRShaderFormat::ShaderType type);
	/// シェーダオブジェクトの作成
	bool CreateShader(std::string vShaderFile, std::string fShaderFile, GRHandler& shader);
	/// シェーダオブジェクトの作成、GRDeviceGL::shaderへの登録（あらかじめShaderFile名を登録しておく必要がある）	
	GRHandler CreateShader();
	/// シェーダのソースプログラムをメモリに読み込み、シェーダオブジェクトと関連付ける
	bool ReadShaderSource(GRHandler shader, std::string file);	
	/// ロケーション情報の取得（SetShaderFormat()でシェーダフォーマットを設定しておく必要あり）
	void GetShaderLocation(GRHandler shader, void* location);	
};

/**	@brief	グラフィックスレンダラーのインタフェース（デバイスの設定、カメラの設定） */
struct GRRenderIf: public GRRenderBaseIf{
	SPR_IFDEF(GRRender);
	///	デバイスの設定
	void SetDevice(GRDeviceIf* dev);
	///	デバイスの取得
	GRDeviceIf* GetDevice();
	///	カメラの設定
	void SetCamera(const GRCameraDesc& cam);
	///	カメラの設定
	const GRCameraDesc& GetCamera();
	///	スクリーン(ウィンドウ)サイズ変更時のViewportと射影行列を設定
	void Reshape(Vec2f pos, Vec2f screenSize);
	/// Viewportの基点座標の取得
	Vec2f GetViewportPos();
	/// Viewportのサイズの取得
	Vec2f GetViewportSize();

};

/**	@brief	グラフィックスレンダラーのデバイスクラス．OpenGLやDirectXのラッパ */
struct GRDeviceIf: public GRRenderBaseIf{
	SPR_IFDEF(GRDevice);
	///	初期化
	void Init();
	///	デバッグ用の状態レポート
	void Print(std::ostream& os) const;
};

/**	@brief	OpenGLのレンダラー基本クラス */
struct GRDeviceGLIf: public GRDeviceIf{
	SPR_IFDEF(GRDeviceGL);
};

/**	@brief	DirectXのレンダラー基本クラス */
/*
struct GRDeviceD3DIf: public GRDeviceIf{
	SPR_IFDEF(GRDeviceD3D);
};
*/

/**	@brief	デバッグ情報レンダラーのインタフェース */
struct GRDebugRenderIf:public GRRenderIf{
	SPR_IFDEF(GRDebugRender);
	///	レンダラーで用意してある材質(24種類)
	enum TMaterialSample {
		RED,
		GREEN,
		BLUE,
		YELLOW,
		CYAN,
		MAGENTA,
		WHITE,
		GRAY,
		ORANGE,
		BLOWN,
		LIGHT_BLUE,
		MIDIUM_PURPLE,
		DARK_GREEN,
		DARK_VIOLET,
		DARK_CYAN,
		GREEN_YELLOW,
		LIME_GREEN,
		INDIAN_RED,
		INDIGO,
		GREEN_INDIGO,
		OLIVE_GREEN,
		NAVY_BLUE,
		TURQUOISE_BLUE,
		EMERALD_GREEN,
		MATERIAL_SAMPLE_END
	};

	/**  @brief シーン内の全てのオブジェクトをレンダリングする
	     @param  scene		シーン  */
	void DrawScene(PHSceneIf* scene);

	/** @brief シーンのWorld座標系の座標軸をレンダリングする
		@param  scene		シーン
	*/
	// 引数が冗長な気がしなくもないです・・・　		toki
	void DrawWorldAxis(PHSceneIf* scene);

	/**  @brief 剛体をレンダリングする
	     @param	solid　　　	剛体  */
	void DrawSolid(PHSolidIf* solid);
	
	/**  @brief 面をレンダリングをする
		 @param	face　　　	面  
		 @param	base　　　	凸形状の頂点群  */
	//
	// APIとして公開するのは変では？	tazz
	//
	//void DrawFace(CDFaceIf* face, Vec3f * base);
	
	/**  @brief 指定したマテリアルを割り当てる
	     @param matname		マテリアルサンプル  */
	void SetMaterialSample(GRDebugRenderIf::TMaterialSample matname);

	/**	 @brief 描画モードの設定
		 @param solid 面描画
		 @param wire ワイヤ描画
	 */
	void SetRenderMode(bool solid = true, bool wire = false);
	
	/** @brief ワールド座標系の座標軸を描画 */
	void EnableRenderWorldAxis(bool enable = true);

	/**	 @brief 各関節の座標軸の描画 */
	void EnableRenderAxis(bool enable = true, float scale = 1.0f);

	/**  @brief 各関節の力の描画 */
	void EnableRenderForce(bool enable = true, float scale = 1.0f);

	/**  @brief 接触面の描画 */
	void EnableRenderContact(bool enable = true);
	///	面の描画(塗りつぶしあり)
	void DrawFaceSolid(CDFaceIf* face, Vec3f * base);
	///	面の描画(ワイヤフレーム)	
	void DrawFaceWire(CDFaceIf* face, Vec3f * base);
};

//@}


}
#endif

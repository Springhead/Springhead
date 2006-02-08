#ifndef SPR_GRRENDER_H
#define SPR_GRRENDER_H

#include <float.h>
#include <Physics/SprPHSolid.h>

namespace Spr{;

/**	@struct	GRFont
    @brief	テキスト描画のフォント */
class SPR_DLL GRFont{
public:
	int height;
	int width;
	int weight;
	std::string face;
	unsigned long color;
	bool bItalic;
	GRFont(int h=20, const char* f=NULL){
		height=h; width=0;
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
};

/**	@struct	GRLight
    @brief	光源		*/
struct GRLight{
    Vec4f diffuse;		///<	拡散光
    Vec4f specular;		///<	鏡面光
    Vec4f ambient;		///<	環境光
    Vec4f position;		///<	光源の位置，w=0とすると無限遠(方向光源)になる．
    float range;		///<	光が届く範囲
    ///@name	減衰率．Atten = 1/( att0 + att1 * d + att2 * d^2)
	//@{
	float attenuation0;	///<	att0　　 0..∞
    float attenuation1;	///<	att1　　 0..∞
    float attenuation2;	///<	att2　　 0..∞
	//@}
	Vec3f spotDirection;///<	スポットライトの場合の向き
    float spotFalloff;	///<	減衰の早さ(大きいほど急峻)　　　 0..∞
	float spotInner;	///<	スポットライトの中心部分　　　　 0..spotCutoff
	float spotCutoff;	///<	スポットライトの光が当たる範囲　 0..π(pi)
	GRLight(){
		range = FLT_MAX;
		attenuation0 = 0.0f;
		attenuation1 = 0.0f;
		attenuation2 = 0.0f;
		spotFalloff = 0.0f;
		spotInner = 0.0f;
		spotCutoff = 0.0f;
	}
};

/**	@struct	GRMaterial
    @brief	グラフィックスの材質 */
struct GRMaterial{
	Vec4f diffuse;					///<	拡散光に対する反射率
	Vec4f ambient;					///<	環境光に対する反射率
	Vec4f specular;					///<	鏡面光に対する反射率
	//Vec4f emissive;				///<	放射輝度
	Vec4f emission;					///<	放射輝度
	float power;					///<	
	//float shininess				///<	鏡面反射の強度、鏡面係数
	std::string texture;			///<	テクスチャファイル名
	GRMaterial(){ power = 0.0f; }
	GRMaterial(Vec4f d, Vec4f a, Vec4f s, Vec4f e, float p):
		diffuse(d), ambient(a), specular(s), emission(e), power(p){}
	GRMaterial(Vec4f c, float p):
		diffuse(c), ambient(c), specular(c), emission(c), power(p){}
	GRMaterial(Vec4f c):
		diffuse(c), ambient(c), specular(c), emission(c), power(0.0f){}
	/**	W()要素は、アルファ値(0.0～1.0で透明度を表す). 1.0が不透明を表す.
		materialのW()要素を判定して、不透明物体か、透明物体かを判定する. 
		透明なオブジェクトを描くとき、遠くのものから順番に描画しないと、意図に反した結果となる. */
	bool IsOpaque(){		
		return diffuse.W() >= 1.0 && ambient.W() >= 1.0 && specular.W() >= 1.0 && emission.W() >= 1.0;
	}
};

/**	@struct	GRCamera
    @brief	カメラの情報 */
struct GRCamera{
	Vec2f size;				///<	スクリーンのサイズ
	Vec2f center;			///<	カメラからのスクリーンのずれ
	float front, back;		///<	視点からクリップ面までの相対距離（正の値で指定）
	GRCamera(Vec2f initSize=Vec2f(480.0, 360.0), 
				Vec2f initCenter=Vec2f(0.0, 0.0),
				float initFront=1.0,
				float initBack=5000.0) 
			: size(initSize), center(initCenter), front(initFront), back(initBack) {}
};

struct GRDeviceIf;

/**	@struct	GRRenderBaseIf
    @brief	グラフィックスレンダラーの基本クラス */
struct GRRenderBaseIf: public ObjectIf{
	IF_DEF(GRRenderBase);
	///	プリミティブの種類
	enum TPrimitiveType {
		POINTS,
		LINES,
		LINE_STRIP,
		TRIANGLES,
		TRIANGLE_STRIP,
		TRIANGLE_FAN
	};
	///	Zバッファテスト関数
	enum TDepthFunc{
		DF_NEVER, DF_LESS, DF_EQUAL, DF_LEQUAL, DF_GREATER, DF_NOTEQUAL, 
		DF_GEQUAL, DF_ALWAYS
	};
	///	アルファブレンド関数
	enum TBlendFunc{
		BF_ZERO = 1,
		BF_ONE = 2,
		BF_SRCCOLOR = 3,
		BF_INVSRCCOLOR = 4,
		BF_SRCALPHA = 5,
		BF_INVSRCALPHA = 6,
		BF_DESTALPHA = 7,
		BF_INVDESTALPHA = 8,
		BF_DESTCOLOR = 9,
		BF_INVDESTCOLOR = 10,
		BF_SRCALPHASAT = 11,
		BF_BOTHSRCALPHA = 12,
		BF_BOTHINVSRCALPHA = 13,
		BF_BLENDFACTOR = 14,
		BF_INVBLENDFACTOR = 15
	};
	///	バッファクリア
	virtual void ClearBuffer()=0;
	///	レンダリングの開始前に呼ぶ関数
	virtual void BeginScene()=0;
	///	レンダリングの終了後に呼ぶ関数
	virtual void EndScene()=0;
	///	モデル行列をかける
	virtual void MultModelMatrix(const Affinef& afw)=0;
	///	モデル行列の行列スタックをPush
	virtual void PushModelMatrix()=0;
	///	モデル行列の行列スタックをPop
	virtual void PopModelMatrix()=0;
	///	モデル行列を設定
	virtual void SetModelMatrix(const Affinef& afw)=0;
	///	視点行列を設定
	virtual void SetViewMatrix(const Affinef& afv)=0;
	///	投影行列を設定
	virtual void SetProjectionMatrix(const Affinef& afp)=0;
	///	頂点座標を指定してプリミティブを描画
	virtual void DrawDirect(TPrimitiveType ty, Vec3f* begin, Vec3f* end)=0;
	///	頂点座標とインデックスを指定してプリミティブを描画
	virtual void DrawIndexed(TPrimitiveType ty, size_t* begin, size_t* end, Vec3f* vtx)=0;
	///	2次元テキストの描画
	virtual void DrawText(Vec2f pos, std::string str, const GRFont& font)=0;
	///	描画の材質の設定
	virtual void SetMaterial(const GRMaterial& mat)=0;
	///	描画する点・線の太さの設定
	virtual void SetLineWidth(float w)=0;
	///	光源スタックをPush
	virtual void PushLight(const GRLight& m)=0;
	///	光源スタックをPop
	virtual void PopLight()=0;
	///	描画時に深度バッファへ書き込むかどうか
	virtual void SetDepthWrite(bool b)=0;
	///	描画時に深度テストを行うかどうか
	virtual void SetDepthTest(bool b)=0;
	///	深度テストのテスト関数
	virtual void SetDepthFunc(TDepthFunc f)=0;
	///	アルファブレンドのモード設定
	virtual void SetAlphaMode(TBlendFunc src, TBlendFunc dest)=0;
};

/**	@struct	GRRenderIf
    @brief	レンダラの基本クラス */
struct GRRenderIf: public GRRenderBaseIf{
	IF_DEF(GRRender);
	virtual void SetDevice(GRDeviceIf* dev)=0;
	virtual void SetCamera(GRCamera& cam)=0;
};

/**	@struct	GRDeviceIf
    @brief	グラフィックスレンダラーのデバイスクラス．OpenGLやDirectXのラッパ */
struct GRDeviceIf: public GRRenderBaseIf{
	IF_DEF(GRDevice);
	///	初期化
	virtual void Init()=0;
	///	デバッグ用の状態レポート
	virtual void Print(std::ostream& os) const=0;
};

/**	@struct	GRDeviceGLIf
    @brief	OpenGLのレンダラー基本クラス */
struct GRDeviceGLIf: public GRDeviceIf{
	IF_DEF(GRDeviceGL);
	/// ウィンドウIDを設定する
	virtual void SetWindow(int w)=0;
};

/**	@struct	GRDeviceD3DIf
    @brief	DirectXのレンダラー基本クラス */
struct GRDeviceD3DIf: public GRDeviceIf{
	IF_DEF(GRDeviceD3D);
};

/**	@struct	GRDebugRenderIf
    @brief	デバッグ情報レンダラーの基本クラス */
struct GRDebugRenderIf:public GRRenderIf{
	IF_DEF(GRDebugRender);
	///	Viewportと射影行列を設定
	virtual void Reshape(Vec2f screen)=0;
	/// 剛体をレンダリングする
	virtual void DrawSolid(PHSolidIf* so)=0;
	/// 面をレンダリングする
	virtual void DrawFace(CDFaceIf* face, Vec3f * base)=0;
};


}
#endif

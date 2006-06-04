/**
 *	@file GRDeviceGL.h
 *	@brief OpenGLによるグラフィックス描画の実装　　
 */
#ifndef GRDEVICEGL_H
#define GRDEVICEGL_H

#include <map>
#include <SprGraphics.h>

namespace Spr{;

/**	@class	GRDeviceGL
    @brief	OpenGLによるグラフィックス描画の実装　 */
class GRDeviceGL: public InheritGRDevice<GRDeviceGLIf, GRDevice>{
	OBJECT_DEF(GRDeviceGL);
protected:
	int		window;					///< ウィンドウID	
	/**
	 *	@name	マトリックス変数
	 *　　 GLではModelを変えずにViewだけを変えるということができない。 \n
	 *　　 視点を動かすには、ユーザがModelを覚えておく必要がある。	\n
	 *　　 （Direct3Dの場合は、Modelを書き換えずにViewだけ書き換え、視点を動かすことができる）
	 *	@{ 
	 */
	Affinef	viewMatrix;				///<	視点行列
	Affinef	modelMatrix;			///<	モデル行列 
	Affinef	projectionMatrix;		///<	射影行列 
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

	///	バッファクリア
	virtual void ClearBuffer();
	///	レンダリングの開始前に呼ぶ関数
	virtual void BeginScene();
	///	レンダリングの終了後に呼ぶ関数
	virtual void EndScene();
	///	モデル行列をかける
	virtual void MultModelMatrix(const Affinef& afw);
	///	モデル行列の行列スタックをPush
	virtual void PushModelMatrix();
	///	モデル行列の行列スタックをPop
	virtual void PopModelMatrix();
	///	モデル行列を設定
	virtual void SetModelMatrix(const Affinef& afw);
	///	視点行列を設定
	virtual void SetViewMatrix(const Affinef& afv);
	///	投影行列を設定
	virtual void SetProjectionMatrix(const Affinef& afp);
	///	頂点座標を指定してプリミティブを描画
	virtual void DrawDirect(TPrimitiveType ty, Vec3f* begin, Vec3f* end);
	///	頂点座標とインデックスを指定してプリミティブを描画
	virtual void DrawIndexed(TPrimitiveType ty, size_t* begin, size_t* end, Vec3f* vtx);
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
	virtual void PushLight(const GRLight& light);
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
};

}
#endif

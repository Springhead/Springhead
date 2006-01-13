#ifndef GRDEVICEGL_H
#define GRDEVICEGL_H

#include <SprGraphics.h>

namespace Spr{;

/**	OpenGLによるグラフィックス描画の実装	*/
class GRDeviceGL: public GRDevice, public GRDeviceGLIf{
	OBJECTDEF(GRDeviceGL);
	BASEIMP_OBJECT(GRDevice);
protected:
	int window;
public:
	GRDeviceGL(int w=0):window(w){}
	virtual void SetWindow(int w){window=w;}
	virtual void Print(std::ostream& os) const{GRDevice::Print(os);}
	virtual void Init();

	///	バッファクリア
	virtual void ClearBuffer();
	///	レンダリングの開始前に呼ぶ関数
	virtual void BeginScene();
	///	レンダリングの終了後に呼ぶ関数
	virtual void EndScene();
	///	Viewportと射影行列を設定
	virtual void Resize(Vec2f screen);
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
	///	2次元テキストの描画
	virtual void DrawText(Vec2f pos, std::string str, const GRFont& font);
	///	描画の材質の設定
	virtual void SetMaterial(const GRMaterial& mat);
	///	描画する点・線の太さの設定
	virtual void SetLineWidth(float w);
	///	光源スタックをPush
	virtual void PushLight(const GRLight& m);
	///	光源スタックをPop
	virtual void PopLight();
	///	描画時に深度バッファへ書き込むかどうか
	virtual void SetDepthWrite(bool b);
	///	描画時に深度テストを行うかどうか
	virtual void SetDepthTest(bool b);
	///	深度テストのテスト関数
	virtual void SetDepthFunc(TDepthFunc f);
	///	アルファブレンドのモード設定
	virtual void SetAlphaMode(TBlendFunc src, TBlendFunc dest);
};

}
#endif

#ifndef SPR_GRRENDER_H
#define SPR_GRRENDER_H

#include <Springhead.h>

namespace Spr{;

struct GRDeviceIf;

/**	グラフィックスレンダラーの基本クラス	*/
struct GRRenderIf: public ObjectIf{
	IF_DEF(GRRender);
	///	プリミティブの種類
	enum TPrimitiveType {
		POINTS,
		LINES,
		LINESTRIP,
		TRIANGLES,
		TRIANGLESTRIP,
		TRIANGLEFAN
	};
	///	プリミティブの描画
	virtual void DrawDirect(TPrimitiveType ty, Vec3f* begin, Vec3f* end)=0;
	///	インデックスによるプリミティブの描画
	virtual void DrawIndexed(TPrimitiveType ty, size_t* begin, size_t* end, Vec3f* vtx)=0;
	///	材質の設定
	virtual void SetMaterial(const GRMaterial& mat)=0;
	///	描画デバイスの設定
	virtual void SetDevice(GRDeviceIf* dev)=0;
};
/**	グラフィックスレンダラーのデバイスクラス．OpenGLやDirectXのラッパ	*/
struct GRDeviceIf: public ObjectIf{
	IF_DEF(GRDevice);
	typedef GRRenderIf::TPrimitiveType TPrimitiveType;
	///	プリミティブの描画
	virtual void DrawDirect(TPrimitiveType ty, Vec3f* begin, Vec3f* end)=0;
	///	インデックスによるプリミティブの描画
	virtual void DrawIndexed(TPrimitiveType ty, size_t* begin, size_t* end, Vec3f* vtx)=0;
	///	材質の設定
	virtual void SetMaterial(const GRMaterial& mat)=0;
};

/**	デバッグ情報レンダラー	*/
struct GRDebugRenderIf:public GRRenderIf{
	IF_DEF(GRDebugRender);
	virtual void DrawSolid(PHSolidIf* so)=0;
	virtual void DrawFace(CDFaceIf* face, Vec3f * base)=0;
};


}
#endif

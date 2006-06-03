/**	\addtogroup	gpGraphics	*/
//@{

#ifndef SPR_GRFrame_H
#define SPR_GRFrame_H
#include <Graphics/SprGRRender.h>

namespace Spr{;


/**	@brief	グラフィックスで表示に影響を与えるもの	*/
struct GRVisualIf: public SceneObjectIf{
	IF_DEF(GRVisual);
	///	レンダリング．子ノード，弟ノードのレンダリングより前に呼ばれる．
	virtual void Render(GRRenderIf* r)=0;
	///	レンダリング終了処理．子ノード，弟ノードのレンダリングが終わってから呼ばれる．
	virtual void Rendered(GRRenderIf* r)=0;
};
	
	
///	DirectX の Frame の変換を表す FrameTransformMatrix ノードを読むためのDesc
struct GRFrameTransformMatrix{
	Affinef transform;
};

///	@brief GRFrame のDesc．座標系を指定する
struct GRFrameDesc{
	Affinef transform;
};
	
/**	@brief	グラフィックスシーングラフのツリーのノード．座標系を持つ．*/
struct GRFrameIf: public SceneObjectIf{
	IF_DEF(GRFrame);
	virtual GRFrameIf* GetParent()=0;
	virtual void SetParent(GRFrameIf* fr)=0;
	virtual bool AddChildObject(ObjectIf* v)=0;
	virtual bool DelChildObject(ObjectIf* v)=0;

	ObjectIf* CreateObject(const IfInfo* info, const void* desc)=0;
	virtual size_t NChildObject() const=0;
	virtual ObjectIf* GetChildObject(size_t pos)=0;
};

//@}
}
#endif

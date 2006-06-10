/**	\addtogroup	gpGraphics	*/
//@{

#ifndef SPR_GRFrame_H
#define SPR_GRFrame_H
#include <SprFoundation.h>

namespace Spr{;

struct GRRenderIf;
/**	@brief	グラフィックスで表示に影響を与えるもの	*/
struct GRVisualIf: public NamedObjectIf{
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
struct GRFrameIf: public GRVisualIf{
	IF_DEF(GRFrame);
	virtual GRFrameIf* GetParent()=0;
	virtual void SetParent(GRFrameIf* fr)=0;
	virtual Affinef GetWorldTransform()=0;
};

//@}
}
#endif

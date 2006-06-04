#ifndef GRFrame_H
#define GRFrame_H

#include <SprGraphics.h>

namespace Spr{;

/**	@class	GRVisual
    @brief	 */
class GRVisual: public InheritSceneObject<GRVisualIf, SceneObject>{
public:
	OBJECT_DEF_ABST(GRVisual);
	virtual void Render(GRRenderIf* render)=0;
	virtual void Rendered(GRRenderIf* render)=0;
};
template <class intf, class base>
struct InheritGRVisual:public InheritSceneObject<intf, base>{};

/**	@class	GRFrame
    @brief	グラフィックスシーングラフのツリーのノード 座標系を表す */
class GRFrame: public InheritGRVisual<GRFrameIf, GRVisual>, public GRFrameDesc{
public:
	OBJECT_DEF(GRFrame);
	GRFrame* parent;
	typedef std::vector<GRVisual*> GRVisuals;
	GRVisuals children;


	virtual GRFrameIf* GetParent(){ return parent; }
	virtual void SetParent(GRFrameIf* fr);
	virtual bool AddChildObject(ObjectIf* v);
	virtual bool DelChildObject(ObjectIf* v);

	virtual void Render(GRRenderIf* r);
	virtual void Rendered(GRRenderIf* r);
	virtual ObjectIf* CreateObject(const IfInfo* info, const void* desc);
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t pos);
};
}
#endif

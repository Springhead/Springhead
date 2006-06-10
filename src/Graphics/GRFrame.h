#ifndef GRFrame_H
#define GRFrame_H

#include <SprGraphics.h>

namespace Spr{;

/**	@class	GRVisual
    @brief	 */
class GRVisual: public InheritNamedObject<GRVisualIf, NamedObject>{
public:
	OBJECT_DEF_ABST(GRVisual);
	virtual void Render(GRRenderIf* render){}
	virtual void Rendered(GRRenderIf* render){}
};
template <class intf, class base>
struct InheritGRVisual:public InheritNamedObject<intf, base>{
	virtual void Render(GRRenderIf* render){ base::Render(render); }
	virtual void Rendered(GRRenderIf* render){ base::Rendered(render); }
};

/**	@class	GRFrame
    @brief	グラフィックスシーングラフのツリーのノード 座標系を表す */
class GRFrame: public InheritGRVisual<GRFrameIf, GRVisual>, public GRFrameDesc{
public:
	OBJECT_DEF(GRFrame);
	GRFrame* parent;
	typedef std::vector<GRVisual*> GRVisuals;
	GRVisuals children;
	GRFrame(const GRFrameDesc& desc=GRFrameDesc());


	virtual GRFrameIf* GetParent(){ return parent; }
	virtual void SetParent(GRFrameIf* fr);
	virtual void SetNameManager(NameManager* n);
	virtual GRSceneIf* GetScene();
	virtual bool AddChildObject(ObjectIf* v);
	virtual bool DelChildObject(ObjectIf* v);

	virtual void Render(GRRenderIf* r);
	virtual void Rendered(GRRenderIf* r);
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t pos);
};
}
#endif

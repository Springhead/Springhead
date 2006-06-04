#ifndef GRMesh_H
#define GRMesh_H

#include <SprGraphics.h>
#include "GRFrame.h"

namespace Spr{;

/**	@class	GRMesh
    @brief	グラフィックスシーングラフでの座標系を表す． */
class GRMesh: public InheritGRVisual<GRMeshIf, GRVisual>, public GRMeshDesc{
public:
	OBJECT_DEF(GRMesh);
	void Render(GRRenderIf* r);
	void Rendered(GRRenderIf* r);
};
}
#endif

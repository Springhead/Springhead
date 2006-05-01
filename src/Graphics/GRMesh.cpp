#include "Graphics.h"
#include "GRMesh.h"

namespace Spr{;
OBJECTIMP(GRMesh, SceneObject);
IF_IMP(GRMesh, SceneObject);

void GRMesh::Render(GRRenderIf* r){
//TODO	r->DrawIndexed(GRRender::TRIANGLES)
}
void GRMesh::Rendered(GRRenderIf* r){
}

}

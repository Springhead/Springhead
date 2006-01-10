#ifndef GRDEBUGRENDER_H
#define GRDEBUGRENDER_H

#include <Springhead.h>

namespace Spr{;

/**	デバッグ情報レンダラー	*/
class GRDebugRender:public GRRender, public GRDebugRenderIf{
	OBJECTDEF(GRDebugRender);
	BASEIMP_GRRENDER(GRRender);
	void DrawSolid(PHSolidIf* so);
	void DrawFace(CDFaceIf* face, Vec3f * base);
	void Print(std::ostream& os) const{ GRRender::Print(os); }
};

}
#endif

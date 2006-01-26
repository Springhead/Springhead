#include "Graphics.h"
#pragma hdrstop
#include <gl/glut.h>

namespace Spr {;
//----------------------------------------------------------------------------
//	GRDebugRender
OBJECTIMP(GRDebugRender, GRRender);
IF_IMP(GRDebugRender, GRRender);

// 剛体をレンダリングする
void GRDebugRender::DrawSolid(PHSolidIf* so){
	for (int i=0; i<so->GetNShapes(); ++i) {	
		CDShapeIf **shapes = so->GetShapes();
		CDConvexMeshIf* mesh = ICAST(CDConvexMeshIf, shapes[i]);
		Vec3f* base = mesh->GetVertices();
		for (size_t f=0; f<mesh->GetNFaces(); ++f) {	
			CDFaceIf* face = mesh->GetFace(f);
			this->DrawFace(face, base);
		}
	}
}
// 面をレンダリングする
void GRDebugRender::DrawFace(CDFaceIf* face, Vec3f * base){
	glBegin(GL_POLYGON);
	for (int v=0; v<face->GetNIndices(); ++v) {
		glVertex3fv(base[face->GetIndices()[v]].data);
	}
	glEnd();
}

}	//	Spr

